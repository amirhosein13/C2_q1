#include "tree_input.h"

int main() {
	registerValidation();
	vector<Tree> trees = read_all_trees();
	for (const Tree& tr : trees)
		ensuref(tr.n <= 15, "n = %d > 15", tr.n);
	ensuref(sum_n(trees) <= 300, "sum of n = %lld > 300", sum_n(trees));
	return 0;
}
