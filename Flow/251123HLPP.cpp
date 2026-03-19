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

long long Flow(Graph<std::pair<int, long long>>& g, int S, int T) {
    int n = g.n, m = g.m;
    int nowh = 0;
    std::queue<int> q;
    std::vector<int> dep(n + 1, n + 1), gap(n + 2);
    std::vector<long long> extra(n + 1);
    std::vector<std::stack<int>> B(n + 2);
    auto update_gap = [&](int pos, int val) -> void {
        if (pos > n) {
            return;
        }
        gap[pos] += val;
        if (gap[pos] <= 0) {
            for (int i = 1; i <= n; i++) {
                if (i != S && i != T && dep[i] > pos) {
                    dep[i] = n + 1;
                }
            }
        }
    };
    auto push = [&](int u) -> bool {
        if (extra[u] <= 0) {
            return 0;
        }
        for (auto it = g[u].begin(); it != g[u].end(); ++it) {
            auto& [v, w] = it.get();
            if (w > 0 && dep[v] == dep[u] - 1) {
                long long ff = std::min(extra[u], w);
                if (v != S && v != T && extra[v] == 0 && dep[v] <= n) {
                    B[dep[v]].push(v);
                    nowh = std::max(nowh, dep[v]);
                }
                extra[u] -= ff;
                extra[v] += ff;
                it.rget().second += ff;
                w -= ff;
                if (extra[u] <= 0) {
                    return 0;
                }
            }
        }
        return 1;
    };
    auto relabel = [&](int u) -> void {
        int& minh = dep[u];
        update_gap(minh, -1);
        minh = n + 1;
        for (auto& [v, w] : g[u]) {
            if (w > 0) {
                minh = std::min(minh, dep[v]);
            }
        }
        minh++;
        update_gap(minh, 1);
        if (minh <= n) {
            B[minh].push(u);
            nowh = std::max(nowh, minh);
        }
    };
    auto bfs = [&]() -> bool {
        q.push(T);
        dep[T] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto it = g[u].begin(); it != g[u].end(); ++it) {
                auto [v, w] = it.get();
                if (w == 0 && dep[v] == n + 1) {
                    dep[v] = dep[u] + 1;
                    q.push(v);
                }
            }
        }
        return dep[S] != n + 1;
    };
    if (bfs()) {
        for (int i = 1; i <= n; i++) {
            if (dep[i] <= n) {
                gap[dep[i]]++;
            }
        }
        gap[dep[S]]--;
        dep[S] = n;
        gap[n]++;
        for (auto it = g[S].begin(); it != g[S].end(); ++it) {
            auto& [v, w] = it.get();
            if (w > 0) {
                long long ff = w;
                if (v != S && v != T && extra[v] == 0 && dep[v] <= n) {
                    B[dep[v]].push(v);
                    nowh = std::max(nowh, dep[v]);
                }
                extra[v] += ff;
            }
        }
        nowh = n;
        while (true) {
            for (; nowh > 0 && B[nowh].size() == 0; nowh--) {};
            if (nowh == 0) {
                break;
            }
            int u = B[nowh].top();
            B[nowh].pop();
            if (push(u)) {
                relabel(u);          
            }
        }
    }
    return extra[T];
}

#define MP std::make_pair