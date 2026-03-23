template<typename T>
class DuSieve {

private:

    std::vector<T> pre_f;
    std::map<long long, T> memory;

public:

    DuSieve(std::vector<T>& _pre_f) : pre_f(_pre_f) {};

    T query(long long n, auto d_conv, auto sum_g) {
        if (n < pre_f.size()) {
            return pre_f[n];
        }
        auto k = memory.find(n);
        if (k != memory.end()) {
            return k->second;
        }
        T res = d_conv(n);
        for (long long l = 2, r = l; l <= n; l = r + 1) {
            r = n / (n / l);
            res -= query(n / l, d_conv, sum_g) * (sum_g(r) - sum_g(l - 1));
        }
        return memory[n] = res;
    }

};