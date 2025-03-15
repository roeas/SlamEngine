#pragma once

// Platforms and compilers
#if defined(_WIN64)
    #if !defined(_MSC_VER)
        #error "Use Visual Studio on Windows."
    #endif
#else
    #error "Only support Windows for now."
#endif

// Debug break
#if defined(_MSC_VER)
    #define SL_DEBUGBREAK() __debugbreak()
#else
    #define SL_DEBUGBREAK()
#endif

// Force inline
#if defined(SL_FINAL)
    #if defined(_MSC_VER)
        #define SL_FORCEINLINE __forceinline
    #else
        #define SL_FORCEINLINE
    #endif
#else
    #define SL_FORCEINLINE
#endif
