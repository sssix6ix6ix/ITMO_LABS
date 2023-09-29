#pragma once

#include <cinttypes>
#include <iterator>

namespace algo {

    template<typename InputIterator, typename UnaryPredicate>
    bool all_of(InputIterator first,
                InputIterator last,
                UnaryPredicate predicate) {

        while (first != last) {
            if (!predicate(*first)) {
                return false;
            }

            ++first;
        }

        return true;
    };

    template<typename InputIterator, typename UnaryPredicate>
    bool any_of(InputIterator first,
                InputIterator last,
                UnaryPredicate predicate) {

        while (first != last) {
            if (predicate(*first)) {
                return true;
            }

            ++first;
        }

        return false;
    }

    template<typename InputIterator, typename UnaryPredicate>
    bool none_of(InputIterator first,
                 InputIterator last,
                 UnaryPredicate predicate) {

        return !(algo::any_of(first, last, predicate));
    }

    template<typename InputIterator, typename UnaryPredicate>
    bool one_of(InputIterator first,
                InputIterator last,
                UnaryPredicate predicate) {

        bool found_one = false;

        while (first != last) {
            if (predicate(*first)) {
                if (found_one) {
                    return false;
                } else {
                    found_one = true;
                }
            }

            ++first;
        }

        return found_one;
    }

    template<typename InputIterator, typename UnaryPredicate>
    bool is_sorted(InputIterator first,
                   InputIterator last,
                   UnaryPredicate predicate) {

        InputIterator prev = first;
        ++first;

        while (first != last) {
            if (!predicate(*prev, *first)) {
                return false;
            }

            ++prev;
            ++first;
        }

        return true;
    }

    template<typename InputIterator, typename UnaryPredicate>
    bool is_partitioned(InputIterator first,
                        InputIterator last,
                        UnaryPredicate predicate) {

        bool found_pivot = false;
        InputIterator prev = first;
        ++first;

        while (first != last) {
            if (predicate(*first) != predicate(*prev)) {
                if (found_pivot) {
                    return false;
                } else {
                    found_pivot = true;
                }
            }

            ++prev;
            ++first;
        }

        return true;
    }

    template<typename InputIterator, typename T>
    InputIterator find_not(InputIterator first,
                           InputIterator last,
                           T target) {
        while (first != last) {
            if (*first != target) {
                return first;
            }

            ++first;
        }

        return last;
    }

    template<typename InputIterator, typename T>
    InputIterator find_backward(InputIterator first,
                                InputIterator last,
                                T target) {

        InputIterator founded_element = last;

        while (first != last) {
            if (*first == target) {
                founded_element = first;
            }

            ++first;
        }

        return founded_element;
    }

    template<typename BidirectionalIterator, typename UnaryPredicate>
    bool is_palindrome(BidirectionalIterator first,
                       BidirectionalIterator last,
                       UnaryPredicate predicate) {

        while (first != last) {
            --last;

            if (last == first) {
                return true;
            }

            if (predicate(*last) != predicate(*first)) {
                return false;
            }

            ++first;
        }

        return true;
    }

    template<typename BidirectionalIterator>
    bool is_palindrome(BidirectionalIterator first,
                       BidirectionalIterator last) {

        while (first != last) {
            --last;

            if (last == first) {
                return true;
            }

            if (*first != *last) {
                return false;
            }

            ++first;
        }

        return true;
    }

    template<typename T>
    class IteratorXRange;

    template<typename T>
    class xrange {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using iterator = IteratorXRange<xrange<T>>;
        using const_iterator = IteratorXRange<const xrange<T>>;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using iterator_category = std::input_iterator_tag;

        xrange(value_type start, value_type end) :
                start_(start),
                end_(end),
                step_(1) {};

        xrange(value_type end) :
                start_(),
                end_(end),
                step_(1) {};

        xrange(value_type start, value_type end, value_type step) :
                start_(start),
                end_(end),
                step_(step) {};

        iterator begin() {
            return iterator(start_, step_);
        }

        iterator end() {
            return iterator(end_, step_);
        }

    private:
        value_type start_;
        value_type end_;
        value_type step_;
    };

    template<typename XRange>
    class IteratorXRange {
    public:

        using value_type = typename XRange::value_type;
        using reference = typename XRange::reference;
        using const_reference = typename XRange::const_reference;
        using pointer = value_type*;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using iterator_category = std::input_iterator_tag;

        IteratorXRange(value_type value, value_type step) :
                value_(value),
                step_(step) {}

        IteratorXRange& operator++() {
            value_ += step_;

            return *this;
        }

        IteratorXRange operator++(int) {
            IteratorXRange tmp = *this;

            ++(*this);

            return tmp;
        }

        bool operator==(IteratorXRange other) {
            return value_ == *other;
        }

        bool operator!=(IteratorXRange other) {
            return (*other > value_);
        }

        value_type& operator*() {
            return value_;
        }

        value_type& operator->() {
            return *this;
        }


    private:
        value_type value_;
        value_type step_;
    };

    template<typename Container1, typename Container2>
    class IteratorZip;

    template<typename Container1, typename Container2>
    class zip {
    public:
        using type1 = typename Container1::value_type;
        using type2 = typename Container2::value_type;
        using value_type = std::pair<type1, type2>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using iterator = IteratorZip<Container1, Container2>;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using iterator_category = std::input_iterator_tag;

        zip(Container1& lhs_container, Container2& rhs_container) :
                lhs_container_(lhs_container),
                rhs_container_(rhs_container) {}

        iterator begin() {
            return iterator(lhs_container_.begin(), rhs_container_.begin());
        }

        iterator end() {
            return iterator(lhs_container_.end(), rhs_container_.end());
        }

    private:
        Container1& lhs_container_;
        Container2& rhs_container_;
    };

    template<typename Container1, typename Container2>
    class IteratorZip {
    public:
        using type_lhs = typename Container1::value_type;
        using type_rhs = typename Container2::value_type;
        using iterator_lhs = typename Container1::iterator;
        using iterator_rhs = typename Container2::iterator;
        using value_type = std::pair<type_lhs, type_rhs>;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;

        IteratorZip(iterator_lhs lhs_iterator, iterator_rhs rhs_iterator) :
                lhs_iterator_(lhs_iterator),
                rhs_iterator_(rhs_iterator) {}

        value_type operator*() {
            return {*lhs_iterator_, *rhs_iterator_};
        }

        pointer operator->() {
            return *this;
        }

        bool operator!=(IteratorZip other) {
            return (other.lhs_iterator_ != lhs_iterator_ &&
                    other.rhs_iterator_ != rhs_iterator_);
        }

        bool operator==(IteratorZip other) {
            return !(*this != other);
        }

        IteratorZip& operator++() {
            ++lhs_iterator_;
            ++rhs_iterator_;

            return *this;
        }

        IteratorZip operator++(int) {
            IteratorZip tmp = *this;

            ++(*this);

            return tmp;
        }

    private:
        iterator_lhs lhs_iterator_;
        iterator_rhs rhs_iterator_;
    };
}
