#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

using int64 = long long;

namespace fft {
  const int N = 1 << 20, M = 31768;
  struct Complex {
    double x , y;
    Complex () {x = y = 0;}
    Complex (double _x , double _y) {x = _x , y = _y;}
    Complex operator + (const Complex& r) const {
      return Complex(x + r.x , y + r.y);
    }
    Complex operator - (const Complex& r) const {
      return Complex(x - r.x , y - r.y);
    }
    Complex operator * (const double k) const {
      return Complex(x * k, y * k);
    }
    Complex operator / (const double k) const {
      return Complex(x / k, y / k);
    }
    Complex operator * (const Complex& r) const {
      return Complex(x * r.x - y * r.y , x * r.y + y * r.x);
    }
    int operator = (const int a) {
      *this = Complex(a , 0);
      return a;
    }
    Complex conj() const {
      return Complex(x, -y);
    }
  };
  const double pi = acos(-1.0);
  Complex w[N];
  int rev[N];
  void init(int L) {
    int n = 1 << L;
    for (int i = 0 ; i < n ; ++ i) {
      double ang = 2 * pi * i / n;
      w[i] = Complex(cos(ang) , sin(ang));
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (L - 1));
    }
  }
  void trans(Complex P[], int n, int oper) {
    for (int i = 0 ; i < n ; i ++) {
      if (i < rev[i]) {
        std::swap(P[i], P[rev[i]]);
      }
    }
    for (int d = 0; (1 << d) < n; d++) {
      int m = 1 << d, m2 = m * 2 , rm = n / m2;
      for (int i = 0; i < n; i += m2) {
        for (int j = 0; j < m; j++) {
          Complex &P1 = P[i + j + m], &P2 = P[i + j];
          Complex t = w[rm * j] * P1;
          P1 = P2 - t;
          P2 = P2 + t;
        }
      }
    }
    if (oper == -1) {
      for (int i = 0; i < n; ++i) {
        P[i] = P[i] / n;
      }
    }
  }
  Complex A[N] , B[N] , C1[N] , C2[N];
  std::vector<int64> conv(const std::vector<int> &a, const std::vector<int> &b) {
    int n = a.size(), m = b.size(), L = 0, s = 1;
    while (s <= n + m - 2) s <<= 1, ++L;
    init(L);
    for (int i = 0; i < s; ++i) {
      A[i] = i < n ? Complex(a[i], 0) : Complex();
      B[i] = i < m ? Complex(b[i], 0) : Complex();
    }
    trans(A, s, 1);
    trans(B, s, 1);
    for (int i = 0; i < s; ++i) {
      A[i] = A[i] * B[i];
    }
    for (int i = 0; i < s; ++i) {
      w[i] = w[i].conj();
    }
    trans(A, s, -1);
    std::vector<int64> res(n + m - 1);
    for (int i = 0; i < n + m - 1; ++i) {
      res[i] = (int64)(A[i].x + 0.5);
    }
    return res;
  }
  std::vector<int64> fast_conv(const std::vector<int> &a, const std::vector<int> &b) {
    int n = a.size(), m = b.size(), L = 0, s = 1;
    for (; s <= n + m - 2; s <<= 1, ++L);
    s >>= 1, --L;
    init(L);
    for (int i = 0; i < s; ++i) {
      A[i].x = (i << 1) < n ? a[i << 1] : 0;
      B[i].x = (i << 1) < m ? b[i << 1] : 0;
      A[i].y = (i << 1 | 1) < n ? a[i << 1 | 1] : 0;
      B[i].y = (i << 1 | 1) < m ? b[i << 1 | 1] : 0;
    }
    trans(A, s, 1); trans(B, s, 1);
    for (int i = 0; i < s; ++i) {
      int j = (s - i) & (s - 1);
      C1[i] = (Complex(4, 0) * (A[j] * B[j]).conj() - (A[j].conj() - A[i]) * (B[j].conj() - B[i]) * (w[i] + Complex(1, 0))) * Complex(0, 0.25);
    }
    std::reverse(w + 1, w + s);
    trans(C1, s, -1);
    std::vector<int64> res(n + m);
    for (int i = 0; i <= (n + m - 1) / 2; ++i) {
      res[i << 1] = int64(C1[i].y + 0.5);
      res[i << 1 | 1] = int64(C1[i].x + 0.5);
    }
    res.resize(n + m - 1);
    return res;
  }
  // arbitrary modulo convolution
  void conv(int a[], int b[], int n, int m, int mod, int res[]) {
    int s = 1, L = 0;
    while (s <= n + m - 2) s <<= 1, ++L;
    init(L);
    for (int i = 0; i < s; ++i) {
      A[i] = i < n ? Complex(a[i] / M, a[i] % M) : Complex();
      B[i] = i < m ? Complex(b[i] / M, b[i] % M) : Complex();
    }
    trans(A, s, 1); trans(B, s, 1);
    for (int i = 0; i < s; ++i) {
      int j = i ? s - i : i;
      Complex a1 = (A[i] + A[j].conj()) * Complex(0.5, 0);
      Complex a2 = (A[i] - A[j].conj()) * Complex(0, -0.5);
      Complex b1 = (B[i] + B[j].conj()) * Complex(0.5, 0);
      Complex b2 = (B[i] - B[j].conj()) * Complex(0, -0.5);
      Complex c11 = a1 * b1 , c12 = a1 * b2;
      Complex c21 = a2 * b1 , c22 = a2 * b2;
      C1[j] = c11 + c12 * Complex(0, 1);
      C2[j] = c21 + c22 * Complex(0, 1);
    }
    trans(C1, s, -1); trans(C2, s, -1);
    for (int i = 0 ; i < n + m - 1; ++i) {
      int x = int64(C1[i].x + 0.5) % mod;
      int y1 = int64(C1[i].y + 0.5) % mod;
      int y2 = int64(C2[i].x + 0.5) % mod;
      int z = int64(C2[i].y + 0.5) % mod;
      res[i] = ((int64)x * M * M + (int64)(y1 + y2) * M + z) % mod;
    }
  }
}
