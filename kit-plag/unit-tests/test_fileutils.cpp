#include <UnitTest++.h>
#include "fileutils.h"
#include "testutils.h"

/*
 * Tests for reading the input.
 */

using namespace std;
using namespace boost::assign;

/* Test opening an input file. */
TEST(read_open) {
	ifstream fin;
	open_input("test/refs.txt", fin);

	string test;
	fin >> test;
}

/* Test reading whole stream. */
TEST(read_whole) {
	string str = "that's a test\n for reading from a whole stream";
	istringstream in(str);
	CHECK_EQUAL(str, read_whole_stream(in));
}

/* Test reading list of words. */
TEST(read_list) {
	string str = "that's a test: , for\t the input\n routines !\"$";
	vector<string> expected = list_of("that's")("a")("test:")(",")("for")("the")("input")("routines")("!\"$");
	istringstream in(str);
	CHECK_EQUAL(expected, read_word_list(in));
}

/* Test reading set of words. */
TEST(read_set) {
	string str = "word1 another pair of words \n words the pair is not the pair";
	string words[] = {"word1", "another", "pair", "of", "the", "is", "not", "words"};
	unordered_set<string> expected(begin(words), end(words));
	istringstream in(str);
	CHECK_EQUAL(expected, read_word_set(in));
}