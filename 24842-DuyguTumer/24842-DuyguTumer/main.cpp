#include "MPQ.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void mergeSort(vector<holdCoo> & left, vector<holdCoo> & right, vector<holdCoo> & coordinates)
{
	int sizeOfLeft = left.size();
    int sizeOfRight = right.size();
    int i = 0, j = 0, k = 0;

    while (j < sizeOfLeft && k < sizeOfRight) 
    {
        if (left[j].x_val < right[k].x_val) {
            coordinates[i] = left[j];
            j++;
        }
        else {
            coordinates[i] = right[k];
            k++;
        }
        i++;
    }
    while (j < sizeOfLeft) {
        coordinates[i] = left[j];
        j++; i++;
    }
    while (k < sizeOfRight) {
        coordinates[i] = right[k];
        k++; i++;
    }
}

void sort(vector<holdCoo> & coordinates)
{
	if (coordinates.size() <= 1) return;

    int mid = coordinates.size() / 2;

    vector<holdCoo> left(coordinates.begin(), coordinates.begin() + mid);
    vector<holdCoo> right(coordinates.begin() + mid, coordinates.end());

    sort(left);
    sort(right);
    mergeSort(left, right, coordinates);
}


int main()
{
	ifstream text;
	string fileName, line;
	int x_left, x_right, height, numberOfBuildings;

	fileName = "input.txt";

	text.open(fileName);	

	vector<building> buildings;
	vector<holdCoo> coordinates;
	//holdCoo zerothelement("non", 0, 0);
	//coordinates.push_back(zerothelement);
	getline(text, line);

	istringstream input_file(line);

	// first line will be the number of the buildings in this file
	input_file >> numberOfBuildings;

	MPQ<int> prioraty_queue = MPQ<int>(numberOfBuildings);
	
	int num = 1;
	while(getline(text, line))
	{
		// Point cityCoord;
		istringstream input_file(line); 
		
		// taking the values from the file
		input_file >> x_left;
		input_file >> height;
		input_file >> x_right;

		// create a new building element which name is build
		building build(x_left, height, x_right);
		// insert the build element into the buildings vector
		buildings.push_back(build);

		// create a new cooLeft & cooRight for the coordinates which I want to hold
		holdCoo cooLeft("left", x_left, num);
		holdCoo cooRight("right", x_right, num);
		coordinates.push_back(cooLeft);
		coordinates.push_back(cooRight);
		num++;
	}
	
	// After get the all x values then sort it
	sort(coordinates);

	bool printFirst = true; // for the first element 
	int generalGetMax; // since if the max heap is the same we don't have to print so this variable holds max
	
	for(unsigned int i = 0; i < coordinates.size(); i++)
	{
		if(coordinates[i].side == "left")
		{
			prioraty_queue.insert(buildings[coordinates[i].label-1].height, coordinates[i].label);
		}
		else if(coordinates[i].side == "right")
		{
			prioraty_queue.Remove(coordinates[i].label);
		}

		// TWO CASES: We look the first element and if is not 0 then you have to print 0 0 and then first element.
		//CASE-1:
		//////////////////////first one is not zero//////////////////////
		if(coordinates[0].x_val != 0 && printFirst)
		{
			cout << 0 << " " << 0 << endl;
			cout << coordinates[i].x_val << " " << prioraty_queue.GetMax() << endl;
			generalGetMax = prioraty_queue.GetMax();
			printFirst = false;
		}
		else if(coordinates[0].x_val != 0 && !printFirst && generalGetMax != prioraty_queue.GetMax() && coordinates[i].x_val != coordinates[(i+1)%coordinates.size()].x_val)
		{
			// coordinates[i].x_val != coordinates[(i+1)%coordinates.size()].x_val -> this case is for test4 and test5 
			cout << coordinates[i].x_val << " " << prioraty_queue.GetMax() << endl;
			generalGetMax = prioraty_queue.GetMax();
		}
		//CASE-2:
		//////////////////////first one is zero//////////////////////
		else if(coordinates[0].x_val == 0 && printFirst)
		{
			cout << coordinates[i].x_val << " " << prioraty_queue.GetMax() << endl;
			generalGetMax = prioraty_queue.GetMax();
			printFirst = false;
		}
		else if(coordinates[0].x_val == 0 && !printFirst && generalGetMax != prioraty_queue.GetMax() && coordinates[i].x_val != coordinates[(i+1)%coordinates.size()].x_val)
		{
			cout << coordinates[i].x_val << " " << prioraty_queue.GetMax() << endl;
			generalGetMax = prioraty_queue.GetMax();
		}
	}

	text.clear();
	text.seekg(0);
	text.close();	

	return 0;
}