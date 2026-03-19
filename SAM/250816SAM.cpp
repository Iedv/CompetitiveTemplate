template<typename T> struct SAM {
    // custom info : cnt, 
    struct state {
        int len, link, cnt;
        std::map<T, int> next;
        state() {
            len = 0, link = -1, cnt = 0;
            next.clear();
        }
    };
    std::vector<state> st;
    int tot, size, last;
    SAM(int _size) : size(_size) {
        st.resize(size * 2 + 5, state());
        tot = 1;
        last = 0;
    }
    void extend(T c) {
        int cur = tot++;
        st[cur].cnt = 1;
        st[cur].len = st[last].len + 1;
        int p = last;
        while (p != -1 && !st[p].next[c]) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = tot++;
                st[clone].len = st[p].len + 1;
                st[clone].next = st[q].next;
                st[clone].link = st[q].link;
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }
    // extra info below
    std::vector<std::vector<int> > g;
    void build_parent_tree() {
        g.resize(tot);
        for (int i = 1; i < tot; i++) {
            if (st[i].link != -1) {
                g[st[i].link].push_back(i);
            }
        }
        auto update_cnt = [&](auto self, int u) -> void {
            for (int v : g[u]) {
                self(self, v);
                st[u].cnt += st[v].cnt;
            }
        };
        update_cnt(update_cnt, 0);
    }
};