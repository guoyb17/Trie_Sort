/**
* Guo Yuanbo, Tsinghua Univ., All rights reserved.
* @file trie.h
* @brief Trie tree template
* @author Guo Yuanbo
* @email guoyb17@mails.tsinghua.edu.cn
* @version 0.1
* @date 2019-11-24
* @update 2019-11-24
* @note None
* @warning the tree does NOT maintain Info (see below)
* @todo None
* @history None
*/

#pragma once

// Trie-Node class template
// Char: dict keyword (e.g. a-zA-Z0-9, 0/1, etc.)
// Info: node info, may be use to store the entire word or other things (e.g. route info)
//     Char needs:
//         method int seq()   - to get sequence of this Char
//         static member size - to get number of kinds of Char
//         operator==         - to compare Char sequence
// WARN: the tree does NOT maintain Info!

template <class Char, class Info>
class Trie {

	Info* info; // also marks that there is a word ending up here
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie& b) = delete;
	Trie* next;

public:

	inline static const int size() { return Char::size; }

	Trie(Info* i = nullptr) { info = i; }

	bool empty() { return info == nullptr && next == nullptr; }

	void insert(Char* word, Info* iinfo) { // word needs to end up with nullptr as a sign
		if (word == nullptr) {
			info = iinfo; // will overwrite previous info
			return;
		}
		if (next == nullptr) next = new Trie[size()];
		next[word[0].seq()].insert(&word[1], iinfo);
	}

	// @return
	// 0 - matched and deleted
	// 1 - this node is clear
	// -1 - not matched due to non-null rinfo with null info
	// -2 - not matched due to unequal rinfo and info
	// -3 - ERROR not matched due to no children
	// -4 - recursively not matched
	int remove(Char* word, Info* rinfo = nullptr) { // if rinfo == nullptr, ignore it
		if (next == nullptr) return -3;
		if (word == nullptr) {
			if (rinfo != nullptr && info != nullptr && *rinfo == *info) {
				info = nullptr;
				if (next == nullptr) return 1;
				else return 0;
			}
			else if (rinfo == nullptr) {
				info = nullptr;
				return 0;
			}
			else if (info == nullptr) return -1;
			else return -2;
		}
		int ans = next[word[0].seq()].remove(&word[1], rinfo);
		if (ans == 1) {
			for (int i = 0; i < size(); i++) {
				if (!next[i].empty()) return 0;
			}
			delete[] next;
			next = nullptr;
			if (info == nullptr) return 1;
			else return 0;
		}
		else if (ans == 0) return 0;
		else return -4;
	}

	const Info* lookup(Char* word) {
		if (word[0] == nullptr) return info;
		return word[1].lookup(&word[1]);
	}
};


