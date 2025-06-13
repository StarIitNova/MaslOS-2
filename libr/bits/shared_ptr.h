#pragma once

#include <libr/std/atomic>
#include <libr/std/exception>
#include <libr/std/utility>

#include <libr/bits/unique_ptr.h>

namespace std {

namespace __bits {

template<typename T>
struct shrd_control_block {
    T *ptr_;
    std::atomic<size_t> shared_count_;
    std::atomic<size_t> weak_count_;

    shrd_control_block(T *__p) : ptr_(__p), shared_count_(1), weak_count_(0) {}

    virtual ~shrd_control_block() = default;
};

template<typename T>
struct __CBWO_x : __bits::shrd_control_block<T> {
    T obj_;
    template<typename ...Args2>
    __CBWO_x(Args2 &&...__args2) : obj_(std::forward<Args2>(__args2)...) {
        this->ptr_ = &obj_;
        this->shared_count_ = 1;
        this->weak_count_ = 0;
    }
};

template<typename T, class Deleter>
struct __CBWD_x : __bits::shrd_control_block<T> {
    Deleter deleter_;

    __CBWD_x(T *__ptr, Deleter __deleter) : shrd_control_block<T>(__ptr), deleter_(__deleter) {}

    ~__CBWD_x() {
        if (this->ptr)
            deleter_(this->__ptr);
    }
};

}

class bad_weak_ptr : exception {
public:
    const char *what() const override {
        return "std::bad_weak_ptr";
    }
};

template<typename T> class weak_ptr;
template<typename T> class shared_ptr;

template<typename T>
class shared_ptr {
public:
    shared_ptr() noexcept: mPtr_(nullptr), mCtrl_(nullptr) {}

    explicit shared_ptr(T *__ptr) : mPtr_(__ptr), mCtrl_(new __bits::shrd_control_block<T>(__ptr)) {}

    shared_ptr(const shared_ptr &__other) noexcept : mPtr_(__other.mPtr_), mCtrl_(__other.mCtrl_) {
        if (mCtrl_) ++mCtrl_->shared_count_;
    }

    explicit shared_ptr(const weak_ptr<T> &__wptr) {
        mCtrl_ = __wptr.mCtrl_;
        if (!mCtrl_ || mCtrl_->shared_count_ == 0) {
            throw std::bad_weak_ptr();
        }

        mPtr_ = mCtrl_->ptr_;
        ++mCtrl_->shared_count_;
    }

    shared_ptr &operator=(const shared_ptr &__other) noexcept {
        if (this != &__other) {
            release();
            mPtr_ = __other.mPtr_;
            mCtrl_ = __other.mCtrl_;
            if (mCtrl_) ++mCtrl_->shared_count_;
        }

        return *this;
    }

    template<class Y>
    shared_ptr &operator=(const shared_ptr<Y> &__r) noexcept {
        release();
        mPtr_ = __r.mPtr_;
        mCtrl_ = __r.mCtrl_;
        if (mCtrl_) ++mCtrl_->shared_count_;

        return *this;
    }

    shared_ptr &operator=(shared_ptr &&__r) noexcept {
        if (this != &__r) {
            release();
            mPtr_ = __r.mPtr_;
            mCtrl_ = __r.mCtrl_;
            __r.mPtr_ = nullptr;
            __r.mCtrl_ = nullptr;
        }

        return *this;
    }

    template<class Y>
    shared_ptr &operator=(shared_ptr<Y> &&__r) noexcept {
        release();
        mPtr_ = __r.mPtr_;
        mCtrl_ = __r.mCtrl_;
        __r.mPtr_ = nullptr;
        __r.mCtrl_ = nullptr;

        return *this;
    }

    template<class Y, class Deleter>
    shared_ptr &operator=(std::unique_ptr<Y, Deleter> &&__r) {
        release();
        if (__r.get()) {
            mPtr_ = __r.get();
            mCtrl_ = new __bits::__CBWD_x<Y, Deleter>(__r.get(), __r.get_deleter());
        } else {
            mPtr_ = nullptr;
            mCtrl_ = nullptr;
        }
    }

    ~shared_ptr() {
        release();
    }

    T *get() const noexcept { return mPtr_; }
    T &operator*() const noexcept { return *mPtr_; }
    T *operator->() const noexcept { return mPtr_; }

    T &operator[](size_t __idx) noexcept { return mPtr_[__idx]; }

    size_t use_count() const noexcept {
        return mCtrl_ ? mCtrl_->shared_count_.load() : 0;
    }

    bool unique() const noexcept {
        return use_count() == 1;
    }

    explicit operator bool() const noexcept {
        return mPtr_ != nullptr;
    }

private:
    T* mPtr_;
    __bits::shrd_control_block<T> *mCtrl_;

    void release() {
        if (mCtrl_) {
            if (--mCtrl_->shared_count_ == 0) {
                delete mPtr_;
                if (--mCtrl_->weak_count_ == 0) {
                    delete mCtrl_;
                }
            }
        }
    }

    friend class weak_ptr<T>;
};

template<typename T>
class weak_ptr {
public:
    weak_ptr() noexcept : mCtrl_(nullptr) {}

    weak_ptr(const shared_ptr<T> &__spt) noexcept : mCtrl_(__spt.mCtrl_) {
        if (mCtrl_) ++mCtrl_->weak_count_;
    }

