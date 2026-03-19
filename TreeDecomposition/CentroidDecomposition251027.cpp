// template of centroid decomposition
    std::vector<int> vis(n + 1), siz(n + 1);
    // add something...
    // {BEGIN}
    std::vector<int> dis(n + 1);
    // {END}

    /**
     * Get Centroid of a subtree (bounded by vis=0)
     */
    auto get_centroid = [&](int x, int tot_size) -> int {
        int mn = tot_size - 1;
        int res = x;
        auto cdfs = [&](auto self, int u, int par) -> void {
            if (vis[u]) return;
            siz[u] = 1;
            int mx_siz = 0;
            for (auto [v, w] : g[u]) {
                if (v != par && ! vis[v]) {
                    self(self, v, u);
                    siz[u] += siz[v];
                    mx_siz = std::max(mx_siz, siz[v]);
                }
            }
            mx_siz = std::max(mx_siz, tot_size - siz[u]);
            if (mx_siz < mn) {
                mn = mx_siz;
                res = u;
            }
        };
        cdfs(cdfs, x, 0);
        return res;
    };

    auto work = [&](auto self, int x, int sz) -> void {
        x = get_centroid(x, sz);
        dis[x] = 0;
        // do something...
        // {BEGIN}
        std::stack<int> to_reset;
        auto dfs1 = [&](auto self, int u, int par) -> void {
            if (vis[u]) return;
            siz[u] = 1;
            for (int i = 0; i < m; i++) {
                if (!ans[i] && q[i] >= dis[u]) {
                    if (d.test(q[i] - dis[u])) {
                        ans[i] = 1;
                    }
                }
            }
            for (auto [v, w] : g[u]) {
                if (v != par && !vis[v]) {
                    dis[v] = dis[u] + w;
                    self(self, v, u);
                    siz[u] += siz[v];
                }
            }
        };
        auto dfs2 = [&](auto self, int u, int par) -> void {
            if (vis[u]) return;
            d.set(dis[u]);
            to_reset.push(dis[u]);
            for (auto [v, w] : g[u]) {
                if (v != par && !vis[v]) {
                    self(self, v, u);
                }
            }
        };
        for (auto [v, w] : g[x]) {
            dis[v] = w;
            dfs1(dfs1, v, x);
            dfs2(dfs2, v, x);
        }
        while (!to_reset.empty()) {
            d.set(to_reset.top(), 0);
            to_reset.pop();
        }
        // {END}
        vis[x] = 1;
        for (auto [v, w] : g[x]) {
            if (!vis[v]) {
                self(self, v, siz[v]);
            }
        }
    };

    work(work, 1, n);