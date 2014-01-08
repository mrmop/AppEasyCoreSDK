// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see www.appeasymobile.com for licensing details and support
//
//

#if !defined(_CCZ_SLOT_ARRAY_H_)
#define _CCZ_SLOT_ARRAY_H_

#include "CzUtil.h"
#include "IzPlatformSys.h"

/**
 @addtogroup Core
 @{
 */

/**
 @class	CzSlotArray

 @brief	Basic resizable slot array
 
 A slot array differs from a normal array in that the array is not shrank and elements are not compacted when elements are removed. Instead array elements that are removed are marked 
 with NULL. With this, the Size of the array denotes the total number of items added to the array and not the number of valid elements. To get the number of valid elements call count()

 @tparam	T	Generic type parameter.
 */

template <class T> class CzSlotArray
{
	// Properties
protected:
	int		Growth;			///< Number of elements to grow array when we run out of space
public:
	void	setGrowth(int growth)	{ Growth = growth; }
	int		getSize() const			{ return Size; }
	// Properties End
protected:
	T*		Elements;		///< Array elements
	int		Size;			///< Max size of array
	int		LastFreedSlot;	///< Slot that was last freed

public:
	CzSlotArray()						///< Constructs an array with 4 elements and a groth of 4
	{
		LastFreedSlot = 0;
		Growth = 4;
		Size = 4;
		Elements = (T*)CZ_Malloc(sizeof(T) * Size);
		for (int t = 0; t < Size; t++)
			Elements[t] = NULL;
	}
	CzSlotArray(int size, int growth)	///< Constructs an array of the specified size and growth
	{
		if (size == 0) size = 1;
		LastFreedSlot = 0;
		Growth = growth;
		Size = size;
		Elements = (T*)CZ_Malloc(sizeof(T) * Size);
		for (int t = 0; t < Size; t++)
			Elements[t] = NULL;
	}
	
	virtual ~CzSlotArray()
	{
		if (Elements != NULL)
			CZ_Free(Elements);
	}

	int count() const		///< Gets the number of active elements, not the arrays size.
	{
		int count = 0;
		for (int t = 0; t < Size; t++)
		{
			if (Elements[t] != NULL)
				count++;
		}
		return count;
	}
	bool add(T element)		///< Finds a free slot and adds the element. If none free then the array size is grown.
	{
//		LastFreedSlot = -1;
		if (LastFreedSlot >= 0)
		{
			Elements[LastFreedSlot] = element;
			if (LastFreedSlot < Size - 1 && Elements[LastFreedSlot + 1] == NULL)	// Optimisation - If next slot is free then set LastFreedSlot as the next slot
				LastFreedSlot++;
			else
				LastFreedSlot = -1;
			return true;
		}
		// find a free slot
		int t;
		for (t = 0; t < Size; t++)
		{
			if (Elements[t] == NULL)
			{
				Elements[t] = element;
				if (t < Size - 1 && Elements[t + 1] == NULL)	// Optimisation - If next slot is free then set LastFreedSlot as the next slot
					LastFreedSlot = t + 1;
				return true;
			}
		}

		// If we are not allowed to grow then return error
		if (Growth == 0)
			return false;

		// No free slots so grow the array
		grow(Growth);
		Elements[t] = element;
		LastFreedSlot = t + 1;

		return true;
	}

	void remove(T element)	///< Removes the specified element from the array.
	{
		for (int t = 0; t < Size; t++)
		{
			if (Elements[t] == element)
			{
				Elements[t] = NULL;
				LastFreedSlot = t;
				break;
			}
		}
	}

	void remove(int index)	///< Removes the element at the specified index from the array.
	{
		if (index < 0 || index >= Size)
			return;
		Elements[index] = NULL;
		LastFreedSlot = index;
	}

	T element_at(int index) const	///< Returns the element at the specified index.
	{
		if (index < 0 || index >= Size)
			return NULL;
		return Elements[index];
	}

	bool set(int index, T element)	///< Sets the element at the specific index.
	{
		if (index < 0 || index >= Size)
			return false;
		Elements[index] = element;
		if (index == LastFreedSlot)
			LastFreedSlot = -1;

		return true;
	}

	void grow(int growth)	///< Grow the array
	{
		if (growth <= 0)
			return;
		T* elements = (T*)CZ_Realloc(Elements, sizeof(T) * (Size + growth));
		if (elements == NULL)
		{
			elements = (T*)CZ_Malloc(sizeof(T) * Size);
			for (int t = 0; t < Size; t++)
				elements[t] = Elements[t];
			CZ_Free(Elements);
			Elements = elements;
		}
		else
			Elements = elements;
		for (int t = 0; t < growth; t++)
			elements[t + Size] = NULL;
		Size += growth;
	}

	void growToSize(int new_size)	///< Grow the array to the new size
	{
		if (new_size <= 0 || new_size <= Size)
			return;
		grow(new_size - Size);
	}

	void resize(int size)	///< Change the size of the array, if new size is smaller than current size then it will be shrank
	{
		if (size <= 0 || size == Size)
			return;

		if (size > Size)
			grow(size - Size);
		else
		{
			T* elements = (T*)CZ_Realloc(Elements, sizeof(T) * size);
			if (elements == NULL)
			{
				elements = (T*)CZ_Malloc(sizeof(T) * size);
				for (int t = 0; t < size; t++)
					elements[t] = Elements[t];
				CZ_Free(Elements);
				Elements = elements;
			}
			else
				Elements = elements;

			Size = size;
			if (LastFreedSlot >= size)
				LastFreedSlot = -1;
		}
	}

	int getLastUsedSlot() const	///< Returns the last used array index
	{
		for (int t = Size - 1; t >= 0; t--)
		{
			if (Elements[t] != NULL)
				return t;
		}

		return Size - 1;
	}

	void clear(bool delete_elements = false)	///< Clear the array, optionally deleting contents
	{
		for (int t = 0; t < Size; t++)
		{
			if (delete_elements)
			{
				if (Elements[t] != NULL)
					delete Elements[t];
			}
			Elements[t] = NULL;
		}
		LastFreedSlot = 0;
	}

	void compact()	///< Shrink the array to its smallest possible size
	{
		int last_free = 0;
		for (int t = 0; t < Size; t++)
		{
			if (Elements[t] != NULL)
			{
				for (int t2 = last_free; t2 < Size; t2++)
				{
					if (Elements[t2] == NULL)
					{
						last_free = t2;
						Elements[last_free] = Elements[t];
						Elements[t] = NULL;
					}
				}
			}
		}
	}
};

/// @}

#endif	// _CCZ_SLOT_ARRAY_H_
