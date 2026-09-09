#include <bits/stdc++.h>
using namespace std;

struct Tree {
	int n, LOG, timer;
	vector<int> depth, euler_tour, tin, tout;
	vector<vector<int>> G, ancestor;

	Tree(const vector<vector<int>> &graph, int root = 0) {
		G = graph;
		n = (int)G.size();
		LOG = __lg(n) + 1;
		timer = 0;
		euler_tour.assign(2 * n, 0);
		tin.assign(n, 0);
		tout.assign(n, 0);
		depth.assign(n, 0);
		ancestor.assign(n, vector<int>(LOG, 0));
		DFS(root, root);
	}

	void DFS(int u, int p) {
		euler_tour[timer] = u;
		tin[u] = timer++;
		ancestor[u][0] = p;
		for (int i = 1; i < LOG; i++) {
			ancestor[u][i] = ancestor[ancestor[u][i - 1]][i - 1];
		}
		for (int v : G[u]) {
			if (v != p) {
				depth[v] = depth[u] + 1;
				DFS(v, u);
			}
		}
		euler_tour[timer] = u;
		tout[u] = timer++;
	}

	bool is_ancestor(int u, int v) {
		return tin[u] <= tin[v] && tout[u] >= tout[v];
	}

	int query(int u, int v) {
		if (depth[u] < depth[v]) {
			swap(u, v);
		}
		for (int i = 0, k = depth[u] - depth[v]; i < LOG; i++) {
			if (k >> i & 1) {
				u = ancestor[u][i];
			}
		}
		if (u == v) {
			return u;
		}
		for (int i = LOG - 1; i >= 0; i--) {
			if (ancestor[u][i] != ancestor[v][i]) {
				u = ancestor[u][i];
				v = ancestor[v][i];
			}
		}
		return ancestor[u][0];
	}
};

signed main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<int> a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	vector<vector<int>> G(n, vector<int>());
	for (int i = 0; i < n - 1; i++) {
		int u, v;
		cin >> u >> v;
		u--, v--;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	Tree tree(G);

	vector<array<int, 4>> queries(m);
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		u--, v--;
		if (tree.tin[u] > tree.tin[v]) {
			swap(u, v);
		}
		int lca = tree.query(u, v);
		if (u == lca) {
			queries[i] = {tree.tin[u], tree.tin[v], -1, i};
		} else {
			queries[i] = {tree.tout[u], tree.tin[v], lca, i};
		}
	}

	int sz = (int)sqrt(n);
	sort(queries.begin(), queries.end(), [&](const array<int, 4> &x, const array<int, 4> &y) {
		int t1 = x[0] / sz, t2 = y[0] / sz;
		return (t1 < t2) || (t1 == t2 && x[1] < y[1]);
	});

	int cntcnt = 0;
	vector<bool> seen(n, false);
	unordered_map<int, int> cnt;
	auto add_value = [&](int u) {
		if (seen[u]) {
			if (--cnt[a[u]] == 0) {
				cntcnt--;
			}
		} else {
			if (cnt[a[u]]++ == 0) {
				cntcnt++;
			}
		}
		seen[u] = !seen[u];
	};
	vector<int> ans(m);
	for (int i = 0, l = 0, r = -1; i < m; i++) {
		while (l > queries[i][0]) {
			add_value(tree.euler_tour[--l]);
		}
		while (r < queries[i][1]) {
			add_value(tree.euler_tour[++r]);
		}
		while (l < queries[i][0]) {
			add_value(tree.euler_tour[l++]);
		}
		while (r > queries[i][1]) {
			add_value(tree.euler_tour[r--]);
		}

		int lc = queries[i][2];
		if (lc != -1) {
			add_value(lc);
		}
		ans[queries[i][3]] = cntcnt;
		if (lc != -1) {
			add_value(lc);
		}
	}
	for (int i = 0; i < m; i++) {
		cout << ans[i] << '\n';
	}
}
