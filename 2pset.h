#include <iostream>
#include <ostream>
#include <unordered_set>
#include <vector>

template <typename T> class TwoPhaseSet {
public:
  // TODO: add quality of life ctors.
  TwoPhaseSet() = default;

  TwoPhaseSet<T> insert(const T &value) {
    TwoPhaseSet<T> delta;

    auto [it, inserted] = m_added.insert(value);
    if (inserted)
      delta.m_added.insert(value);

    return delta;
  }

  TwoPhaseSet<T> remove(const T &value) {
    TwoPhaseSet<T> delta;

    auto [it, inserted] = m_removed.insert(value);
    if (inserted)
      delta.m_removed.insert(value);

    return delta;
  }

  bool in(const T &value) const {
    return m_added.count(value) == 1 && m_removed.count(value) == 0;
  }

  std::unordered_set<T> elements() const {
    // idea: cache elements to make this operation efficient
    std::unordered_set<T> result;

    for (const auto &value : m_added) {
      if (m_removed.count(value) == 0)
        result.insert(value);
    }

    return result;
  }

  template <typename... TwoPhaseSet>
  void join(const TwoPhaseSet &...decompositions) {
    (..., m_added.insert(decompositions.m_added.begin(),
                         decompositions.m_added.end()));
    (..., m_removed.insert(decompositions.m_removed.begin(),
                           decompositions.m_removed.end()));
  }

  void join(const std::vector<TwoPhaseSet<T>> &decompositions) {
    for (const auto &set : decompositions) {
      m_added.insert(set.m_added.begin(), set.m_added.end());
      m_removed.insert(set.m_removed.begin(), set.m_removed.end());
    }
  }

  std::vector<TwoPhaseSet<T>> split() const {
    auto present = elements();

    std::vector<TwoPhaseSet<T>> decompositions;
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

  TwoPhaseSet<T> operator+(const TwoPhaseSet<T> &other) const {
    // warn: this performs a copy... should it?
    TwoPhaseSet<T> result = *this;
    result.join(other);

    return result;
  }

  TwoPhaseSet<T> operator-(const TwoPhaseSet<T> &other) const {
    TwoPhaseSet<T> result = *this;

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
