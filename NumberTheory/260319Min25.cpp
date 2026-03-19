// Luogu P5325
#include <bits/stdc++.h>

template<long long MOD>
class ModInt {
    
    private:

    long long x;

    void up_safe() {
        if (x >= MOD) {
            x -= MOD;
        }
    }
    void down_safe() {
        if (x < 0) {
            x += MOD;
        }
    }

    public:

    template<typename qp_T>
    static qp_T mod_qp(qp_T p, long long q) {
        qp_T r = 1;
        while (q) {
            if (q & 1) {
                r = r * p % MOD;
            }
            p = p * p % MOD;
            q >>= 1;
        }
        return r;
    }

    long long& val() {
        return x;
    }

    ModInt() : x(0) {};
    ModInt(long long _x) : x(_x % MOD) { down_safe(); };
    ModInt(const ModInt& _x) : x(_x.x) {};
    
    ModInt& operator+= (ModInt o) {
        x += o.x;
        up_safe();
        return *this;
    }
    ModInt& operator-= (ModInt o) {
        x -= o.x;
        down_safe();
        return *this;
    }
    ModInt& operator*= (ModInt o) {
        x = x * o.x % MOD;
        return *this;
    }
    ModInt& operator/= (ModInt o) {
        x = x * mod_qp(o.x, MOD - 2) % MOD;
        return *this;
    }

    friend ModInt operator+ (ModInt lhs, ModInt rhs) {
        lhs += rhs;
        return lhs;
    }
    friend ModInt operator- (ModInt lhs, ModInt rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend ModInt operator* (ModInt lhs, ModInt rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend ModInt operator/ (ModInt lhs, ModInt rhs) {
        lhs /= rhs;
        return lhs;
    }
    friend bool operator== (ModInt lhs, ModInt rhs) {
        return lhs.x == rhs.x;
    }
    friend bool operator< (ModInt lhs, ModInt rhs) {
        return lhs.x < rhs.x;
    }
    friend bool operator> (ModInt lhs, ModInt rhs) {
        return lhs.x > rhs.x;
    }
    friend bool operator<= (ModInt lhs, ModInt rhs) {
        return lhs.x <= rhs.x;
    }
    friend bool operator>= (ModInt lhs, ModInt rhs) {
        return lhs.x >= rhs.x;
    }
    ModInt& operator= (const ModInt& other) {
        if (this != &other) {
            x = other.x;
        }
        return *this;
    }
    ModInt& operator= (ModInt&& other) noexcept {
        if (this != &other) {
            x = other.x;
        }
        return *this;
    }

    friend std::istream &operator>>(std::istream &lhs, ModInt &rhs) {
        long long _x;
        lhs >> _x;
        rhs = ModInt(_x);
        return lhs;
    }
    friend std::ostream &operator<<(std::ostream &lhs, const ModInt &rhs) {
        return lhs << rhs.x;
    }
};

template<long long MOD>
ModInt<MOD> qp(ModInt<MOD> p, long long q) {
    ModInt<MOD> r = 1;
    while (q) {
        if (q & 1) r *= p;
        p *= p;
        q >>= 1;
    }
    return r;
}

// using Z = ModInt<998'244'353>;
using Z = ModInt<1'000'000'007>;

using i64 = long long;

constexpr i64 qp(i64 p, i64 q, i64 m) {
    i64 r = 1;
    while (q) {
        if (q & 1) {
            r = r * p % m;
        }
        p = p * p % m;
        q >>= 1;
    }
    return r;
}

// constexpr i64 MOD = 1e9 + 7;
Z INV2 = Z(1) / 2;
Z INV6 = Z(1) / 6;

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

    // define f_p
    auto f_p = [](i64 p) -> Z {
        return Z(p) * Z(p - 1);
    };

    // dp for min25
    std::vector<Z> g1(w.size()), g2(w.size());
    for (int i = 0; i < w.size(); i++) {
        g1[i] = (Z)w[i] * (w[i] + 1) * INV2 - 1;
        g2[i] = (Z)w[i] * (w[i] + 1) * (w[i] * 2 + 1) * INV6 - 1;
    }
    for (int j = 0; j < primes.size(); j++) {
        i64 p = primes[j];
        i64 p2 = p * p;
        for (int i = 0; i < w.size(); i++) {
            if (p2 > w[i]) {
                break;
            }
            g1[i] = g1[i] - Z(p) * (g1[get_idx(get_val(i) / p)] - g1[get_idx(p - 1)]);
            g2[i] = g2[i] - Z(p2) * (g2[get_idx(get_val(i) / p)] - g2[get_idx(p - 1)]);
        }
    }

    // dfs for min25
    auto get_g = [&](i64 x) -> Z {
        return g2[get_idx(x)] - g1[get_idx(x)];
    };
    auto dfs = [&](auto&& self, i64 x, int pi) -> Z {
        if (pi != -1 && primes[pi] >= x) {
            return 0;
        }
        Z res = get_g(x) - (pi == -1 ? 0 : get_g(primes[pi]));
        for (int pj = pi + 1; pj < primes.size(); pj++) {
            i64 p = primes[pj];
            i64 p2 = p * p;
            if (p2 > x) {
                break;
            }
            Z ext = 0;
            for (i64 pn = p; pn <= x; pn *= p) {
                res += f_p(pn) * (self(self, x / pn, pj) + ext);
                ext = 1;
            }
        }
        return res;
    };

    // output ans
    std::cout << dfs(dfs, n, -1) + 1 << '\n';

    return 0;
}