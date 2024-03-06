#pragma once

#include <ostream>
#include <unordered_set>
#include <vector>

namespace delta {

template <typename T> class GrowOnlySet {
  using Delta = GrowOnlySet<T>;

public:
  // TODO: add quality of life ctors.
  GrowOnlySet() = default;

  Delta insert(const T &value) {
    Delta delta;

    auto [it, inserted] = m_elements.insert(value);
    if (inserted)
      delta.m_elements.insert(value);

    return delta;
  }

  bool contains(const T &value) const { return m_elements.contains(value); }

  std::unordered_set<T> elements() const { return m_elements; }

  template <typename... Delta> void join(const Delta &...deltas) {
    (...,
     m_elements.insert(deltas.m_elements.begin(), deltas.m_elements.end()));
  }

  void join(const std::vector<Delta> &deltas) {
    for (const auto &set : deltas) {
      m_elements.insert(set.m_elements.begin(), set.m_elements.end());
    }
  }

  std::vector<Delta> split() const {
    std::vector<Delta> decompositions;
    decompositions.reserve(m_elements.size());

    for (const auto &value : m_elements) {
      decompositions.emplace_back();
      decompositions.back().m_elements.insert(value);
    }

    return decompositions;
  }

  bool operator==(const Delta &other) const {
    return m_elements == other.m_elements;
  }

  Delta operator+(const Delta &other) const {
    Delta result = *this;
    result.join(other);

    return result;
  }

  Delta operator-(const Delta &other) const {
    Delta result = *this;

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

} // namespace delta
