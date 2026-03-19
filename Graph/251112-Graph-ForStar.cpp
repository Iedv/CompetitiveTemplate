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