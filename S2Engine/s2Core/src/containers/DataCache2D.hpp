// DataCache2D.h
//
#include "DataCache2D.h"
//#include "TimedBlock.h"

//#include <iostream>

// DataCache2D: cache di blocchi di dati anche non contigui. Ogni blocco è delimitato da una coppia bIndex, eIndex
// -----------------------------------------------------------------------------------------------
template <typename T>
DataCache2D<T>::DataCache2D( int32_t maxSize )
: _lastReqBeginIndex(0)
, maxCacheSize(maxSize)
{}

// -----------------------------------------------------------------------------------------------
template <typename T>
DataCache2D<T>::~DataCache2D()
{}

// -----------------------------------------------------------------------------------------------
template <typename T>
IntersectType DataCache2D<T>::_usedBlocks(const Request &r, std::vector<UsedBlock> &usedBlocks) const
{
	//Si ricercano i blocchi che intersecano r. Si hanno i seguenti possibili risultati come operazione globale su r.
	//0: no intersezione
	//1: intersezione a sx
	//2: intersezione a dx
    //3: margine sx e dx di r intersecano un blocco
    //128: blocco/i totalmente contenuto/i senza intersezione

    //Ogni singolo usedBlock, indica il tipo di operazione che lo interessa, ovvero:
    //0: no intersezione
    //1: intersezione a sx
    //2: intersezione a dx
    //3: margine sx e dx di r intersecano un blocco
    //128: blocco/i totalmente contenuto/i senza intersezione

    IntersectType globalResult = IntersectType::none;
    auto inRange = [](int value, int lower, int upper) {return (value >= lower) && (value <= upper); };
    auto contained = [](const DataCache &inner, const DataCache &outer) {return (outer._bIndex < inner._bIndex) && (outer._eIndex > inner._eIndex); };
	int i = 0;
	for (const auto &d : _cache)
	{
		if (r.endIndex < d._bIndex)
		{
            //                   bIndex..............eIndex
            //r.b..........r.e
            if (usedBlocks.empty()) usedBlocks.push_back(UsedBlock(i, IntersectType::none));
			break;
		}

		int id = -1;
        IntersectType partialResult = IntersectType::none;    //Risultato sul singolo blocco
		if (inRange(r.beginIndex, d._bIndex, d._eIndex))
		{
			//bIndex..............eIndex
			//            r.b...................r.e
			//Intersezione a sx
            id = i;
			partialResult = IntersectType::left;
		}
		if (inRange(r.endIndex, d._bIndex, d._eIndex))
		{
			//        bIndex................eIndex
			//r.b...................r.e
			//Intersezione a dx
            id = i;
            //Se ret != none allora c'è intersezione a sx e dx
            partialResult = partialResult == IntersectType::none ? IntersectType::right : IntersectType::left_right;
		}
        if (contained(d, DataCache(r.beginIndex, r.endIndex)))
        {
            //Un blocco in cache non può essere intersecato e contenuto contemporaneamente
            partialResult = IntersectType::contained;
            id = i;
        }
		if (id != -1)
		{
			usedBlocks.push_back(UsedBlock(id, partialResult));
		}
		i++;
        if (partialResult == IntersectType::contained) {
            if (globalResult == IntersectType::none)
            {
                globalResult = partialResult;
            }
        } else
        {
            int ret = (int)globalResult & 0x7f; //Resetto il bit 8
            ret |= (int)partialResult;
            globalResult = (IntersectType)ret;
        }
	}

	return globalResult;
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::store(const Request &r)
{
	std::lock_guard<std::mutex> lock(_mutex);

    //std::cout << "1 - _store - TotalDataBlocks: " << _cache.size() << " , PR Index: " << r.beginIndex << " , " << r.endIndex << std::endl;
    //for (const auto &d : _cache)
    //{
    //    std::cout << "1 - _store - Index: " << d._bIndex << " , " << d._eIndex << " , DataSize: " << d._data.columnsCount() << std::endl;
    //}

    auto intersect = [](IntersectType type) { return (type == IntersectType::left) || (type == IntersectType::right) || (type == IntersectType::left_right); };
    //Se il blocco che devo memorizzare è maggiore della dimensione della cache, 
    //allora svuoto la cache e memorizzo maxCacheSize elementi del nuovo blocco
    int blockSize = r.endIndex - r.beginIndex + 1;
    if ((maxCacheSize > 0) && (blockSize >= maxCacheSize))
    {
        //@todo: aumentare dimensione cache dinamicamente???
        //Il blocco da aggiungere supera la dimensione della cache
        //Si svuota la cache e si inserisce il blocco
        _cache.clear();
        DataCache d(r.beginIndex, r.beginIndex + maxCacheSize - 1);
        for (int i = 0; i < maxCacheSize; i++)
        {
            d._data.appendColumn(r.outputData.column(i).values());
        }
        _cache.insert(_cache.end(), d);
    } else
    {
        std::vector<UsedBlock> ub;
        IntersectType iType = _usedBlocks(r, ub);
        //std::cout << "STORE RET = " << iType << std::endl;

        assert(iType == 0 || iType == 1 || iType == 2 || iType == 3|| iType == 128);

        if (iType == IntersectType::none)
        {
            _storeNoInterBlock(r, ub);
        }
        if (intersect(iType))
        {
            //Il blocco da storare interseca blocchi esistenti
            switch (iType) {
            case IntersectType::left: _storeLeft(r, ub); break;
            case IntersectType::right: _storeRight(r, ub); break;
            case IntersectType::left_right: _storeLeftRight(r, ub); break;
            default: assert(false);
            }

            //Rimozione dalla cache di tutti i blocchi di tipo contained
            for (int i = ub.size() - 1; i >= 0; i--)
            {
                if (ub[i].iType == IntersectType::contained)
                {
                    _cache.erase(_cache.begin() + ub[i].blockId);
                }
            }
        }
        if (iType == IntersectType::contained)
        {
            //Uno o più blocchi di cache di cache sono totalmente contenuti in r (no intersezione)
            _storeNoInterBlock(r, ub);
        }

        //Controllo dimensione cache
        const int32_t csize = size();
        if ((maxCacheSize != 0) && (csize > maxCacheSize))
        {
            int deltaCol = csize - maxCacheSize;
            bool forward = r.beginIndex >= _lastReqBeginIndex;  //Se true, mi sto spostando in avanti
            _removeFromCache(deltaCol, forward);
        }
        _lastReqBeginIndex = r.beginIndex;
    }
	//std::cout << "3 - _store - TotalDataBlocks: " << _cache.size() << " , cache size: " << size() << std::endl;
	//for (const auto &d : _cache)
	//{
	//	std::cout << "3 - _store - Index: " << d._bIndex << " , " << d._eIndex << " , DataSize: " << d._data.columnsCount() << std::endl;
	//}
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_storeLeft(const Request &r, const std::vector<UsedBlock> &ub)
{
    //TimedBlock t("_storeLeft");
    //r interseca un solo blocco dal margine sx. Eventuali altri blocchi contenuti saranno rimossi
    assert(ub.size() >= 1);
    assert(ub[0].iType == IntersectType::left);
    //bIndex..............eIndex   bIndex.......eIndex
    //            r.b.......................................r.e
    //Si aggiungono le nuove colonne in coda al blocco selezionato più a sinistra
    DataCache &d = _cache.at(ub[0].blockId);
    int32_t colToAdd = r.endIndex - d._eIndex;
    int32_t startCol = r.outputData.columnsCount() - colToAdd;
    for (int i = startCol; i < startCol + colToAdd; i++)
    {
        d._data.appendColumn(r.outputData.column(i).values());
    }
    d._eIndex = r.endIndex;
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_storeRight(const Request &r, const std::vector<UsedBlock> &ub)
{
    //TimedBlock t("_storeRight");
    //r interseca un solo blocco dal margine dx. Eventuali altri blocchi contenuti saranno rimossi
    assert(ub.size() >= 1);
    assert(ub[ub.size() - 1].iType == IntersectType::right);
    //        bIndex..............eIndex   bIndex...........eIndex
    //r.b............................................r.e

    //Array2D<short> tmp;
    //DataCache &d = _cache.at(ub[ub.size() - 1].blockId);
    //int32_t colToAdd = d._bIndex - r.beginIndex;

    //for (int i = 0; i < colToAdd; i++)
    //{
    //    tmp.appendColumn(r.outputData.column(i).values());
    //}

    //for (int i = 0; i < d._data.columnsCount(); i++)
    //{
    //    tmp.appendColumn(d._data.column(i).values());
    //}
    //d._data = tmp;

    //Si aggiungono le nuove colonne in testa al blocco selezionato più a destra. Vanno aggiunte in ordine inverso
    DataCache &d = _cache.at(ub[ub.size() - 1].blockId);
    for (int i = d._bIndex - r.beginIndex - 1; i >= 0; i--)
    {
        d._data.pushColumnFront(r.outputData.column(i).values());
    }
    d._bIndex = r.beginIndex;
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_storeLeftRight(const Request &r, const std::vector<UsedBlock> &ub)
{
    //Il blocco da aggiungere interseca sia a sx che a dx blocchi esistenti.
    //CASO 1) Se ub contiene un solo valore, allora il blocco da aggiungere è già contenuto in un blocco di cache esistente.
    //CASO 2) Se ub contiene due valori, allora il blocco da aggiungere sta a cavallo di due blocchi in cache.
    //        Si deve fare il merge di tutti i blocchi che stanno tra ub[0] e ub[1].
    assert(ub.size() >= 1);
    assert(ub[0].iType == IntersectType::left || ub[0].iType == IntersectType::left_right);
    assert(ub[ub.size() - 1].iType == IntersectType::right || ub[ub.size() - 1].iType == IntersectType::left_right);
    //CASO 1
    //eIndex1...........................eIndex1
    //          r.b.............r.e
    //CASO 2
    //eIndex1............eIndex1    bIndex2..........eIndex2   bIndex3..............eIndex3
    //            r.b......................................................r.e
    if (ub.size() == 1)
    {
        //std::cout << "Blocco gia' in cache" << std::endl;
    } else
    {
        //Merge di due o più blocchi
        int numBlocksToMerge = ub[ub.size() - 1].blockId - ub[0].blockId + 1;
        DataCache &d1 = _cache.at(ub[0].blockId);
        DataCache &d2 = _cache.at(ub[ub.size() - 1].blockId);

        //Si aggiungono le nuove colonne in coda al blocco selezionato di sx (d1)
        int32_t colToAdd = r.endIndex - d1._eIndex;
        int32_t startCol = r.outputData.columnsCount() - colToAdd;
        for (int i = startCol; i < startCol + colToAdd; i++)
        {
            d1._data.appendColumn(r.outputData.column(i).values());
        }
        d1._eIndex = r.endIndex;

        //Si aggiungono le colonne restanti del blocco selezionato di dx (d2)
        colToAdd = d2._eIndex - d1._eIndex;
        startCol = d2._data.columnsCount() - colToAdd;
        for (int i = startCol; i < startCol + colToAdd; i++)
        {
            d1._data.appendColumn(d2._data.column(i).values());
        }
        d1._eIndex = d2._eIndex;

        //Rimozione del blocco più a destra perché è stato mergiato con d1
        _cache.erase(_cache.begin() + ub[ub.size() - 1].blockId);
    }
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_storeNoInterBlock(const Request &r, const std::vector<UsedBlock> &ub)
{
    //Il blocco da storare non interseca o contiene alcun blocco già presente in cache.

    //Caso 1: la cache è vuota
    //Caso 2: blocchi presenti ma no intersezione
    //                 bIndex1.....eIndex1    bIndex2.....eIndex2
    //r.b........r.e

    //bIndex1.....eIndex1                      bIndex2.....eIndex2
    //                      r.b........r.e

    //Caso 3:
    //Il blocco da aggiungere contiene uno o più blocchi di cache ma senza intersecarli. 
    //I blocchi contenuti vanno rimossi.
    //       bIndex1.....eIndex1    bIndex2.....eIndex2
    //r.b.................................................r.e

    int blockId = -1;
    //Ricerca di eventuali blocchi contenuti. Vanno rimossi
    for (int i = ub.size() - 1; i >= 0; i--)
    {
        if (ub[i].iType == IntersectType::contained)
        {
            blockId = ub[i].blockId;    //Mi salvo il punto di inserimento del nuovo blocco
            _cache.erase(_cache.begin() + blockId);
        }
    }

    if (blockId == -1)
    {
        //Non ho trovato alcun blocco contenuto. 
        //Se ub contiene un indice, questo rappresenta il blocco in cache
        //immediatamente a destra di quello da inserire (ordinamento per beginIndex).
        //Se ub è vuoto, allora il nuovo blocco va inserito in coda alla cache
        assert(ub.size() <= 1);
        if (!ub.empty())
        {
            //Inserimento in posizione i-esima. Blocchi mantenuti ordinati per beginIndex
            blockId = ub[0].blockId;
        }
    }

    bool forward = r.beginIndex >= _lastReqBeginIndex;  //Se true, mi sto spostando in avanti
    auto it = forward ? _cache.end(): _cache.begin(); //Inserimento in coda o in testa a seconda della direzione di spostamento
    if (blockId != -1 && blockId < _cache.size()) it = _cache.begin() + blockId;    //Inserimento in posizione blockId

    //Aggiungo il nuovo blocco
    DataCache d(r.beginIndex, r.endIndex);
    d._data = r.outputData;
    _cache.insert(it, d);
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_removeFromCache(int numColsToRemove, bool fromBegin) 
{
    //Le colonne da rimuovere potrebbero non stare tutte in un blocco
    while (numColsToRemove > 0)
    {
        DataCache &dc = fromBegin ? _cache.front() : _cache.back();
        assert(!dc._data.isEmpty());
        if (fromBegin)
        {
            //Si rimuovono colonne dal blocco più a sx.
            dc._data.removeColumns(0, 0);
            dc._bIndex += 1;
        } else
        {
            //Si rimuovono colonne dal blocco più a dx.
            int32_t col = dc._data.columnsCount() - 1;
            dc._data.removeColumns(col, col);
            dc._eIndex -= 1;
        }
        numColsToRemove--;
        //Se il blocco rimane senza colonne, va rimosso dalla cache
        if (dc._data.isEmpty())
        {
            if (fromBegin) _cache.erase(_cache.begin());
            else _cache.pop_back();
        }
    }
}

// -----------------------------------------------------------------------------------------------
template <typename T>
bool DataCache2D<T>::fetch(Request &r) const
{
    std::lock_guard<std::mutex> lock(_mutex);
    //La cache request deve contenere un solo blocco di input che è quello della richiesta a video
    assert(r.inputs.size() == 1);
    //Il vettore degli outputData deve essere grande quanto la richiesta
    assert(r.inputs[0].data.columnsCount() == r.outputData.columnsCount());

    auto intersect = [](IntersectType type) {return (type == IntersectType::left) || (type == IntersectType::right) || (type == IntersectType::left_right); };
    //std::cout << "\n1 - load - TotalDataBlocks: " << _cache.size() << " , PR Index: " << r.beginIndex << " , " << r.endIndex << std::endl;
    //for (const auto &d : _cache)
    //{
    //    std::cout << "1 - load - Index: " << d._bIndex << " , " << d._eIndex << " , DataSize: " << d._data.columnsCount() << std::endl;
    //}

    int sCol = 0, eCol = 0;

    auto it = _cache.end();
    std::vector<UsedBlock> ub;
    IntersectType iType = _usedBlocks(r, ub);

    assert(iType == 0 || iType == 1 || iType == 2 || iType == 3 || iType == 128);
    //std::cout << "LOAD RET = " << ret << std::endl;

    int numColFromCache = 0;
    int numColToProcess = 0;
    if (iType == IntersectType::none)
    {
        //Dati in cache non trovati. Va processato tutto r
        numColToProcess = r.inputs[0].data.columnsCount();
    }

    if (intersect(iType))
    {
        //Il blocco richiesto interseca blocchi in cache esistenti
        switch (iType)
        {
        case IntersectType::left: _loadLeft(r, ub, numColFromCache, numColToProcess); break;
        case IntersectType::right: _loadRight(r, ub, numColFromCache, numColToProcess); break;
        case IntersectType::left_right: _loadLeftRight(r, ub, numColFromCache, numColToProcess); break;
        default: assert(false);
        }

        //Rimozione di eventuali blocchi contenuti ma non intersecati
        for (auto block : ub)
        {
            if (block.iType == IntersectType::contained)
            {
                //Blocco completo già processato
                const DataCache &d = _cache.at(block.blockId);
                sCol = 0;
                eCol = d._eIndex - d._bIndex;
                int offset = d._bIndex - r.beginIndex;
                for (int i = sCol; i <= eCol; i++)
                {
                    r.outputData.setColumnValues(d._data.column(i).values(), offset + i);
                    numColFromCache++;
                }
            }
        }
    }

    if (iType == IntersectType::contained)
    {
        //Il blocco richiesto contiene uno o più blocchi in cache ma senza intersecarli
        _loadContained(r, ub, numColFromCache, numColToProcess);
    }
    //std::cout << "2 - load - numColFromCache: " << numColFromCache << " , numColToProcess: " << numColToProcess << std::endl;
    //for (const auto &ib : r.inputs)
    //{
    //    std::cout << "3 - load - inputblock - procindex: " << ib.procIndex << " , datasize: " << ib.data.columnsCount() << std::endl;
    //}
    return numColToProcess > 0;
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_loadLeft(Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess) const
{
    //r interseca a sx un solo blocco in cache. Altri blocchi possono essere inclusi
    assert(ub.size() >= 1);
    //bIndex..............eIndex    bIndex.........eIndex
    //            r.b.......................................r.e

    int sCol = 0, eCol = 0;

    //Blocco di cache più a sinistra (intersecato da r da sx)
    const UsedBlock &block = ub[0];
    assert(block.iType == IntersectType::left);
    const DataCache &d = _cache.at(block.blockId);
    sCol = r.beginIndex - d._bIndex;
    eCol = d._eIndex - d._bIndex;
    int offset = 0;
    for (int i = sCol; i <= eCol; i++)
    {
        r.outputData.setColumnValues(d._data.column(i).values(), offset + i - sCol);
        numColFromCache++;
    }

    //La cache request contiene un solo blocco di input che è quello della richiesta a video
    const Block input = r.inputs[0]; //Copia della richiesta di dati (da video)
    int lastIndex = r.endIndex;
    r.inputs.clear();
    //Scandisco ub in ordine inverso per determinare i nuovi blocchi da processare
    for (auto rit = std::rbegin(ub); rit != std::rend(ub); ++rit)
    {
        const DataCache &d = _cache.at(rit->blockId);
        sCol = (d._eIndex - r.beginIndex) + 1;
        eCol = lastIndex - r.beginIndex;
        if (eCol >= sCol)
        {
            Block newInput;
            newInput.data = input.data.columns(sCol, eCol);
            newInput.procIndex = sCol;
            r.inputs.insert(r.inputs.begin(), newInput);
            numColToProcess += newInput.data.columnsCount();
        }
        lastIndex = std::max(d._bIndex - 1, 0);
    }
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_loadRight(Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess) const
{
    //r interseca a dx un blocco esistente 
    assert(ub.size() >= 1);
    //         bIndex.......eIndex     bIndex..............eIndex
    //r.b........................................r.e
    
    int sCol = 0, eCol = 0;

    //Blocco di cache più a destra (intersecato da r da dx)
    const UsedBlock &block = ub[ub.size() - 1];
    assert(block.iType == IntersectType::right);
    const DataCache &d = _cache.at(block.blockId);
    //Dati già processati
    sCol = 0;
    eCol = r.endIndex - d._bIndex;
    int offset = d._bIndex - r.beginIndex;
    for (int i = sCol; i <= eCol; i++)
    {
        r.outputData.setColumnValues(d._data.column(i).values(), offset + i - sCol);
        numColFromCache++;
    }

    //La cache request contiene un solo blocco di input che è quello della richiesta a video
    const Block input = r.inputs[0]; //Copia della richiesta di dati (da video)
    int lastIndex = r.beginIndex;
    r.inputs.clear();
    //Scandisco ub per determinare i nuovi blocchi da processare
    for (auto it = std::begin(ub); it != std::end(ub); ++it)
    {
        const DataCache &d = _cache.at(it->blockId);
        //I dati da processare sono quelli più a sinistra di input
        sCol = lastIndex - r.beginIndex;
        eCol = (d._bIndex - r.beginIndex) - 1;
        if (eCol >= sCol)
        {
            Block newInput;
            newInput.data = input.data.columns(sCol, eCol);
            newInput.procIndex = sCol;
            r.inputs.push_back(newInput);
            numColToProcess += newInput.data.columnsCount();
        }
        lastIndex = d._eIndex + 1;
    }
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_loadLeftRight(Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess) const
{
    //r interseca a sx e a dx uno o due blocchi già esistente 
    assert(ub.size() >= 1);
    //CASO 1 - Blocco totalmente in cache
    //eIndex1...........................eIndex1
    //          r.b.............r.e
    //CASO 2
    //eIndex1............eIndex1    lIndex2..........uIndex2   lIndex3..............uIndex3
    //            r.b......................................................r.e
    
    int sCol = 0, eCol = 0;
    
    if (ub.size() == 1)
    {
        //Blocco totalmente in cache
        const DataCache &d = _cache.at(ub[0].blockId);
        sCol = r.beginIndex - d._bIndex;
        eCol = r.endIndex - d._bIndex;
        for (int i = sCol; i <= eCol; i++)
        {
            r.outputData.setColumnValues(d._data.column(i).values(), i - sCol);
            numColFromCache++;
        }
        r.inputs.clear();    //Processing non necessario
    } else
    {
        //Più blocchi intersecati
        int offset = 0;
        int lastIndex = r.endIndex;
        //Blocco di cache più a sinistra (intersecato da r da sx)
        const UsedBlock &block1 = ub[0];
        assert(block1.iType == IntersectType::left);
        const DataCache &d1 = _cache.at(block1.blockId);
        //Dati già processati
        if (block1.iType == IntersectType::left)
        {
            //Blocco di cache più a sinistra (intersecato da r da sx)
            sCol = r.beginIndex - d1._bIndex;
            eCol = d1._eIndex - d1._bIndex;
            //r.outputData = d._data.columns(sCol, eCol);
            for (int i = sCol; i <= eCol; i++)
            {
                r.outputData.setColumnValues(d1._data.column(i).values(), offset + i - sCol);
                numColFromCache++;
            }
        }

        //Blocco di cache più a destra (intersecato da r da dx)
        const UsedBlock &block2 = ub[ub.size() - 1];
        assert(block2.iType == IntersectType::right);
        const DataCache &d2 = _cache.at(block2.blockId);
        if (block2.iType == IntersectType::right)
        {
            //Blocco di cache più a destra (intersecato da r da dx)
            sCol = 0;
            eCol = r.endIndex - d2._bIndex;
            offset = d2._bIndex - r.beginIndex;
            for (int i = sCol; i <= eCol; i++)
            {
                r.outputData.setColumnValues(d2._data.column(i).values(), offset + i - sCol);
                numColFromCache++;
            }
            lastIndex = d2._bIndex - 1;

        }

        //La cache request contiene un solo blocco di input che è quello della richiesta a video
        const Block input = r.inputs[0]; //Copia della richiesta di dati (da video)
        r.inputs.clear();
        //Scandisco ub in ordine inverso per determinare i nuovi blocchi da processare
        for (auto rit = std::rbegin(ub); rit != std::rend(ub); ++rit)
        {
            const DataCache &d = _cache.at(rit->blockId);
            //I dati da processare sono quelli più a destra di input
            sCol = (d._eIndex - r.beginIndex) + 1;
            eCol = lastIndex - r.beginIndex;
            if (eCol >= sCol)
            {
                Block newInput;
                newInput.data = input.data.columns(sCol, eCol);
                newInput.procIndex = sCol;
                r.inputs.insert(r.inputs.begin(), newInput);
                numColToProcess += newInput.data.columnsCount();
            }
            lastIndex = std::max(d._bIndex - 1, 0);
        }
    }
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::_loadContained(Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess) const
{
    //r contiene totalmente uno o più blocchi senza intersecarli
    //        eIndex1.......eIndex1    lIndex2......uIndex2
    // r.b.....................................................r.e
    
    int sCol = 0, eCol = 0;
    int offset = 0;

    for (auto block : ub)
    {
        const DataCache &d = _cache.at(block.blockId);
        //Dati già processati
        if (block.iType == IntersectType::contained)
        {
            //Blocco completo già processato
            sCol = 0;
            eCol = d._eIndex - d._bIndex;
            offset = d._bIndex - r.beginIndex;
            for (int i = sCol; i <= eCol; i++)
            {
                r.outputData.setColumnValues(d._data.column(i).values(), offset + i);
                numColFromCache++;
            }
        }
    }

    int lastIndex = r.endIndex;
    //La cache request contiene un solo blocco di input che è quello della richiesta a video
    const Block input = r.inputs[0]; //Copia della richiesta di dati (da video)
    r.inputs.clear();
    for (auto rit = std::rbegin(ub); rit != std::rend(ub); ++rit)
    {
        const DataCache &d = _cache.at(rit->blockId);
        //I dati da processare sono quelli più a destra di input
        sCol = (d._eIndex - r.beginIndex) + 1;
        eCol = lastIndex - r.beginIndex;
        if (eCol >= sCol)
        {
            Block newInput;
            newInput.data = input.data.columns(sCol, eCol);
            newInput.procIndex = sCol;
            r.inputs.insert(r.inputs.begin(), newInput);
            numColToProcess += newInput.data.columnsCount();
        }
        lastIndex = std::max(d._bIndex - 1, 0);
    }
    //Devo copiare l'ultimo pezzo di input
    sCol = 0;
    eCol = lastIndex - r.beginIndex;
    if (eCol >= sCol)
    {
        Block newInput;
        newInput.data = input.data.columns(sCol, eCol);
        newInput.procIndex = sCol;
        r.inputs.insert(r.inputs.begin(), newInput);
        numColToProcess += newInput.data.columnsCount();
    }
}


// -----------------------------------------------------------------------------------------------
template <typename T>
int32_t DataCache2D<T>::size() const
{
	int32_t size = 0;
	for (auto d : _cache)
	{
		size += d._data.columnsCount();
	}
	return size;
}

// -----------------------------------------------------------------------------------------------
template <typename T>
void DataCache2D<T>::clear()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _cache.clear();
    _lastReqBeginIndex = 0;
}
