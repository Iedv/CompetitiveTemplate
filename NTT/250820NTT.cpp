long long qp(long long p, long long q, long long mod);

const int NTTN = 2.1e6;
const long long NTT_MOD = 469762049;
const long long NTT_ROOT = 3;
const long long NTT_IROOT = qp(NTT_ROOT, NTT_MOD - 2, NTT_MOD);

long long qp(long long p, long long q, long long mod) {
    long long r = 1;
    while (q) {
        if (q & 1) r = r * p % mod;
        p = p * p % mod;
        q >>= 1;
    }
    return r;
}

void NTT_sort(auto a, int sz) {
    static int *rev = (int*)malloc(sizeof(int) * NTTN);
    int hsz = sz >> 1;
    rev[0] = 0;
    for (int i = 1; i < sz; i++) {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= hsz;
        if (i < rev[i]) {
            std::swap(a[i], a[rev[i]]);
        }
    }
}

void NTT(auto a, int sz, int opt) {
    NTT_sort(a, sz);
    for (int k = 2; k <= sz; k <<= 1) {
        int dw;
        if (opt == 1) {
            dw = qp(NTT_ROOT, (NTT_MOD - 1) / k, NTT_MOD);
        } else {
            dw = qp(NTT_IROOT, (NTT_MOD - 1) / k, NTT_MOD);
        }
        int h = k >> 1;
        for (int j = 0; j < sz; j += k) {
            int w = 1;
            for (int i = j; i < j + h; i++, w = 1ll * w * dw % NTT_MOD) {
                int tmp = 1ll * w * a[h + i] % NTT_MOD;
                a[i + h] = a[i] - tmp;
                a[i + h] += (a[i + h] < 0) * NTT_MOD;
                a[i] = a[i] + tmp;
                a[i] -= (a[i] >= NTT_MOD) * NTT_MOD;
            }
        }
    }
    if (opt == -1) {
        int inv = qp(sz, NTT_MOD - 2, NTT_MOD);
        for (int i = 0; i < sz; i++) {
            a[i] = 1ll * a[i] * inv % NTT_MOD;
        }
    }
}