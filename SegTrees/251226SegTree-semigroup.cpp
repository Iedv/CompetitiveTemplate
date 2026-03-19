template<class T>
class SegTree {

    private:

    int n, root = 1;
    std::vector<T> ds;
    std::function<T(T&, T&)> op;
    T e;
    
    void push_up(int p) {
        ds[p] = op(ds[p << 1], ds[p << 1 | 1]);
    }

    public:

    SegTree(std::function<T(T&, T&)> _operator, T _e) : op(_operator), e(_e) {};
    SegTree(std::function<T(T&, T&)> _operator, T _e, int _size) : op(_operator), e(_e) {
        n = _size;
        ds.resize(n * 4);
    };
    SegTree(std::function<T(T&, T&)> _operator, T _e, int _size, T _val) : op(_operator), e(_e) {
        n = _size;
        ds.resize(n * 4, _val);
    };
    /**
     * The initial array should be 0-indexed
     */
    SegTree(std::function<T(T&, T&)> _operator, T _e, const std::vector<T>& _array) : op(_operator), e(_e) {
        n = _array.size();
        ds.resize(n * 4);
        build(_array);
    };
    /**
     * Built from range [l, r] of the array;
     */
    SegTree(std::function<T(T&, T&)> _operator, T _e, const std::vector<T>& _array, int _l, int _r) : op(_operator), e(_e) {
        n = _r - _l + 1;
        ds.resize(n * 4);
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
            push_up(p);
        };
        work(work, 1, 1, n);
    }
    /**
     * Assign ds[pos] = x;
     */
    void set(int pos, T x) {
        set(1, 1, n, pos, x);
    }
    void set(int p, int l, int r, int pos, T x) {
        if (l == r) {
            ds[p] = x;
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            set(p << 1, l, mid, pos, x);
        } else {
            set(p << 1 | 1, mid + 1, r, pos, x);
        }
        push_up(p);
    }
    /**
     * Update ds[pos] = op(ds[pos], x);
     */
    void update(int pos, T x) {
        update(1, 1, n, pos, x);
    }
    void update(int p, int l, int r, int pos, T x) {
        if (l == r) {
            ds[p] = op(ds[p], x);
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            update(p << 1, l, mid, pos, x);
        } else {
            update(p << 1 | 1, mid + 1, r, pos, x);
        }
        push_up(p);
    }
    /**
     * Query op(ds[l], ds[l + 1], ..., ds[r]);
     */
    T query(int l, int r) {
        if (l > r) {
            return e;
        } else {
            return query(1, 1, n, l, r);
        }
    }
    T query(int p, int l, int r, int nl, int nr) {
        if (nl <= l && r <= nr) {
            return ds[p];
        }
        int mid = (l + r) >> 1;
        T res = e;
        if (nl <= mid) {
            T rr = query(p << 1, l, mid, nl, nr);
            res = op(res, rr);
        }
        if (nr > mid) {
            T rr = query(p << 1 | 1, mid + 1, r, nl, nr);
            res = op(res, rr);
        }
        return res;
    }
    /**
     * Find min j such that f(a[j]) = true in range [l, r];
     */
    template<class F>
    int first(int l, int r, F&& f) {
        return first(1, 1, n, l, r, f);
    }
    template<class F>
    int first(int p, int l, int r, int nl, int nr, F&& f) {
        if (l > nr || r < nl) {
            return -1;
        }
        if (nl <= l && r <= nr && !f(ds[p])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        int mid = (l + r) >> 1;
        int res = first(p << 1, l, mid, nl, nr, f);
        if (res == -1) {
            res = first(p << 1 | 1, mid + 1, r, nl, nr, f);
        }
        return res;
    }
    /**
     * Find max j such that f(a[j]) = true in range [l, r];
     */
    template<class F>
    int last(int nl, int nr, F&& f) {
        return last(1, 1, n, nl, nr, f);
    }
    template<class F>
    int last(int p, int l, int r, int nl, int nr, F&& f) {
        if (l > nr || r < nl) {
            return -1;
        }
        if (nl <= l && r <= nr && !f(ds[p])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        int mid = (l + r) >> 1;
        int res = last(p << 1 | 1, mid + 1, r, nl, nr, f);
        if (res == -1) {
            res = last(p << 1, l, mid, nl, nr, f);
        }
        return res;
    }
};