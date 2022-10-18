// CSE687 Project - Phase 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Reduce.h"

int main()
{
	vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9 , 10};
	//vector<int>::iterator test_iterator;
	//test_iterator = vec.begin();
	
	//	for (test_iterator = vec.begin(); test_iterator < vec.end(); test_iterator++) {
//		cout << *test_iterator;
//	}
		
	Reduce testreduce;
	testreduce.reduce("abc", vec);
	//testreduce.reduce("abc", test_iterator);

}
