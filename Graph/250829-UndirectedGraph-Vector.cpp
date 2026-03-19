struct Graph {
    int n, m;
    std::vector<std::vector<int> > adj;
    void resize(int _n, int _m) {
        n = _n, m = _m;
        adj.resize(n + 1, std::vector<int>());
    }
    Graph() {
        n = m = 0;
        adj.clear();
    }
    Graph(int _n, int _m) {
        resize(_n, _m);
    }
    void add(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    std::vector<int> operator[](int o) {
        return adj[o];
    }
};