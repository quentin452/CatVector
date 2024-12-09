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

  // 23.3.11.2, construct/copy/destroy:
  vector() noexcept;
  explicit vector(size_type n);
  vector(size_type n, const T &val);
  vector(typename vector<T>::iterator first, typename vector<T>::iterator last);
  vector(std::initializer_list<T>);
  vector(const vector<T> &);
  vector(vector<T> &&) noexcept;
  ~vector();
  vector<T> &operator=(const vector<T> &);
  vector<T> &operator=(vector<T> &&);
  vector<T> &operator=(std::initializer_list<T>);
  void assign(size_type, const T &value);
  void assign(typename vector<T>::iterator, typename vector<T>::iterator);
  void assign(std::initializer_list<T>);

  // iterators:
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator crbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator crend() const noexcept;

  // 23.3.11.3, capacity:
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  size_type capacity() const noexcept;
  void resize(size_type);
  void resize(size_type, const T &);
  void reserve(size_type);
  void shrink_to_fit();

  // element access
  reference operator[](size_type);
  const_reference operator[](size_type) const;
  reference at(size_type);
  const_reference at(size_type) const;
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  // 23.3.11.4, data access:
  T *data() noexcept;
  const T *data() const noexcept;

  // 23.3.11.5, modifiers:
  template <class... Args>
  void emplace_back(Args &&...args);
  void push_back(const T &);
  void push_back(T &&);
  void pop_back();

  template <class... Args>
  iterator emplace(const_iterator, Args &&...);
  iterator insert(const_iterator, const T &);
  iterator insert(const_iterator, T &&);
  iterator insert(const_iterator, size_type, const T &);
  template <class InputIt>
  iterator insert(const_iterator, InputIt, InputIt);
  iterator insert(const_iterator, std::initializer_list<T>);
  iterator erase(const_iterator);
  iterator erase(const_iterator, const_iterator);
  void swap(vector<T> &);
  void clear() noexcept;

  bool operator==(const vector<T> &) const;
  bool operator!=(const vector<T> &) const;
  bool operator<(const vector<T> &) const;
  bool operator<=(const vector<T> &) const;
  bool operator>(const vector<T> &) const;
  bool operator>=(const vector<T> &) const;

  friend void Print(const vector<T> &v, const std::string &vec_name);

 private:
  size_type rsrv_sz = 4;
  size_type vec_sz = 0;
  T *arr;

  inline void reallocate();
};

template <typename T>
inline vector<T>::vector() noexcept {
  arr = new T[rsrv_sz];
}

template <typename T>
inline vector<T>::vector(typename vector<T>::size_type n) {
  size_type i;
  rsrv_sz = n << 2;
  arr = new T[rsrv_sz];
  for (i = 0; i < n; ++i) arr[i] = T();
  vec_sz = n;
}

template <typename T>
inline vector<T>::vector(typename vector<T>::size_type n, const T &value) {
  size_type i;
  rsrv_sz = n << 2;
  arr = new T[rsrv_sz];
  for (i = 0; i < n; ++i) arr[i] = value;
  vec_sz = n;
}

template <typename T>
inline vector<T>::vector(typename vector<T>::iterator first, typename vector<T>::iterator last) {
  size_type i, count = last - first;
  rsrv_sz = count << 2;
  arr = new T[rsrv_sz];
  for (i = 0; i < count; ++i, ++first) arr[i] = *first;
  vec_sz = count;
}

template <typename T>
inline vector<T>::vector(std::initializer_list<T> lst) {
  rsrv_sz = lst.size() << 2;
  arr = new T[rsrv_sz];
  for (auto &item : lst) arr[vec_sz++] = item;
}

template <typename T>
inline vector<T>::vector(const vector<T> &other) {
  size_type i;
  rsrv_sz = other.rsrv_sz;
  arr = new T[rsrv_sz];
  for (i = 0; i < other.vec_sz; ++i) arr[i] = other.arr[i];
  vec_sz = other.vec_sz;
}

template <typename T>
inline vector<T>::vector(vector<T> &&other) noexcept {
  size_type i;
  rsrv_sz = other.rsrv_sz;
  arr = new T[rsrv_sz];
  for (i = 0; i < other.vec_sz; ++i) arr[i] = std::move(other.arr[i]);
  vec_sz = other.vec_sz;
}

template <typename T>
inline vector<T>::~vector() {
  if (arr) {
    delete[] arr;
  }
}

