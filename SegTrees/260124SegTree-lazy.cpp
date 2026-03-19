template<class Policy>
class SegTree {

    using T = typename Policy::T;
    using H = typename Policy::H;

private:

    Policy policy;
    long long n;
    int root = 1;
    std::vector<T> ds;
    std::vector<H> tag;
    T e = policy.e();
    H id = policy.id();
    
    void push_up(int p, long long size) {
        ds[p] = policy.op(
            {ds[p << 1], (size + 1) / 2},
            {ds[p << 1 | 1], size / 2}
        );
    }

    void push_down(int p, long long size) {
        ds[p << 1] = policy.map({ds[p << 1], (size + 1) / 2}, tag[p]);
        ds[p << 1 | 1] = policy.map({ds[p << 1 | 1], size / 2}, tag[p]);
        tag[p << 1] = policy.comp(tag[p], tag[p << 1]);
        tag[p << 1 | 1] = policy.comp(tag[p], tag[p << 1 | 1]);
        tag[p] = id;
    }

    long long get_valid_length(long long l, long long r, long long nl, long long nr) {
        long long res = std::min(r, nr) - std::max(l, nl) + 1;
        return (res < 0 ? 0 : res);
    }

public:

    SegTree() {};
    SegTree(int _size) {
        n = _size;
        ds.resize(n * 4, e);
        tag.resize(n * 4, id);
    };
    /**
     * The initial array should be 0-indexed
     */
    SegTree(const std::vector<T>& _array) {
        n = _array.size();
        ds.resize(n * 4);
        tag.resize(n * 4, id);
        build(_array);
    };
    /**
     * Built from range [l, r] of the array;
     */
    SegTree(const std::vector<T>& _array, int _l, int _r) {
        n = _r - _l + 1;
        ds.resize(n * 4);
        tag.resize(n * 4, id);
        build(_array, _l);
    };

    /**
     * Bias describe the starting index of the array;
     */
    void build(const std::vector<T>& _array, int __bias = 0) {
        auto work = [&](auto&& self, int p, int l, int r) -> void {
            if (l == r) {
                ds[p] = _array[l - 1 + __bias];
                return;
            }
            int mid = (l + r) >> 1;
            self(self, p << 1, l, mid);
            self(self, p << 1 | 1, mid + 1, r);
            push_up(p, r - l + 1);
        };
        work(work, 1, 1, n);
    }

    /**
     * Apply H to ds[l..r]
     */
    void apply(long long l, long long r, const H& x) {
        apply(root, 1, n, l, r, x);
    }
    void apply(int p, long long l, long long r, long long nl, long long nr, const H& x) {
        long long len = r - l + 1;
        if (nl <= l && r <= nr) {
            ds[p] = policy.map({ds[p], len}, x);
            tag[p] = policy.comp(x, tag[p]);
            return;
        }
        if (tag[p] != id) {
            push_down(p, len);
        }
        int mid = (l + r) >> 1;
        if (nl <= mid) {
            apply(p << 1, l, mid, nl, nr, x);
        }
        if (nr > mid) {
            apply(p << 1 | 1, mid + 1, r, nl, nr, x);
        }
        push_up(p, len);
    }

    /**
     * Assign ds[pos] = x;
     */
    void set(int pos, const T& x) {
        set(root, 1, n, pos, x);
    }
    void set(int p, long long l, long long r, long long pos, const T& x) {
        long long len = r - l + 1;
        if (l == r) {
            ds[p] = x;
            return;
        }
        if (tag[p] != id) {
            push_down(p, len);
        }
        long long mid = (l + r) >> 1;
        if (pos <= mid) {
            set(p << 1, l, mid, pos, x);
        } else {
            set(p << 1 | 1, mid + 1, r, pos, x);
        }
        push_up(p, len);
    }
    /**
     * Update ds[pos] = op(ds[pos], x);
     */
    void update(int pos, const T& x) {
        update(root, 1, n, pos, x);
    }
    void update(int p, long long l, long long r, long long pos, const T& x) {
        long long len = r - l + 1;
        if (l == r) {
            ds[p] = policy.op({ds[p], len}, {x, len});
            return;
        }
        if (tag[p] != id) {
            push_down(p, len);
        }
        long long mid = (l + r) >> 1;
        if (pos <= mid) {
            update(p << 1, l, mid, pos, x);
        } else {
            update(p << 1 | 1, mid + 1, r, pos, x);
        }
        push_up(p, len);
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
        if (nl <= l && r <= nr) {
            return ds[p];
        }
        long long len = r - l + 1;
        if (tag[p] != id) {
            push_down(p, len);
        }
        long long mid = (l + r) >> 1;
        T res = e;
        long long lll = 0;
        if (nl <= mid) {
            T rr = query(p << 1, l, mid, nl, nr);
            long long nll = get_valid_length(l, mid, nl, nr);
            res = policy.op({res, lll}, {rr, nll});
            lll += nll;
        }
        if (nr > mid) {
            T rr = query(p << 1 | 1, mid + 1, r, nl, nr);
            res = policy.op({res, lll}, {rr, get_valid_length(mid + 1, r, nl, nr)});
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
            push_down(p, r - l + 1);
        }
        long long mid = (l + r) >> 1;
        long long res = first(p << 1, l, mid, nl, nr, f);
        if (res == -1) {
            res = first(p << 1 | 1, mid + 1, r, nl, nr, f);
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
        if (tag[p] != id) {
            push_down(p, r - l + 1);
        }
        long long mid = (l + r) >> 1;
        long long res = last(p << 1, mid + 1, r, nl, nr, f);
        if (res == -1) {
            res = last(p << 1 | 1, l, mid, nl, nr, f);
        }
        return res;
    }
};

using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128;
using u128 = unsigned __int128;

/*
    struct Policy {
        using T = Z;
        using H = std::pair<Z, Z>;
        T op(std::pair<const T&, i64> x, std::pair<const T&, i64> y) {
            return x.first + y.first;
        }
        T map(std::pair<const T&, i64> x, const H& y) {
            return x.first * y.first + y.second * x.second;
        }
        H comp(const H& x, const H& y) {
            return {x.first * y.first, y.second * x.first + x.second};
        }
        T e() {
            return Z(0);
        }
        H id() {
            return {Z(1), Z(0)};
        }
    };
*/