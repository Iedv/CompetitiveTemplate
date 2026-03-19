template<typename T> struct SegTree {
    long long clamp(long long l, long long r, long long x) {
        if (x < l) x = l;
        if (x > r) x = r;
        return x;
    }
    int size, bias;
    T* ds;
    void build(int p, int l, int r) {
        ds[p] = T(INF);
        if (l == r) {
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid);
        build(p << 1 | 1, mid + 1, r);
    }
    void resize(int _size) {
        size = _size;
        ds = (T*) malloc((4 * size + 10) * sizeof(T));
        build(1, 1, size);
    }
    void reset() {
        free(ds);
        size = 0;
    }
    void push_up(int p) {
        ds[p] = std::min(ds[p << 1], ds[p << 1 | 1]);
    }
    void point_update(int p, int l, int r, int pos, T val) {
        if (l == r) {
            ds[p] = std::min(ds[p], val);
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            point_update(p << 1, l, mid, pos, val);
        } else {
            point_update(p << 1 | 1, mid + 1, r, pos, val);
        }
        push_up(p);
    }
    T range_query(int p, int l, int r, int nl, int nr) {
        if (nl <= l && r <= nr) {
            return ds[p];
        }
        int mid = (l + r) >> 1;
        T res = T(INF);
        if (nl <= mid) {
            res = std::min(res, range_query(p << 1, l, mid, nl, nr));
        }
        if (nr > mid) {
            res = std::min(res, range_query(p << 1 | 1, mid + 1, r, nl, nr));
        }
        return res;
    }
    SegTree(int _min, int _max) {
        bias = 1 - _min;
        size = _max - _min + 1;
        resize(size);
    }
    void point_update(int pos, T val) {
        pos = clamp(1, size, bias + pos);
        point_update(1, 1, size, pos, val);
    }
    T range_query(int l, int r) {
        l = clamp(1, size, l + bias);
        r = clamp(1, size, r + bias);
        if (l > r) return T(INF);
        return range_query(1, 1, size, l, r);
    }
};