// Using Matrix
// A produce triple with b + 1 = c
// B produce triple with |a - b| = 1
// C produce triple with a + 2 = c
const int A[3][3] = {{1,-2,2},{2,-1,2},{2,-2,3}};
const int B[3][3] = {{1,2,2},{2,1,2},{2,2,3}};
const int C[3][3] = {{-1,2,2},{-2,1,2},{-2,2,3}};
//传入{3,4,5}, 枚举所有互素的勾股数对, 改成bfs可能会快一点
void dfs(int a[3]) {
  int b[3];
  for (int i=0;i<3;++i)b[i]=A[i][0]*a[0]+A[i][1]*a[1]+A[i][2]*a[2];
  dfs(b);
  for (int i=0;i<3;++i)b[i]=B[i][0]*a[0]+B[i][1]*a[1]+B[i][2]*a[2];
  dfs(b);
  for (int i=0;i<3;++i)b[i]=C[i][0]*a[0]+C[i][1]*a[1]+C[i][2]*a[2];
  dfs(b);
}

// Using formula 1
// a = m^2 - n^2, b = 2mn, c = m^2 + n^2
// m > n > 0, gcd(n, m) = 1, of opposite parity

// Using formula 2
// a = uv, b = (u^2 - v^2) / 2, c = (u^2 + v^2) / 2
// u > v > 0,m gcd(u, v) = 1, u and v are odd

// A = {{2, -1}, {1, 0}}, B = {{2, 1}, {1, 0}}, C = {{1, 2}, {0, 1}}
// using above matrix to generate (m, n) or (u, v)
