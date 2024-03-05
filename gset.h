#pragma once

#include <ostream>
#include <unordered_set>
#include <vector>

template <typename T> class GrowOnlySet {
public:
  // TODO: add quality of life ctors.
  GrowOnlySet() = default;

  GrowOnlySet<T> insert(const T &value) {
    GrowOnlySet<T> delta;

    auto [it, inserted] = m_elements.insert(value);
    if (inserted)
      delta.m_elements.insert(value);

    return delta;
  }

  bool contains(const T &value) const { return m_elements.contains(value); }

  std::unordered_set<T> elements() const { return m_elements; }

  template <typename... GrowOnlySet> void join(const GrowOnlySet &...sets) {
    (..., m_elements.insert(sets.m_elements.begin(), sets.m_elements.end()));
  }

  void join(std::vector<GrowOnlySet<T>> &sets) {
    for (const auto &set : sets) {
      m_elements.insert(set.m_elements.begin(), set.m_elements.end());
    }
  }

  std::vector<GrowOnlySet<T>> split() const {
    std::vector<GrowOnlySet<T>> decompositions;
    decompositions.reserve(m_elements.size());

    for (const auto &value : m_elements) {
      decompositions.emplace_back();
      decompositions.back().m_elements.insert(value);
    }

    return decompositions;
  }

  bool operator==(const GrowOnlySet<T> &other) const {
    return m_elements == other.m_elements;
  }

  GrowOnlySet<T> operator+(const GrowOnlySet<T> &other) const {
    GrowOnlySet<T> result = *this;
    result.join(other);

    return result;
  }

  GrowOnlySet<T> operator-(const GrowOnlySet<T> &other) const {
    GrowOnlySet<T> result = *this;

    for (const auto &value : other.m_elements) {
      result.m_elements.erase(value);
    }

    return result;
  }

  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const GrowOnlySet<U> &obj) {
    os << '{' << ' ';
    for (const auto &value : obj.m_elements) {
      os << value << ' ';
    }
    os << '}';

    return os;
  }

private:
  std::unordered_set<T> m_elements;
};
