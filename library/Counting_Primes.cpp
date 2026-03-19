#include <bits/stdc++.h>

using i64 = long long;

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    
    i64 n;
    std::cin >> n;

    i64 srn = std::sqrt(n);

    // sieve
    std::vector<i64> primes;
    std::vector<uint8_t> nprime(srn + 1);
    for (int i = 2; i <= srn; i++) {
        if (!nprime[i]) {
            primes.push_back(i);
        }
        for (i64 prime : primes) {
            if (prime * i > srn) {
                break;
            }
            nprime[prime * i] = 1;
            if (i % prime == 0) {
                break;
            }
        }
    }

    // sqrt decomp
    std::vector<i64> w;
    std::vector<int> id1(srn + 1), id2(srn + 1);
    for (i64 l = 1, r = l; l <= n; l = r + 1) {
        r = n / (n / l);
        i64 val = n / l;
        w.push_back(val);
        if (val <= srn) {
            id1[val] = w.size() - 1;
        } else {
            id2[n / val] = w.size() - 1;
        }
    }
    auto get_idx = [&](i64 val) -> int {
        if (val <= srn) {
            return id1[val];
        } else {
            return id2[n / val];
        }
    };
    auto get_val = [&](int idx) -> i64 {
        return w[idx];
    };

    // dp for min25
    std::vector<i64> g(w.size());
    for (int i = 0; i < w.size(); i++) {
        g[i] = w[i] - 1;
    }
    for (int j = 0; j < primes.size(); j++) {
        i64 p = primes[j];
        i64 p2 = p * p;
        for (int i = 0; i < w.size(); i++) {
            if (p2 > w[i]) {
                break;
            }
            g[i] = g[i] - (g[get_idx(get_val(i) / p)] - g[get_idx(p - 1)]);
        }
    }

    std::cout << g[get_idx(n)] << '\n';

    return 0;
}