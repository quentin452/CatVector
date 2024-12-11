#include <cstddef>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

#ifndef CATZ_VECTOR
#define CATZ_VECTOR
#define CATZ_VECTOR_MAX_SZ std::numeric_limits<size_type>::max() / sizeof(T)

namespace lni {

template <typename T>
class vector {
 public:
  // types:
  typedef T value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  vector() noexcept { arr = new T[rsrv_sz]; }

  inline vector(typename vector<T>::size_type n) {
    rsrv_sz = n << 2;
    arr = static_cast<T *>(operator new[](rsrv_sz * sizeof(T)));
    std::uninitialized_default_construct_n(arr, n);
    vec_sz = n;
  }

  inline vector(typename vector<T>::size_type n, const T &value) {
    rsrv_sz = n << 2;
    arr = static_cast<T *>(operator new[](rsrv_sz * sizeof(T)));
    std::uninitialized_fill_n(arr, n, value);
    vec_sz = n;
  }

  inline vector(typename vector<T>::iterator first, typename vector<T>::iterator last) {
    size_type count = last - first;
    allocate_and_copy(count, first);
  }

  inline vector(std::initializer_list<T> lst) {
    size_type count = lst.size();
    allocate_and_copy(count, lst.begin());
  }

  inline vector(const vector<T> &other) {
    size_type count = other.vec_sz;
    allocate_and_copy(count, other.arr);
  }

  inline vector(vector<T> &&other) noexcept {
    rsrv_sz = other.rsrv_sz;
    vec_sz = other.vec_sz;
    arr = static_cast<T *>(operator new[](rsrv_sz * sizeof(T)));
    std::uninitialized_move(other.arr, other.arr + vec_sz, arr);
    other.vec_sz = 0;
    other.arr = nullptr;
    other.rsrv_sz = 0;
  }

  ~vector() {
    if (arr) {
      delete[] arr;
    }
  }

  inline vector<T> &operator=(const vector<T> &other) {
    if (this != &other) {
      ensure_capacity(other.vec_sz);
      std::copy(other.arr, other.arr + other.vec_sz, arr);
      vec_sz = other.vec_sz;
    }
    return *this;
  }

  inline vector<T> &operator=(vector<T> &&other) {
    if (this != &other) {
      ensure_capacity(other.vec_sz);
      std::move(other.arr, other.arr + other.vec_sz, arr);
      vec_sz = other.vec_sz;
      other.vec_sz = 0;
      other.arr = nullptr;
      other.rsrv_sz = 0;
    }
    return *this;
  }

  inline vector<T> &operator=(std::initializer_list<T> lst) {
    ensure_capacity(lst.size());
    std::copy(lst.begin(), lst.end(), arr);
    vec_sz = static_cast<size_type>(lst.size());
    return *this;
  }

  inline void assign(typename vector<T>::size_type count, const T &value) {
    ensure_capacity(count);
    std::fill(arr, arr + count, value);
    vec_sz = count;
  }

  inline void assign(typename vector<T>::iterator first, typename vector<T>::iterator last) {
    size_type count = static_cast<size_type>(last - first);
    ensure_capacity(count);
    std::copy(first, last, arr);
    vec_sz = count;
  }

  inline void assign(std::initializer_list<T> lst) {
    size_type count = static_cast<size_type>(lst.size());
    ensure_capacity(count);
    std::copy(lst.begin(), lst.end(), arr);
    vec_sz = count;
  }

  inline typename vector<T>::iterator begin() noexcept { return arr; }

  inline typename vector<T>::const_iterator begin() const noexcept { return arr; }

  inline typename vector<T>::const_iterator cbegin() const noexcept { return arr; }

  inline typename vector<T>::iterator end() noexcept { return arr + vec_sz; }

  inline typename vector<T>::const_iterator end() const noexcept { return arr + vec_sz; }

  inline typename vector<T>::const_iterator cend() const noexcept { return arr + vec_sz; }

  inline typename vector<T>::reverse_iterator rbegin() noexcept {
    return reverse_iterator(arr + vec_sz);
  }

  inline typename vector<T>::const_reverse_iterator crbegin() const noexcept {
    return reverse_iterator(arr + vec_sz);
  }

  inline typename vector<T>::reverse_iterator rend() noexcept { return reverse_iterator(arr); }

  inline typename vector<T>::const_reverse_iterator crend() const noexcept {
    return reverse_iterator(arr);
  }

  inline bool empty() const noexcept { return vec_sz == 0; }

  inline typename vector<T>::size_type size() const noexcept { return vec_sz; }

