template<class Policy>
class SegTree {

    using T = typename Policy::T;
    using H = typename Policy::H;

    private:

    Policy policy;
    long long n;
    int root = 0, tot = 0;
    std::vector<T> ds;
    std::vector<H> tag;
    std::vector<int> ls, rs;
    T e = policy.e();
    H id = policy.id();
    
    void push_up(int p, long long size) {
        ds[p] = policy.op(
            {ds[ls[p]], (size + 1) / 2},
            {ds[rs[p]], size / 2}
        );
    }

    void push_down(int p, long long size) {
        if (!ls[p]) {
            ls[p] = create_node((size + 1) / 2);
        }
        if (!rs[p]) {
            rs[p] = create_node(size / 2);
        }
        ds[ls[p]] = policy.map({ds[ls[p]], (size + 1) / 2}, tag[p]);
        ds[rs[p]] = policy.map({ds[rs[p]], size / 2}, tag[p]);
        tag[ls[p]] = policy.comp(tag[p], tag[ls[p]]);
        tag[rs[p]] = policy.comp(tag[p], tag[rs[p]]);
        tag[p] = id;
    }

    int create_node(long long size) {
        ds.push_back(e);
        tag.push_back(id);
        ls.push_back(0);
        rs.push_back(0);
        tot++;
        return tot;
    }

    long long get_valid_length(long long l, long long r, long long nl, long long nr) {
        long long res = std::min(r, nr) - std::max(l, nl) + 1;
        return (res < 0 ? 0 : res);
    }

    public:

    SegTree(long long _size) : n(_size) {
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
        long long len = r - l + 1;
        if (!p) {
            p = create_node(len);
        }
        if (nl <= l && r <= nr) {
            ds[p] = policy.map({ds[p], len}, x);
            tag[p] = policy.comp(x, tag[p]);
            return p;
        }
        if (tag[p] != id) {
            push_down(p, len);
        }
        int mid = (l + r) >> 1;
        if (nl <= mid) {
            ls[p] = apply(ls[p], l, mid, nl, nr, x);
        }
        if (nr > mid) {
            rs[p] = apply(rs[p], mid + 1, r, nl, nr, x);
        }
        push_up(p, len);
        return p;
    }

    /**
     * Assign ds[pos] = x;
     */
    void set(int pos, const T& x) {
        root = set(root, 1, n, pos, x);
    }
    int set(int p, long long l, long long r, long long pos, const T& x) {
        long long len = r - l + 1;
        if (!p) {
            p = create_node(len);
        }
        if (l == r) {
            ds[p] = x;
            return p;
        }
        if (tag[p] != id) {
            push_down(p, len);
        }
        long long mid = (l + r) >> 1;
        if (pos <= mid) {
            ls[p] = set(ls[p], l, mid, pos, x);
        } else {
            rs[p] = set(rs[p], mid + 1, r, pos, x);
        }
        push_up(p, len);
        return p;
    }
    /**
     * Update ds[pos] = op(ds[pos], x);
     */
    void update(int pos, const T& x) {
        root = update(root, 1, n, pos, x);
    }
    int update(int p, long long l, long long r, long long pos, const T& x) {
        long long len = r - l + 1;
        if (!p) {
            p = create_node(len);
        }
        if (l == r) {
            ds[p] = policy.op({ds[p], len}, {x, len});
            return p;
        }
        if (tag[p] != id) {
            push_down(p, len);
        }
        long long mid = (l + r) >> 1;
        if (pos <= mid) {
            ls[p] = update(ls[p], l, mid, pos, x);
        } else {
            rs[p] = update(rs[p], mid + 1, r, pos, x);
        }
        push_up(p, len);
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
        long long len = r - l + 1;
        if (tag[p] != id) {
            push_down(p, len);
        }
        long long mid = (l + r) >> 1;
        T res = e;
        long long lll = 0;
        if (nl <= mid) {
            T rr = query(ls[p], l, mid, nl, nr);
            long long nll = get_valid_length(l, mid, nl, nr);
            res = policy.op({res, lll}, {rr, nll});
            lll += nll;
        }
        if (nr > mid) {
            T rr = query(rs[p], mid + 1, r, nl, nr);
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
            push_down(p, r - l + 1);
        }
        long long mid = (l + r) >> 1;
        long long res = last(ls[p], mid + 1, r, nl, nr, f);
        if (res == -1) {
            res = last(rs[p], l, mid, nl, nr, f);
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