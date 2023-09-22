#pragma once

#include <iostream>
#include <vector> 
#include <set>

template <typename T>
class gset
{
private:
    std::set<T> s;

public:
    gset();

    bool operator == (const gset<T> & o) const;

    gset<T> add (const T& val);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const gset<U>& set);

    bool in (const T& val);

    template <typename... GSets>
    void join (const GSets&... sets);

    void join(const std::vector<gset<T>>& sets);

    gset<T> operator+(const gset<T>& other) const;
    gset<T> operator-(const gset<T>& other) const;

    std::vector<gset<T>> split() const;
};

#include "gset.cpp"

