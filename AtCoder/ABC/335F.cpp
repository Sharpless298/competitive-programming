#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

signed main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}

	int k = sqrt(n);
	vector<int> dp(n, 1);
	vector<vector<int>> pref(k + 1, vector<int>(k + 1));
	for (int i = n - 1; i >= 0; i--) {
		if (a[i] > k) {
			for (int j = i + a[i]; j < n; j += a[i]) {
				dp[i] += dp[j];
				dp[i] %= MOD;
			}
		} else {
			dp[i] += pref[a[i]][i % a[i]];
			dp[i] %= MOD;
		}
		for (int j = 1; j <= k; j++) {
			pref[j][i % j] += dp[i];
			pref[j][i % j] %= MOD;
		}
	}

	cout << dp[0] << '\n';
}
