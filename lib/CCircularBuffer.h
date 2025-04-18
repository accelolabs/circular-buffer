#ifndef CCIRCULARBUFFER_H
#define CCIRCULARBUFFER_H

#include <cassert>
#include <iterator>
#include <memory>
#include <algorithm>

#include <debug/vector>


template<typename T, typename A = std::allocator<T>>
class CCircularBuffer {
    using value_type = T;
    using allocator_type = A;
    using size_type = typename std::allocator_traits<A>::size_type;
    using difference_type = typename std::allocator_traits<A>::difference_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename std::allocator_traits<A>::pointer;
    using const_pointer = typename std::allocator_traits<A>::const_pointer;

private:
    allocator_type allocator_;
    pointer data_;
    size_type capacity_;
    size_type size_;
    size_type head_;
    size_type tail_;

public:
    explicit CCircularBuffer(const size_type& capacity, const allocator_type& allocator = allocator_type()) :
            allocator_(allocator),
            data_(std::allocator_traits<allocator_type>::allocate(allocator_, capacity)),
            capacity_(capacity),
            size_(0),
            head_(0),
            tail_(0)
    {}


    ~CCircularBuffer() {
        clear();

        std::allocator_traits<allocator_type>::deallocate(
            allocator_,
            data_,
            capacity_
        );
    }


    /* Copy */

    CCircularBuffer(CCircularBuffer& other) = delete;
    CCircularBuffer& operator=(const CCircularBuffer& other) = delete;


    /* Move */

    CCircularBuffer(CCircularBuffer&&) = delete;
    CCircularBuffer& operator=(CCircularBuffer&&) = delete;


    /* Container status */

    size_type size() const noexcept {
        return size_;
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    bool full() const noexcept {
        return capacity_ > 0 && size_ == capacity_;
    }


    /* Data access */

    reference front() noexcept {
        assert(!empty());

        return data_[head_];
    }

    const_reference front() const noexcept {
        assert(!empty());

        return data_[head_];
    }

    reference back() noexcept {
        assert(!empty());

        return data_[(tail_ + capacity_ - 1) % capacity_];
    }

    const_reference back() const noexcept {
        assert(!empty());

        return data_[(tail_ + capacity_ - 1) % capacity_];
    }

    reference operator[](size_type index) noexcept {
        assert(index < size_);

        return data_[(head_ + index) % capacity_];
    }

    const_reference operator[](size_type index) const noexcept {
        assert(index < size_);

        return data_[(head_ + index) % capacity_];
    }


    /* Data modification */

    void push_back(const value_type& value) {
        assert(!full());

        std::allocator_traits<allocator_type>::construct(
            allocator_,
            &data_[tail_],
            value
        );

        tail_ = (tail_ + 1) % capacity_;
        size_ += 1;
    }

    void push_back(value_type&& value) {
        assert(!full());

        std::allocator_traits<allocator_type>::construct(
            allocator_,
            &data_[tail_],
            std::move(value)
        );

        tail_ = (tail_ + 1) % capacity_;
        size_ += 1;
    }

    void pop_front() {
        assert(!empty());

        std::allocator_traits<allocator_type>::destroy(
            allocator_,
            &data_[head_]
        );

        head_ = (head_ + 1) % capacity_;
        size_ -= 1;
    }

    void clear() {
        while (!empty()) pop_front();
        size_ = 0;
    }


    /* Swap */

    void swap(CCircularBuffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
    }


    /* Get allocator */

    allocator_type get_allocator() const noexcept {
        return allocator_;
    }


    class CCircularBufferIterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = typename CCircularBuffer::difference_type;
            using pointer = T*;
            using reference = T&;
            using iterator = CCircularBufferIterator;

        private:
            CCircularBuffer* buf_;
            size_type pos_;

        public:
            CCircularBufferIterator():
                buf_(nullptr),
                pos_(0)
            {}

            CCircularBufferIterator(CCircularBuffer<T, A>* buf, size_type pos):
                buf_(buf),
                pos_(pos)
            {}

            CCircularBufferIterator(const CCircularBufferIterator& other):
                buf_(other.buf_),
                pos_(other.pos_)
            {}


            /* Increment / Decrement */

            iterator& operator++() noexcept {
                assert(buf_);
                assert(buf_ -> size() > 0);
                pos_++;
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator tmp(*this);
                operator++();
                return tmp;
            }

            iterator& operator--() noexcept {
                assert(buf_);
                assert(buf_ -> size() > 0);
                pos_--;
                return *this;
            }

            iterator operator--(int) noexcept {
                iterator tmp(*this);
                operator--();
                return tmp;
            }


            /* Arithmetic */

            iterator& operator+=(const difference_type& n) {
                assert(buf_);
                pos_ += n;
                return *this;
            }

            iterator operator+(const difference_type& n) {
                iterator tmp(*this);
                tmp += n;
                return tmp;
            }

            iterator& operator-=(const difference_type& n) {
                assert(buf_);
                pos_ -= n;
                return *this;
            }

            iterator operator-(const difference_type& n) {
                iterator tmp(*this);
                tmp -= n;
                return tmp;
            }


            /* Element access */

            reference operator*() {
                assert(buf_);

                return (*buf_)[pos_];
            }

            reference operator*() const {
                assert(buf_);

                return (*buf_)[pos_];
            }

            pointer operator->() {
                return &(**this);
            }

            pointer operator->() const {
                return &(**this);
            }

            reference operator[](const size_type& index) {
                return (*buf_)[pos_ + index];
            }

            reference operator[](const size_type& index) const {
                return (*buf_)[pos_ + index];
            }


            /* Comparisons */

            bool operator==(const iterator& other) const {
                return pos_ == other.pos_;
            }

            bool operator!=(const iterator& other) const {
                return !(*this == other);
            }

            bool operator<(const iterator& other) const {
                return pos_ < other.pos_;
            }

            bool operator>(const iterator& other) const {
                return pos_ > other.pos_;
            }

            bool operator>=(const iterator& other) const {
                return pos_ >= other.pos_;
            }

            bool operator<=(const iterator& other) const {
                return pos_ <= other.pos_;
            }
    };


    using iterator = CCircularBufferIterator;
    using const_iterator = const CCircularBufferIterator; 


    iterator begin() {
        return iterator(this, this -> head_);
    }

    const_iterator begin() const {
        return const_iterator(this, this -> head_);
    }

    iterator end() {
        return iterator(this, this -> tail_);
    }

    const_iterator end() const {
        return const_iterator(this, this -> tail_);
    }

};



#endif // CCIRCULARBUFFER_H
