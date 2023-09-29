#ifndef LABWORK_8_SSSIX6IX6IX_CIRCULARBUFFER_H
#define LABWORK_8_SSSIX6IX6IX_CIRCULARBUFFER_H

#include <cinttypes>
#include <memory>
#include <stdexcept>

template <typename T>
class CBIterator;

template <typename T, class Allocator = std::allocator<T>>
class CircularBuffer {
public:
    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef const T*                            const_pointer;
    typedef T&                                  reference;
    typedef CBIterator<CircularBuffer>          iterator;
    typedef CBIterator<const CircularBuffer>    const_iterator;
    typedef std::reverse_iterator<iterator>     const_reverse_iterator;
    typedef std::reverse_iterator<iterator>		reverse_iterator;
    typedef size_t					            size_type;
    typedef ptrdiff_t				            difference_type;
    typedef Allocator					        allocator_type;


    CircularBuffer() :
                size_(0),
                shift_(0),
                capacity_(0),
                values_(nullptr),
                allocator()
    {}

    CircularBuffer(size_type size) {
        size_ = size;
        capacity_ = size;
        shift_ = 0;
        values_ = allocator.allocate(size);

        for (size_t i = 0; i < size_; i++) {
            allocator.construct(&values_[i]);
        }
    };

    CircularBuffer(size_type size, value_type value) {
        assign(size, value);
    }

    CircularBuffer(std::initializer_list<value_type> init_list) {
        *this = init_list;
    }

    CircularBuffer(const CircularBuffer& other_buffer) {
        size_ = other_buffer.size_;
        capacity_ = other_buffer.capacity_;
        shift_ = other_buffer.shift;
        values_ = allocator.allocate(capacity_);

        for (size_t i = 0; i < size_; i++) {
            value_type value = other_buffer[i];

            allocator.construct(&values_[i], value);
       }
    }

    ~CircularBuffer() {
        clear();
        allocator.deallocate(values_, capacity_);
    }

    CircularBuffer& operator=(std::initializer_list<value_type> init_list) {
        clear();

        if (capacity_ < init_list.size()) capacity_ = init_list.size();
        size_ = 0;
        values_ = allocator.allocate(init_list.size());

        for (const value_type& value : init_list) {
            push_back(value);
        }

        return *this;
    }

    size_t capacity() {
        return capacity_;
    }

    void clear() {
        for (size_t i = 0; i < size_ - shift_; i++) {
            allocator.destroy(values_ + i);
        }

        for (size_t i = 1; i <= shift_; i++) {
            allocator.destroy(values_ + capacity_ - i);
        }

        size_ = 0;
    }

    iterator insert(iterator _iterator, const value_type& value) {
        if (capacity_ > size_) {
            push_back({});
        }

        iterator tmp_iterator = MakeIterator(_iterator.index_);
        value_type tmp_value = value;
        value_type now_value = value;

        for (iterator current = tmp_iterator; current != end(); current++) {
            tmp_value = *current;
            *current = now_value;
            now_value = tmp_value;
        }

        return tmp_iterator;
    }

    iterator insert(iterator _iterator, const size_t& count, const value_type& value) {
        pointer inserted_pointer = &(*_iterator);
        size_t inserted_index = _iterator.index_;

        for (int i = 0; i < count; i++) {
            push_back({});
        }

        size_t index = size_ - shift_ - 1;

        while (index >= (inserted_index + count) % capacity_) {
            values_[index] = values_[(index - count + capacity_) % capacity_];
            index = (index - 1 + capacity_) % capacity_;
        }

        for (size_t i = 0; i < count; i++) {
            values_[(i + inserted_index) % capacity_] = value;
        }

        return MakeIterator(inserted_index);
    }

    iterator erase(iterator erased_iterator) {
        size_t erased_index = &(*erased_iterator) - values_;

        for (size_t i = erased_index; i != size_ - shift_; i++) {
            if (i == capacity_) {
                i = 0;
            }

            values_[i] = values_[(i + 1) % capacity_];
        }

        --size_;
        allocator.destroy(values_ + size_ - shift_);

        erased_iterator.ChangeSize(size_);

        return erased_iterator;
    }

    virtual void push_back(const value_type& value) {

        if (size_ < capacity_) {
            ++size_;
        }

        pointer current = (values_ + size_ - shift_ - 1);
        allocator.construct(current, value);
    }

    virtual void push_front(const value_type& value) {
        if (size_ == 0) {
            ++size_;
            allocator.construct(values_, value);
        } else {

            if (size_ < capacity_) {
                ++size_;
                ++shift_;
            }

            pointer current = values_ + capacity_ - shift_;
            allocator.construct(current, value);
        }
    }

