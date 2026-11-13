#include "lice.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace {

// Fast reader over the whole standard input.
// The grader reads every test case before calling min_days and then closes
// stdin, so the contestant code cannot read the input stream itself.
char *buf;
size_t buf_len, buf_pos;

void load_input() {
	size_t cap = 1 << 20;
	buf = (char*)malloc(cap);
	buf_len = 0;
	while (true) {
		if (buf_len == cap) {
			cap *= 2;
			buf = (char*)realloc(buf, cap);
			assert(buf != nullptr);
		}
		size_t r = fread(buf + buf_len, 1, cap - buf_len, stdin);
		if (r == 0) break;
		buf_len += r;
	}
	buf_pos = 0;
}

int read_int() {
	while (buf_pos < buf_len && (buf[buf_pos] < '0' || buf[buf_pos] > '9'))
		buf_pos++;
	assert(buf_pos < buf_len);
	int x = 0;
	while (buf_pos < buf_len && buf[buf_pos] >= '0' && buf[buf_pos] <= '9')
		x = x * 10 + (buf[buf_pos++] - '0');
	return x;
}

struct TestCase {
	int N;
	std::vector<int> U, V;
};

} // namespace

int main() {
	load_input();
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
	free(buf);
	fclose(stdin);

	std::vector<int> answers(T);
	for (int t = 0; t < T; t++) {
		answers[t] = min_days(tests[t].N, tests[t].U, tests[t].V);
		std::vector<int>().swap(tests[t].U);
		std::vector<int>().swap(tests[t].V);
	}

	for (int t = 0; t < T; t++)
		printf("%d\n", answers[t]);
	fclose(stdout);
	return 0;
}
