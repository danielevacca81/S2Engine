// SelectionRequest.h
// 

//// --------------------------------------------------------------------------------------------
//// Simple structure to handle multiple requests
//template <typename T>
//class SelectionRequest
//{
//public:
//	// --------------------------------------------------------------------------------------------
//	std::vector<T> types;
//	std::vector<int> reqQty;
//	std::vector<int> currQty;
//	int totObjectRequested;
//	// --------------------------------------------------------------------------------------------
//	
//public:
//	// --------------------------------------------------------------------------------------------
//	SelectionRequest(T t, int qty)
//	{
//		// ignore quantities on negative values of qty
//
//		types.clear();
//		reqQty.clear();
//		currQty.clear();
//
//		types.push_back(t);
//		reqQty.push_back(qty);
//		currQty.push_back(qty);
//		totObjectRequested = qty;
//	}
//
//	// --------------------------------------------------------------------------------------------
//	SelectionRequest(std::vector<T> t, std::vector<int> qty)
//	{
//		types.clear();
//		reqQty.clear();
//		currQty.clear();
//		totObjectRequested = 0;
//
//		for(size_t i=0; i<t.size(); ++i)
//		{
//			types.push_back(t[i]);
//			reqQty.push_back(qty[i]);
//			currQty.push_back(qty[i]);
//			totObjectRequested += qty[i];
//		}
//	}
//};