template <typename T>
inline vector<T> &vector<T>::operator=(const vector<T> &other) {
  size_type i;
  if (rsrv_sz < other.vec_sz) {
    rsrv_sz = other.vec_sz << 2;
    reallocate();
  }
  for (i = 0; i < other.vec_sz; ++i) arr[i] = other.arr[i];
  vec_sz = other.vec_sz;
  return *this;
}

template <typename T>
inline vector<T> &vector<T>::operator=(vector<T> &&other) {
  size_type i;
  if (rsrv_sz < other.vec_sz) {
    rsrv_sz = other.vec_sz << 2;
    reallocate();
  }
  for (i = 0; i < other.vec_sz; ++i) arr[i] = std::move(other.arr[i]);
  vec_sz = other.vec_sz;
  return *this;
}

template <typename T>
inline vector<T> &vector<T>::operator=(std::initializer_list<T> lst) {
  if (rsrv_sz < lst.size()) {
    rsrv_sz = lst.size() << 2;
    reallocate();
  }
  vec_sz = 0;
  for (auto &item : lst) arr[vec_sz++] = item;
  return *this;
}

template <typename T>
inline void vector<T>::assign(typename vector<T>::size_type count, const T &value) {
  size_type i;
  if (count > rsrv_sz) {
    rsrv_sz = count << 2;
    reallocate();
  }
  for (i = 0; i < count; ++i) arr[i] = value;
  vec_sz = count;
}

template <typename T>
inline void vector<T>::assign(
    typename vector<T>::iterator first, typename vector<T>::iterator last) {
  size_type i, count = static_cast<size_type>(last - first);
  if (count > rsrv_sz) {
    rsrv_sz = count << 2;
    reallocate();
  }
  for (i = 0; i < count; ++i, ++first) arr[i] = *first;
  vec_sz = count;
}

template <typename T>
inline void vector<T>::assign(std::initializer_list<T> lst) {
  size_type i, count = lst.size();
  if (count > rsrv_sz) {
    rsrv_sz = count << 2;
    reallocate();
  }
  i = 0;
  for (auto &item : lst) arr[++i] = item;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::begin() noexcept {
  return arr;
}

template <typename T>
inline typename vector<T>::const_iterator vector<T>::begin() const noexcept {
  return arr;
}

template <typename T>
inline typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
  return arr;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::end() noexcept {
  return arr + vec_sz;
}

template <typename T>
inline typename vector<T>::const_iterator vector<T>::end() const noexcept {
  return arr + vec_sz;
}

template <typename T>
inline typename vector<T>::const_iterator vector<T>::cend() const noexcept {
  return arr + vec_sz;
}

template <typename T>
inline typename vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
  return reverse_iterator(arr + vec_sz);
}

template <typename T>
inline typename vector<T>::const_reverse_iterator vector<T>::crbegin() const noexcept {
  return reverse_iterator(arr + vec_sz);
}

template <typename T>
inline typename vector<T>::reverse_iterator vector<T>::rend() noexcept {
  return reverse_iterator(arr);
}

template <typename T>
inline typename vector<T>::const_reverse_iterator vector<T>::crend() const noexcept {
  return reverse_iterator(arr);
}

template <typename T>
inline void vector<T>::reallocate() {
  T *tarr = new (std::nothrow) T[rsrv_sz];
  memcpy(tarr, arr, vec_sz * sizeof(T));
  delete[] arr;
  arr = tarr;
}

template <typename T>
inline bool vector<T>::empty() const noexcept {
  return vec_sz == 0;
}

template <typename T>
inline typename vector<T>::size_type vector<T>::size() const noexcept {
  return vec_sz;
}

template <typename T>
inline typename vector<T>::size_type vector<T>::max_size() const noexcept {
  return CATZ_VECTOR_MAX_SZ;
}

template <typename T>
inline typename vector<T>::size_type vector<T>::capacity() const noexcept {
  return rsrv_sz;
}

template <typename T>
inline void vector<T>::reserve(typename vector<T>::size_type _sz) {
  if (_sz > rsrv_sz) {
    rsrv_sz = _sz;
    reallocate();
  }
}

template <typename T>
inline void vector<T>::shrink_to_fit() {
  if (rsrv_sz != vec_sz) {
    rsrv_sz = vec_sz;
    reallocate();
  }
}

template <typename T>
inline typename vector<T>::reference vector<T>::operator[](typename vector<T>::size_type idx) {
  return arr[idx];
}

template <typename T>
inline typename vector<T>::const_reference vector<T>::operator[](
    typename vector<T>::size_type idx) const {
  return arr[idx];
}

