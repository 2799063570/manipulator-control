/** @file error_stack.h
 *  @brief 汇总错误码
 */
#ifndef AUBO_SDK_ERROR_STACK_H
#define AUBO_SDK_ERROR_STACK_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iomanip>

#include <aubo/global_config.h>

// 格式化占位符，默认是 fmt 的格式
#ifndef _PH1_
#define _PH1_ "{}"
#define _PH2_ "{}"
#define _PH3_ "{}"
#define _PH4_ "{}"
#endif

namespace arcs {
namespace error_stack {

constexpr int ARCS_ABI_EXPORT codeCompose(int aa, int bb, int cccc)
{
    return (int)((aa * 1000000) + (bb * 10000) + cccc);
}

constexpr int ARCS_ABI_EXPORT mod(int x)
{
    return (x % 1000000);
}

#include <aubo/error_stack/hal_error.h>
#include <aubo/error_stack/rtm_error.h>
#include <aubo/error_stack/system_error.h>

#define ARCS_ERROR_CODES \
    SYSTEM_ERRORS        \
    HAL_ERRORS           \
    RTM_ERRORS           \
    _D(ARCS_MAX_ERROR_CODE, -1, "Max error code", "suggest...")

// 错误代码枚举
enum ErrorCodes
{
#define _D(n, v, s, r) n = (int)v,
    ARCS_ERROR_CODES
#undef _D
};

inline int str2ErrorCode(const char *err_code_name)
{
#define _D(n, v, s, r)                  \
    if (strcmp(#n, err_code_name) == 0) \
        return v;
    ARCS_ERROR_CODES
#undef _D
    return ARCS_MAX_ERROR_CODE;
}

inline const char *errorCode2Str(int err_code)
{
    static const char *errcode_str[] = {
#define _D(n, v, s, r) s,
        ARCS_ERROR_CODES
#undef _D
    };

    enum arcs_index
    {
#define _D(n, v, s, r) n##_INDEX,
        ARCS_ERROR_CODES
#undef _D
    };

    int index = -1;

#define _D(n, v, s, r) \
    if (err_code == v) \
        index = n##_INDEX;
    ARCS_ERROR_CODES
#undef _D

    if (index == -1) {
        index = ARCS_MAX_ERROR_CODE_INDEX;
    }

    return errcode_str[(unsigned)index];
}

inline std::ostream &dump(std::ostream &os)
{
#define _D(n, v, s, r)                                               \
    os << std::setw(20) << #n << "\t" << v << "\t" << s << "\t" << r \
       << std::endl;

    ARCS_ERROR_CODES
#undef _D

    return os;
}

} // namespace error_stack
} // namespace arcs

#endif // AUBO_SDK_ERROR_STACK_H
