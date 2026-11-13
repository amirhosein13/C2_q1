#include "tree_input.h"

int main() {
	registerValidation();
	vector<Tree> trees = read_all_trees();
	for (const Tree& tr : trees)
		ensuref(tr.n <= 5000, "n = %d > 5000", tr.n);
	ensuref(sum_n(trees) <= 5000, "sum of n = %lld > 5000", sum_n(trees));
	return 0;
}
