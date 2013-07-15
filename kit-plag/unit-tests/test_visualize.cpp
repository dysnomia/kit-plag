#include <UnitTest++.h>
#include "common.h"
#include "testutils.h"
#include "visualize.h"

using namespace std;
using namespace boost::assign;

/*
 * Calls the whole program for some sample texts and reference texts.
 * Uses visualize, but the output has to be checked manually.
 */

/* Simple test */
TEST(visualize_simple) {
	ofstream os("output/output_test.html");
	auto normalized = list_of(normalize("Test 1", "ABC"))(normalize("Test 2", "DEF"));
	visualize_begin(os, "Test", normalized);
	visualize_end(os);
}