    void pop_back() {
        if (size_ != 0) {
            allocator.destroy(&values_[size_ - 1]);
            --size_;
        }
    }

    void pop_front() {
        erase(begin());
    }

    value_type& front() {
        return *begin();
    }

    value_type& back() {
        return *(end() - 1);
    }

    void reserve(size_type capacity) {
        capacity_ = capacity;
        values_ = allocator.allocate(capacity);
    };

    void assign(size_type size, value_type value) {

        for (size_t i = 0; i < size_; i++) {
            allocator.destroy(&values_[i]);
        }

        size_ = size;
        capacity_ = size;
        values_ = allocator.allocate(size);

        for (size_t i = 0; i < size; i++) {
            allocator.construct(&values_[i], value);
        }
    }

    iterator begin() {
        return MakeIterator(0);
    }

    iterator end() {
        return MakeIterator(size_);
    }

    const_iterator cbegin() {
        return const_iterator(values_);
    }

    const_iterator cend() {
        return const_iterator(values_ + size_);
    }

    size_t size() {
        return size_;
    }

    size_t max_size() {
        return (size_t)(-1) / sizeof(value_type);
    }

    bool empty() {
        return size_ == 0;
    }

    bool full() {
        return size_ == capacity_;
    }

    reference at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Buffer index out of range");
        }

        return values_[(index + capacity_ - shift_) % capacity_];
    }

    reference& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Buffer index out of range");
        }

        return values_[(index + capacity_ - shift_) % capacity_];
    }

    bool operator<(const CircularBuffer<T>& other) {
        size_t min_size = size();

        if (other.size() < min_size) {
            min_size = other.size();
        }

        for (int i = 0; i < min_size; i++) {
            if (*this[i] < other[i]) {
                return true;
            }
            if (*this[i] > other[i]) {
                return false;
            }
        }

        return (size() < other.size_);
    }

    bool operator>=(const CircularBuffer<T>& other) {
        return !(*this<other);
    }

    friend bool operator==(const CircularBuffer<value_type>& left, const CircularBuffer<value_type>& right) {
        if (left.size_ != right.size_) {
            return false;
        }

        for (size_t i = 0; i < left.size_; i++) {
            if (left[i] != right[i]) {
                return false;
            }
        }

        return true;
    }


    bool operator!=(const CircularBuffer<T>& other) {
        return !(*this == other);
    }

    bool operator>(const CircularBuffer<T>& other) {
        return ((other < *this) && (other != *this));
    }

    bool operator<=(const CircularBuffer<T>& other) {
        return !(*this > other);
    }


protected:
    pointer values_;
    size_t size_{};
    size_t shift_{};
    size_t capacity_{};
    allocator_type allocator;

    iterator MakeIterator(size_t index) {
        return iterator(index, values_, capacity_, size_, shift_);
    }
};

template <typename T, class Allocator = std::allocator<T>>
class CCircularBufferExt : public CircularBuffer<T, Allocator> {
public:
    typedef T                                                 value_type;
    typedef T*                                                pointer;
    typedef const T*                                          const_pointer;
    typedef T&                                                reference;
    typedef CBIterator<CircularBuffer<T, Allocator>>          iterator;
    typedef CBIterator<const CircularBuffer<T, Allocator>>    const_iterator;
    typedef std::reverse_iterator<iterator>                   const_reverse_iterator;
    typedef std::reverse_iterator<iterator>		              reverse_iterator;
    typedef size_t					                          size_type;
    typedef ptrdiff_t				                          difference_type;
    typedef Allocator					                      allocator_type;
    typedef CircularBuffer<T, Allocator>                      not_ext_buffer;

    using not_ext_buffer::values_;
    using not_ext_buffer::capacity_;
    using not_ext_buffer::size_;
    using not_ext_buffer::shift_;
    using not_ext_buffer:: allocator;

    CCircularBufferExt() : not_ext_buffer() {};

    CCircularBufferExt(size_type size) {
        size_ = size;
        capacity_ = size;
        shift_ = 0;
        values_ = allocator.allocate(size);

        for (size_t i = 0; i < size_; i++) {
            allocator.construct(&values_[i]);
        }
    };

    CCircularBufferExt(size_type size, value_type value) :
        not_ext_buffer(size, value)
    {}

    CCircularBufferExt(std::initializer_list<value_type> init_list) :
        not_ext_buffer(init_list)
    {}

    CCircularBufferExt(const CCircularBufferExt& other_buffer) {
        size_ = other_buffer.size_;
        capacity_ = other_buffer.capacity_;
        shift_ = other_buffer.shift;
        values_ = allocator.allocate(capacity_);

        for (size_t i = 0; i < size_; i++) {
            value_type value = other_buffer[i];

            allocator.construct(&values_[i], value);
        }
    }

