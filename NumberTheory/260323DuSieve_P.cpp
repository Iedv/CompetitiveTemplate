// Luogu P4213
#include <bits/stdc++.h>

template<typename T>
class DuSieve {

private:

    std::vector<T> pre_f;
    std::map<long long, T> memory;

public:

    DuSieve(std::vector<T>& _pre_f) : pre_f(_pre_f) {};

    T query(long long n, auto d_conv, auto sum_g) {
        if (n < pre_f.size()) {
            return pre_f[n];
        }
        auto k = memory.find(n);
        if (k != memory.end()) {
            return k->second;
        }
        T res = d_conv(n);
        for (long long l = 2, r = l; l <= n; l = r + 1) {
            r = n / (n / l);
            res -= query(n / l, d_conv, sum_g) * (sum_g(r) - sum_g(l - 1));
        }
        return memory[n] = res;
    }

};

template<typename T>
class DuSieveFixed {

private:

    long long n, srn;
    std::vector<T> pre_f;
    std::function<T(long long)> d_conv, sum_g;
    std::vector<int> id1, id2;
    std::vector<long long> w;
    std::vector<T> memory;

    T query(long long now_n) {
        if (now_n < pre_f.size()) {
            return pre_f[now_n];
        }
        T k = get_mem(now_n);
        if (k != -1) {
            return k;
        }
        T res = d_conv(now_n);
        for (long long l = 2, r = l; l <= now_n; l = r + 1) {
            r = now_n / (now_n / l);
            res -= query(now_n / l) * (sum_g(r) - sum_g(l - 1));
        }
        return k = res;
    }

public:

    DuSieveFixed(
        long long _n, 
        std::vector<T>& _pre_f, 
        std::function<T(long long)> _d_conv, 
        std::function<T(long long)> _sum_g
    ) : n(_n), pre_f(_pre_f), d_conv(_d_conv), sum_g(_sum_g) {
        srn = std::sqrt(n);
        id1.resize(srn + 1);
        id2.resize(srn + 1);
        w.resize(0);
        for (long long l = 1, r = l; l <= n; l = r + 1) {
            r = n / (n / l);
            long long val = n / l;
            w.push_back(val);
            memory.push_back(-1);
            if (val <= srn) {
                id1[val] = w.size() - 1;
            } else {
                id2[n / val] = w.size() - 1;
            }
        }
    };

    int get_idx(long long val) {
        if (val <= srn) {
            return id1[val];
        } else {
            return id2[n / val];
        }
    }

    long long get_val(int idx) {
        return w[idx];
    }

    T& get_mem(long long val) {
        return memory[get_idx(val)];
    }

    T query() {
        return query(n);
    }

};

using i64 = long long;

constexpr i64 N = 2e6;

std::vector<uint8_t> nprime(N);
std::vector<i64> mu(N), phi(N);
std::vector<int> primes;

DuSieve<i64>* s_mu = nullptr;
DuSieve<i64>* s_phi = nullptr;

constexpr i64 sum_epsilon(i64 n) { return 1; }
constexpr i64 sum_one(i64 n) { return n; }
constexpr i64 sum_n(i64 n) { return n * (n + 1) / 2; }

void solve() {
    i64 n;
    std::cin >> n;

    std::cout << s_phi->query(n, sum_n, sum_one) << ' ' << s_mu->query(n, sum_epsilon, sum_one) << '\n';
    // std::cout << DuSieveFixed<i64>(n, phi, sum_n, sum_one).query() << ' ' << DuSieveFixed<i64>(n, mu, sum_epsilon, sum_one).query() << '\n';
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    mu[1] = phi[1] = 1;
    for (int i = 2; i < N; i++) {
        if (!nprime[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int prime : primes) {
            int now = prime * i;
            if (now >= N) {
                break;
            }
            nprime[now] = 1;
            
            if (i % prime == 0) {
                phi[now] = phi[i] * prime;
                mu[now] = 0;
                break;
            } else {
                phi[now] = phi[prime] * phi[i];
                mu[now] = mu[prime] * mu[i];
            }
        }
    }

    for (int i = 1; i < N; i++) {
        mu[i] += mu[i - 1];
        phi[i] += phi[i - 1];
    }

    s_mu = new DuSieve<i64>(mu);
    s_phi = new DuSieve<i64>(phi);

    int t;
    std::cin >> t;
    while (t --> 0) {
        solve();
    }
}