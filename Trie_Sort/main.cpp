#include <iostream>
#include <stdint.h>
#include "trie.h"

using namespace std;

struct Bin {
	uint8_t bin;

	static const int size = 2;

	int seq() {
		if (bin == 0) return 0;
		else if (bin == 1) return 1;
		else throw "INVALID_BIN";
	}
};

bool operator==(const Bin& a, const Bin& b) { return a.bin == b.bin; }

typedef struct {
	uint32_t addr;
	uint32_t len;
	uint32_t if_index;
	uint32_t nexthop;
} RoutingTableEntry;

int main() {
	Trie<Bin, RoutingTableEntry>* root = new Trie<Bin, RoutingTableEntry>(nullptr);

	delete root;
	return 0;
}