#pragma once

#include <libr/std/cstdlib>

#include <libm/syscallManager.h>

namespace std {
    using terminate_handler = void (*)();

    namespace __bits {
        inline void __terminate_handler() noexcept {
            std::abort();
        }

        inline void __unexpected_handler() {
            __terminate_handler();
        }

        static terminate_handler __term_handler = __terminate_handler;
    }

    class exception {
    public:
        exception() noexcept = default;
        exception(const exception &) noexcept = default;
        exception &operator=(const exception &) noexcept = default;
        virtual ~exception() noexcept {}

        virtual const char *what() const noexcept {
            return "std::exception";
        }
    };

    class bad_exception : public exception {
    public:
        const char *what() const noexcept override {
            return "std::bad_exception";
        }
    };

    [[noreturn]] inline void terminate() noexcept {
        __bits::__terminate_handler();
        programExit(1);
        __builtin_unreachable();
    }

    inline terminate_handler set_terminate(terminate_handler __h) noexcept {
        auto __old = __bits::__term_handler;
        __bits::__term_handler = __h;
        return __old;
    }

    inline terminate_handler get_terminate() noexcept {
        return __bits::__term_handler;
    }
}