    weak_ptr(const weak_ptr &__other) noexcept : mCtrl_(__other.mCtrl_) {
        if (mCtrl_) ++mCtrl_->weak_count_;
    }

    weak_ptr &operator=(const weak_ptr &__other) noexcept {
        if (this != &__other) {
            if (mCtrl_ && --mCtrl_->weak_count_ == 0 && mCtrl_->shared_count_ == 0) {
                delete mCtrl_;
            }

            mCtrl_ = __other.mCtrl_;
            if (mCtrl_) ++mCtrl_->weak_count_;
        }

        return *this;
    }

    template<class Y>
    weak_ptr &operator=(const weak_ptr<Y> &__r) noexcept {
        static_assert(std::is_convertible<Y *, T *>::value, "Incompatible weak_ptr assignment");
        weak_ptr(__r).swap(*this);
        return *this;
    }

    template<class Y>
    weak_ptr &operator=(const shared_ptr<Y> &__r) noexcept {
        static_assert(std::is_convertible<Y *, T *>::value, "Incompatible shared_ptr assignment to weak_ptr");
        weak_ptr(__r).swap(*this);
        return *this;
    }

    weak_ptr &operator=(weak_ptr &&__r) noexcept {
        if (this != &__r) {
            if (mCtrl_ && --mCtrl_->weak_count_ == 0 && mCtrl_->shared_count_ == 0) {
                delete mCtrl_;
            }

            mCtrl_ = __r.mCtrl_;
            __r.mCtrl_ = nullptr;
        }
    }

    template<class Y>
    weak_ptr &operator=(const weak_ptr<Y> &&__r) noexcept {
        static_assert(std::is_convertible<Y *, T *>::value, "Incompatible weak_ptr move assignment");
        weak_ptr(std::move(__r)).swap(*this);
        return *this;
    }

    ~weak_ptr() {
        if (mCtrl_ && --mCtrl_->weak_count_ == 0 && mCtrl_->shared_count_ == 0) {
            delete mCtrl_;
        }
    }

    shared_ptr<T> lock() const noexcept {
        if (mCtrl_ && mCtrl_->shared_count_ > 0) {
            return shared_ptr<T>(*this);
        }

        return shared_ptr<T>();
    }

    bool expired() const noexcept {
        return !mCtrl_ || mCtrl_->shared_count_ == 0;
    }

private:
    __bits::shrd_control_block<T> *mCtrl_;

    friend class shared_ptr<T>;
};

template<typename T, typename ...Args>
static inline shared_ptr<T> make_shared(Args &&...__args) {
    auto *__cb = new __bits::__CBWO_x<T>(std::forward<Args>(__args)...);
    return shared_ptr<T>(__cb->ptr_, __cb);
}

template<class Y, class V>
static inline bool operator==(const shared_ptr<Y> &__lhs, const shared_ptr<Y> &__rhs) {
    return __lhs.get() == __rhs.get();
}

template<class Y, class V>
static inline bool operator!=(const shared_ptr<Y> &__lhs, const shared_ptr<Y> &__rhs) {
    return __lhs.get() != __rhs.get();
}

template<class Y, class V>
static inline bool operator>(const shared_ptr<Y> &__lhs, const shared_ptr<Y> &__rhs) {
    return __lhs.get() > __rhs.get();
}

template<class Y, class V>
static inline bool operator<(const shared_ptr<Y> &__lhs, const shared_ptr<Y> &__rhs) {
    return __lhs.get() < __rhs.get();
}

template<class Y, class V>
static inline bool operator>=(const shared_ptr<Y> &__lhs, const shared_ptr<Y> &__rhs) {
    return __lhs.get() >= __rhs.get();
}

template<class Y, class V>
static inline bool operator<=(const shared_ptr<Y> &__lhs, const shared_ptr<Y> &__rhs) {
    return __lhs.get() <= __rhs.get();
}

template<class Y, class V>
static inline bool operator==(const shared_ptr<Y> &__lhs, std::nullptr_t) {
    return __lhs.get() == nullptr;
}

template<class Y, class V>
static inline bool operator!=(const shared_ptr<Y> &__lhs, std::nullptr_t) {
    return __lhs.get() != nullptr;
}

template<class Y, class V>
static inline bool operator>(const shared_ptr<Y> &__lhs, std::nullptr_t) {
    return __lhs.get() > nullptr;
}

template<class Y, class V>
static inline bool operator<(const shared_ptr<Y> &__lhs, std::nullptr_t) {
    return __lhs.get() < nullptr;
}

template<class Y, class V>
static inline bool operator>=(const shared_ptr<Y> &__lhs, std::nullptr_t) {
    return __lhs.get() >= nullptr;
}

template<class Y, class V>
static inline bool operator<=(const shared_ptr<Y> &__lhs, std::nullptr_t) {
    return __lhs.get() <= nullptr;
}

template<class Y, class V>
static inline bool operator>(std::nullptr_t, const shared_ptr<Y> &__rhs) {
    return nullptr > __rhs.get();
}

template<class Y, class V>
static inline bool operator<(std::nullptr_t, const shared_ptr<Y> &__rhs) {
    return nullptr < __rhs.get();
}

template<class Y, class V>
static inline bool operator>=(std::nullptr_t, const shared_ptr<Y> &__rhs) {
    return nullptr >= __rhs.get();
}

template<class Y, class V>
static inline bool operator<=(std::nullptr_t, const shared_ptr<Y> &__rhs) {
    return nullptr <= __rhs.get();
}

}
