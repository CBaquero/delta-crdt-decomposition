#pragma once

#include <ostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace delta {

template <typename T, typename Timestamp>
  requires std::totally_ordered<Timestamp>
class LastWriterWinsSet {
  using Delta = LastWriterWinsSet<T, Timestamp>;
  using Tag = std::pair<Timestamp, bool>;

public:
  // idea: provide a mechanism to be either add or remove wins
  // TODO: add quality of life ctors.
  LastWriterWinsSet() = default;

  Delta insert(const T &value, const Timestamp timestamp) {
    Delta delta;

    auto [it, inserted] = m_elements.try_emplace(value, timestamp, true);

    if (!inserted) {
      // the client must provide monotonic timestamps
      auto previous_timestamp = it->second.first;
      if (previous_timestamp > timestamp)
        throw std::invalid_argument("timestamp is not monotic");

      // idempotency when there when timestamps are tied
      if (previous_timestamp == timestamp && it->second.second)
        return delta;
    }

    // update the new timestamp and produce the corresponding delta
    it->second = std::make_pair(timestamp, true);
    delta.m_elements.try_emplace(value, timestamp, true);
    return delta;
  }

  Delta remove(const T &value, const Timestamp timestamp) {
    Delta delta;

    auto [it, inserted] = m_elements.try_emplace(value, timestamp, true);

    if (!inserted) {
      // the client must provide monotonic timestamps
      auto previous_timestamp = it->second.first;
      if (previous_timestamp > timestamp)
        throw std::invalid_argument("timestamp is not monotic");

      // add-wins when there when timestamps are tied
      if (previous_timestamp == timestamp && it->second.second)
        return delta;
    }

    delta.m_elements.try_emplace(value, timestamp, false);
    return delta;
  }

  bool contains(const T &value) const {
    auto it = m_elements.find(value);
    return it != m_elements.end() && it->second.second;
  }

  std::unordered_set<T> elements() const {
    // idea: cache elements to make this operation efficient
    std::unordered_set<T> result;

    for (const auto &[value, tag] : m_elements) {
      if (tag.second)
        result.insert(value);
    }

    return result;
  }

  template <typename... Delta> void join(const Delta &...deltas) {
    // idea: use iterators to receive decompositions or a initializer_list
    join({deltas...});
  }

  void join(const std::vector<Delta> &deltas) {
    for (const auto &set : deltas) {
      for (const auto &element : set.m_elements) {
        auto [it, inserted] = m_elements.insert(element);
        if (!inserted)
          it->second = winner(it->second, element.second);
      }
    }
  }

  std::vector<Delta> split() const {
    std::vector<Delta> decompositions;
    decompositions.reserve(m_elements.size());

    for (const auto &element : m_elements) {
      decompositions.emplace_back();
      decompositions.back().m_elements.insert(element);
    }

    return decompositions;
  }

  bool operator==(const Delta &other) const {
    // the semantics of this operator are a bit foggy...
    return elements() == other.elements();
  }

  Delta operator+(const Delta &other) const {
    Delta result = *this;

    for (const auto &element : other.m_elements) {
      auto [it, inserted] = result.m_elements.insert(element);
      if (!inserted)
        it->second = winner(it->second, element.second);
    }

    return result;
  }

  Delta operator-(const Delta &other) const {
    Delta result = *this;

    for (const auto &[value, tag] : other.m_elements) {
      // element is not present in the remote set, keep it
      auto it = result.m_elements.find(value);
      if (it == result.m_elements.end())
        continue;

      // local operation is newer, keep it
      if (it->second.first > tag.first)
        continue;

      // concurrent operations, but local add wins over remote remove, keep it
      if (it->second.first == tag.first && it->second.second && !tag.second)
        continue;

      result.m_elements.erase(it);
    }

    return result;
  }

  template <typename U, typename V>
  friend std::ostream &operator<<(std::ostream &os,
                                  const LastWriterWinsSet<U, V> &obj) {
    auto elements = obj.elements();

    os << '{' << ' ';
    for (const auto &element : elements) {
      os << element << ' ';
    }
    os << '}';

    return os;
  }

private:
  static Tag winner(const Tag &lhs, const Tag &rhs) {
    if (lhs.first > rhs.first)
      return lhs;
    if (lhs.first < rhs.first)
      return rhs;

    // add wins when the timestamps are equal
    return std::make_pair(lhs.first, lhs.second || rhs.second);
  }

private:
  // key order is not maintained but allows for faster lookup and insertion
  std::unordered_map<T, Tag> m_elements;
};

} // namespace delta
