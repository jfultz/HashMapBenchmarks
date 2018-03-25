#pragma once

#include <functional>
#include <cstdint>
#include <iostream>
#include <fstream>

typedef int64_t 			intpp;
typedef intpp				NHashValue;		// Our hashing needs to consider how many bits in our word size.

template<typename T>
class NSetLP
{
public:

	/// a node of a set
	class Node
	{
	public:
		T key_;					///< the item
		NHashValue hashValue_;		///< the hash value of the item (so we don't have to keep recomputing it)
	};


	
	NSetLP(intpp theCapacity) : 	size_(0), nodeArraySize_(0), nodeArray_(nullptr)
	{
		if (theCapacity > 0)
			ensureCapacity(theCapacity); }

	virtual ~NSetLP()
	{
		delete [] nodeArray_;
	}

	

	virtual void addItem(const T& theItem)
	{
		NHashValue theHashValue = std::hash<T>()(theItem);
		if (findItem(theItem, theHashValue))
			return;
			
		ensureCapacity(++size_);
		

		// A simple forward probe
		size_t position = theHashValue % nodeArraySize_;
		while (nodeArray_[position].hashValue_ != 0)
		{
			if (++position == nodeArraySize_)
				position = 0;
		}
		nodeArray_[position].hashValue_ = theHashValue;
		nodeArray_[position].key_ = theItem;
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
		const double maxLoadFactor = 0.3;	

		if ((nodeArraySize_ * maxLoadFactor) < theCapacity)
		{
			intpp newArraySize = hashArraySize(theCapacity, maxLoadFactor);
			if (newArraySize < 0)
				return;

			Node* newArray = (Node*) std::calloc(newArraySize, sizeof(Node));
			
			// if (nodeArraySize_ < 100000)
			// {
			// 	std::fstream outfile(std::to_string(size_) + ".txt", std::ios_base::out | std::ios_base::trunc);
			// 	for (intpp i = 0; i < nodeArraySize_; i++)
			// 		if (nodeArray_[i].hashValue_ == 0 && nodeArray_[i].key_ == nullptr)
			// 			outfile << '.';
			// 		else if (nodeArray_[i].key_ == nullptr)
			// 			outfile << 'e';
			// 		else
			// 			outfile << '*';
			// }
			for (intpp i = 0; i < nodeArraySize_; i++)
			{
				size_t position = nodeArray_[i].hashValue_ % newArraySize;
				while (newArray[position].hashValue_ != 0)
					position = (position + 1) % newArraySize;
				newArray[position] = nodeArray_[i];
			}
			free(nodeArray_);
			nodeArray_ = newArray;
			nodeArraySize_ = newArraySize;
		}
	}



  public:
	/// finds the first node for a given item
	inline const T findItem(const T& theItem) const
	{
		NHashValue theHashValue = std::hash<T>()(theItem);
		return findItem(theItem, theHashValue);
	}

	inline const T findItem(const T& theItem, NHashValue theHashValue) const
	{
		size_t position = theHashValue % nodeArraySize_;
		while (true) {
			if (nodeArray_[position].hashValue_ == 0)
				return nullptr;
			if (nodeArray_[position].hashValue_ == theHashValue && std::equal_to<T>()(nodeArray_[position].key_, theItem))
				return nodeArray_[position].key_;
			if (++position == nodeArraySize_)
				position = 0;
		}
	}



  protected:

	intpp size_;				///< the number of items currently contained in the NSetLP
	intpp nodeArraySize_;		///< the current size of the node array
	Node* nodeArray_;		///< an array to hold the set nodes
};
