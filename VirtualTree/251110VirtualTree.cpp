std::vector<int> a(K);
        std::sort(K.begin(), K.end(), [&](int x, int y) -> bool {
            return dfn[x] < dfn[y];
        });
        stx[stx_top = 1] = 1;
        a.push_back(1);
        for (int xx : K) {
            if (xx == 1) {
                continue;
            }
            int ll = lca(stx[stx_top], xx);
            if (ll == stx[stx_top]) {
                stx[++stx_top] = xx;
            } else {
                while (dfn[stx[stx_top - 1]] > dfn[ll]) {
                    gg[stx[stx_top - 1]].emplace_back(stx[stx_top], mw[stx[stx_top]]);
                    stx_top--;
                }
                if (stx[stx_top - 1] == ll) {
                    gg[stx[stx_top - 1]].emplace_back(stx[stx_top], mw[stx[stx_top]]);
                    stx[stx_top] = xx;
                } else {
                    gg[ll].emplace_back(stx[stx_top], mw[stx[stx_top]]);
                    stx[stx_top] = ll;
                    stx[++stx_top] = xx;
                    a.push_back(ll);
                }
            }
        }
        for (; stx_top > 1; stx_top--) {
            gg[stx[stx_top - 1]].emplace_back(stx[stx_top], mw[stx[stx_top]]);
        }
        // for (int i = 1; i < m + 1; i++) {
        //     // std::cout << "AVO: " << ' ' << K[i - 1] << ' ' << K[i] << ' ' << lca(K[i - 1], K[i]) << std::endl;
        //     K.push_back(lca(K[i - 1], K[i]));
        // }
        // std::sort(K.begin(), K.end(), [&](int x, int y) -> bool {
        //     return dfn[x] < dfn[y];
        // });
        // int tot = std::unique(K.begin(), K.end()) - K.begin();
        // // std::cout << tot << '\n';
        // std::set<int> td;
        // for (int i = 1; i < tot; i++) {
        //     int u = lca(K[i - 1], K[i]);
        //     int v = K[i];
        //     td.insert(u);
        //     td.insert(v);
        //     gg[u].emplace_back(v, mw[v]);
        //     gg[v].emplace_back(u, mw[v]);
        // }