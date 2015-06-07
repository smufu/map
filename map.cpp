#include "map.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

using namespace std;

vector<unsigned int> lade_prim(unsigned int max) {
	ifstream p("primes.txt");
	vector<unsigned int> v;
	unsigned int ui;
	while(!p.eof()&&v.size()<max) {
		p >> ui;
		v.push_back(ui);
	}
	return v;	
}


map::map(unsigned int size, unsigned int chunk) {
	this->chunk = chunk;
	d = vector<list<pair> >(size);
}

void map::set_collision(int cs) {
	this->cs = cs;
}

unsigned int map::hashValue(int k, int index) {
	unsigned int s = (d.size()-1);
	switch(cs) {
		case 0:
			return k%s;
		break;
		case 1:
			return (k%s +index)%s;
		break;
		case 2:
			return (k%s +index*index)%s;
		break;
		case 3:
			return (k%s+ lade_prim()[index])%s;
			break;
		default:
			return 0;
	}
}

int map::get(int k) {
	unsigned int t = 0;
	unsigned int i = hashValue(k, 0);
	while(true) {
		if(!d[i].empty()) {
			for(const pair& p : d[i])
				if(p.k == k)
					return p.v;
			i = hashValue(k,++t);
		} else
			break;
	}
	return -1;
}
void map::add(int k, int v) {
	unsigned int t = 0;
	unsigned int i = hashValue(k, 0);
	while(cs != 0) {
		if(!d[i].empty()) {
			for(const pair& p : d[i])
				if(p.k == k)
					break;
			i = hashValue(k,++t);
		} else
			break;
	}
	d[i].push_back(pair(k,v));
	if((double)++items / d.size() > 0.6) {
		map im(d.size()+chunk);
		im.cs = cs;
		for(const auto& it : d) {
			if(!it.empty())
				for(const pair& p : it)
					im.add(p.k, p.v);
		}
		d = im.d;
		std::cout << "rehashing" << std::endl;
	}
}

void map::print() {
	for(unsigned int i=0; i < d.size(); ++i) {
		if(!d[i].empty()) {
			std::cout << i << " : " << std::endl;
			for(const pair& p : d[i]) {
				std::cout << "\t\t" << p.k << " <=> " << p.v << std::endl;
				}
		}
	}
}