template<int M>
struct Mint {
  int x;
  Mint(): x(0) { }
  Mint(int y) {
    x = y % M;
    if (x < 0) x += M;
  }
  Mint(long long y) {
    x = y % M;
    if (x < 0) x += M;
  }
  int get() const {return x;}
  bool operator==(const Mint &r) const {return x==r.x;}
  Mint &operator+=(const Mint &r) {if((x+=r.x)>=M) x-=M; return *this;}
  Mint &operator-=(const Mint &r) {if((x+=M-r.x)>=M) x-=M; return *this;}
  Mint &operator*=(const Mint &r) {x=(long long)x*r.x%M; return *this;}
  Mint &operator/=(const Mint &r) {x=(long long)x*r.inv().x%M; return *this;}
  Mint operator+(const Mint &r) const {return Mint(*this)+=r;}
  Mint operator-(const Mint &r) const {return Mint(*this)-=r;}
  Mint operator*(const Mint &r) const {return Mint(*this)*=r;}
  Mint operator/(const Mint &r) const {return Mint(*this)/=r;}
  Mint inv() const {
    int a = x, b = M, u = 1, v = 0;
    while(b) {
      int t = a / b;
      a -= t * b; std::swap(a, b);
      u -= t * v; std::swap(u, v);
    }
    if(u < 0) u += M;
    Mint res; res.x = (unsigned)u;
    return res;
  }
};
