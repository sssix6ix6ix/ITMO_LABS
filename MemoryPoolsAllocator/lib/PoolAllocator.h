#ifndef LABWORK_9_SSSIX6IX6IX_PULLALLOCATOR_H
#define LABWORK_9_SSSIX6IX6IX_PULLALLOCATOR_H

#include <cinttypes>
#include <vector>

constexpr size_t default_number_of_pools = 2;
constexpr size_t default_size_of_pool = 100000;
constexpr size_t default_size_of_block = 16;

template <typename T>
class Pool {
public:
    using value_type = T;
    using pointer    = T*;
    using interval   = std::pair<size_t, size_t>;

    Pool(size_t size_of_pool = default_size_of_pool,
         size_t size_of_block = default_size_of_block)
    {
        size_of_pool_ = size_of_pool;
        size_of_block_ = size_of_block;
        size_t number_bytes  = size_of_pool * size_of_block_ * sizeof(value_type);
        begin_date_ = reinterpret_cast<pointer>(malloc(number_bytes));
        intervals_.reserve(size_of_pool);
        deallocate(begin_date_, size_of_pool_);
    }

    ~Pool() {
        delete[] begin_date_;
    }

    pointer allocate(size_t need_blocks) {
        for (size_t i = 0; i < intervals_.size(); i++) {
            size_t& l = intervals_[i].first;
            size_t& r = intervals_[i].second;

            if (r - l == need_blocks) {
                intervals_.erase(intervals_.begin() + i);

                return (begin_date_ + l * size_of_block_);
            }

            if (r - l > need_blocks) {
                size_t tmp = l;
                l += need_blocks;

                return (begin_date_ + tmp * size_of_block_);
            }
        }

        return nullptr;
    }

    void deallocate(pointer ptr,size_t need_blocks) {
        size_t deallocated_first = (ptr - begin_date_) / size_of_block_;
        size_t deallocated_last = deallocated_first + need_blocks;

        for (int i = 0; i < intervals_.size(); i++) {
            size_t& l = intervals_[i].first;
            size_t& r = intervals_[i].second;

            if (deallocated_last == l) {
                l = deallocated_first;

                return;
            }

            if (deallocated_first == r) {
                r = deallocated_last;
                i++;

                if (i < intervals_.size()){
                    if (intervals_[i].first == r) {
                        r = intervals_[i].second;
                        intervals_.erase(intervals_.begin() + i);
                    }
                }

                return;
            }

            if (deallocated_last < l) {
                intervals_.insert(intervals_.begin() + i, {deallocated_first, deallocated_last});

                return;
            }

            if (l == deallocated_last) {
                r = deallocated_last;

                return;
            }
        }

        intervals_.insert(intervals_.begin(), {deallocated_first, deallocated_last});

        return;
    }

    pointer GetBeginDate() {
        return begin_date_;
    }

private:
    std::vector<interval> intervals_;
    size_t size_of_pool_;
    size_t size_of_block_;
    pointer begin_date_;

};

template <typename T,
        size_t number_of_pools_ = default_number_of_pools,
        size_t size_of_pool_ = default_size_of_pool,
        size_t size_of_block_ = default_size_of_block
>
class PoolAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using pool = Pool<value_type> ;

    template<class U>
    struct rebind {
        using other = PoolAllocator<U, number_of_pools_, size_of_pool_, size_of_block_>;
    };

    PoolAllocator() {
        pools_ = new pool[number_of_pools_];
    }

    template<class U>
    constexpr PoolAllocator (const PoolAllocator<U>&) noexcept {}

    ~PoolAllocator() {
        delete[] pools_;
    }

    pointer allocate(size_t n) {
        size_t need_space = NeedBlocksFor(n);

        for (size_t i = 0; i < number_of_pools_; i++) {
            pointer returned_pointer = pools_[i].allocate(need_space);

            if (returned_pointer != nullptr) {
                return returned_pointer;
            }
        }

        throw std::bad_alloc();
    }

    void deallocate(pointer ptr, size_t n) noexcept {
        size_t need_blocks = NeedBlocksFor(n);

        for (size_t i = 0; i < number_of_pools_; i++) {
            if (ptr > pools_[i].GetBeginDate()) {
                pools_[i].deallocate(ptr, need_blocks);
            }
        }
    }

private:
    pool* pools_;

    size_t NeedBlocksFor(size_t n) {
        return (n + size_of_block_ - 1) / size_of_block_;
    }
};

#endif //LABWORK_9_SSSIX6IX6IX_PULLALLOCATOR_H