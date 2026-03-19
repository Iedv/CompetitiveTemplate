template<typename T> struct Fenwick {
	T *ds;
	int size = 0, bias = 0;
    void resize(int _size) {
        size = _size;
        ds = (T*) malloc((size + 2) * sizeof(T));
        for (int i = 0; i <= size; i++) ds[i] = 0;
    }
	void add(int pos, T val) {
        pos = pos + bias;
        for (int i = pos; i <= size; i += i & -i) {
            ds[i] += val;
		}
	}
	T query(int pos) {
        pos = bias + pos;
        if (pos <= 0) return T();
        T res = 0;
		for (int i = pos; i; i -= i &- i) {
            res += ds[i];
		}
		return res;
	}
    T query(int l, int r) {
        if (l > r) return T();
        else return query(r) - query(l - 1);
    }
    Fenwick(int _size) {
        bias = 0;
        resize(_size);
    }
    Fenwick(int _min, int _max) {
        bias = 1 - _min;
        size = _max - _min + 1;
        resize(size);
    }
};