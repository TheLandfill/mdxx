#ifndef MDXX_COMPILATION_INFO_H
#define MDXX_COMPILATION_INFO_H

#if defined(_MSC_FULL_VER)
#define MDXX_ABI_COMPILER "msvc"
#define MDXX_ABI_VERSION _MSC_FULL_VER
#elif defined(__clang__)
#define MDXX_ABI_COMPILER "clang"
#define MDXX_ABI_VERSION __clang_major__ << "." << __clang_minor__
#elif defined(__GXX_ABI_VERSION)
#define MDXX_ABI_COMPILER "gcc"
#define MDXX_ABI_VERSION __GXX_ABI_VERSION
#elif defined(__MINGW32_MAJOR_VERSION)
#define MDXX_ABI_COMPILER "mingw-32"
#define MDXX_ABI_VERSION __MINGW32_MAJOR_VERSION << "." << __MINGW32_MINOR_VERSION
#elif defined(__MINGW64_MAJOR_VERSION)
#define MDXX_ABI_COMPILER "mingw-64"
#define MDXX_ABI_VERSION __MINGW64_MAJOR_VERSION << "." << __MINGW64_MINOR_VERSION
#else
#error "Please add ABI info for your compiler in the header file\nmd++/includes/compilation_info.h and submit a pull request."
#endif

#define MDXX_COMPILATION_INFO MDXX_ABI_COMPILER << " version " << MDXX_ABI_VERSION

#endif
