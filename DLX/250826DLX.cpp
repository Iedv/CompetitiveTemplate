struct DLX {
    int n, m, tot, atp;
    std::vector<int> h, r, l, u, d, pn, pm, size, ans;
    DLX(int _n, int _m) : n(_n), m(_m) {
        l.resize(m + 1), r.resize(m + 1), u.resize(m + 1), d.resize(m + 1);
        size.resize(m + 1), h.resize(n + 1), pn.resize(m + 1), pm.resize(m + 1);
        ans.resize(n);
        tot = m;
        for (int i = 1; i <= m; i++) {
            r[i - 1] = i, l[i] = i - 1;
            u[i] = d[i] = i;
            pm[i] = i;
        }
        r[m] = 0, l[0] = m;
        u[0] = d[0] = 0;
    }
    void remove(int y) {
        r[l[y]] = r[y], l[r[y]] = l[y];
        for (int i = d[y]; i != y; i = d[i]) {
            for (int j = r[i]; j != i; j = r[j]) {
                d[u[j]] = d[j], u[d[j]] = u[j];
                size[pm[j]]--;
            }
        }
    }
    void recover(int y) {
        for (int i = u[y]; i != y; i = u[i]) {
            for (int j = l[i]; j != i; j = l[j]) {
                d[u[j]] = j, u[d[j]] = j;
                size[pm[j]]++;
            }
        }
        r[l[y]] = y, l[r[y]] = y;
    }
    void insert(int x, int y) {
        tot++;
        pn.push_back(x), pm.push_back(y);
        int val;
        if (h[x] == 0) {
            h[x] = tot;
            l.push_back(tot);
            r.push_back(tot);
        } else {
            val = r[h[x]];
            r.push_back(val);
            l.push_back(h[x]);
            l[r[h[x]]] = tot;
            r[h[x]] = tot;
        }
        u.push_back(y);
        u[d[y]] = tot;
        val = d[y];
        d.push_back(val);
        d[y] = tot;
        size[y]++;
    }
    bool solve(int now) {
        if (r[0] == 0) {
            return true;
        }
        if (now >= n) {
            return false;
        }
        int ch = 0;
        for (int i = r[0]; i != 0; i = r[i]) {
            if (ch == 0 || size[i] < size[ch]) {
                ch = i;
            }
        }
        remove(ch);
        for (int i = d[ch]; i != ch; i = d[i]) {
            for (int j = r[i]; j != i; j = r[j]) {
                remove(pm[j]);
            }
            ans[now] = pn[i];
            atp++;
            if (solve(now + 1)) {
                return true;
            }
            atp--;
            for (int j = l[i]; j != i; j = l[j]) {
                recover(pm[j]);
            }
        }
        recover(ch);
        return false;
    }
    bool solve() {
        atp = 0;
        return solve(0);
    }
    void print_ans() {
        for (int i = 0; i < atp; i++) {
            std::cout << ans[i] << ' ';
        }
        std::cout << '\n';
    }
};