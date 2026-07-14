/*
  global_config.h
  this file is generated. Do not change!
*/

#ifndef ARCS_GLOBALCONFIG_H
#define ARCS_GLOBALCONFIG_H

/* #undef ARCS_BUILD_SHARED_LIBS */
/* #undef ARCS_ENABLE_THREADING_SUPPORT */

//-------------------------------------------------------------------
// Header Availability
//-------------------------------------------------------------------

/* #undef ARCS_HAVE_CXXABI_H */

//-------------------------------------------------------------------
// Version information
//-------------------------------------------------------------------

#define INTERFACE_VERSION_MAJOR 0
#define INTERFACE_VERSION_MINOR 24
#define INTERFACE_VERSION_PATCH 0
#define INTERFACE_VERSION       "0.24.0"

//-------------------------------------------------------------------
// Platform defines
//-------------------------------------------------------------------

#if defined(__APPLE__)
#define ARCS_PLATFORM_APPLE
#endif

#if defined(__linux__)
#define ARCS_PLATFORM_LINUX
#endif

#if defined(_WIN32) || defined(_WIN64)
#define ARCS_PLATFORM_WINDOWS
#else
#define ARCS_PLATFORM_POSIX
#endif

/* #undef ARCS_BIG_ENDIAN */
/* #undef ARCS_LITTLE_ENDIAN */

#define ARCS_LIB_PREFIX "lib"
#define ARCS_LIB_EXT    ".so"
#define ARCS_EXE_EXT    ""

#ifdef NDEBUG // Defined by cmake UNLESS Debug build type is chosen
#define ARCS_LIB_POSTFIX ""
#else
#define ARCS_LIB_POSTFIX \
    "" // Set in top level CMakeList.txt
#endif

#define ARCS_FUNCTION  // 函数接口
#define ARCS_INSTRUCT  // 指令接口

///-------------------------------------------------------------------
// Macros for import/export declarations
//-------------------------------------------------------------------

#if defined(ARCS_PLATFORM_WINDOWS)
#define ARCS_ABI_EXPORT __declspec(dllexport)
#define ARCS_ABI_IMPORT __declspec(dllimport)
#define ARCS_ABI_LOCAL
#elif defined(ARCS_HAVE_VISIBILITY_ATTRIBUTE)
#define ARCS_ABI_EXPORT __attribute__((visibility("default")))
#define ARCS_ABI_IMPORT __attribute__((visibility("default")))
#define ARCS_ABI_LOCAL  __attribute__((visibility("hidden")))
#else
#define ARCS_ABI_EXPORT
#define ARCS_ABI_IMPORT
#define ARCS_ABI_LOCAL
#endif

#ifdef ARCS_BUILDING_STAGE
#define ARCS_ABI ARCS_ABI_EXPORT
#else
#define ARCS_ABI ARCS_ABI_IMPORT
#endif

//-------------------------------------------------------------------
// Macros for suppressing warnings
//-------------------------------------------------------------------

#ifdef _MSC_VER
#define ARCS_MSVC_PUSH_DISABLE_WARNING(wn) \
    __pragma(warning(push)) __pragma(warning(disable : wn))
#define ARCS_MSVC_POP_WARNING         __pragma(warning(pop))
#define ARCS_MSVC_DISABLE_WARNING(wn) __pragma(warning(disable : wn))
#else
#define ARCS_MSVC_PUSH_DISABLE_WARNING(wn)
#define ARCS_MSVC_POP_WARNING
#define ARCS_MSVC_DISABLE_WARNING(wn)
#endif

#ifdef __GNUC__
#define aubo_gcc_pragma_expand(x) _Pragma(#x)
#define ARCS_GCC_PUSH_DISABLE_WARNING(wn) \
    _Pragma("GCC diagnostic push")        \
        aubo_gcc_pragma_expand(GCC diagnostic ignored "-W" #wn)
#define ARCS_GCC_POP_WARNING _Pragma("GCC diagnostic pop")
#else
#define ARCS_GCC_PUSH_DISABLE_WARNING(wn)
#define ARCS_GCC_POP_WARNING
#endif

#if defined(__GNUC__)
#define ARCS_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define ARCS_DEPRECATED __declspec(deprecated)
#else
#pragma message( \
    "WARNING: You need to implement ARCS_DEPRECATED for your compiler!")
#define ARCS_DEPRECATED
#endif

// Do not warn about the usage of deprecated unsafe functions
ARCS_MSVC_DISABLE_WARNING(4996)

// Mark a variable or expression result as unused
#define ARCS_UNUSED(x) (void)(x)

//-------------------------------------------------------------------
// C++ Language features
//-------------------------------------------------------------------

/* #undef ARCS_HAVE_THREAD_LOCAL */

//-------------------------------------------------------------------
// C++ Library features
//-------------------------------------------------------------------

/* #undef ARCS_HAVE_REGEX */

//-------------------------------------------------------------------
// Hash Container
//-------------------------------------------------------------------

#define ARCS_HASH_FUNCTION_BEGIN(type)                \
    namespace std {                                   \
    template <>                                       \
    struct hash<type>                                 \
    {                                                 \
        std::size_t operator()(const type &arg) const \
        {
#define ARCS_HASH_FUNCTION_END \
    }                          \
    }                          \
    ;                          \
    }

//-------------------------------------------------------------------
// Utility macros
//-------------------------------------------------------------------

#define ARCS_STR_(x)       #x
#define ARCS_STR(x)        ARCS_STR_(x)
#define ARCS_CONCAT_(x, y) x##y
#define ARCS_CONCAT(x, y)  ARCS_CONCAT_(x, y)

//-------------------------------------------------------------------
// Backwards compatibility macros
//-------------------------------------------------------------------

#if !defined(__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define ARCS_FUTURE_READY   true
#define ARCS_FUTURE_TIMEOUT false
#else
#define ARCS_FUTURE_READY   std::future_status::ready
#define ARCS_FUTURE_TIMEOUT std::future_status::timeout
#endif

#endif // ARCS_GLOBALCONFIG_H
