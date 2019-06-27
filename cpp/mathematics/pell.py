from math import sqrt
from itertools import takewhile

# count trailing zeros of n in base b
def ctz(n, b):
  r = 0
  while n % b == 0:
    r += 1
    n /= b
  return r

# return the greatest common divisor between x and y
# using Euclid's algorithm.
def gcd(x, y):
  return x if y == 0 else gcd(y, x % y)

# return a pair of coefficient (x, y) that satisfies
# Bezout's identity: ax + by = gcd(a, b).
def exgcd(a, b):
  if b == 0:
    return (1, 0)
  q, r = divmod(a, b)
  s, t = exgcd(b, r)
  return t, s - q * t

# return s such that s*s = x if x is a perfect square.
# otherwise return -1. This implementation is based on using sqrt,
# so only valid for 'small' integers.
def sqrt_int(x):
  s = int(sqrt(x))
  return s if s * s == x else -1

# return x such that x^2 = n mod p using Tonelli–Shanks algorithm
# if p is an odd prime, pow(n, (p - 1) / 2, p) = 1
# otherwise return -1
def sqrt_mod(n, p):
  n %= p
  if p == 2:
    return 1 if n == 1 else -1
  if pow(n, p / 2, p) != 1:
    return -1
  if p % 3 == 4:
    return pow(n, (p + 1) / 4, p)
  s = ctz(p - 1, 2)
  q, z, m = p >> s, 2, s
  while pow(z, p / 2, p) == 1:
    z += 1
  c, r, t = pow(z, q, p), pow(n, (q + 1) / 2, p), pow(n, q, p)
  while t != 1:
    i, tmp = 0, t
    while tmp != 1:
      tmp = tmp * tmp % p
      i += 1
    m -= 1
    while i < m:
      c = c * c % p
      m -= 1
    r = r * c % p
    c = c * c % p
    t = t * c % p
  return r

# PQa algorithm as described in: http://www.jpr2718.org/pell.pdf
# This performs the continued fraction expansion for (p0+sqrt(d))/q0
def pqa(p0, q0, d):
  sqrt_d = sqrt(d)
  assert 0 < d, "D cannot be negative or zero"
  assert q0 != 0, "Q0 cannot be zero"
  assert (p0*p0 - d) % q0 == 0, "P0^2 cannot be different from D modulo Q0"
  assert int(sqrt_d) * int(sqrt_d) != d, "D cannot be a perfect square"
  a_i, a_im = 1, 0
  b_i, b_im = 0, 1
  g_i, g_im = q0, -p0
  p_i, q_i = p0, q0
  i, ir = -1, None
  alphas = []
  while True:
    i += 1
    xi_i = (p_i + sqrt_d) / q_i
    xibar_i = (p_i - sqrt_d) / q_i
    alpha_i = int(xi_i)
    a_i, a_im = alpha_i * a_i + a_im, a_i
    b_i, b_im = alpha_i * b_i + b_im, b_i
    g_i, g_im = alpha_i * g_i + g_im, g_i
    # Everything is now as of i
    if ir == None and 1 < xi_i and -1 < xibar_i and xibar_i < 0:
      ir, p_ir, q_ir = i, p_i, q_i
    if ir != None and ir != i and p_i == p_ir and q_i == q_ir:
      break
    alphas.append(alpha_i)
    # Update p_i and q_i for i+1
    p_i = alpha_i * q_i - p_i
    q_i = (d - p_i * p_i) / q_i
  return alphas, i - ir

# Get the minimal solution for x^2 - d.y^2 = epsilon, where epsilon can be 1 or -1
def pell1_min(d, eps):
  assert eps == 1 or eps == -1, "epsilon is different from -1 and 1"
  alphas, l = pqa(0, 1, d)
  if l & 1:
    index = 2 * l - 1 if eps == 1 else l - 1
  else:
    if eps == -1: return None
    index = l - 1
  b_i, b_im = 0, 1
  g_i, g_im = 1, 0
  pl = len(alphas) - l
  for i in xrange(0, index + 1):
    alpha_i = alphas[i] if i < pl else alphas[pl + (i - pl) % l]
    b_i, b_im = alpha_i * b_i + b_im, b_i
    g_i, g_im = alpha_i * g_i + g_im, g_i
  return g_i, b_i