template <typename T>
inline typename vector<T>::reference vector<T>::at(size_type pos) {
  return (pos < vec_sz) ? arr[pos] : throw std::out_of_range("accessed position is out of range");
}

template <typename T>
inline typename vector<T>::const_reference vector<T>::at(size_type pos) const {
  return (pos < vec_sz) ? arr[pos] : throw std::out_of_range("accessed position is out of range");
}

template <typename T>
inline typename vector<T>::reference vector<T>::front() {
  return arr[0];
}

template <typename T>
inline typename vector<T>::const_reference vector<T>::front() const {
  return arr[0];
}

template <typename T>
inline typename vector<T>::reference vector<T>::back() {
  return arr[vec_sz - 1];
}

template <typename T>
inline typename vector<T>::const_reference vector<T>::back() const {
  return arr[vec_sz - 1];
}

template <typename T>
inline T *vector<T>::data() noexcept {
  return arr;
}

template <typename T>
inline const T *vector<T>::data() const noexcept {
  return arr;
}

template <typename T>
template <class... Args>
inline void vector<T>::emplace_back(Args &&...args) {
  if (vec_sz == rsrv_sz) {
    rsrv_sz <<= 2;
    reallocate();
  }
  arr[vec_sz] = std::move(T(std::forward<Args>(args)...));
  ++vec_sz;
}

template <typename T>
inline void vector<T>::push_back(const T &val) {
  if (vec_sz == rsrv_sz) {
    rsrv_sz <<= 2;
    reallocate();
  }
  arr[vec_sz] = val;
  ++vec_sz;
}

template <typename T>
inline void vector<T>::push_back(T &&val) {
  if (vec_sz == rsrv_sz) {
    rsrv_sz <<= 2;
    reallocate();
  }
  arr[vec_sz] = std::move(val);
  ++vec_sz;
}

template <typename T>
inline void vector<T>::pop_back() {
  --vec_sz;
  if constexpr (!std::is_trivially_destructible<T>::value) {
    arr[vec_sz].~T();
  }
}

template <typename T>
template <class... Args>
inline typename vector<T>::iterator vector<T>::emplace(
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

template <typename T>
inline typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator it, const T &val) {
  iterator iit = &arr[it - arr];
  if (vec_sz == rsrv_sz) {
    rsrv_sz <<= 2;
    reallocate();
  }
  memmove(iit + 1, iit, (vec_sz - (it - arr)) * sizeof(T));
  (*iit) = val;
  ++vec_sz;
  return iit;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator it, T &&val) {
  iterator iit = &arr[it - arr];
  if (vec_sz == rsrv_sz) {
    rsrv_sz <<= 2;
    reallocate();
  }
  memmove(iit + 1, iit, (vec_sz - (it - arr)) * sizeof(T));
  (*iit) = std::move(val);
  ++vec_sz;
  return iit;
}
template <typename T>
inline typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator it, typename vector<T>::size_type cnt, const T &val) {
  iterator f = &arr[it - arr];
  if (!cnt) return f;
  if (vec_sz + cnt > rsrv_sz) {
    rsrv_sz = (vec_sz + cnt) << 2;
    reallocate();
  }
  memmove(f + cnt, f, (vec_sz - (it - arr)) * sizeof(T));
  vec_sz += cnt;
  for (iterator cur = f; cnt--; ++cur) (*cur) = val;
  return f;
}