  inline typename vector<T>::size_type max_size() const noexcept { return CATZ_VECTOR_MAX_SZ; }

  inline typename vector<T>::size_type capacity() const noexcept { return rsrv_sz; }

  inline void reserve(typename vector<T>::size_type _sz) {
    if (_sz > rsrv_sz) {
      rsrv_sz = _sz;
      reallocate();
    }
  }

  inline void shrink_to_fit() {
    if (rsrv_sz != vec_sz) {
      rsrv_sz = vec_sz;
      reallocate();
    }
  }

  inline typename vector<T>::reference operator[](typename vector<T>::size_type idx) {
    return arr[idx];
  }

  inline typename vector<T>::const_reference operator[](typename vector<T>::size_type idx) const {
    return arr[idx];
  }

  inline typename vector<T>::reference at(vector<T>::size_type pos) {
    return (pos < vec_sz) ? arr[pos] : throw std::out_of_range("accessed position is out of range");
  }

  inline typename vector<T>::const_reference at(vector<T>::size_type pos) const {
    return (pos < vec_sz) ? arr[pos] : throw std::out_of_range("accessed position is out of range");
  }

  inline typename vector<T>::reference front() { return arr[0]; }

  inline typename vector<T>::const_reference front() const { return arr[0]; }

  inline typename vector<T>::reference back() { return arr[vec_sz - 1]; }

  inline typename vector<T>::const_reference back() const { return arr[vec_sz - 1]; }

  inline T *data() noexcept { return arr; }

  inline const T *data() const noexcept { return arr; }

  template <class... Args>
  inline void emplace_back(Args &&...args) {
    if (vec_sz == rsrv_sz) {
      rsrv_sz <<= 2;
      reallocate();
    }
    arr[vec_sz] = std::move(T(std::forward<Args>(args)...));
    ++vec_sz;
  }

  inline void push_back(const T &val) {
    if (vec_sz == rsrv_sz) {
      rsrv_sz <<= 2;
      reallocate();
    }
    arr[vec_sz] = val;
    ++vec_sz;
  }

  inline void push_back(T &&val) {
    if (vec_sz == rsrv_sz) {
      rsrv_sz <<= 2;
      reallocate();
    }
    arr[vec_sz] = std::move(val);
    ++vec_sz;
  }

  inline void pop_back() {
    --vec_sz;
    if constexpr (!std::is_trivially_destructible<T>::value) {
      arr[vec_sz].~T();
    }
  }

  template <class... Args>
  inline typename vector<T>::iterator emplace(
      typename vector<T>::const_iterator it, Args &&...args) {
    iterator iit = &arr[it - arr];
    if (vec_sz == rsrv_sz) {
      rsrv_sz <<= 2;
      reallocate();
    }
    memmove(iit + 1, iit, (vec_sz - (it - arr)) * sizeof(T));
    (*iit) = std::move(T(std::forward<Args>(args)...));
    ++vec_sz;
    return iit;
  }

  inline typename vector<T>::iterator insert(typename vector<T>::const_iterator it, const T &val) {
    iterator f = insert_impl(it, 1);  
    (*f) = val;
    return f;
  }

  inline typename vector<T>::iterator insert(typename vector<T>::const_iterator it, T &&val) {
    iterator f = insert_impl(it, 1);  
    (*f) = std::move(val);
    return f;
  }

  inline typename vector<T>::iterator insert(
      typename vector<T>::const_iterator it, size_type cnt, const T &val) {
    iterator f = insert_impl(it, cnt);  
    for (iterator cur = f; cnt--; ++cur) {
      (*cur) = val;
    }
    return f;
  }

  template <class InputIt>
  inline typename vector<T>::iterator insert(
      typename vector<T>::const_iterator it, InputIt first, InputIt last) {
    size_type cnt = static_cast<size_type>(last - first);
    iterator f = insert_impl(it, cnt);  
    for (iterator cur = f; first != last; ++cur, ++first) {
      (*cur) = *first;
    }
    return f;
  }

  inline typename vector<T>::iterator insert(
      typename vector<T>::const_iterator it, std::initializer_list<T> lst) {
    size_type cnt = lst.size();
    iterator f = insert_impl(it, cnt);  
    iterator iit = f;
    for (auto &item : lst) {
      (*iit) = item;
      ++iit;
    }
    return f;
  }

  inline typename vector<T>::iterator erase(typename vector<T>::const_iterator it) {
    iterator iit = &arr[it - arr];
    erase_impl(it, it + 1);
    return iit;
  }

