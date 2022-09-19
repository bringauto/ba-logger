/**
 * @brief This file defines size literals
 */
#pragma once

namespace bringauto::logging {
/**
 * @brief Operator returning number in bytes (which is tha same as input, it is just for prettier code)
 * @param x number in bytes to be converted
 * @return number in bytes
 */
inline auto operator ""_B(unsigned long long const x)
-> unsigned long long { return x; }

/**
 * @brief Convert Bytes to KibyBytes
 * @param x  number in bytes
 * @return  number in kibyBytes
 */
inline auto operator ""_KiB(unsigned long long const x)
-> unsigned long long { return 1024L*x; }

/**
 * @brief Convert bytes to kiloBytes
 * @param x number in bytes
 * @return number in kiloBytes
 */
inline auto operator ""_KB(unsigned long long const x)
-> unsigned long long { return 1000L*x; }

/**
 * @brief Convert bytes to mebiBytes
 * @param x number in bytes
 * @return number in mebiBytes
 */
inline auto operator ""_MiB(unsigned long long const x)
-> unsigned long long { return 1024L*1024L*x; }

/**
 * @brief Convert bytes to megaBytes
 * @param x number in bytes
 * @return number in megaBytes
 */
inline auto operator ""_MB(unsigned long long const x)
-> unsigned long long { return 1000L*1000L*x; }

/**
 * @brief Convert bytes to GibiBytes
 * @param x number in bytes
 * @return number in GibiBytes
 */
inline auto operator ""_GiB(unsigned long long const x)
-> unsigned long long { return 1024L*1024L*1024L*x; }

/**
 * @brief Convert bytes to gigaBytes
 * @param x number in bytes
 * @return number in GigaBytes
 */
inline auto operator ""_GB(unsigned long long const x)
-> unsigned long long { return 1000L*1000L*1000L*x; }
}
