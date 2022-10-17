#include "Reduce.h"
#include "FileManager.h"

#include <sstream>

 //========== CONSTRUCTOR INITIALIZER ==========


 //========== reduce w/ ITERATOR ========== 
void Reduce::reduce(string key, vector<int>::iterator it) {
	for (int x = 0; x < 10; x++) {
		cout << *it << "\n";
		it++;
	}
}


// ========== reduce w/ VECTOR ==========
void Reduce::reduce(string key, vector<int> vec) {
	sum = 0;

	// loop iterates and sums through vector
	for (it = vec.begin(); it < vec.end(); it++) {
		sum = sum + *it;
		//cout << "\n" << "it = " << key << " || sum = " << sum << "\n";
	}

	// calls output method
	output(key, sum);
}

void Reduce::output(string key, int sum) {

	// writes results in format "key, sum" to directory

	std::ostringstream formattedKvpElement;
	formattedKvpElement << "\n" << key << ", " << sum << endl;

	fileManager.test_output(formattedKvpElement.str());

	//ofstream writeresult;
	//writeresult.open("test.txt");
	//writeresult << "\n" << key << ", " << sum << endl;
	//writeresult.close();

	// creates SUCCESS.txt file
	//ofstream successfile("SUCCESS.txt");
}



// getter/setter for string key
string Reduce::getkey() {
	return key;
}
void Reduce::setkey(string k) {
	key = k;
}


// getter/setter for int sum
int Reduce::getsum() {
	return sum;
}
void Reduce::setsum(int s) {
	sum = s;
}