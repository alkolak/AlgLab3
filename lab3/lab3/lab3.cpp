#include <iostream>
#include <stdexcept>

using namespace std;

class Iterator
{
public:
	virtual int next() = 0;
	virtual bool has_next() = 0;
	virtual size_t get_cur_index() = 0;
};

class BiniaryHeap
{
	int* Heap;
	size_t Size;
	size_t Capacity;

	Iterator* bftIterator;
	Iterator* dftIterator;

	void resize()
	{
		int* NewHeap = new int[Capacity + 10];
		for (int i = 0;i < Capacity;i++)
			NewHeap[i] = Heap[i];
		Capacity += 10;
		delete[] Heap;
		Heap = NewHeap;
	}

	void move_higher(size_t Elem_index)
	{
		if (Elem_index != 0)
		{
			int temp;
			size_t parent_index = (Elem_index - 1) / 2;
			if (Heap[Elem_index] > Heap[parent_index])
			{
				temp = Heap[Elem_index];
				Heap[Elem_index] = Heap[parent_index];
				Heap[parent_index] = temp;
				move_higher(parent_index);
			}
		}
	}

	void move_lower(size_t Elem_index)
	{
		size_t child1 = (Elem_index * 2) + 1;
		size_t child2 = (Elem_index * 2) + 2;
		int temp;
		if ((child1 < Size) && (child2 < Size))
		{
			size_t child_max;
			if (Heap[child2] > Heap[child1])
				child_max = child2;
			else
				child_max = child1;
			if (Heap[child_max] > Heap[Elem_index])
			{
				temp = Heap[child_max];
				Heap[child_max] = Heap[Elem_index];
				Heap[Elem_index] = temp;
				move_lower(child_max);
			}
		}
		else if (child1 < Size)
		{
			if (Heap[child1] > Heap[Elem_index])
			{
				temp = Heap[child1];
				Heap[child1] = Heap[Elem_index];
				Heap[Elem_index] = temp;
			}
		}
	}

public:

	BiniaryHeap(int Head)
	{
		Heap = new int[10];
		Capacity = 10;
		Size = 1;
		Heap[0] = Head;
		bftIterator = nullptr;
		dftIterator = nullptr;
	}

	void remove_iterators()
	{
		if (bftIterator != nullptr)
		{
			delete bftIterator;
			bftIterator = nullptr;
		}
		if (dftIterator != nullptr)
		{
			delete dftIterator;
			dftIterator = nullptr;
		}
	}

	void insert(int Element)
	{
		if (bftIterator != nullptr || dftIterator != nullptr)
			throw runtime_error("Iterators must be removed\n");

		if (Size == Capacity)
			resize();
		Heap[Size] = Element;
		move_higher(Size);
		Size += 1;
	}

	class bft_Iterator : public Iterator
	{
		size_t current;
		BiniaryHeap* BHeap;

	public:

		bft_Iterator(size_t Start, BiniaryHeap* heap)
		{
			current = Start;
			BHeap = heap;
		}

		size_t get_cur_index() { return current; }

		int next() override
		{
			if (!has_next())
				throw out_of_range("No more elements\n");
			current += 1;
			return BHeap->Heap[current - 1];
		}

		bool has_next() override { return (current != (BHeap->Size)); }
	};

	Iterator* create_bft_iterator()
	{
		if (bftIterator != nullptr)
			delete bftIterator;
		bftIterator = new bft_Iterator(0, this);
		return bftIterator;
	}

	class dft_Iterator : public Iterator
	{
		size_t current;
		BiniaryHeap* BHeap;
		bool* visited;

	public:

		dft_Iterator(size_t Start, BiniaryHeap* heap)
		{
			current = Start;
			BHeap = heap;
			visited = new bool[BHeap->Size];
			for (int i = 0;i < BHeap->Size;i++)
			{
				visited[i] = 0;
			}
		}

		size_t get_cur_index() { return current; }

		int next() override
		{
			if (!has_next())
				throw out_of_range("No more elements\n");
			size_t prevcur = current;
			visited[prevcur] = 1;
			if (!has_next())
				return BHeap->Heap[prevcur];
			size_t child1, child2;
			while (1)
			{
				child1 = current * 2 + 1;
				child2 = current * 2 + 2;
				if (child1 < BHeap->Size)
				{
					if (!visited[child1])
					{
						current = child1;
						break;
					}
				}
				if (child2 < BHeap->Size)
				{
					if (!visited[child2])
					{
						current = child2;
						break;
					}
				}
				current = (current - 1) / 2;
			}
			return BHeap->Heap[prevcur];
		}

		bool has_next() override
		{
			if (BHeap->Size == 0)
				return false;
			size_t nextcur = current;
			size_t child1, child2;
			while (1)
			{
				child1 = (nextcur * 2) + 1;
				child2 = (nextcur * 2) + 2;
				if (child1 < BHeap->Size)
				{
					if (!visited[child1])
						return true;
				}
				if (child2 < BHeap->Size)
				{
					if (!visited[child2])
						return true;
				}
				if (nextcur != 0)
					nextcur = (nextcur - 1) / 2;
				else if (!visited[nextcur])
					return true;
				else
					return false;
			}
			
		}

		~dft_Iterator()
		{
			delete[] visited;
		}
	};

	Iterator* create_dft_iterator()
	{
		if (dftIterator != nullptr)
			delete dftIterator;
		dftIterator = new dft_Iterator(0, this);
		return dftIterator;
	}

	bool contains(int Element)
	{
		if (bftIterator != nullptr || dftIterator != nullptr)
			throw runtime_error("Iterators must be removed\n");

		Iterator* I = create_dft_iterator();
		while (I->has_next())
		{
			if (I->next() == Element)
			{
				remove_iterators();
				return true;
			}
		}
		remove_iterators();
		return false;
	}

	void remove(int Element)
	{
		if (bftIterator != nullptr || dftIterator != nullptr)
			throw runtime_error("Iterators must be removed\n");
		if (!contains(Element))
			throw invalid_argument("Heap isn't contain element\n");

		size_t indx;
		Iterator* I = create_bft_iterator();
		while (I->has_next())
		{
			if (I->next() == Element)
			{
				indx = I->get_cur_index() - 1;
				break;
			}
		}
		if (indx != (Size - 1))
		{
			Heap[indx] = Heap[Size - 1];
			Size -= 1;
			int temp = Heap[indx];
			move_higher(indx);
			if (temp == Heap[indx])
				move_lower(indx);
		}
		else
			Size -= 1;
		remove_iterators();
	}

	~BiniaryHeap()
	{
		remove_iterators();
		delete[] Heap;
	}
};

int main()
{
	BiniaryHeap B(5);
	Iterator* I;

	B.insert(3);
	B.insert(4);
	B.insert(2);
	B.insert(1);
	B.insert(4);

	I = B.create_bft_iterator();
	cout << "BFT: ";
	while (I->has_next())
		cout << I->next();
	cout << endl;

	I = B.create_dft_iterator();
	cout << "DFT: ";
	while (I->has_next())
		cout << I->next();
	cout << endl;

	try
	{
		B.remove(1);
	}
	catch (exception& exception)
	{
		cout << exception.what();
	}

	B.remove_iterators();

	B.remove(1);
	B.remove(5);

	I = B.create_bft_iterator();
	cout << "BFT (1 and 5 removed): ";
	while (I->has_next())
		cout << I->next();
	cout << endl;
}
