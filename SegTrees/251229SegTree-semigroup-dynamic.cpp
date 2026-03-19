template<class T>
class SegTree {

    private:

    long long n;
    int root = 0, tot = 0;
    std::vector<T> ds;
    std::vector<int> ls, rs;
    std::function<T(T&, T&)> op;
    T e;
    
    void push_up(int p) {
        ds[p] = op(ds[ls[p]], ds[rs[p]]);
    }

    int create_node() {
        ds.push_back(e);
        ls.push_back(0);
        rs.push_back(0);
        tot++;
        return tot;
    }

    public:

    SegTree(std::function<T(T&, T&)> _operator, T _e) : op(_operator), e(_e) {};
    SegTree(std::function<T(T&, T&)> _operator, T _e, long long _size) : op(_operator), e(_e) {
        n = _size;
        ds.resize(1, e);
        ls.resize(1, 0);
        rs.resize(1, 0);
    };
    /**
     * Assign ds[pos] = x;
     */
    void set(int pos, T x) {
        root = set(root, 1, n, pos, x);
    }
    int set(int p, long long l, long long r, long long pos, T x) {
        if (!p) {
            p = create_node();
        }
        if (l == r) {
            ds[p] = x;
            return p;
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
    void update(int pos, T x) {
        root = update(root, 1, n, pos, x);
    }
    int update(int p, long long l, long long r, long long pos, T x) {
        if (!p) {
            p = create_node();
        }
        if (l == r) {
            ds[p] = op(ds[p], x);
            return p;
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
        long long mid = (l + r) >> 1;
        long long res = last(ls[p], mid + 1, r, nl, nr, f);
        if (res == -1) {
            res = last(rs[p], l, mid, nl, nr, f);
        }
        return res;
    }
};