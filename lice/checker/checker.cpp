#include "testlib.h"
#include <string>
using namespace std;

// Output layout produced by the judge grader:
//   line 1: output secret (proves the output was written by the grader)
//   line 2: grader result ("OK", or WA/PV/SV/FAIL with an optional message)
//   then:   one integer per test case = return value of min_days
// The answer is unique, so each value is compared exactly with the judge answer.

const string output_secret = "jWOBUomOQXYGoHFMgCx9nRlbRay1nnwf";

int main(int argc, char* argv[]) {
	registerChecker("lice", argc, argv);
	readBothSecrets(output_secret);
	readBothGraderResults();

	int t = inf.readInt();
	for (int tc = 1; tc <= t; tc++) {
		int expected = ans.readInt();
		// Any reading problem on ouf (missing value, non-integer, out of range)
		// is reported by testlib as a wrong answer, never as a crash.
		int got = ouf.readInt(1, 3, format("answer of test case %d", tc));
		if (got != expected)
			quitf(_wa, "test case %d: expected %d, found %d", tc, expected, got);
	}
	if (!ans.seekEof())
		quitf(_fail, "judge answer has extra data");
	// quitf(_ok) also checks that the contestant output has nothing left (else WA).
	quitf(_ok, "%d test cases correct", t);
}
