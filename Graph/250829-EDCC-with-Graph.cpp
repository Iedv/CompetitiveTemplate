struct Graph {
    int n, m, tot;
    int* h;
    struct edge {
        int n, v;
    };
    edge* g;
    void resize(int _n, int _m) {
        free(h), free(g);
        n = _n, m = _m, tot = 1;
        h = (int*) malloc((n + 1) * sizeof(int));
        g = (edge*) malloc((m * 2 + 2) * sizeof(edge));
        memset(h, 0x00, (n + 1) * sizeof(int));
    }
    void clear() {
        free(h), free(g);
        n = m = tot = 0;
    }
    Graph() : n(0), m(0), tot(0), h(nullptr), g(nullptr) {};
    Graph(int _n, int _m) {
        resize(_n, _m);
    }
    ~Graph() {
        free(h), free(g);
    }
    void add(int u, int v) {
        g[++tot] = {h[u], v};
        h[u] = tot;
        g[++tot] = {h[v], u};
        h[v] = tot;
    }
    struct range {
        int u;
        Graph* G;
        struct iterator {
            int i;
            range* R;
            int operator*() const {
                return R -> G -> g[i].v;
            }
            void operator++() {
                i = R -> G -> g[i].n;
            }
            bool operator==(const iterator rhs) const {
                return i == rhs.i;
            }
            bool operator!=(const iterator rhs) const {
                return i != rhs.i;
            }
            iterator() : i(0), R(nullptr) {}
            iterator(int _i, range* _R) : i(_i), R(_R) {};
        };
        iterator begin() {
            return iterator(G -> h[u], this);
        }
        iterator end() {
            return iterator(0, this);
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
};

struct DCC {
    Graph g;
    int n, m, tot;
    DCC(int _n, int _m) : n(_n), m(_m) {
        g.resize(n, m);
    }
    void adde(int u, int v) {
        if (u == v) return;
        g.add(u, v);
    }

    std::vector<int> dfn, low, cut, clr, stx;
    std::vector<std::vector<int> > dcc;
    int dcc_cnt, dft, stx_top;
    void tarjan(int u, int par){
        dfn[u] = low[u] = ++dft;
        stx[++stx_top] = u;
        int link_cnt = 0;
        for (auto v : g[u]) {
            if (v != par || link_cnt > 0) {
                if (dfn[v] == 0) {
                    tarjan(v, u);
                    low[u] = std::min(low[u], low[v]);
                } else if (clr[v] == 0) {
                    low[u] = std::min(low[u], dfn[v]);
                }
            } else {
                link_cnt++;
            }
        }
        if (dfn[u] == low[u]) {
            dcc_cnt++;
            int v;
            do {
                v = stx[stx_top];
                clr[v] = dcc_cnt;
                dcc[dcc_cnt].push_back(v);
                stx_top--;
            } while (v != u);
        }
    };
    void work() {
        dfn.resize(n + 1);
        low.resize(n + 1);
        clr.resize(n + 1);
        stx.resize(n + 1);
        dcc.resize(n + 1, std::vector<int>());
        dcc_cnt = 0, dft = 0, stx_top = 0;
        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) {
                tarjan(i, 0);
            }
        }
    }
    // build Tree
    void compress(Graph* res) {
        res -> resize(dcc_cnt, dcc_cnt - 1);
        for (int u = 1; u <= n; u++) {
            for (int v : g[u]) {
                if (clr[u] != clr[v] && u < v) {
                    res -> add(clr[u], clr[v]);
                }
            }
        }
    }
};