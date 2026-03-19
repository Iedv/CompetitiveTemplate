// use SegTree template
// this is a part of a template problem

long long MOD;

void solve() {
    int n, m, root;
    std::cin >> n >> m >> root >> MOD;

    struct Policy {
        using T = i64;
        using H = i64;
        T op(std::pair<const T&, i64> x, std::pair<const T&, i64> y) {
            return (x.first + y.first) % MOD;
        }
        T map(std::pair<const T&, i64> x, const H& y) {
            return (x.first + x.second * y) % MOD;
        }
        H comp(const H& x, const H& y) {
            return (x + y) % MOD;
        }
        T e() {
            return 0;
        }
        H id() {
            return 0;
        }
    };

    std::vector<i64> val(n + 1);
    std::vector<int> dfn(n + 1), rdfn(n + 1), sz(n + 1), top(n + 1), dep(n + 1), lst(n + 1), up(n + 1);
    for (int i = 1 ; i <= n; i++) {
        std::cin >> val[i];
        val[i] %= MOD;
    }
    std::vector<std::vector<int>> g(n + 1);
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto dfs1 = [&](auto&& self, int u, int par) -> void {
        sz[u]++;
        dep[u] = dep[par] + 1;
        for (int v : g[u]) {
            if (v != par) {
                self(self, v, u);
                sz[u] += sz[v];
            }
        }
    };
    dfs1(dfs1, root, 0);
    for (int i = 1; i <= n; i++) {
        std::sort(g[i].begin(), g[i].end(), [&](int x, int y) -> bool {
            return sz[x] > sz[y];
        });
    }
    int dft = 0;
    top[root] = root;
    auto dfs2 = [&](auto&& self, int u, int par) -> void {
        dfn[u] = ++dft;
        rdfn[dfn[u]] = u;
        up[u] = par;
        bool flag = 0;
        for (int v : g[u]) {
            if (v != par) {
                if (!flag) {
                    top[v] = top[u];
                    flag = 1;
                } else {
                    top[v] = v;
                }
                self(self, v, u);
            }
        }
        lst[u] = dft;
    };
    dfs2(dfs2, root, 0);
    std::vector<i64> a(n + 1);
    for (int i = 1; i <= n; i++) {
        a[i] = val[rdfn[i]];
    }
    SegTree<Policy> ds(a, 1, n);
    auto UpdatePath = [&](int x, int y, i64 z) -> void {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) {
                std::swap(x, y);
            }
            ds.apply(dfn[top[x]], dfn[x], z);
            x = up[top[x]];
        }
        if (dep[x] < dep[y]) {
            std::swap(x, y);
        }
        ds.apply(dfn[y], dfn[x], z);
    };
    auto QueryPath = [&](int x, int y) -> Policy::T {
        static Policy policy;
        Policy::T res = policy.e();
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) {
                std::swap(x, y);
            }
            res = policy.op({res, 0}, {ds.query(dfn[top[x]], dfn[x]), 0});
            x = up[top[x]];
        }
        if (dep[x] < dep[y]) {
            std::swap(x, y);
        }
        res = policy.op({res, 0}, {ds.query(dfn[y], dfn[x]), 0});
        return res;
    };
    while (m --> 0) {
        int op;
        std::cin >> op;
        if (op == 1) {
            int x, y; i64 z;
            std::cin >> x >> y >> z;
            UpdatePath(x, y, z);
        } else if (op == 2) {
            int x, y;
            std::cin >> x >> y;
            std::cout << QueryPath(x, y) << '\n';
        } else if (op == 3) {
            int x; i64 z;
            std::cin >> x >> z;
            ds.apply(dfn[x], lst[x], z);
        } else {
            int x;
            std::cin >> x;
            std::cout << ds.query(dfn[x], lst[x]) << '\n';
        }
    }
}