#include <UnitTest++.h>
#include "common.h"
#include "trie.h"
#include "testutils.h"

/*
 * Tests for the generic trie class.
 */

using namespace std;
using namespace boost::assign;

/* Basic insertion tests */
TEST(trie_basic) {
	vector<string> names = list_of("")("A")("BCD")("DEF")("GHI");
	Trie<int> trie;
	for (const auto& str: names) {
		trie[str]++;
	}

	for (const auto& str: names) {
		for (int idx = 0; idx < int(str.size()); ++idx) {
			CHECK_EQUAL(idx == 0, trie.has(str.substr(idx))); 
		}
	}
}

/* Tests that prefixes are handled correctly and values initialised correctly. */
TEST(trie_prefix) {
	vector<string> strs = list_of("A")("AB")("ABC")("ABD")("$$$$$");
	Trie<int> trie;
	for (const auto& str: strs) {
		trie[str]++;
	}

	for (const auto& str: strs) {
		CHECK_EQUAL(1, trie[str]);
	}
}

/* Tests constant tries and move constructor */
TEST(trie_const) {
	vector<string> strs = list_of("A")("AB")("ABC")("ABD")("$$$$$");
	Trie<string> trie;
	for (const auto& str: strs) {
		trie[str] = str;
	}

	const Trie<string> trie_const = move(trie);
	for (const auto& str: strs) {
		CHECK_EQUAL(str, trie_const[str]);
	}
	CHECK_THROW(trie_const["KKK"], invalid_argument);
	CHECK_THROW(trie_const["$$$"], invalid_argument);
}

/* Tests trie iterators */
TEST(trie_iterator) {
	vector<string> strs = list_of("")("AAAAAAAAAAAAAAAAAAAAAAAAAAA")("KJLKJL")("JKHKJH")("MNKJH")("ABC")("ABB");
	sort(begin(strs), end(strs));
	Trie<vector<int>> trie;
	for (const auto& str: strs) {
		trie[str].push_back(1);
	}

	/* Checks for correct iteration order and modifies content vector again. */
	int idx = 0;
	for (auto& val: trie) {
		CHECK_EQUAL(strs[idx], val.first);
		CHECK_EQUAL(1, val.second.size());
		val.second.push_back(42);
		idx++;
	}
	CHECK_EQUAL(int(strs.size()), idx);

	/* Has the content vector actually been modified? */
	for (const auto& val: trie) {
		CHECK_EQUAL(42, val.second[1]);
	}

	/* Correct iterator errors and comparators? */
	auto it = end(trie);
	CHECK_THROW(*it, range_error);
	auto b1 = begin(trie), b2 = begin(trie);
	b1++;
	CHECK(b1++ == ++b2);
	CHECK(b1++ != it);
}