#include <UnitTest++.h>
#include "common.h"
#include "testutils.h"
#include "process.h"

using namespace std;

/*
 * Tests for the complete system. (Results judged manually)
 */
TEST(test_raven) {
	process("test/refs_raven.txt", "test/queries_raven.txt", "output/");
	CHECK_EQUAL(true, true);
}