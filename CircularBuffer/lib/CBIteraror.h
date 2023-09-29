//
// Created by 123 on 23.03.2023.
//

#ifndef LAB8_CBITERAROR_H
#define LAB8_CBITERAROR_H

template <typename T>
class CBIterator {
    T* pointer;

    T* operator++() {
        ++pointer;
        return pointer;
    }

    T* operator--() {
        --pointer;
        return  pointer;
    }

    T* operator--(CBIterator<T> iterator) {
    }

    T& operator*() {
        return *pointer;
    }

    T* operator->() {
        return pointer;
    }

    bool operator<(const CBIterator<T>& rhs) {
        return (pointer < pointer);
    }
};

#endif //LAB8_CBITERAROR_H
