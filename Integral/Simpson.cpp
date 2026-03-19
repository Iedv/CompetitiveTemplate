template<typename T>
T simpson(auto&& f, T L, T R) {

    static constexpr T SIMPSON_EPS = 1e-9;
    static constexpr T SIMPSON_MIN_DEPTH = 15;

    auto integral = [&](T l, T r) -> T {
        return (r - l) / 6 * (f(l) + f(r) + f((l + r) / 2) * 4);
    };

    auto work = [&](auto&& self, T l, T r, T res, int dep) -> T {
        T mid = (l + r) / 2;
        T lr = integral(l, mid), rr = integral(mid, r);
        if (std::abs(lr + rr - res) < SIMPSON_EPS && dep >= SIMPSON_MIN_DEPTH) {
            return res;
        }
        return self(self, l, mid, lr, dep + 1) + self(self, mid, r, rr, dep + 1);
    };

    return work(work, L, R, 0, 0);
}