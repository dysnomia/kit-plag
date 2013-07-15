#include <UnitTest++.h>
#include "preprocess.h"
#include "testutils.h"

using namespace std;
using namespace boost::assign;

/*
 * Various tests for text normalization.
 */

/* Normalize case */
TEST(preprocess_case) {
	string input = "test for the PRePROCESSiNG STEP of thE PLAGiARISM detecTOR";
	vector<string> expected = list_of("test")("preprocessing")("step")("plagiarism")("detector");
	CHECK_EQUAL(expected, normalize("Test", input).words);
}

/* Delete stop words and parse words */
TEST(preprocess_parse) {
	string input = "this is a test; for++)=)=)=(( the stop-word\n\t   not at all \"quot''atations\". and additIONal sentences...";
	vector<string> expected = list_of("test")("stopword")("quotatations")("additional")("sentences");
	CHECK_EQUAL(expected, normalize("Test", input).words);
}

/* Tests index into original text. */
TEST(preprocess_index) {
	string input = "(((simple test 'not included' and more stuff...";
	vector<string> expected_words = list_of("simple")("test")("included")("stuff");
	vector<int> expected_index = list_of(3)(10)(20)(39);
	auto got = normalize("Test", input);
	CHECK_EQUAL(expected_words, got.words);
	CHECK_EQUAL(expected_index, got.indexes);
}