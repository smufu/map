//#define DEBUG true
#include <iostream>
#include "map.hpp"
using namespace std;

int main(int argc, char** argv) {

	map cow_map(10);
	
	/* Kolisions strategie */
	// 0 := Keine
	// 1 := linear
	// 2 := quadratisch
	// 3 := dopeltes hashing
	cow_map.set_collision(0);


	for(int i=0; i< 60; i++) {
		cow_map.add(i,i);
	}

	cout << cow_map.get(42) << endl;
	cow_map.print();


	return 0;
}
