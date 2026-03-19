#include <bits/stdc++.h>

const long long INF = 2e16;

struct Min {
    int val;
    Min() {
        val = INF;
    }
    Min(int _val) : val(_val) {};
    friend Min operator+ (Min x, Min y) {
        return x.val < y.val ? x : y;
    }
    friend Min operator* (int k, Min x) {
        return x;
    }
};

/**
 * type T should define + as a combine, and integar * ;
 */
template<typename T>
struct SegTree {
    int size;
    std::vector<T> ds;
    std::vector<T> tag;

    void resize(int _size) {
        size = _size;
        ds.resize(size * 4 + 1, T());
        tag.resize(size * 4 + 1, T());
    }
    /**
     * build a segtree of size n with each element T();
     */
    SegTree(int _size) {
        resize(_size);
    }
    void pull(int p) {
        ds[p] = F(ds[p << 1], ds[p << 1 | 1]);
    }
    void init(std::vector<int> a) {
        resize(a.size());
        auto build = [&](auto self, int p, int l, int r) -> void {
            if (l == r) {
                ds[p] = a[l - 1];
                return;
            }
            int mid = (l + r) >> 1;
            self(self, p << 1, l, mid);
            self(self, p << 1 | 1, mid + 1, r);
            pull(p);
        };
        build(build, 1, 1, a.size());
    }
    /**
     * build a segtree with array;
     */
    SegTree(std::vector<int> _init_array) {
        init(_init_array);
    }
    void apply(int p, T val) {

    }
    void push(int p) {

    }
};