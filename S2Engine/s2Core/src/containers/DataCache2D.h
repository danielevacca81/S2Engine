// DataCache2D.h
//
#ifndef CORE_DATACACHE2D_H
#define CORE_DATACACHE2D_H

//#include "s2Core_API.h"
#include "Array2D.h"

#include <vector>
#include <mutex>

namespace s2 {

namespace {
enum IntersectType
{
	none = 0,           //Nessuna intersezione
	left = 1,           //Intersezione a sx
	right = 2,          //Intersezione a dx
	left_right = 3,     //Intersezione a sx e dx
	contained = 128,     //Blocchi contenuti
};
}
/**
* Questa classe modella una cache di dati secondo il paradigma <dati da elaborare, dati elaborati>.
* La cache è organizzata come sequenze di blocchi di dati disgiunti. Ogni blocco contiene dati organizzati come un Array2D appartenenti all'intervallo [beginIndex, endIndex].
* I dati in cache sono da intendere come i dati salvati ad esempio a seguito di una elaborazione.
* La cache contiene al piu' maxCacheSize colonne di dati elaborati.
*/
template <typename T>
class /*S2CORE_API*/ DataCache2D
{
public:
	/**
	 * Rappresenta un blocco di dati contigui da processare e da inserire a partire dall'indice procIndex
	 */
	struct Block
	{
		Block()
			: procIndex( 0 )
		{}

		Block( const Array2D<T> &d )
			: procIndex( 0 )
			, data( d )
		{}
		int procIndex;  //Indica l'indice a cui questi dati si riferiscono. Una volta elaborati saranno inseriti in questo punto all'interno del vettore outputData
		Array2D<T> data;
	};

	/**
	 * La CacheRequest rappresenta una richiesta di dati per la cache.
	 * Il vettore inputs rappresenta la richiesta di dati relativi all'intervallo [beginIndex, endIndex]. Sono i dati da elaborare.
	 * Il vettore outputData viene rimepito con i dati presenti in cache appartenenti allo stesso intervallo. Sono i dati elaborati.
	 */
	struct Request
	{
		Request()
			: beginIndex( 0 )
			, endIndex( 0 )
		{}

		Request( int begin, int end, const Block &blk )
			: beginIndex( begin )
			, endIndex( end )
		{
			inputs.push_back( blk );
			outputData = blk.data;
		}

		bool isEmpty() const { return beginIndex != 0 && endIndex != 0; }

		void merge( const Array2D<T> &blk, int index )
		{
			if( blk.isEmpty() )
				return;

			for( int i = 0; i < blk.columnsCount(); ++i )
			{
				//I nuovi dati vanno inseriti in outputData
				outputData.setColumnValues( blk.column( i ).values(), index + i );
			}
		}


		int beginIndex;     //L'indice assoluto di partenza della request
		int endIndex;       //L'indice assoluto finale della richiesta
		std::vector<Block> inputs; //Vettore dei dati da elaborare. Nella prima richiesta (chiamata a DataCache2D::load), i dati richiesti corrispondono a tutto l'intervallo beginIndex...endIndex
		Array2D<T> outputData;  //Vettore dei dati elaborati
	};

	/**
	 * Rappresenta un blocco di dati in cache delimitato dagli indici assoluti [bIndex, eIndex]
	 */
	struct DataCache
	{
		DataCache( int32_t b, int32_t e )
		{
			_bIndex = b;
			_eIndex = e;
		}
		int32_t _bIndex;        //Begin index del blocco
		int32_t _eIndex;        //End index del blocco
		Array2D<T> _data;   //Dati processati
	};

	DataCache2D( int32_t maxSize = 1024 );
	virtual ~DataCache2D();

	/**
	* Memorizza dati elaborati in cache.
	* Quando il metodo store viene chiamato, i dati elaborati contenuti in outputData e corrispondenti all'intervallo [beginIndex, endIndex],
	* vengono salvati in cache. Se necessario, blocchi di dati gia' presenti in cache saranno uniti tra loro.
	* Se il totale delle colonne di tutti i blocchi presenti in cache supera maxCacheSize, saranno rimosse le colonne in eccesso in base alla direzione di ricerca.
	* @param r La cache request
	*/
	void store( const Request &r );

	/**
	* Ricerca dati in cache.
	* Quando il metodo load viene chiamato, inputs contiene una sola entry pari a tutti i dati da elaborare nell'intervallo [beginIndex, endIndex].
	* La load verifica se all'interno della cache, ci sono blocchi di dati elaborati che intersecano l'intervallo richiesto.
	* Il vettore inputs viene modificato per riflettere i blocchi di dati effettivamente da elaborare perche' non presenti in cache (al più coincidente con quello iniziale).
	* Una volta elaborati, i dati dovranno essere inseriti in outputData nell'indice indicato dal campo procIndex di InputData.
	* Il vettore outputData deve essere grande quanto la richiesta e viene rimepito con i dati già elaborati presenti in cache.
	* @param r La cache request
	* @return True se ci sono dati elaborare, false se la richiesta di dati (inputs) e' totalmente contenuta in cache
	*/
	bool fetch( Request &r ) const;

	/**
	 * @return The number of column stored in the cache
	 */
	int32_t size() const;

	/**
	* Set the cache size
	* @param size The cache size. 0 means unlimited
	*/
	void setMaxCacheSize( uint32_t size ) { maxCacheSize = size; }

	/**
	* Clear the cache
	*/
	void clear();

private:
	struct UsedBlock
	{
		UsedBlock( int id, IntersectType type )
		{
			blockId = id;
			iType = type;
		}
		int blockId;    //Indice del blocco in cache
		IntersectType iType;
	};

	IntersectType _usedBlocks( const Request &r, std::vector<UsedBlock> &usedBlocks ) const;

	void _storeNoInterBlock( const Request &r, const std::vector<UsedBlock> &ub );
	void _storeLeft( const Request &r, const std::vector<UsedBlock> &ub );
	void _storeRight( const Request &r, const std::vector<UsedBlock> &ub );
	void _storeLeftRight( const Request &r, const std::vector<UsedBlock> &ub );

	void _loadLeft( Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess ) const;
	void _loadRight( Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess ) const;
	void _loadLeftRight( Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess ) const;
	void _loadContained( Request &r, const std::vector<UsedBlock> &ub, int &numColFromCache, int &numColToProcess ) const;

	void _removeFromCache( int numColsToRemove, bool fromBegin );

private:
	mutable std::mutex _mutex;
	int32_t _lastReqBeginIndex;
	std::vector<DataCache> _cache;
	int maxCacheSize;
};

}

#include "DataCache2D.hpp"

#endif