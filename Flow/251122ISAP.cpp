template<typename T> struct Graph {
    int n, m, tot;
    std::vector<int> h;
    struct edge {
        int n;
        T v;
        edge(int _n, T _v) : n(_n), v(_v) {};
        edge() = default;
    };
    std::vector<edge> g;
    void resize(int _n) {
        n = _n, m = 1, tot = 1;
        h.resize(n + 1, 0);
        g.resize(2);
    }
    void set_tot() {
        tot = 0;
    }
    Graph() {
        resize(0);
    }
    Graph(int _n) {
        resize(_n);
    }
    ~Graph() {
        std::vector<int>().swap(h);
        std::vector<edge>().swap(g);
        n = m = tot = 1;
    }
    void add(int u, T v) {
        ++tot; ++m;
        g.emplace_back(h[u], v);
        h[u] = tot;
    }
    T& get(int i) {
        return g[i].v;
    }
    T& rget(int i) {
        return g[i ^ 1].v;
    }
    
    struct range {
        int u;
        Graph* G;
        struct iterator {
            int i;
            Graph* G;
            int index() {
                return i;
            }
            int rindex() {
                return i ^ 1;
            }
            T& get() {
                return G -> g[i].v;
            }
            T& rget() {
                return G -> g[i ^ 1].v;
            }
            T& operator*() const {
                return G -> g[i].v;
            }
            void operator++() {
                i = G -> g[i].n;
            }
            bool operator==(const iterator rhs) const {
                return i == rhs.i;
            }
            bool operator!=(const iterator rhs) const {
                return i != rhs.i;
            }
            iterator() : i(0), G(nullptr) {}
            iterator(int _i, Graph* _G) : i(_i), G(_G) {};
        };
        iterator begin() {
            return iterator(G -> h[u], G);
        }
        iterator end() {
            return iterator(0, G);
        }
        bool empty() {
            return (G -> h[u]) == 0;
        }
        range() : u(0), G(nullptr) {};
        range(int _u, Graph* _G) : u(_u), G(_G) {};
    };
    range operator[](int o) {
        return range(o, this);
    }
    typename range::iterator iter(int i) {
        return typename range::iterator(i, this);
    }
};

constexpr long long FLOW_INF = 2e16;

long long Flow(Graph<std::pair<int, long long>>& g, int S, int T) {
    int n = g.n;
    int m = g.m;
    long long mx_flow = 0;
    std::queue<int> q;
    std::vector<int> dep(n + 1), cur(n + 1), gap(n + 2);
    // std::cerr << "G: " << gap.size() << '\n';
    auto bfs = [&]() -> bool {
        std::queue<int>().swap(q);
        for (int i = 1; i <= n; i++) {
            dep[i] = 0;
            cur[i] = g.h[i];
        }
        q.push(T);
        dep[T] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto it = g[u].begin(); it != g[u].end(); ++it) {
                auto [v, w] = it.get();
                if (w != 0 || dep[v] > 0) {
                    continue;
                }
                dep[v] = dep[u] + 1;
                q.push(v);
            }
        }
        return dep[S] > 0;
    };
    auto dfs = [&](auto self, int u, long long flow) -> long long {
        if (flow == 0 || u == T) {
            return flow;
        }
        long long usage = 0;
        for (auto it = g.iter(cur[u]); it != g[u].end(); ++it, cur[u] = it.index()) {
            auto [v, w] = it.get();
            if (w > 0 && dep[v] == dep[u] - 1) {
                long long r = self(self, v, std::min(flow - usage, w));
                it.get().second -= r;
                it.rget().second += r;
                usage += r;
                if (flow - usage == 0) {
                    return flow;
                }
            }
        }
        if (dep[S] <= n && dep[u] <= n) {
            gap[dep[u]]--;
            dep[u]++;
            gap[dep[u]]++;
            // note if u == S, the statement below shall not run before dep[u]++;
            if (gap[dep[u] - 1] == 0) {
                dep[S] = n + 1;
            }
        }
        
        return usage;
    };

    if (bfs()) {
        for (int i = 1; i <= n; i++) {
            gap[dep[i]]++;
        }
        while (dep[S] <= n) {
            for (int i = 1; i <= n; i++) {
                cur[i] = g.h[i];
            }
            mx_flow += dfs(dfs, S, FLOW_INF);
        }
    }
    return mx_flow;
}

#define MP std::make_pair