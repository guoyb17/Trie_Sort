#include <iostream>
#include <stdint.h>
#include <cstdlib>
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

void mk_word(unsigned int word, Bin**& ans) {
	if (ans == nullptr) ans = new Bin*[33];
	for (int i = 0; i < 32; i++) {
		ans[i] = new Bin;
		ans[i]->bin = ((word & (0x1 << (31 - i))) == 0) ? 0 : 1;
	}
	ans[32] = nullptr;
}

bool operator==(const Bin& a, const Bin& b) { return a.bin == b.bin; }

Trie<Bin, unsigned int> rtable(nullptr);

void insert(unsigned int ipa, unsigned int ipb, unsigned int ipc, unsigned int ipd) {
	unsigned int value = (ipa << 24) + (ipb << 16) + (ipc << 8) + ipd;
	cout << "Insert value: " << value << " -> ";
	Bin** tmp = nullptr;
	mk_word(value, tmp);
	for (int i = 0; i < 32; i++) cout << +tmp[i]->bin;
	cout << endl;
	rtable.insert(tmp, &value);
}

bool remove(unsigned int ipa, unsigned int ipb, unsigned int ipc, unsigned int ipd) {
	unsigned int value = (ipa << 24) + (ipb << 16) + (ipc << 8) + ipd;
	cout << "Remove value: " << value << " -> ";
	Bin** tmp = nullptr;
	mk_word(value, tmp);
	for (int i = 0; i < 32; i++) cout << +tmp[i]->bin;
	cout << endl;
	return rtable.remove(tmp, &value) >= 0;
}

void print() {
	system("cls");
	rtable.print(0);
}

int main() {
	// TODO: check methods
	insert(192, 168, 0, 1);
	insert(192, 168, 0, 2);
	insert(10, 0, 2, 1);

	print();
	return 0;
}