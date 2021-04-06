#ifndef _HEADER_H
#define _HEADER_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct holdCoo
{
	string side; //which side "left" or "right"
	int label; // 2 or 5 for example
	int x_val;
	holdCoo(string _x, int _y, int _z) 
    { 
        side = _x; 
        x_val = _y; 
		label = _z;
    }
};

struct building
{
	int x_left;
	int height;
	int x_right;

	building(int _x, int _y, int _z) 
    { 
        x_left = _x; 
        height = _y; 
		x_right = _z;
    } 
};


template <class Comparable>
class MPQ
{
public:
	MPQ(int numberOfBuildings); //constructor
	//~MPQ(); //destructor

    bool isEmpty() const; 

	void followTheLocations(int oldV, int newV); // swaps the vector of the locations when heap changes
	void insert(Comparable value, int label); // percolate up in also in this function
	Comparable Remove(int label); // This method removes the value with this label and returns it. 
	Comparable GetMax();

private:
	struct heapElement
	{
		Comparable value; // height
		int label; // which building
		
		//template <class Comparable>
		heapElement::heapElement(Comparable v, int l)
		{
			value = v;
			label = l;
		}
	};

    vector<heapElement> Heap; 
	vector<int> Locations; 
	void percolateDown(int idx);
};

////////////constructor////////////
template <class Comparable>
MPQ<Comparable>::MPQ(int numberOfBuildings)
{
	heapElement zerothElement(Comparable(), 0);
	Heap.push_back(zerothElement);
	Locations.push_back(0);
	Locations.resize(numberOfBuildings + 1); // create a vector with the given num of buildings with all zeros
	
}

////////////followTheLocations////////////
template <class Comparable>
void MPQ<Comparable>::followTheLocations(int oldV, int newV)
{
	Locations[Heap[oldV].label] = newV;
	Locations[Heap[newV].label] = oldV;
}
////////////insert////////////
template <class Comparable>
void MPQ<Comparable>::insert(const Comparable value, int label)
{
	int hole = Heap.size(); // it will be at the end
	heapElement element(0, 0);
	Heap.push_back(element); // but since I have to resize my vector with one more element 
	for( ; hole > 1 && value > Heap[hole/2].value; hole/= 2 )
	{
		Heap[hole] = Heap[hole / 2];
		followTheLocations(hole, hole/2);
	}
	Heap[hole].value = value;
	Heap[hole].label = label;
	Locations[Heap[hole].label] = hole;

}

////////////percolateDown////////////
template <class Comparable>
void MPQ<Comparable>::percolateDown(int idx)
{
	int Lchild; // left child (2i)
	int Rchild; // right child (2i + 1)
	int child; // general child (which one???) -> holds the child value
	Comparable tmpVal = Heap[idx].value;
	int tmpLabel = Heap[idx].label;

    for( ; idx < Heap.size()/2; idx = child )
    {
        Lchild = idx * 2;
		Rchild = idx * 2 + 1;
		child = Lchild;
        if(Heap[Rchild].value > Heap[Lchild].value)
            child = Rchild;
        if(Heap[child].value > tmpVal)
		{
			Heap[idx] = Heap[child];
			
		}
        else
            break;
		followTheLocations(idx, child);
	}
	
	Heap[idx].value = tmpVal;
	Heap[idx].label = tmpLabel;
	Locations[Heap[idx].label] = idx;
}

////////////isEmpty////////////
template <class Comparable>
bool MPQ<Comparable>::isEmpty() const
{
	if(Heap.size() <= 1)
	{
		return true;
	}
	return false;
}

////////////Remove////////////
template <class Comparable>
Comparable MPQ<Comparable>::Remove(int label)
{
	if(!isEmpty())
	{
		Comparable exVal = Heap[Locations[label]].value; // holds the removable value since we have to return it at the end

		// If you don't understand, you are welcome! Just draw it and follow all the steps 
		Heap[Locations[label]] = Heap[Heap.size()-1]; // find the index of the value which has this given label in it and change this index with the last index
		int prevIdx = Locations[label];
		followTheLocations(Locations[label], Heap.size()-1); // put it to follow up
		
		Locations[Heap[Heap.size()-1].label] = 0; // let's again make all them zero when we delete it
		Heap.pop_back();
		if(prevIdx != Heap.size()) // this means that if you delete the last element you do not have to look percolateDown
			percolateDown(prevIdx);
		return exVal;
	}
	return 0; // give the first index
}

////////////GetMax////////////
template <class Comparable>
Comparable MPQ<Comparable>::GetMax()
{
	if(!isEmpty())
	{
		return Heap[1].value;
	}
	return 0;
}
#endif