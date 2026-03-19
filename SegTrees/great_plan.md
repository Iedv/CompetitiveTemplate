基础线段树：

二元运算 $\cdot$
（幺）半群，满足封闭性，结合律，

1. 通过数组构建，需要结合律（或单位元）

```cpp
void build(int p, int l, int r) {
    if (l == r) {
        ds[p] = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build (ls(p), l, mid);
    build (rs(p), mid + 1, r);
    ds[p] = f(ds[ls(p)], ds[rs(p)]);
}
```

2. 区间修改：给出 $l,r,v$，实现 $\forall i\in[l,r],seg_i\gets seg_i\cdot v$

3. 区间查询：给出 $l,r$，求 $seg_l\cdot seg_{l+1}\cdot...\cdot seg_r$

4. 区间赋值：给出 $l,r,v$，实现 $\forall i\in[l,r],seg_i\gets v$

eg 