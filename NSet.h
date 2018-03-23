#pragma once

#include <functional>
#include <cstdint>
#include <iostream>

typedef int64_t 			intpp;
typedef intpp				NHashValue;		// Our hashing needs to consider how many bits in our word size.

template<typename T>
class NSet
{
	typedef NSet<T> SetT;
public:

	/// a node of a set
	class Node
	{
	public:
		/// constructor
		Node(T theItem, NHashValue theHashValue, Node* theNextNode) : item_(theItem), hashValue_(theHashValue), next_(theNextNode) { }

		T item_;					///< the item
		NHashValue hashValue_;		///< the hash value of the item (so we don't have to keep recomputing it)
		Node* next_;				///< the next node that contains a item with a similar (hashValue % arraySize) hash value
	};
	typedef Node* NodePtr;


	
	NSet(intpp theCapacity) : 	size_(0), nodeArraySize_(0), nodeArray_(nullptr)
	{
		if (theCapacity > 0)
			ensureCapacity(theCapacity);
	}

	virtual ~NSet()
	{
		delete [] nodeArray_;
	}

	

	virtual void addItem(const T& theItem)
	{
		if (findItemNode(theItem))
			return;
			
		ensureCapacity(++size_);
		
		NHashValue theHashValue = std::hash<T>()(theItem);
		Node* theNode = nodeArray_[theHashValue % nodeArraySize_];
		
		// insert the new node directly into the array, chaining any existing nodes off of it
		nodeArray_[theHashValue % nodeArraySize_] = new Node(theItem, theHashValue, theNode);
	}


	intpp hashArraySize(intpp theCapacity, double maxLoadFactor)
	{
		const intpp growthMatrix[] =
			{5, 11, 17, 29, 47, 71, 107, 163, 251, 379, 569, 857, 1289, 1949,
			2927, 4391, 6599, 9901, 14867, 22303, 33457, 50207, 75323, 112997, 169501,
			254257, 381389, 572087, 858149, 1287233, 1930879, 2896319, 4344479,
			6516739, 9775111, 14662727, 21994111, 32991187, 49486793, 74230231,
			111345347, 167018021, 250527047, 375790601, 563685907, 845528867,
			1268293309, 1902439967};
		const intpp growthMatrixLen = 49;
		intpp minNewArraySize = (intpp)(theCapacity / maxLoadFactor);
		for (intpp si = 0; si < growthMatrixLen; si++)
			if (growthMatrix[si] > minNewArraySize)
				return growthMatrix[si];

		return -1;	// An error value
	}
	
	/**
	 * \brief ensures there is enough room in the set to store theCapacity items
	 *
	 * This method will take the resize and growth factors into account when
	 * determining if the requested capacity is available.
	 *
	 * @param theCapacity the requested number of items to be stored in the set
	 */
	virtual void ensureCapacity(intpp theCapacity)
	{
		// Once our hash table has been loaded over the given capacity we resize
		const double maxLoadFactor = 0.66;	

		if ((nodeArraySize_ * maxLoadFactor) < theCapacity)
		{
			intpp newArraySize = hashArraySize(theCapacity, maxLoadFactor);
			if (newArraySize < 0)
				return;

			NodePtr* newArray = new NodePtr[newArraySize];
			memset(newArray, 0, newArraySize * sizeof(NodePtr));
			
			for (intpp i = 0; i < nodeArraySize_; i++)
			{
				Node* theNode = nodeArray_[i];
				while (theNode != nullptr)
				{
					nodeArray_[i] = theNode->next_;
					
					theNode->next_ = newArray[theNode->hashValue_ % newArraySize];
					newArray[theNode->hashValue_ % newArraySize] = theNode;
					
					theNode = nodeArray_[i];
				}
			}
			delete [] nodeArray_;
			nodeArray_ = newArray;
			nodeArraySize_ = newArraySize;
		}
	}



  public:
	/// finds the first node for a given item
	virtual Node* findItemNode(const T& theItem) const
	{
		NHashValue theHashValue = std::hash<T>()(theItem);
		Node* theNode = nodeArray_[theHashValue % nodeArraySize_];
		const bool match = (theNode != nullptr && std::equal_to<T>()(theNode->item_, theItem));
		return match ? theNode : findNextItemNode(theItem, theNode);
	}


	/// finds the next node for a given item and given node
	virtual Node* findNextItemNode(const T& theItem, Node* theStartNode) const
	{
		if (theStartNode)
			for (Node* theNode = theStartNode->next_; theNode != nullptr; theNode = theNode->next_)
				if (std::equal_to<T>()(theNode->item_, theItem))
					return theNode;
		return nullptr;
	}


  protected:

	intpp size_;				///< the number of items currently contained in the NSet
	intpp nodeArraySize_;		///< the current size of the node array
	NodePtr* nodeArray_;		///< an array to hold the set nodes
};