    void push_front(const value_type& value) override {
        if (size_ + 1 >= capacity_) {
            Reallocate();
        }

        ++size_;
        ++shift_;

        pointer current = values_ + capacity_ - shift_;
        allocator.construct(current, value);
    }

    void push_back(const value_type& value) override {
        if (size_ + 1 >= capacity_) {
            Reallocate();
        }

        ++size_;

        pointer current = (values_ + size_ - shift_ - 1);
        allocator.construct(current, value);
    }

private :
    void Reallocate() {
        size_t new_capacity = 2 * capacity_;

        if (new_capacity == 0) {
            new_capacity = 2;
        }

        pointer new_values = allocator.allocate(new_capacity);

        for (size_t i = 0; i < size_ - shift_; i++) {
            allocator.construct(new_values + i, values_[i]);
        }

        for (size_t i = 1; i <= shift_; i++) {
            allocator.construct(&new_values[new_capacity - i], values_[capacity_ - i]);
        }

        size_t size = size_;
        this->clear();
        size_ = size;
        allocator.deallocate(values_, capacity_);

        values_ = new_values;
        capacity_ = new_capacity;
    }

};

template <typename Buffer>
class CBIterator {
public:
    typedef std::random_access_iterator_tag           iterator_category;
    typedef typename Buffer::value_type               value_type;
    typedef ptrdiff_t                                 difference_type;
    typedef typename Buffer::pointer                  pointer;
    typedef typename Buffer::reference                reference;
    typedef typename Buffer::iterator                 iterator;


    CBIterator() :
            index_(0),
            begin_pointer_(nullptr),
            capacity_(0),
            size_(0),
            shift_(0)
            {}

    CBIterator(size_t index, pointer begin_pointer, size_t capacity, size_t size, size_t shift) :
            index_(index),
            begin_pointer_(begin_pointer),
            capacity_(capacity),
            size_(size),
            shift_(shift)
            {}

    void ChangeSize(size_t size) {
        size_ = size;
    }

    void ChangeCapacity(size_t capacity) {
        capacity_ = capacity;
    }

    CBIterator& operator=(const CBIterator& rhs) {
        index_ = rhs.index_;
        begin_pointer_ = rhs.begin_pointer_;
        capacity_ = rhs.capacity_;
        size_ = rhs.size_;
        shift_ = rhs.shift_;

        return *this;
    }

    iterator& operator++() {
        ++index_;

        if (index_ > size_) index_ = 0;

        return *this;
    }

    iterator& operator--() {
        --index_;

        if (index_ < 0) {
            index_ = size_;
        }

        return *this;
    }

    iterator operator++(int) {
        iterator temporary = *this;
        ++(*this);

        return temporary;
    }

    iterator operator--(int) {
        iterator temporary = *this;
        --(*this);

        return temporary;
    }

    reference& operator*() {
        return *(begin_pointer_ + (index_ - shift_ + capacity_) % capacity_);
    }

    pointer operator->() {
        return (begin_pointer_ + (index_ - shift_ + capacity_) % capacity_);
    }

    bool operator<(const iterator& rhs) {
        return (*this - rhs > 0);
    }

    bool operator>(const iterator& rhs) {
        return (rhs < *this);
    }

    bool operator<=(const iterator rhs) {
        return !(*this > rhs);
    }

    bool operator >=(const iterator rhs) {
        return !(*this < rhs);
    }

    bool operator==(const iterator& rhs) {
        return (index_ == rhs.index_);
    }

    bool operator!=(const iterator& rhs) {
        return index_ != rhs.index_;
    }

    difference_type operator-(iterator other) {
        return index_ - other.index_;
    }

    CBIterator operator+(size_t n) {
        iterator temporary = *this;

        return temporary += n;
    }

    CBIterator operator-(size_t n) {
        iterator temporary = *this;

        return temporary -= n;
    }

    CBIterator& operator-=(size_t n) {
        return (*this) += -n;
    }

    CBIterator& operator+=(size_t n) {
        difference_type m = n;
        if (m >= 0) {
            while (m--) {
                ++(*this);
            }
        } else {
            while (m++) {
                --(*this);
            }
        }

        return *this;
    }

    reference operator[](size_t n) {
        return *(*this + n);
    }

    size_t get_index() {
        return index_;
    }

    friend CircularBuffer<value_type>;

private:
    size_t index_;
    size_t capacity_;
    size_t size_;
    size_t shift_;
    pointer begin_pointer_;
};

template <typename T>
CBIterator<T> operator+(int n, const CBIterator<T>& rhs) {
    return rhs + n;
}

#endif //LABWORK_8_SSSIX6IX6IX_CIRCULARBUFFER_H
