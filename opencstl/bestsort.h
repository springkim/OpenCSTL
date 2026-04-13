//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                Open Source C Container Library like STL in C++
//
//               Copyright (C) 2026, Kim Bomm, all rights reserved.
//
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//

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