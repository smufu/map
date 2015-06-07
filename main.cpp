//#define DEBUG true
#include <iostream>
#include <fstream>
#include <string>
#include "map.hpp"
#include <map>
using namespace std;

int main(int argc, char** argv) {

	try {

//	std::map<string,string> std_map;
	cow::map<int,int> cow_map(10);
	
	/* Kolisions strategie */
	cow_map.set_collision_strategy(cow_map.NONE);

/*
	cow_map["hallo"] = "foo";
	std_map["hallo"] = "foo";
	
	cow_map["welt"] = "bar";
	std_map["welt"] = "bar";

	cout << cow_map["hallo"] << cow_map["welt"] << endl;	
	cout << std_map["hallo"] << std_map["welt"] << endl;

*/

/*	cow_map["eins"] = "1";
	cow_map["zwei"] = "2";
	cow_map["drei"] = "3";
	cow_map["vier"] = "4";
*/
	for(int i=73; i< 80; i++) {
		cow_map[i] = i;
	}

	cow_map.dump();

	ofstream csv("exported.csv");
	cow_map.exportCSV(csv);

	//cout << cow_map[42] << endl;

	} catch(const char* msg) {
		cout << endl << "ERROR : " << msg << endl;
	}

	return 0;
}
