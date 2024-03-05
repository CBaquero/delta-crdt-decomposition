#pragma once

#include <ostream>
#include <unordered_set>
#include <vector>

template <typename T> class TwoPhaseSet {
  using Delta = TwoPhaseSet<T>;

public:
  // TODO: add quality of life ctors.
  TwoPhaseSet() = default;

  Delta insert(const T &value) {
    Delta delta;

    auto [it, inserted] = m_added.insert(value);
    if (inserted)
      delta.m_added.insert(value);

    return delta;
  }

  Delta remove(const T &value) {
    Delta delta;

    auto [it, inserted] = m_removed.insert(value);
    if (inserted)
      delta.m_removed.insert(value);

    return delta;
  }

  bool contains(const T &value) const {
    return m_added.contains(value) && !m_removed.contains(value);
  }

  std::unordered_set<T> elements() const {
    // idea: cache elements to make this operation efficient
    std::unordered_set<T> result;

    for (const auto &value : m_added) {
      if (!m_removed.contains(value))
        result.insert(value);
    }

    return result;
  }

  template <typename... Delta> void join(const Delta &...sets) {
    (..., m_added.insert(sets.m_added.begin(), sets.m_added.end()));
    (..., m_removed.insert(sets.m_removed.begin(), sets.m_removed.end()));
  }

  void join(const std::vector<Delta> &sets) {
    for (const auto &set : sets) {
      m_added.insert(set.m_added.begin(), set.m_added.end());
      m_removed.insert(set.m_removed.begin(), set.m_removed.end());
    }
  }

  std::vector<Delta> split() const {
    auto present = elements();

    std::vector<Delta> decompositions;
    decompositions.reserve(present.size() + m_removed.size());

    // add decompositions just for elements that have not been removed
    for (const auto &value : present) {
      decompositions.emplace_back();
      decompositions.back().m_added.insert(value);
    }

    for (const auto &value : m_removed) {
      decompositions.emplace_back();
      decompositions.back().m_removed.insert(value);
    }

    return decompositions;
  }

  bool operator==(const TwoPhaseSet<T> &other) const {
    // the semantics of this operator are a bit foggy...
    // should we take into account both added and removed elements?
    // or just lookup to the contents of the present elements?
    return elements() == other.elements();
  }

  Delta operator+(const Delta &other) const {
    Delta result = *this;
    result.join(other);

    return result;
  }

  Delta operator-(const Delta &other) const {
    Delta result = *this;

    for (const auto &value : other.m_added) {
      result.m_added.erase(value);
    }

    for (const auto &value : other.m_removed) {
      result.m_removed.erase(value);
    }

    return result;
  }

  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const TwoPhaseSet<U> obj) {
    auto present = obj.elements();

    os << '{' << ' ';
    for (const auto &value : present) {
      os << value << ' ';
    }
    os << '}';

    return os;
  }

private:
  // key order is not maintained but allows for faster lookup and insertion
  std::unordered_set<T> m_added;
  std::unordered_set<T> m_removed;
};
