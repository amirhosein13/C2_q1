#include "lice.h"
#include <cassert>
#include <cstdio>
#include <unistd.h>
#include <vector>
// BEGIN SECRET
#include <string>
// END SECRET

namespace {

// Buffered reader: refills with read(2), which returns whatever data is
// available (e.g. one typed line), so it never waits for end-of-file and
// typing the input in a terminal works. It only reads the integers it needs.
// The grader reads every test case before calling min_days and then closes
// stdin, so the contestant code cannot read the input stream itself.
char buf[1 << 16];
int buf_len = 0, buf_pos = 0;

int next_char() {
	if (buf_pos == buf_len) {
		ssize_t r = read(0, buf, sizeof(buf));
		if (r <= 0)
			return EOF;
		buf_len = (int)r;
		buf_pos = 0;
	}
	return (unsigned char)buf[buf_pos++];
}

int read_int() {
	int c = next_char();
	while (c != EOF && (c < '0' || c > '9'))
		c = next_char();
	assert(c != EOF);  // input ended too early
	int x = 0;
	while (c >= '0' && c <= '9') {
		x = x * 10 + (c - '0');
		c = next_char();
	}
	return x;
}

struct TestCase {
	int N;
	std::vector<int> U, V;
};

} // namespace

int main() {
	// BEGIN SECRET
	const std::string output_secret = "jWOBUomOQXYGoHFMgCx9nRlbRay1nnwf";
	// END SECRET
	int T = read_int();
	std::vector<TestCase> tests(T);
	for (int t = 0; t < T; t++) {
		int N = read_int();
		tests[t].N = N;
		tests[t].U.resize(N - 1);
		tests[t].V.resize(N - 1);
		for (int i = 0; i < N - 1; i++) {
			tests[t].U[i] = read_int() - 1;
			tests[t].V[i] = read_int() - 1;
		}
	}
	fclose(stdin);

	std::vector<int> answers(T);
	for (int t = 0; t < T; t++) {
		answers[t] = min_days(tests[t].N, tests[t].U, tests[t].V);
		std::vector<int>().swap(tests[t].U);
		std::vector<int>().swap(tests[t].V);
	}

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	for (int t = 0; t < T; t++)
		printf("%d\n", answers[t]);
	fclose(stdout);
	return 0;
}
