#include "basic.hpp"
#include <vector>

// 传入y = f(x)上的n个点, 拟合出对应的一元n-1次方程, 返回各项系数
// 类型需支持加, 减, 乘, 除, 取反, 加等于这六种操作
template<typename T>
std::vector<T> interpolation(const T x[], const T y[], int n){
  std::vector<T> u(y, y + n), ret(n), sum(n);
  ret[0] = u[0], sum[0] = 1;
  for (int i = 1; i < n; ++i) {
    for (int j = n - 1; j >= i; --j) {
      u[j] = (u[j] - u[j - 1]) / (x[j] - x[j - i]);
    }
    for (int j = i; j; --j) {
      sum[j] = -sum[j] * x[i - 1] + sum[j - 1];
      ret[j] += sum[j] * u[i];
    }
    sum[0] = -sum[0] * x[i - 1];
    ret[0] += sum[0] * u[i];
  }
  return ret;
}
