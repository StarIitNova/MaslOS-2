#pragma once

#include <libr/std/new>
#include <libr/std/utility>

namespace std {

template<typename T, typename Deleter = void(*)(T *)>
class unique_ptr {
public:
    explicit unique_ptr(T *__ptr = nullptr, Deleter __deleter = nullptr) : mPtr_(__ptr), mDeleter_(__deleter ? __deleter : [](T *__p){ ::operator delete(__p); }) {}

    ~unique_ptr() {
        reset();
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    unique_ptr(unique_ptr &&__other) noexcept
        : mPtr_(__other.mPtr_), mDeleter_(std::move(__other.mDeleter_)) {
            __other.mPtr_ = nullptr;
        }

    unique_ptr &operator=(unique_ptr &&__other) noexcept {
        if (this != &__other) {
            reset();
            mPtr_ = __other.mPtr_;
            mDeleter_ = std::move(__other.mDeleter_);
            __other.mPtr_ = nullptr;
        }

        return *this;
    }

    T *get() const noexcept { return mPtr_; }

    T &operator*() const { return *mPtr_; }
    T *operator->() const { return mPtr_; }

    void reset(T *__ptr = nullptr) {
        if (mPtr_ != __ptr) {
            if (mPtr_) mDeleter_(mPtr_);
            mPtr_ = __ptr;
        }
    }

    T *release() noexcept {
        T *temp = mPtr_;
        mPtr_ = nullptr;
        return temp;
    }

private:
    T *mPtr_;
    Deleter mDeleter_;
};

template<typename T, typename ...Args>
static inline unique_ptr<T> make_unique(Args &&...args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}
