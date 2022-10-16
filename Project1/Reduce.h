#ifndef Reduce_h
#define Reduce_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;

class Reduce {
private:
	string key;
	int sum;
	vector<int>::iterator it;
	vector<int> vec;

public:

	// constructor initializer, takes key and iterator
	// Reduce(string key, vector<int>::iterator it);
	
	// takes a key and iterator of ints - sums iterators and calls export function
	// ========== reduce w/ ITERATOR ==========
	void reduce(string key, vector<int>::iterator it);
	// ========== reduce w/ VECTOR ==========
	void reduce(string key, vector<int> vec);



	// writes result to output directoy - creates empty txt file names SUCCESS upon success
	void output(string key, int sum);


	// getter/setter for string key
	string getkey();
	void setkey(string k);

	// getter/setter for int sum
	int getsum();
	void setsum(int s);
};

#endif // Reduce_h