#pragma once

#include <libm/syscallManager.h>

#include <libr/std/csignal>

namespace std {

[[noreturn]] inline void abort() noexcept {
    programExit(SIGABRT);
    __builtin_unreachable();
}

}
