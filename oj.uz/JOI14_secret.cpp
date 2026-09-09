#include <bits/stdc++.h>
#include "secret.h"
using namespace std;

vector<int> a;
vector<vector<int>> data(32, vector<int>(1024));
vector<int> mask;

void f(int level, int l, int r) {
	if (r - l <= 1) {
		return;
	}

	int m = (l + r) / 2;
	::data[level][m - 1] = a[m - 1];
	for (int i = m - 2; i >= l; i--) {
		::data[level][i] = Secret(a[i], ::data[level][i + 1]);
	}
	::data[level][m] = a[m];
	for (int i = m + 1; i < r; i++) {
		::data[level][i] = Secret(::data[level][i - 1], a[i]);
	}
	for (int i = m; i < r; i++) {
		mask[i] ^= (1 << level);
	}
	f(level + 1, l, m);
	f(level + 1, m, r);
}

void Init(int N, int A[]) {
	a.resize(N);
	mask.assign(N, 0);
	for (int i = 0; i < N; i++) {
		a[i] = A[i];
	}
	f(0, 0, N);
}

int Query(int L, int R) {
	if (L == R) {
		return a[L];
	} else {
		int bits = __builtin_ctz(mask[L] ^ mask[R]);
		return Secret(::data[bits][L], ::data[bits][R]);
	}
}
