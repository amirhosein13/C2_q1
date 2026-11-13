#include "tree_input.h"

int main() {
	registerValidation();
	vector<Tree> trees = read_all_trees();
	for (const Tree& tr : trees)
		ensuref(tr.n <= 300, "n = %d > 300", tr.n);
	ensuref(sum_n(trees) <= 2000, "sum of n = %lld > 2000", sum_n(trees));
	return 0;
}
