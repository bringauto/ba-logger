/**
 * @brief This file defines size literals
 */
#pragma once

auto operator ""_B(unsigned long long const x)
-> long { return x; }

auto operator ""_KiB(unsigned long long const x)
-> long { return 1024L*x; }

auto operator ""_KB(unsigned long long const x)
-> long { return 1000L*x; }

auto operator ""_MiB(unsigned long long const x)
-> long { return 1024L*1024L*x; }

auto operator ""_MB(unsigned long long const x)
-> long { return 1000L*1000L*x; }

auto operator ""_GiB(unsigned long long const x)
-> long { return 1024L*1024L*1024L*x; }

auto operator ""_GB(unsigned long long const x)
-> long { return 1000L*1000L*1000L*x; }
