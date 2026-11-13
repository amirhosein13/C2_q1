// Shared reader for all validators: reads and validates the general format
// and the global constraints, and returns the trees for subtask-specific checks.
#include "testlib.h"
#include <vector>
#include <numeric>
using namespace std;

const int MAXT = 200000;
const int MAXN = 1000000;
const long long MAXSUMN = 1000000;

struct Tree {
	int n;
	vector<int> u, v;
};

static int dsu_find(vector<int>& p, int x) {
	while (p[x] != x) {
		p[x] = p[p[x]];
		x = p[x];
	}
	return x;
}

static vector<Tree> read_all_trees() {
	vector<Tree> trees;
	int t = inf.readInt(1, MAXT, "t");
	inf.readEoln();
	long long sumn = 0;
	for (int tc = 0; tc < t; tc++) {
		Tree tr;
		tr.n = inf.readInt(2, MAXN, "n");
		inf.readEoln();
		sumn += tr.n;
		ensuref(sumn <= MAXSUMN, "sum of n exceeds %lld", MAXSUMN);
		vector<int> p(tr.n + 1);
		iota(p.begin(), p.end(), 0);
		tr.u.resize(tr.n - 1);
		tr.v.resize(tr.n - 1);
		for (int i = 0; i < tr.n - 1; i++) {
			tr.u[i] = inf.readInt(1, tr.n, "u");
			inf.readSpace();
			tr.v[i] = inf.readInt(1, tr.n, "v");
			inf.readEoln();
			ensuref(tr.u[i] != tr.v[i], "self loop in test %d", tc + 1);
			int a = dsu_find(p, tr.u[i]), b = dsu_find(p, tr.v[i]);
			ensuref(a != b, "edges of test %d do not form a tree (cycle or multi-edge)", tc + 1);
			p[a] = b;
		}
		// n-1 edges without a cycle on n vertices => connected tree
		trees.push_back(move(tr));
	}
	inf.readEof();
	return trees;
}

[[maybe_unused]] static long long sum_n(const vector<Tree>& trees) {
	long long s = 0;
	for (const Tree& tr : trees) s += tr.n;
	return s;
}