  inline typename vector<T>::iterator erase(
      typename vector<T>::const_iterator first, typename vector<T>::const_iterator last) {
    erase_impl(first, last);
    return &arr[first - arr];
  }

  inline void swap(vector<T> &rhs) {
    std::swap(vec_sz, rhs.vec_sz);
    std::swap(rsrv_sz, rhs.rsrv_sz);
    std::swap(arr, rhs.arr);
  }

  inline void clear() noexcept {
    if constexpr (!std::is_trivially_destructible<T>::value) {
      for (size_type i = 0; i < vec_sz; ++i) {
        arr[i].~T();
      }
    }
    vec_sz = 0;
  }

  inline bool operator==(const vector<T> &rhs) const { return compare(rhs, std::equal_to<T>()); }
  inline bool operator!=(const vector<T> &rhs) const {
    return compare(rhs, std::not_equal_to<T>());
  }
  inline bool operator<(const vector<T> &rhs) const { return compare(rhs, std::less<T>()); }
  inline bool operator<=(const vector<T> &rhs) const { return compare(rhs, std::less_equal<T>()); }
  inline bool operator>(const vector<T> &rhs) const { return compare(rhs, std::greater<T>()); }
  inline bool operator>=(const vector<T> &rhs) const {
    return compare(rhs, std::greater_equal<T>());
  }

  inline void resize(typename vector<T>::size_type sz) { resize_impl(sz); }

  inline void resize(typename vector<T>::size_type sz, const T &c) { resize_impl(sz, &c); }

  friend void Print(const vector<T> &v, const std::string &vec_name);

 private:
  inline void ensure_capacity(vector<T>::size_type required_sz) {
    if (required_sz > rsrv_sz) {
      rsrv_sz = required_sz << 2;
      reallocate();
    }
  }
  template <typename Compare>
  inline bool compare(const vector<T> &rhs, Compare comp) const {
    size_type ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
    for (size_type i = 0; i < ub; ++i) {
      if (arr[i] != rhs.arr[i]) {
        return comp(arr[i], rhs.arr[i]);
      }
    }
    return comp(static_cast<T>(vec_sz), static_cast<T>(rhs.vec_sz));
  }
  inline void reallocate() {
    T *tarr = new (std::nothrow) T[rsrv_sz];
    memcpy(tarr, arr, vec_sz * sizeof(T));
    delete[] arr;
    arr = tarr;
  }
  void allocate_and_copy(size_type count, const T *first) {
    rsrv_sz = count << 2;
    arr = static_cast<T *>(operator new[](rsrv_sz * sizeof(T)));
    std::uninitialized_copy(first, first + count, arr);
    vec_sz = count;
  }
  inline void resize_impl(typename vector<T>::size_type sz, const T *c = nullptr) {
    if (sz > vec_sz) {
      if (sz > rsrv_sz) {
        rsrv_sz = sz;
        reallocate();
      }
      size_type i;
      for (i = vec_sz; i < sz; ++i) {
        if (c) {
          arr[i] = *c;
        } else {
          new (&arr[i]) T();
        }
      }
    } else {
      size_type i;
      for (i = vec_sz; i < sz; ++i) arr[i].~T();
    }
    vec_sz = sz;
  }
  inline void erase_impl(
      typename vector<T>::const_iterator first, typename vector<T>::const_iterator last) {
    iterator f = &arr[first - arr];
    if constexpr (!std::is_trivially_destructible<T>::value) {
      for (; first != last; ++first) {
        (*first).~T();
      }
    }
    memmove(f, last, (vec_sz - (last - arr)) * sizeof(T));
    vec_sz -= last - first;
  }
  inline typename vector<T>::iterator insert_impl(
      typename vector<T>::const_iterator it, size_type cnt) {
    iterator f = &arr[it - arr];
    if (vec_sz + cnt > rsrv_sz) {
      rsrv_sz = (vec_sz + cnt) << 2;
      reallocate();
    }
    memmove(f + cnt, f, (vec_sz - (it - arr)) * sizeof(T));
    vec_sz += cnt;
    return f;
  }

  vector<T>::size_type rsrv_sz = 4, vec_sz = 0;
  T *arr;
};
template <typename T>
inline void Print(const vector<T> &v, const std::string &vec_name) {
  typename vector<T>::size_type i = 0;
  for (const auto &item : v) {
    std::cout << vec_name << "[" << i++ << "] = " << item << std::endl;
  }
}
}  // namespace lni

#endif  // CATZ_VECTOR
