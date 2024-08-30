#pragma once

#include <queue>

using std::less;
using std::priority_queue;
using std::vector;

template<class T,
    class CONTAINER = vector<T>,
    class COMPARE = less<typename CONTAINER::value_type>>
class priority_heap : public priority_queue<T, CONTAINER, COMPARE>
{
public:
    typedef typename priority_queue<T, CONTAINER, COMPARE>::container_type::const_iterator const_iterator;

    const_iterator begin()
    {
        return this->c.begin();
    }

    const_iterator end()
    {
        return this->c.end();
    }

    const_iterator find(const T& val) const
    {
        auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first != last) 
        {
            if (*first == val)
            {
	            return first;
            }

            ++first;
        }

        return last;
    }
};