#include "tree_input.h"

int main() {
	registerValidation();
	vector<Tree> trees = read_all_trees();
	for (const Tree& tr : trees) {
		vector<int> deg(tr.n + 1, 0);
		for (int i = 0; i < tr.n - 1; i++) {
			deg[tr.u[i]]++;
			deg[tr.v[i]]++;
		}
		int big = 0;
		for (int x = 1; x <= tr.n; x++)
			if (deg[x] >= 3) big++;
		ensuref(big <= 1, "%d vertices have degree >= 3 (spider needs <= 1)", big);
	}
	return 0;
}
