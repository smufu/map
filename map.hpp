#ifndef COW_MAP_HPP
#define COW_MAP_HPP
#include <vector>
#include <list>
#include <iostream>

using namespace std;

vector<unsigned int> lade_prim(unsigned int max=150);

class map {
private:
	class pair {
	public:
		int k;
		int v;
		pair(const int& key, const int& val) {k=key;v=val;}
	};

	vector<list<pair> > d;
	unsigned int chunk;
	int cs = 0;
	unsigned int items = 0;

	unsigned int hashValue(int k, int index);
public:

	map(unsigned int size=13, unsigned int chunk=25);
	~map() {}

	void set_collision(int cs);
	int get(int k);
	void add(int k, int v);
	void print();

};


#endif