template <typename T>
template <class InputIt>
inline typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator it, InputIt first, InputIt last) {
  iterator f = &arr[it - arr];
  size_type cnt = static_cast<size_type>(last - first);
  if (!cnt) return f;
  if (vec_sz + cnt > rsrv_sz) {
    rsrv_sz = (vec_sz + cnt) << 2;
    reallocate();
  }
  memmove(f + cnt, f, (vec_sz - (it - arr)) * sizeof(T));
  for (iterator cur = f; first != last; ++cur, ++first) (*cur) = *first;
  vec_sz += cnt;
  return f;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator it, std::initializer_list<T> lst) {
  size_type cnt = lst.size();
  iterator f = &arr[it - arr];
  if (!cnt) return f;
  if (vec_sz + cnt > rsrv_sz) {
    rsrv_sz = (vec_sz + cnt) << 2;
    reallocate();
  }
  memmove(f + cnt, f, (vec_sz - (it - arr)) * sizeof(T));
  iterator iit = f;
  for (auto &item : lst) {
    (*iit) = item;
    ++iit;
  }
  vec_sz += cnt;
  return f;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(typename vector<T>::const_iterator it) {
  iterator iit = &arr[it - arr];
  if constexpr (!std::is_trivially_destructible<T>::value) {
    (*iit).~T();
  }
  memmove(iit, iit + 1, (vec_sz - (it - arr) - 1) * sizeof(T));
  --vec_sz;
  return iit;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(
    typename vector<T>::const_iterator first, typename vector<T>::const_iterator last) {
  iterator f = &arr[first - arr];
  if (first == last) return f;
  if constexpr (!std::is_trivially_destructible<T>::value) {
    for (; first != last; ++first) {
      (*first).~T();
    }
  }
  memmove(f, last, (vec_sz - (last - arr)) * sizeof(T));
  vec_sz -= last - first;
  return f;
}

template <typename T>
inline void vector<T>::swap(vector<T> &rhs) {
  size_t tvec_sz = vec_sz, trsrv_sz = rsrv_sz;
  T *tarr = arr;

  vec_sz = rhs.vec_sz;
  rsrv_sz = rhs.rsrv_sz;
  arr = rhs.arr;

  rhs.vec_sz = tvec_sz;
  rhs.rsrv_sz = trsrv_sz;
  rhs.arr = tarr;
}

template <typename T>
inline void vector<T>::clear() noexcept {
  if constexpr (!std::is_trivially_destructible<T>::value) {
    for (size_type i = 0; i < vec_sz; ++i) {
      arr[i].~T();
    }
  }
  vec_sz = 0;
}

template <typename T>
inline bool vector<T>::operator==(const vector<T> &rhs) const {
  if (vec_sz != rhs.vec_sz) return false;
  size_type i;
  for (i = 0; i < vec_sz; ++i)
    if (arr[i] != rhs.arr[i]) return false;
  return true;
}

template <typename T>
inline bool vector<T>::operator!=(const vector<T> &rhs) const {
  if (vec_sz != rhs.vec_sz) return true;
  size_type i;
  for (i = 0; i < vec_sz; ++i)
    if (arr[i] != rhs.arr[i]) return true;
  return false;
}

template <typename T>
inline bool vector<T>::operator<(const vector<T> &rhs) const {
  size_type i, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
  for (i = 0; i < ub; ++i)
    if (arr[i] != rhs.arr[i]) return arr[i] < rhs.arr[i];
  return vec_sz < rhs.vec_sz;
}

template <typename T>
inline bool vector<T>::operator<=(const vector<T> &rhs) const {
  size_type i, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
  for (i = 0; i < ub; ++i)
    if (arr[i] != rhs.arr[i]) return arr[i] < rhs.arr[i];
  return vec_sz <= rhs.vec_sz;
}

template <typename T>
inline bool vector<T>::operator>(const vector<T> &rhs) const {
  size_type i, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
  for (i = 0; i < ub; ++i)
    if (arr[i] != rhs.arr[i]) return arr[i] > rhs.arr[i];
  return vec_sz > rhs.vec_sz;
}

template <typename T>
inline bool vector<T>::operator>=(const vector<T> &rhs) const {
  size_type i, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
  for (i = 0; i < ub; ++i)
    if (arr[i] != rhs.arr[i]) return arr[i] > rhs.arr[i];
  return vec_sz >= rhs.vec_sz;
}

template <typename T>
inline void vector<T>::resize(typename vector<T>::size_type sz) {
  if (sz > vec_sz) {
    if (sz > rsrv_sz) {
      rsrv_sz = sz;
      reallocate();
    }
  } else {
    size_type i;
    for (i = vec_sz; i < sz; ++i) arr[i].~T();
  }
  vec_sz = sz;
}

template <typename T>
inline void vector<T>::resize(typename vector<T>::size_type sz, const T &c) {
  if (sz > vec_sz) {
    if (sz > rsrv_sz) {
      rsrv_sz = sz;
      reallocate();
    }
    size_type i;
    for (i = vec_sz; i < sz; ++i) arr[i] = c;
  } else {
    size_type i;
    for (i = vec_sz; i < sz; ++i) arr[i].~T();
  }
  vec_sz = sz;
}

template <typename T>
inline void Print(const vector<T> &v, const std::string &vec_name) {
  for (typename vector<T>::size_type i = 0; i < v.size(); ++i) {
    std::cout << vec_name << "[" << i << "] = " << v[i] << std::endl;
  }
}

}  // namespace lni

#endif  // CATZ_VECTOR
