#include <cstdio>
#include <ctime>
#include <utility>
#include <vector>

#include "vector.hpp"

class test {
 public:
  int a, b, c;
  [[maybe_unused]] test() {}
  [[maybe_unused]] test(int &_a, int &_b, int &_c) : a(_a), b(_b), c(_c) {}
  [[maybe_unused]] test(int &&_a, int &&_b, int &&_c)
      : a(std::move(_a)), b(std::move(_b)), c(std::move(_c)) {}
};

int main() {
  int i;
  time_t st;

  st = clock();
  lni::vector<int64_t> v1;
  for (i = 0; i < 10000000; ++i) v1.emplace_back(i);
  printf("lni::vector %.3fs\n", (double)(clock() - st) / CLOCKS_PER_SEC);
#ifdef DEBUG

  puts("\n=== Debugging Messages ===\n");

  std::cout << "max_size(): " << v1.max_size() << std::endl;
  puts("\n");

  puts("Testing emplace and emplace_back ... ");
  lni::vector<test> vt;
  vt.emplace_back(test(1, 2, 3));
  vt.emplace_back(2, 4, 6);
  vt.emplace(vt.begin() + 1, 1, 3, 5);
  for (auto &vti : vt) printf(" %d %d %d\n", vti.a, vti.b, vti.c);
  puts("\n");

  puts("Testing resize ... ");
  v1.resize(5);
  for (auto &n : v1) printf(" %lld", n);
  puts("");
  v1.resize(10, 10);
  for (auto &n : v1) printf(" %lld", n);
  puts("\n");

  puts("Testing assign ... ");
  lni::vector<int64_t> v1_a;
  v1_a.assign(v1.begin() + 1, v1.begin() + 3);
  for (auto &n : v1_a) printf(" %lld", n);
  puts("\n");

  puts("Testing front and back ... ");
  printf(" %lld %lld", v1.front(), v1.back());
  puts("\n");

  puts("Testing pop_back ... ");
  v1.pop_back();
  for (auto &n : v1) printf(" %lld", n);
  puts("\n");

  puts("Testing [] operator overloading ... ");
  v1[0] = 1;
  auto sz = v1.size();
  for (lni::vector<int64_t>::size_type j = 0; j < static_cast<lni::vector<int64_t>::size_type>(sz);
       ++j) {
    printf(" %lld", v1[j]);
  }

  puts("\n");

  puts("Testing initializer list ... ");
  lni::vector<int64_t> v2 = {10, 11, 12, 13, 14};
  for (auto &n : v2) printf(" %lld", n);
  puts("\n");

  puts("Testing swap ... ");
  v1.swap(v2);
  for (auto &n : v1) printf(" %lld", n);
  puts("");
  for (auto &n : v2) printf(" %lld", n);
  puts("\n");

  puts("Testing clear ... ");
  v1.clear();
  for (auto &n : v1) printf(" %lld", n);
  puts("\n");

  puts("Testing constructors ... ");
  lni::vector<double> v3(5, 2.0);
  v3[0] = 1.0;
  v3[4] = 3.0;
  for (auto &f : v3) printf(" %.3f ", f);
  puts("");
  lni::vector<double> v4(v3);
  for (auto &f : v4) printf(" %.3f ", f);
  puts("\n");

  puts("Testing assignment operators ... ");
  lni::vector<double> v5 = v3;
  for (auto &f : v5) printf(" %.3f", f);
  puts("");
  lni::vector<double> v6 = {3.0, 2.0, 2.0, 2.0, 1.0};
  for (auto &f : v6) printf(" %.3f", f);
  puts("\n");

  puts("Testing insert ... ");
  auto it = v6.insert(v6.begin() + 1, 2.5);
  printf(" Just inserted: %.3f\n", *it);
  it = v6.insert(v6.begin(), 4.0);
  printf(" Just inserted: %.3f\n", *it);
  lni::vector<double> vins{-2.0, -3.0};
  it = v6.insert(v6.begin() + 1, vins.begin(), vins.end());
  printf(" Just inserted: %.3f %.3f\n", *it, *(it + 1));
  for (auto &f : v6) printf(" %.3f", f);
  puts("\n");

  it = v6.insert(v6.end(), 2, 0.0);
  it = v6.insert(v6.end() - 2, 3, 0.5);
  it = v6.insert(v6.begin(), {1.0, 2.0, 3.0, 4.0, 5.0});
  printf(" v6:");
  for (auto &f : v6) printf(" %.3f", f);
  puts("");

  lni::vector<double> v7(2, 5.0);
  v7.insert(v7.end(), v6.begin(), v6.end());
  for (auto &f : v7) printf(" %.3f", f);
  puts("");

  v7.insert(v7.begin(), {8.0, 7.0, 6.0});
  v7.insert(v7.end(), {-1.0, -2.0, -2.5});
  for (auto &f : v7) printf(" %.3f", f);
  puts("\n");

  puts("Testing erase ... ");
  v7.erase(v7.begin());
  v7.erase(v7.begin() + 2, v7.begin() + 4);
  printf(" v7:");
  for (auto &f : v7) printf(" %.3f", f);
  puts("\n");

  puts("Testing shrink_to_fit ... ");
  printf("Before: %zu, %zu\n", v7.size(), v7.capacity());
  v7.shrink_to_fit();
  printf("After: %zu, %zu\n", v7.size(), v7.capacity());
  puts("");

  puts("Testing operators ... ");
  lni::vector<double> v8(v7);
  printf(" v6 == v8: %d\n", v6 == v8);
  printf(" v6 != v8: %d\n", v6 != v8);
  printf(" v7 == v8: %d\n", v7 == v8);
  printf(" v7 != v8: %d\n", v7 != v8);

  printf("\n v6 < v8: %d\n", v6 < v8);
  printf(" v6 <= v8: %d\n", v6 <= v8);
  printf(" v8 < v6: %d\n", v8 < v6);
  printf(" v7 <= v8: %d\n", v7 <= v8);
  printf(" v8 < v7: %d\n", v8 < v7);

  printf("\n v6 >= v8: %d\n", v6 >= v8);
  printf(" v6 > v8: %d\n", v6 > v8);
  printf(" v8 >= v6: %d\n", v8 >= v6);
  printf(" v7 > v8: %d\n", v7 > v8);
  printf(" v8 >= v7: %d\n", v8 >= v7);
  puts("\n");

  puts("Testing reverse_iterator ... ");
  for (auto rit = v8.rbegin(); rit != v8.rend(); ++rit) printf(" %.3f", *rit);
  puts("\n");

  puts("Testing issue #4 #5 on github.com/lnishan/vector ... ");
  lni::vector<int64_t> v9(4, 5);
  puts(" v9:");
  for (auto &n : v9) printf(" %lld", n);
  puts("");
  v9.assign(8, 7);
  puts(" v9 (after assign(8, 7)):");
  for (auto &n : v9) printf(" %lld", n);
  puts("\n");

#endif

  st = clock();
  std::vector<int> std_v1;
  for (i = 0; i < 10000000; ++i) std_v1.emplace_back(i);
  printf("std::vector %.3fs\n", (double)(clock() - st) / CLOCKS_PER_SEC);

  return 0;
}
