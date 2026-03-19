void manacher(auto str, auto d, int size, bool is_odd) {
    bool not_odd = !is_odd;
    for (int i = 0, l = 0, r = -1; i < size; i++) {
        if (i - is_odd >= r) {
            d[i] = is_odd;
        } else {
            d[i] = std::min(d[l + r - i - not_odd], r - i + is_odd);
        }
        for (int k = d[i]; i - k >= 0 && i + k + not_odd < size; k++) {
            if (str[i - k] == str[i + k + not_odd]) {
                d[i]++;
                r = i + k + not_odd;
                l = i - k;
            } else {
                break;
            }
        }
    }
}