template<typename T>
struct Point {
    T x = 0, y = 0;
    Point() : x(T(0)), y(T(0)) {};
    Point(T _x, T _y) : x(_x), y(_y) {};
    Point(const Point& o) : x(o.x), y(o.y) {};
    Point(const Point&& o) : x(o.x), y(o.y) {};
    Point operator+ (const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator- (const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
    Point& operator+= (const Point& o) {
        x += o.x;
        y += o.y;
        return *this;
    }
    Point& operator-= (const Point& o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }
    Point operator* (T rhs) const {
        return Point(x * rhs, y * rhs);
    }
    friend Point operator* (T lhs, const Point& rhs) {
        return Point(lhs * rhs.x, lhs * rhs.y);
    }
    Point& operator*= (T o) {
        x *= o;
        y *= o;
        return *this;
    }
    Point operator/ (T rhs) const {
        return Point(x / rhs, y / rhs);
    }
    Point& operator/= (T o) {
        x /= o;
        y /= o;
        return *this;
    }
    T dot(const Point& rhs) const {
        return x * rhs.x + y * rhs.y;
    }
    T operator* (const Point& rhs) const {
        return dot(rhs);
    }
    T cross(const Point& rhs) const {
        return x * rhs.y - y * rhs.x;
    }
    T operator^ (const Point& rhs) const {
        return cross(rhs);
    }
    T length() {
        return std::sqrt(x * x + y * y);
    }
    T distance(const Point& o) const {
        return ((*this) - o).length();
    }
    Point& normalize() {
        T len = length();
        x /= len;
        y /= len;
        return *this;
    }
};

template<typename T>
auto ConvexAndrew(std::vector<Point<T>>& a) {
    std::vector<int> io(a.size());
    std::iota(io.begin(), io.end(), 0);
    std::sort(io.begin(), io.end(), [&](int xx, int yy) -> bool {
        return a[xx].x == a[yy].x ? a[xx].y < a[yy].y : a[xx].x < a[yy].x;
    });
    std::vector<Point<T>> up, dn;
    for (int i = 0; i < io.size(); i++) {
        Point<T>& y = a[io[i]];
        while (up.size() >= 2) {
            Point<T>& o = up[up.size() - 2];
            Point<T>& x = up[up.size() - 1];
            if ((x - o).cross(y - o) < 0) {
                break;
            }
            up.pop_back();
        }
        up.push_back(y);
    }
    for (int i = 0; i < io.size(); i++) {
        Point<T>& y = a[io[i]];
        while (dn.size() >= 2) {
            Point<T>& o = dn[dn.size() - 2];
            Point<T>& x = dn[dn.size() - 1];
            if ((x - o).cross(y - o) > 0) {
                break;
            }
            dn.pop_back();
        }
        dn.push_back(y);
    }
    return std::make_pair(up, dn);
}

using P = Point<double>;