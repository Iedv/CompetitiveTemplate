struct DSU {
    int size = 0;
    std::vector<int> ds;
    void resize(int _size) {
        size = _size;
        ds.resize(size + 1);
        for (int i = 1; i <= size; i++) {
            ds[i] = i;
        }
    }
    DSU(int _size) {
        resize(_size);
    }
    DSU() = default;
    ~DSU() {
        std::vector<int>().swap(ds);
    }
    int find(int x) {
        return ds[x] == x ? x : ds[x] = find(ds[x]);
    }
    void join(int _from, int _to) {
        int rf = find(_from);
        int rt = find(_to);
        if (rf != rt) {
            ds[rf] = rt;
        }
    }
};