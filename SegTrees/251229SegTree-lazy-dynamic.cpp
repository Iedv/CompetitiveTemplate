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

template<class T, class H>
class SegTree {

    private:

    long long n;
    int root = 0, tot = 0;
    std::vector<T> ds;
    std::vector<H> tag;
    std::vector<int> ls, rs;
    // make function objects accept const references
    std::function<T(const T&, const T&)> op;
    std::function<T(const T&, const H&)> map;
    std::function<H(const H&, const H&)> comp;
    H id;
    T e;
    
    void push_up(int p) {
        ds[p] = op(ds[ls[p]], ds[rs[p]]);
    }

    void push_down(int p) {
        if (!ls[p]) {
            ls[p] = create_node();
        }
        if (!rs[p]) {
            rs[p] = create_node();
        }
        ds[ls[p]] = map(ds[ls[p]], tag[p]);
        ds[rs[p]] = map(ds[rs[p]], tag[p]);
        tag[ls[p]] = comp(tag[p], tag[ls[p]]);
        tag[rs[p]] = comp(tag[p], tag[rs[p]]);
        tag[p] = id;
    }

    int create_node() {
        ds.push_back(e);
        tag.push_back(id);
        ls.push_back(0);
        rs.push_back(0);
        tot++;
        return tot;
    }

    public:

    // constructors accept const-ref std::function types
    SegTree(
        std::function<T(const T&, const T&)> _operator,
        T _e, 
        std::function<T(const T&, const H&)> _map,
        std::function<H(const H&, const H&)> _comp,
        H _id
    ) : op(_operator), e(_e), map(_map), comp(_comp), id(_id) {};

    SegTree(
        std::function<T(const T&, const T&)> _operator,
        T _e, 
        std::function<T(const T&, const H&)> _map,
        std::function<H(const H&, const H&)> _comp,
        H _id, 
        long long _size
    ) : op(_operator), e(_e), map(_map), comp(_comp), id(_id) {
        n = _size;
        ds.resize(1, e);
        tag.resize(1, id);
        ls.resize(1, 0);
        rs.resize(1, 0);
    };

    /**
     * Apply H to ds[l..r]
     */
    void apply(long long l, long long r, const H& x) {
        root = apply(root, 1, n, l, r, x);
    }
    int apply(int p, long long l, long long r, long long nl, long long nr, const H& x) {
        if (!p) {
            p = create_node();
        }
        if (nl <= l && r <= nr) {
            ds[p] = map(ds[p], x);
            tag[p] = comp(x, tag[p]);
            return p;
        }
        if (tag[p] != id) {
            push_down(p);
        }
        int mid = (l + r) >> 1;
        if (nl <= mid) {
            ls[p] = apply(ls[p], l, mid, nl, nr, x);
        }
        if (nr > mid) {
            rs[p] = apply(rs[p], mid + 1, r, nl, nr, x);
        }
        push_up(p);
        return p;
    }

    /**
     * Assign ds[pos] = x;
     */
    void set(int pos, const T& x) {
        root = set(root, 1, n, pos, x);
    }
    int set(int p, long long l, long long r, long long pos, const T& x) {
        if (!p) {
            p = create_node();
        }
        if (l == r) {
            ds[p] = x;
            return p;
        }
        if (tag[p] != id) {
            push_down(p);
        }
        long long mid = (l + r) >> 1;
        if (pos <= mid) {
            ls[p] = set(ls[p], l, mid, pos, x);
        } else {
            rs[p] = set(rs[p], mid + 1, r, pos, x);
        }
        push_up(p);
        return p;
    }
    /**
     * Update ds[pos] = op(ds[pos], x);
     */
    void update(int pos, const T& x) {
        root = update(root, 1, n, pos, x);
    }
    int update(int p, long long l, long long r, long long pos, const T& x) {
        if (!p) {
            p = create_node();
        }
        if (l == r) {
            ds[p] = op(ds[p], x);
            return p;
        }
        if (tag[p] != id) {
            push_down(p);
        }
        long long mid = (l + r) >> 1;
        if (pos <= mid) {
            ls[p] = update(ls[p], l, mid, pos, x);
        } else {
            rs[p] = update(rs[p], mid + 1, r, pos, x);
        }
        push_up(p);
        return p;
    }
    /**
     * Query op(ds[l], ds[l + 1], ..., ds[r]);
     */
    T query(long long l, long long r) {
        if (l > r) {
            return e;
        } else {
            return query(root, 1, n, l, r);
        }
    }
    T query(int p, long long l, long long r, long long nl, long long nr) {
        if (!p) {
            return e;
        }
        if (nl <= l && r <= nr) {
            return ds[p];
        }
        if (tag[p] != id) {
            push_down(p);
        }
        long long mid = (l + r) >> 1;
        T res = e;
        if (nl <= mid) {
            T rr = query(ls[p], l, mid, nl, nr);
            res = op(res, rr);
        }
        if (nr > mid) {
            T rr = query(rs[p], mid + 1, r, nl, nr);
            res = op(res, rr);
        }
        return res;
    }
    /**
     * Find min j such that f(a[j]) = true in range [l, r];
     */
    template<class F>
    long long first(long long l, long long r, F&& f) {
        return first(root, 1, n, l, r, f);
    }
    template<class F>
    long long first(int p, long long l, long long r, long long nl, long long nr, F&& f) {
        if (l > nr || r < nl) {
            return -1;
        }
        if (nl <= l && r <= nr && !f(ds[p])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        if (!p) {
            return (f(ds[p]) ? std::max(l, nl) : -1);
        }
        if (tag[p] != id) {
            push_down(p);
        }
        long long mid = (l + r) >> 1;
        long long res = first(ls[p], l, mid, nl, nr, f);
        if (res == -1) {
            res = first(rs[p], mid + 1, r, nl, nr, f);
        }
        return res;
    }
    /**
     * Find max j such that f(a[j]) = true in range [l, r];
     */
    template<class F>
    long long last(long long nl, long long nr, F&& f) {
        return last(root, 1, n, nl, nr, f);
    }
    template<class F>
    long long last(int p, long long l, long long r, long long nl, long long nr, F&& f) {
        if (l > nr || r < nl) {
            return -1;
        }
        if (nl <= l && r <= nr && !f(ds[p])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        if (!p) {
            return (f(ds[p]) ? std::min(r, nr) : -1);
        }
        if (tag[p] != id) {
            push_down(p);
        }
        long long mid = (l + r) >> 1;
        long long res = last(ls[p], mid + 1, r, nl, nr, f);
        if (res == -1) {
            res = last(rs[p], l, mid, nl, nr, f);
        }
        return res;
    }
};