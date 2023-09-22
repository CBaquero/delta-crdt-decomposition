#include "gset.h"

template <typename T>
gset<T>::gset() {}

template <typename T>
bool gset<T>::operator == (const gset<T> & o) const {
    return s == o.s;
}

template <typename T>
gset<T> gset<T>::add(const T& val) {
    gset<T> res; // delta obj

    // Insert value into the state and check if insertion was successful
    auto [it, inserted] = s.insert(val);

    // If insertion was successful, add the value to the delta object
    if (inserted) {
        res.s.insert(val);
    }

    return res;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const gset<U>& set) {
    os << "{ ";
    for (const auto& elem : set.s) {
        os << elem << " ";
    }
    os << "}";
    return os;
}

template <typename T>
bool gset<T>::in(const T& val) {
    return s.count(val) == 1; // Sets have either 0 or 1 occurrences
}

//template <typename T>
//void gset<T>::join(const gset<T>& o) {
//    s.insert(o.s.begin(), o.s.end());
template <typename T>
template <typename... GSets>
void gset<T>::join(const GSets&... sets) {
    // Fold expression to join all sets
    (... , s.insert(sets.s.begin(), sets.s.end()));
}

template <typename T>
void gset<T>::join(const std::vector<gset<T>>& sets) {
    for (const auto& set : sets) {
        s.insert(set.s.begin(), set.s.end());
    }
}

template <typename T>
gset<T> gset<T>::operator+(const gset<T>& other) const {
    gset<T> result = *this;  // Start with the contents of the current set
    result.join(other);      // Use the join function to combine with the other set
    return result;
}

template <typename T>
gset<T> gset<T>::operator-(const gset<T>& other) const {
    gset<T> result = *this;  // Start with the contents of the current set

    for(const auto& elem : other.s) {
        result.s.erase(elem);  // Erase the element from the result set if it exists
    }

    return result;
}

template <typename T>
std::vector<gset<T>> gset<T>::split() const {
    std::vector<gset<T>> result;

    for (const auto& element : s) {
        gset<T> individual;
        individual.add(element);
        result.push_back(individual);
    }

    return result;
}




