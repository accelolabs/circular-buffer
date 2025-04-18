#ifndef LABWORK_8_ACCELOLABS_CCIRCULARBUFFER_H
#define LABWORK_8_ACCELOLABS_CCIRCULARBUFFER_H

#include <memory>
#include <algorithm>
#include <stdexcept>

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBuffer {
public:
    using allocator_type = Allocator;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using value_type = typename Allocator::value_type;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = typename Allocator::difference_type;
    using size_type = typename Allocator::size_type;

protected:
    allocator_type allocator_;
    pointer data_;
    size_type capacity_;
    size_type size_;
    size_type head_;
    size_type tail_;

public:
    explicit CCircularBuffer(const size_type& capacity, const allocator_type& allocator = allocator_type()) :
            allocator_(allocator),
            data_(allocator_traits::allocate(allocator_, capacity)),
            capacity_(capacity),
            size_(0),
            head_(0),
            tail_(0)
    {}

    CCircularBuffer(CCircularBuffer& other) :
            allocator_(other.allocator_),
            data_(allocator_traits::allocate(allocator_, other.capacity_)),
            capacity_(other.capacity_),
            size_(0),
            head_(0),
            tail_(0)
    {
        for(size_type i = 0; i < other.size_; ++i) push_back(other.data_[i]);
    }

    CCircularBuffer& operator=(const CCircularBuffer& other) {
        if (this == &other) return *this;
        CCircularBuffer temp(other);
        swap(temp);
        return *this;
    }

    ~CCircularBuffer() {
        clear();
        allocator_traits::deallocate(allocator_, data_, capacity_);
    }

    CCircularBuffer(CCircularBuffer&&) = delete;
    CCircularBuffer& operator=(CCircularBuffer&&) = delete;

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
        return size_ == capacity_;
    }

    reference front() {
        if (empty()) throw std::runtime_error("Circular buffer is empty. Can't see the front.");
        return data_[head_];
    }

    const_reference front() const {
        if (empty()) throw std::runtime_error("Circular buffer is empty. Can't see the front.");
        return data_[head_];
    }

    reference back() {
        if(empty()) throw std::runtime_error("Circular buffer is empty. Can't see the back.");
        return data_[(tail_ + capacity_ - 1) % capacity_];
    }

    const_reference back() const {
        if(empty()) throw std::runtime_error("Circular buffer is empty. Can't see the back.");
        return data_[(tail_ + capacity_ - 1) % capacity_];
    }

    virtual void push_back(const value_type& value) {
        if (full()) throw std::runtime_error("Circular buffer overflow.");
        allocator_traits::construct(allocator_, &data_[tail_], value);
        tail_ = (tail_ + 1) % capacity_;
        size_++;
    }

    virtual void push_back(value_type&& value) {
        if (full()) throw std::runtime_error("Circular buffer overflow.");
        allocator_traits::construct(allocator_, &data_[tail_], std::move(value));
        tail_ = (tail_ + 1) % capacity_;
        size_++;
    }

    void pop_front() {
        if(empty()) throw std::runtime_error("Circular buffer is empty. Can't pop.");
        allocator_traits::destroy(allocator_, &data_[tail_]);
        head_ = (head_ + 1) % capacity_;
        size_--;
    }

    void clear() noexcept {
        for (int i = 0; i < size_; ++i) pop_front();
        size_ = 0;
    }

    reference operator[](const size_type& index) {
        if (index > size_) throw std::runtime_error("Index is more than circular buffer size.");
        return data_[(head_ + index) % capacity_];
    }

    const_reference operator[](const size_type& index) const {
        if (index > size_) throw std::runtime_error("Index is more than circular buffer size.");
        return data_[(head_ + index) % capacity_];
    }

    void swap(CCircularBuffer& other) noexcept {
        std::swap(allocator_, other.allocator_);
        std::swap(data_, other.data_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
    }

    // Random Access Iterator
    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;

    private:
        CCircularBuffer* buf_;
        size_type pos_;

    public:
        iterator():
                buf_(nullptr),
                pos_(0)
        {}

        iterator(CCircularBuffer<T, Allocator>* buf, size_type pos):
            buf_(buf),
            pos_(pos)
        {}

        iterator(const iterator& other):
            buf_(other.buf_),
            pos_(other.pos_)
        {}

        iterator& operator++() {
            pos_++;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        iterator operator+(const difference_type& n) {
            iterator tmp(*this);
            tmp.pos_ += n;
            return tmp;
        }

        iterator& operator+=(const difference_type& n) {
            pos_ += n;
            return *this;
        }

        iterator& operator--() {
            pos_--;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp(*this);
            --(*this);
            return tmp;
        }

        iterator operator-(const difference_type& n) {
            iterator tmp(*this);
            tmp.pos_ -= n;
            return tmp;
        }

        iterator& operator-=(const difference_type& n) {
            pos_ -= n;
            return *this;
        }

        reference operator*()  {
            return (*buf_)[pos_];
        }

        pointer operator->() {
            return &(operator*());
        }

        bool operator==(const iterator& other) const {
            return (buf_ == other.buf_ and pos_ == other.pos_);
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        bool operator<(const iterator& other) const {
            return (buf_ == other.buf_ and pos_ < other.pos_);
        }

        bool operator>(const iterator& other) const {
            return (buf_ == other.buf_ and pos_ > other.pos_);
        }

        bool operator>=(const iterator& other) const {
            return (buf_ == other.buf_ and pos_ >= other.pos_);
        }

        bool operator<=(const iterator& other) const {
            return (buf_ == other.buf_ and pos_ <= other.pos_);
        }

        reference operator[](const size_type& index) {
            if (index > buf_ -> size_) throw std::runtime_error("Index is more than circular buffer size.");
            return buf_ -> data_[(buf_ -> head_ + index) % buf_ -> capacity_];
        }
    };

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, size_);
    }
};

#endif //LABWORK_8_ACCELOLABS_CCIRCULARBUFFER_H
