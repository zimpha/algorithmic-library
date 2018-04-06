using uint64 = unsigned long long;
using uint128 = __uint128_t;

// \sum_{i=0}^{n-1} \lfloor \frac{a+bi}{m} \rfloor
uint128 lattice_count(uint64 n, uint64 a, uint64 b, uint64 m) {
  if (!b) return (uint128)n * (a / m);
  if (a >= m) return (uint128)n * (a / m) + lattice_count(n, a % m, b, m);
  if (b >= m) return uint128(n - 1) * n / 2 * (b / m) + lattice_count(n, a, b % m, m);
  return lattice_count((a + b * n) / m, (a + b * n) % m, m, b);
}

// \sum_{i=0}^{n-1} i^x (\lfloor \frac{a+bi}{m} \rfloor)^y
uint128 lattice_count(uint64 n, uint64 a, uint64 b, uint64 m, uint64 x, uint64 y) {
  if (x == 0 && y == 1) return lattice_count(n, a, b, m);
}
