#ifndef COW_MAP_HPP
#define COW_MAP_HPP
#ifndef DEBUG
#define DEBUG false
#endif
#define DPRINT(msg) if(DEBUG) std::cout << "\033[43m\033[30m" << msg << "\033[0m" << std::endl;
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#define COW_MAP_HPP
#include <string>

using std::vector;
using std::list;
using std::iostream;
using std::string;

namespace cow {

	unsigned int find_prime(bool (*validate)(unsigned int)) {
		std::ifstream p("primes.txt");
		unsigned int v;
		unsigned int last;
		while(!p.eof()) {
			p >> v;
			if(!validate(v))
				break;
			last = v;
		}
		return last;
	}
	std::vector<unsigned int> lade_prim(unsigned int max=150) {
		std::ifstream p("primes.txt");
		std::vector<unsigned int> v;
		unsigned int ui;
		while(!p.eof()&&v.size()<max) {
			p >> ui;
			v.push_back(ui);
		}
		return v;	
	}

	template<typename KEY, typename VALUE>
	class map;
	template<typename KEY, typename VALUE>
	class proxy {
	private:
		map<KEY,VALUE>* p;
		KEY k;
	public:
		proxy(const KEY& key, map<KEY,VALUE>& parent) {
			k = key;
			p = &parent;
		}
		operator VALUE() const {
			return p->get(k);
		}
		proxy& operator=(const VALUE& c) {
			p->set(k,c);
			return *this;
		}
		void remove() {
			p->remove(k);
		}
	};

	template<typename KEY, typename VALUE>
	std::ostream& operator<< (std::ostream& o, const proxy<KEY, VALUE>& p) {
		o << (VALUE)p;
		return o;
	}

	template<typename KEY=string, typename VALUE=void*>
	class map {
	public:
		enum collision_strategy { NONE, LINEAR, SQUARE, DOUBLE };
	private:
		class pair {
		public:
			KEY k;
			VALUE v;
			pair(const KEY& key, const VALUE& val) {k=key;v=val;}
		};

		vector<list<pair> > d;
		unsigned int chunk;
		unsigned int cprime;
		VALUE v;
		collision_strategy cs = NONE;
		unsigned int items = 0;

		static long int hash(const string& s) {
			long int h=0;
			for(char c:s) {
				h <<= 3;
				h  += c;
			}
			h <<= 3;
			h  += s.length();
			return h;
		}
		static long int hash(int s) { return s; }
		static long int hash(unsigned int s) { return s; }
		static long int hash(const char* s) { return hash(string(s)); }
		
		template <typename T>
		static long int hash(T s) { return hash(std::to_string(s)); }

		unsigned int translated_hash(const KEY& k, unsigned int i=0) const {
			unsigned int s = (d.size()-1);
			switch(cs) {
				case NONE:
					if(!cprime)
						return hash(k)%s;
					return hash(k)%cprime;
				break;
				case LINEAR:
					if(!cprime)
						return (hash(k)%s +i)%s;
					return (hash(k)%cprime +i)%s;
				break;
				case SQUARE:
					if(!cprime)
						return (hash(k)%s +i*i)%s;
					return (hash(k)%cprime +i*i)%s;
				break;
				case DOUBLE:
					if(!cprime)
						return (hash(k)%s+ lade_prim()[i])%s;
					return (hash(k)%cprime+ lade_prim()[i])%s;
				break;
				default:
					throw "logic error, unkown enum for hash collision_strategy";
			}
		}
	public:

		map(unsigned int size=13, unsigned int chunk=25) {
			cprime=0;
			this->chunk = chunk;
			d = vector<list<pair> >(size);
			/*for(unsigned int i=0;i<d.size();i++)
				d[i] = list<pair>();//*/
		}
		map(const map<KEY,VALUE>& m, unsigned int size=0) {
			cprime=0;
			if(size<=0) size = m.d.size();
			this->chunk = m.chunk;
			d = vector<list<pair> >(size);
			cs = m.cs;
			for(const auto& it : m.d) {
				if(!it.empty())
					for(const pair& p : it)
						set(p.k, p.v);
			}
			/*for(unsigned int i=0;i<d.size();i++)
				d[i] = list<pair>();//*/
		}
		~map() {}

		void set_collision_strategy(collision_strategy cs) {
			this->cs = cs;
		}

		const VALUE& get(const KEY& k) const {
			//DPRINT("get")
			
			unsigned int t = 0;
			unsigned int i = translated_hash(k);
			while(true) {
				if(!d[i].empty()) {
					for(const pair& p : d[i])
						if(p.k == k)
							return p.v;
					i = translated_hash(k,++t);
				} else
					break;
				if(t>d.size()) {
					DPRINT("stoping anti colision")
					break;
				}
			}
			throw "can't find item";
		}
		void set(const KEY& k, const VALUE& v) {
			//DPRINT("set")

			unsigned int t = 0;
			unsigned int i = translated_hash(k);
			while(cs != NONE) {
				if(!d[i].empty()) {
					for(const pair& p : d[i])
						if(p.k == k)
							break;
					i = translated_hash(k,++t);
				} else
					break;
				/*if(t>d.size()) {
					DPRINT("stoping anti colision")
					break;
				}*/
			}
			d[i].push_back(pair(k,v));
			if((double)++items / d.size() > 0.6) {
				if(DEBUG) {
					std::cout << "\033[36m";
					dump();
					std::cout << "\0m";
				}
				map<KEY,VALUE> im(*this, d.size()+chunk);
				d = im.d;
				DPRINT("rehashing");
			}
		}
		void remove(const KEY& k) {
			DPRINT("remove")
			d[translated_hash(k)].remove_if([k](pair p){return p.k==k;});
		}

		proxy<KEY,VALUE> operator[](const KEY& k) {
			return proxy<KEY,VALUE>(k,*this);
		}
		void dump(std::ostream& o=std::cout) const {
			for(unsigned int i=0; i < d.size(); ++i) {
				if(!d[i].empty()) {
					o << i << " :" << std::endl;
					for(const pair& p : d[i]) {
						o << "\t[" << p.k << "] ";
						if(DEBUG)
							o << '{' << hash(p.k) << '}';
						o << " : " << p.v << std::endl;
 					}
				}
			}
		}
		void exportCSV(std::ostream& o=std::cout) const {
			for(unsigned int i=0; i < d.size(); ++i) {
				o << i;
				if(!d[i].empty())
					o << ";\t";
				for(const pair& p : d[i]) {
					o << p.k << ";\t";
					if(DEBUG)
						o << hash(p.k) << ";\t";
					o  << p.v;
				}
				o << std::endl;	
			}
		}
	};
}

#endif