# Get the minimal solution for x^2 - d.y^2 = 4*epsilon, where epsilon can be 1 or -1
def pell4_min(d, eps):
  assert eps == 1 or eps == -1, "epsilon is different from -1 and 1"
  d4 = d % 4
  if d4 == 0:
    res = pell1_min(d / 4, eps)
    if res == None: return None
    return 2 * res[0], res[1]
  if d4 == 2 or d4 == 3:
    res = pell1_min(d, eps)
    if res == None: return None
    return 2 * res[0], 2 * res[1]
  assert d4 == 1, "d_mod_4 is different from one"
  alphas, l = pqa(1, 2, d)
  if l % 2 == 0 and eps == -1: return None
  b_i, b_im = 0, 1
  g_i, g_im = 2, -1
  pl = len(alphas) - l
  for i in xrange(0, l):
    alpha_i = alphas[i] if i < pl else alphas[pl + (i - pl) % l]
    b_i, b_im = alpha_i * b_i + b_im, b_i
    g_i, g_im = alpha_i * g_i + g_im, g_i
  if l % 2 == 1 and eps == 1:
    return (g_i ** 2 + b_i ** 2 * d) / 2, g_i * b_i
  return g_i, b_i

# Yield all the solutions for x^2 - d.y^2 = epsilon, where epsilon can be 1 or -1
def pell1(d, eps):
  ms = pell1_min(d, eps)
  if ms == None: return
  t, u = ms
  x, y = t, u
  n = 0
  while True:
    if eps == 1 or n % 2 == 0: yield x, y
    x, y, n = t * x + u * y * d, t * y + u * x, n + 1
# Yield all the solutions for x^2 - d.y^2 = 4*epsilon, where epsilon can be 1 or -1
def pell4(d, eps):
  ms = pell4_min(d, eps)
  if ms == None: return
  t, u = ms
  x, y = t, u
  n = 0
  while True:
    if eps == 1 or n % 2 == 0: yield x, y
    x, y, n = (t * x + u * y * d) / 2, (t * y + u * x) / 2, n + 1

# return all the fundamental solutions for the generalized pell equation: x^2 - d.y^2 = n.
def pell_funds(d, n):
  t, u = pell1_min(d, 1)
  if 0 < n:
    l1, l2 = 0, int(sqrt((n * (t - 1)) / (2.0 * d)))
  else:
    l1, l2 = int(sqrt(-n/(1.0*d))), int(sqrt((-n*(t+1))/(2.0*d)))
  funds = []
  for y in xrange(l1, l2 + 1):
    x = sqrt_int(n + d * y * y)
    if x < 0: continue
    funds.append((x, y))
    if (x * x + d * y * y) % n != 0 or (2 * x * y) % n != 0:
        funds.append((-x, y))
  return funds

# returns all the positive solutions (x, y)
# with x <= max_x for the generalized Pell equation: x^2 - d.y^2 = n.
# The fundamental solutions are generated with a brute-force approach.
def pell_bf(d, n, max_x):
  funds = pell_funds(d, n)
  sols = set()
  for x, y in funds: sols.add((abs(x), abs(y)))
  for t, u in pell1(d, 1):
    added = False
    for r, s in funds:
      x = r * t + s * u * d
      y = r * u + s * t
      if abs(x) <= max_x:
        sols.add((abs(x), abs(y)))
        added = True
    if not added: break
  sols = list(sols)
  sols.sort()
  return sols

# returns all the positive solutions (x, y) with x <= max_x for equation ax^2 - by^2 = c.
def quad_s(a, b, c, max_x):
  res = []
  for x, y in pell_bf(a * b, a * c, a * max_x):
    x, rem = divmod(x, a)
    if rem == 0:
      res.append((x, y))
  return res

'''
print pqa(11, 108, 13)      # ([0, 7, 2, 1, 1, 6, 1, 1], 5)
print pqa(0, 1, 13)         # (([3, 1, 1, 1, 1, 6], 5)
print pell1_min(13, 1)      # (649, 180)
print pell1_min(13, -1)     # (18, 5)
print pell4_min(13, 1)      # (11, 3)
print pell4_min(13, -1)     # (3, 1)
print sqrt_mod(10, 13)      # 7

print list(takewhile(lambda (x, _): x < 1000000, pell1(13, 1)))
print list(takewhile(lambda (x, _): x < 1000000, pell1(13, -1)))

print list(takewhile(lambda (x, _): x < 10000, pell4(13, 1)))
print list(takewhile(lambda (x, _): x < 10000, pell4(13, -1)))

print pell_funds(13, 108)
print pell_funds(157, 12)
print pell_funds(13, 27)

print pell_bf(13, 108, 1000)
print pell_bf(157, 12, 10 ** 8)
print pell_bf(13, 27, 1000)

print quad_s(8, 9, 72, 1000)
'''
