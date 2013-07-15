#include "common.h"
#include "testutils.h"

using namespace std;

/*
 * Debug output routines for data structures
 */

ostream& operator<<(ostream& os, const Normalized& normal) {
	for (const auto& word: normal.words) {
		os << word << " ";
	}
	for (const auto &idx: normal.indexes) {
		os << idx;
	}
	os << endl;
	return os;
}