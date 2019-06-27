template<typename T>
struct fraction {
  T a, b;
  fraction(): a(0), b(1) {}
  fraction(T a, T b):a(a), b(b) {fix();}
  void fix() {
    T g = std::__gcd(abs(a), abs(b));
    a /= g, b /= g;
    if (b < 0) a = -a, b = -b;
  }
  bool operator == (const fraction &r) const {
    return a * r.b == b * r.a;
  }
  bool operator < (const fraction &r) const {
    return a * r.b < b * r.a;
  }
  bool operator <= (const fraction &r) const {
    return a * r.b <= b * r.a;
  }
  fraction operator + (const fraction &r) const {
    return fraction(a * r.b + b * r.a, b * r.b);
  }
  fraction operator + (const T x) const {
    return fraction(a + b * x, b);
  }
  fraction operator - (const fraction &r) const {
    return fraction(a * r.b - b * r.a, b * r.b);
  }
  fraction operator - (const T x) const {
    return fraction(a - b * x, b);
  }
  fraction operator * (const fraction &r) const {
    return fraction(a * r.a, b * r.b);
  }
  fraction operator * (const T x) const {
    return fraction(a * x, b);
  }
  fraction operator / (const fraction &r) const {
    return fraction(a * r.b, b * r.a);
  }
  fraction operator / (const T x) const {
    return fraction(a, b * x);
  }
  fraction& operator += (const fraction &r) {
    a = a * r.b + b * r.a;
    b *= r.b;
    fix();
    return *this;
  }
  fraction& operator += (const T x) {
    a += b * x;
    fix();
    return *this;
  }
  fraction& operator -= (const fraction &r) {
    a = a * r.b - b * r.a;
    b *= r.b;
    fix();
    return *this;
  }
  fraction& operator -= (const T x) {
    a -= b * x;
    fix();
    return *this;
  }
  fraction& operator *= (const fraction &r) {
    a *= r.a;
    b *= r.b;
    fix();
    return *this;
  }
  fraction& operator *= (const T x) {
    a *= x;
    fix();
    return *this;
  }
  fraction& operator /= (const fraction &r) {
    a *= r.b; b *= r.a;
    fix();
    return *this;
  }
  fraction& operator /= (const T x) {
    b *= x;
    fix();
    return *this;
  }
  // 如果乘法会爆long long, 使用下面两个函数比较大小
  static int cmp(T n1, T d1, T n2, T d2){//-1,0,1
    if (!(n1 && n2)) return n1 == n2 ? 0 : n1 < n2 ? -1 : 1;
    if (n1 > 0 ^ n2 > 0) return n1 < 0 ? -1 : 1;
    if (n1 < 0) return rc(d1, -n1, d2, -n2);
    return rc(d2, n2, d1, n1);
  }
  static int rc(T n1, T d1, T n2, T d2){
    T k1 = n1 / d1, k2 = n2 / d2;
    if (k1 != k2)return k1 < k2 ? -1 : 1;
    T r1 = n1 % d1, r2 = n2 % d2;
    if (r1 && r2) return rc(d2, r2, d1, r1);
    if (r1 && !r2) return 1;
    if (r2 && !r1) return -1;
    return 0;
  }
};
