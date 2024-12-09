#include <ctime>
#include <iomanip>
#include <iostream>
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
  std::cout << "lni::vector " << std::fixed << (double)(clock() - st) / CLOCKS_PER_SEC << "s\n";

#ifndef DEBUG

  std::cout << "\n=== Debugging Messages ===\n\n";

  std::cout << "max_size(): " << v1.max_size() << "\n\n";

  std::cout << "Testing emplace and emplace_back ...\n";
  lni::vector<test> vt;
  vt.emplace_back(test(1, 2, 3));
  vt.emplace_back(2, 4, 6);
  vt.emplace(vt.begin() + 1, 1, 3, 5);
  for (const auto &vti : vt) std::cout << " " << vti.a << " " << vti.b << " " << vti.c << "\n";
  std::cout << "\n";

  std::cout << "Testing resize ...\n";
  v1.resize(5);
  for (const auto &n : v1) std::cout << " " << n;
  std::cout << "\n";
  v1.resize(10, 10);
  for (const auto &n : v1) std::cout << " " << n;
  std::cout << "\n\n";

  std::cout << "Testing assign ...\n";
  lni::vector<int64_t> v1_a;
  v1_a.assign(v1.begin() + 1, v1.begin() + 3);
  for (const auto &n : v1_a) std::cout << " " << n;
  std::cout << "\n\n";

  std::cout << "Testing front and back ...\n";
  std::cout << " " << v1.front() << " " << v1.back() << "\n\n";

  std::cout << "Testing pop_back ...\n";
  v1.pop_back();
  for (const auto &n : v1) std::cout << " " << n;
  std::cout << "\n\n";

  std::cout << "Testing [] operator overloading ...\n";
  v1[0] = 1;
  auto sz = v1.size();
  for (lni::vector<int64_t>::size_type j = 0; j < static_cast<lni::vector<int64_t>::size_type>(sz);
       ++j) {
    std::cout << " " << v1[j];
  }
  std::cout << "\n\n";

  std::cout << "Testing initializer list ...\n";
  lni::vector<int64_t> v2 = {10, 11, 12, 13, 14};
  for (const auto &n : v2) std::cout << " " << n;
  std::cout << "\n\n";

  std::cout << "Testing swap ...\n";
  v1.swap(v2);
  for (const auto &n : v1) std::cout << " " << n;
  std::cout << "\n";
  for (const auto &n : v2) std::cout << " " << n;
  std::cout << "\n\n";

  std::cout << "Testing clear ...\n";
  v1.clear();
  for (const auto &n : v1) std::cout << " " << n;
  std::cout << "\n\n";

  std::cout << "Testing constructors ...\n";
  lni::vector<double> v3(5, 2.0);
  v3[0] = 1.0;
  v3[4] = 3.0;
  for (const auto &f : v3) std::cout << " " << f;
  std::cout << "\n";
  lni::vector<double> v4(v3);
  for (const auto &f : v4) std::cout << " " << f;
  std::cout << "\n\n";

  std::cout << "Testing assignment operators ...\n";
  lni::vector<double> v5 = v3;
  for (const auto &f : v5) std::cout << " " << f;
  std::cout << "\n";
  lni::vector<double> v6 = {3.0, 2.0, 2.0, 2.0, 1.0};
  for (const auto &f : v6) std::cout << " " << f;
  std::cout << "\n\n";

  std::cout << "Testing insert ... " << std::endl;
  std::cout << "1Size of v6: " << v6.size() << ", Capacity: " << v6.capacity() << std::endl;
  auto it = v6.insert(v6.begin() + 1, 2.5);
  std::cout << " Just inserted: " << std::fixed << std::setprecision(3) << *it << std::endl;
  std::cout << "2Size of v6: " << v6.size() << ", Capacity: " << v6.capacity() << std::endl;
  it = v6.insert(v6.begin(), 4.0);
  std::cout << " Just inserted: " << std::fixed << std::setprecision(3) << *it << std::endl;
  std::cout << "3Size of v6: " << v6.size() << ", Capacity: " << v6.capacity() << std::endl;
  lni::vector<double> vins{-2.0, -3.0};
  it = v6.insert(v6.begin() + 1, vins.begin(), vins.end());
  std::cout << " Just inserted: " << *it << " " << *(it + 1) << std::endl;
  std::cout << "4Size of v6: " << v6.size() << ", Capacity: " << v6.capacity() << std::endl;
  for (auto &f : v6) std::cout << " " << std::fixed << std::setprecision(3) << f;
  std::cout << std::endl;

  it = v6.insert(v6.end(), 2, 0.0);
  it = v6.insert(v6.end() - 2, 3, 0.5);
  it = v6.insert(v6.begin(), {1.0, 2.0, 3.0, 4.0, 5.0});
  std::cout << " v6:";
  for (auto &f : v6) std::cout << " " << std::fixed << std::setprecision(3) << f;
  std::cout << std::endl;

  std::cout << "Création de v7 avec 2 éléments initialisés à 5.0\n";
  lni::vector<double> v7(2, 5.0);

  std::cout << "Contenu initial de v7 : ";
  for (auto &f : v7) std::cout << " " << std::fixed << std::setprecision(3) << f;
  std::cout << "\n";

  std::cout << "Insertion des éléments de v6 dans v7\n";
  std::cout << "Taille actuelle de v7 : " << v7.size() << ", Capacité : " << v7.capacity() << "\n";

  std::cout << "Taille de v6 : " << v6.size() << "\n";
  std::cout << "Contenu de v6 : ";
  for (auto &f : v6) std::cout << " " << std::fixed << std::setprecision(3) << f;
  std::cout << "\n";

  v7.insert(v7.end(), v6.begin(), v6.end());

  std::cout << "Taille après insertion dans v7 : " << v7.size() << ", Capacité : " << v7.capacity()
            << "\n";
  std::cout << "Contenu de v7 après insertion : ";
  for (auto &f : v7) std::cout << " " << std::fixed << std::setprecision(3) << f;
  std::cout << "\n";
  
  v7.insert(v7.begin(), {8.0, 7.0, 6.0});
  v7.insert(v7.end(), {-1.0, -2.0, -2.5});
  for (auto &f : v7) std::cout << " " << std::fixed << std::setprecision(3) << f;
  std::cout << std::endl;

  std::cout << "Testing erase ... " << std::endl;
  v7.erase(v7.begin());
  v7.erase(v7.begin() + 2, v7.begin() + 4);
  std::cout << " v7:";
  for (auto &f : v7) std::cout << " " << std::fixed << std::setprecision(3) << f;
  std::cout << std::endl;

  std::cout << "Testing shrink_to_fit ... " << std::endl;
  std::cout << "Before: " << v7.size() << ", " << v7.capacity() << std::endl;
  v7.shrink_to_fit();
  std::cout << "After: " << v7.size() << ", " << v7.capacity() << std::endl;
  std::cout << std::endl;

  std::cout << "Testing operators ... " << std::endl;
  lni::vector<double> v8(v7);
  std::cout << " v6 == v8: " << (v6 == v8) << std::endl;
  std::cout << " v6 != v8: " << (v6 != v8) << std::endl;
  std::cout << " v7 == v8: " << (v7 == v8) << std::endl;
  std::cout << " v7 != v8: " << (v7 != v8) << std::endl;

  std::cout << "\n v6 < v8: " << (v6 < v8) << std::endl;
  std::cout << " v6 <= v8: " << (v6 <= v8) << std::endl;
  std::cout << " v8 < v6: " << (v8 < v6) << std::endl;
  std::cout << " v7 <= v8: " << (v7 <= v8) << std::endl;
  std::cout << " v8 < v7: " << (v8 < v7) << std::endl;

  std::cout << "\n v6 >= v8: " << (v6 >= v8) << std::endl;
  std::cout << " v6 > v8: " << (v6 > v8) << std::endl;
  std::cout << " v8 >= v6: " << (v8 >= v6) << std::endl;
  std::cout << " v7 > v8: " << (v7 > v8) << std::endl;
  std::cout << " v8 >= v7: " << (v8 >= v7) << std::endl;
  std::cout << std::endl;

  std::cout << "Testing reverse_iterator ... " << std::endl;
  for (auto rit = v8.rbegin(); rit != v8.rend(); ++rit)
    std::cout << " " << std::fixed << std::setprecision(3) << *rit;
  std::cout << std::endl;

  std::cout << "Testing issue #4 #5 on github.com/lnishan/vector ... " << std::endl;
  lni::vector<int64_t> v9(4, 5);
  std::cout << " v9:";
  for (auto &n : v9) std::cout << " " << n;
  std::cout << std::endl;
  v9.assign(8, 7);
  std::cout << " v9 (after assign(8, 7)):";
  lni::Print(v9, "v9");
  for (auto &n : v9) std::cout << " " << n;
  std::cout << std::endl;
#endif

  st = clock();
  std::vector<int> std_v1;
  for (i = 0; i < 10000000; ++i) std_v1.emplace_back(i);
  std::cout << "std::vector " << std::fixed << (double)(clock() - st) / CLOCKS_PER_SEC << "s\n";

  return 0;
}
