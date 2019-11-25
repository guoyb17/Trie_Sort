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

typedef struct {
	uint32_t addr;
	uint32_t len;
	uint32_t if_index;
	uint32_t nexthop;
} RoutingTableEntry;

bool operator==(const RoutingTableEntry& a, const RoutingTableEntry& b) {
	if (a.addr != b.addr) return false;
	if (a.if_index != b.if_index) return false;
	if (a.len != b.len) return false;
	if (a.nexthop != b.nexthop) return false;
	return true;
}
ostream& operator<<(ostream& out, const RoutingTableEntry& opt) {
	out << "[RoutingTableEntry] addr = 0x" << hex << opt.addr
		<< ", len = " << dec << opt.len
		<< ", if_index = " << opt.if_index
		<< ", nexthop = 0x" << hex << opt.nexthop << '.';
	return out;
}

void mk_word(RoutingTableEntry& info, Bin**& ans) { // ans contains both addr and len
	if (ans == nullptr) ans = new Bin*[info.len + 1];
	uint8_t ip[4];
	ip[0] = info.addr & 0xff;
	ip[1] = (info.addr & 0xff00) >> 8;
	ip[2] = (info.addr & 0xff0000) >> 16;
	ip[3] = (info.addr & 0xff000000) >> 24;

	for (uint32_t j = 0; j < info.len; j++) {
		ans[j] = new Bin;
		ans[j]->bin = ((ip[j / 8] & (0x1 << (7 - j % 8))) == 0) ? 0 : 1;
	}
	ans[info.len] = nullptr;
}

void mk_word(uint32_t& info, Bin**& ans) {
	if (ans == nullptr) ans = new Bin*[32 + 1];
	uint8_t ip[4];
	ip[0] = info & 0xff;
	ip[1] = (info & 0xff00) >> 8;
	ip[2] = (info & 0xff0000) >> 16;
	ip[3] = (info & 0xff000000) >> 24;
	for (uint32_t j = 0; j < 32; j++) {
		ans[j] = new Bin;
		ans[j]->bin = ((ip[j / 8] & (0x1 << (7 - j % 8))) == 0) ? 0 : 1;
	}
	ans[32] = nullptr;
}

bool operator==(const Bin& a, const Bin& b) { return a.bin == b.bin; }

Trie<Bin, RoutingTableEntry> rtable(nullptr);

void print() {
	system("cls");
	rtable.print(0);
}

void lookup_pre(uint32_t addr) {
	cout << "Lookup " << hex << addr << ':' << endl;
	vector<const RoutingTableEntry*> ans;
	Bin** tmp_ipt = nullptr;
	mk_word(addr, tmp_ipt);
	rtable.lookup_pre(tmp_ipt, ans);
	if (!ans.empty()) {
		cout << "Ans:" << endl;
		for (const RoutingTableEntry* i : ans) {
			cout << *i << endl;
		}
		const RoutingTableEntry value = *ans[ans.size() - 1];
		cout << "Select: nexthop = " << hex << value.nexthop << ", if_index = " << dec << value.if_index << endl;
	}
	else cout << "Ans: <empty>" << endl;
}

int main() {
	// TODO: check methods
	RoutingTableEntry i1, i2;
	i1.addr = 0x00030201; // 0x00030201, 24, 9, 0x0203a8c0
	i1.len = 24;
	i1.if_index = 9;
	i1.nexthop = 0x0203a8c0;
	i2.addr = 0x04030201; // 0x04030201, 32, 10, 0x0109a8c0
	i2.len = 32;
	i2.if_index = 10;
	i2.nexthop = 0x0109a8c0;
	//i3.addr = 0x04040808;
	//i3.len = 32;
	//i3.if_index = 2;
	//i3.nexthop = 0x0200007f;

	Bin** ipt1 = nullptr;
	mk_word(i1, ipt1);
	//lookup_pre(i1.addr);
	rtable.insert(ipt1, &i1);
	//lookup_pre(i1.addr);
	Bin** ipt2 = nullptr;
	mk_word(i2, ipt2);
	rtable.insert(ipt2, &i2);
	//lookup_pre(i1.addr);
	//lookup_pre(i2.addr);

	rtable.remove(ipt2);
	rtable.remove(ipt1);

	print();

	//Bin** ipt3 = nullptr;
	//mk_word(i3, ipt3);
	//rtable.insert(ipt3, &i3);
	//print();

	return 0;
}