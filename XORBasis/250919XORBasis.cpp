template<typename T> struct XORBasis {
    int bit_size, size;
    std::vector<int> bit;
    // bool good_pos;
    void resize(int _bit_size) {
        size = 0;
        // good_pos = 1;
        bit_size = _bit_size;
        bit.resize(bit_size);
    }
    XORBasis(int _bit_size) {
        resize(_bit_size);
    }
    ~XORBasis() {
        std::vector<int>().swap(bit);
    }
    // void refresh() {
    //     for (int i = bit_size - 1; i >= 0; i--) {
    //         for (int j = i - 1; j >= 0; j--) {
    //             if (bit[i] >> j & 1) {
    //                 bit[i] ^= bit[j];
    //             }
    //         }
    //     }
    //     good_pos = 1;
    // }
    bool insert(T x, int index) {
        for (int i = bit_size - 1; i >= 0; i--) {
            if ((x >> i) & 1) {
                if (!bit[i]) {
                    bit[i] = x;
                    size++;
                    // good_pos = 0;
                    return true;
                } else {
                    x ^= bit[i];
                }
            } else {
                continue;
            }
        }
        return false;
    }
    T get_kth(T k) {
        assert(k >= 0 && k <= (1ll << size));
        if (k == 0) return T(-1);
        if (k == 1) return T(0);
        T res = T(0);
        T cnt = (1ll << size);
        for (int i = bit_size - 1; i >= 0; i--) {
            if (bit[i]) {
                cnt >>= 1;
                if (k > cnt) {
                    if (!(res >> i & 1)) {
                        res ^= bit[i];
                    }
                    k -= cnt;
                } else {
                    if (res >> i & 1) {
                        res ^= bit[i];
                    }
                }
            }
        }
        return res;
        // if (good_pos == 0) {
        //     refresh();
        // }
        // k--;
        // T res = T(0);
        // for (int i = 0; i < bit_size; i++) {
        //     if (!bit[i]) {
        //         continue;
        //     }
        //     if (k & 1) {
        //         res ^= bit[i];
        //     }
        //     k >>= 1;
        // }
        // return res;
    }
    T lower_bound(T x) {
        if (x < 0) {
            return T(0);
        }
        T res = T(0);
        T cnt = T(1ll << size);
        T mask = T(0);
        for (int i = bit_size - 1; i >= 0; i--) {
            if (bit[i]) {
                cnt >>= 1;
                if ((x >> i) & 1) {
                    res += cnt;
                    if (!(mask >> i & 1)) {
                        mask ^= bit[i];
                    }
                } else {
                    if (mask >> i & 1) {
                        mask ^= bit[i];
                    }
                }
            } else {
                if ((x ^ mask) >> i & 1) {
                    if (x >> i & 1) {
                        return res + cnt + 1;
                    } else {
                        return res + 1;
                    }
                }
            }
        }
        return res + 1;
    }
    // T get_rank(T x) {
    //     if (x < 0) {
    //         return T(0);
    //     }
    //     if (good_pos == 0) {
    //         refresh();
    //     }
    //     T res = T(0), now = T(x), base = T(1ll << (size - 1));
    //     for (int i = bit_size - 1; i >= 0; i--) {
    //         if (!bit[i]) {
    //             continue;
    //         }
    //         if ((now >> i) & 1) {
    //             res += base;
    //             now ^= bit[i];
    //         }
    //         base >>= 1;
    //     }
    //     return res + 1;
    // }
};