#pragma once

#include <string>

using namespace std;

template<typename T> class darray
{
private:
	int increment_amount = 16;

	T* container_;
	unsigned int itemCount_;
	unsigned int containerLength_;

	void ExpandContainer(bool expand)
	{
		// update container length to new length (expand or shrink)
		expand ? containerLength_ += increment_amount : containerLength_ -= increment_amount;

		// create temporary container and fill with values (move memory from container_ to temporary)
		T* temporary = new T[containerLength_];
		memmove(temporary, container_, sizeof(T) * containerLength_);

		// delete the container_ memory block and asign temporary to the container_ pointer
		delete[] container_;
		container_ = temporary;
	}

	void ShiftContainer(int fromIndex, bool rightShift)
	{
		if (rightShift)
		{
			if (itemCount_ >= containerLength_)
				ExpandContainer(true);

			for (int i = itemCount_ - 1; i >= fromIndex; i--)
			{
				container_[i + 1] = container_[i];
			}
		}
		else
		{
			for (int i = fromIndex; i < itemCount_; i++)
			{
				container_[i - 1] = container_[i];
			}
		}
	}

public:
	darray<T>()
	{
		containerLength_ = increment_amount;
		container_ = new T[containerLength_];
		itemCount_ = 0;
	}

	bool Push(T item)
	{
		if (itemCount_ >= containerLength_)
		{
			ExpandContainer(true);
		}

		container_[itemCount_] = item;
		itemCount_++;

		return true;
	}
	bool PushFront(T item)
	{
		ShiftContainer(0, true);
		container_[0] = item;
		itemCount_++;

		return true;
	}
	bool PushTo(unsigned int index, T item)
	{
		if (index < itemCount_ && index >= 0) // validate index
		{
			ShiftContainer(index, true);
			container_[index] = item;
			itemCount_++; // must always increment!

			return true;
		}
		return false;
	}

	bool Remove()
	{
		if (itemCount_ > 0)
		{
			itemCount_--;

			if (itemCount_ + increment_amount < containerLength_)
			{
				ExpandContainer(false);
			}

			return true;
		}
		return false;
	}
	bool RemoveFront()
	{
		if (itemCount_ > 0)
		{
			ShiftContainer(1, false);
			itemCount_--;
			if (itemCount_ + increment_amount < containerLength_)
			{
				ExpandContainer(false);
			}

			return true;
		}
		return false;
	}
	bool RemoveAt(unsigned int index)
	{
		if (itemCount_ > 0)
		{
			if (index < itemCount_ && index >= 0)
			{
				ShiftContainer(index, false);
				itemCount_--;
				if (itemCount_ + increment_amount < containerLength_)
				{
					ExpandContainer(false);
				}
				return true;
			}
		}
		return false;
	}

	T* Get(unsigned int index)
	{
		if (index < itemCount_ && index >= 0)
		{
			return &container_[index];
		}
		else
		{
			return nullptr;
		}
	}

	T* Find(T item)
	{
		for (int i = 0; i < itemCount_; i++)
		{
			if (container_[i] == item)
			{
				return &container_[i];
			}
		}
		return nullptr;
	}

	unsigned int Length()
	{
		return itemCount_;
	}

	void Reverse()
	{
		T* temporary = new T[itemCount_]; // temporary container

		int j = 0;
		for (int i = itemCount_ - 1; i >= 0; i--) // reverse-put values into temporary
		{
			temporary[j] = container_[i];
			j++;
		}
		//move reversed values back to container_
		memmove(container_, temporary, sizeof(T) * containerLength_);

		delete[] temporary;
	}

	bool SetSizeIncrement(unsigned int value)
	{
		if (value > 0)
		{
			increment_amount = value;
			return true;
		}
		return false;
	}

	~darray()
	{
		delete[] container_;
	}
};
