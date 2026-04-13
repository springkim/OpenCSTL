

#ifndef OPENCSTL_BESTSORT_H
#define OPENCSTL_BESTSORT_H
#if defined(OCSTL_OS_MACOS) && defined(OCSTL_CC_CLANG)
#define cstl_best_stable_sort msort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_OS_MACOS) && defined(OCSTL_CC_GCC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_OS_MACOS) && defined(OCSTL_CC_TCC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort qsort

#elif defined(OCSTL_OS_LINUX) && defined(OCSTL_CC_CLANG)
#define cstl_best_stable_sort msort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_OS_LINUX) && defined(OCSTL_CC_GCC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_OS_LINUX) && defined(OCSTL_CC_TCC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort qsort


#elif defined(OCSTL_OS_WINDOWS) && defined(OCSTL_CC_CLANG)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_OS_WINDOWS) && defined(OCSTL_CC_GCC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_OS_WINDOWS) && defined(OCSTL_CC_TCC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_OS_WINDOWS) && defined(OCSTL_CC_MSVC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort pdqsort
#elif defined(OCSTL_CC_NVCC)
#define cstl_best_stable_sort tsort
#define cstl_unstable_sort pdqsort
#endif

#endif //OPENCSTL_BESTSORT_H