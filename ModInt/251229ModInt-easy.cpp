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

using Z = ModInt<998'244'353>;
// using Z = ModInt<1'000'000'007>;