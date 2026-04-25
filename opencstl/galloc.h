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


#if !defined(_HG_B0BA2BCEE842F587B495A9DAC23CC19DF2528F0D7D48D49AA5462B025137A965_H)
#define _HG_B0BA2BCEE842F587B495A9DAC23CC19DF2528F0D7D48D49AA5462B025137A965_H

#define ___OCSTL_1MB   (1024UL * 1024UL)
#define ___OCSTL_128MB (___OCSTL_1MB * 128UL)
#define ___OCSTL_512MB (___OCSTL_1MB * 512UL)

#if defined(OPENCSTL_GBLOCK_MB)
#define ___OCSTL_GBLOCK_SIZE (___OCSTL_1MB * OPENCSTL_GBLOCK_MB)
#else
#define ___OCSTL_GBLOCK_SIZE (___OCSTL_512MB)
#endif

unsigned char OCSTL_GBLOCK[___OCSTL_GBLOCK_SIZE]; // BSS
unsigned char OCSTL_DBLOCK[___OCSTL_1MB] = {0}; //DATA
#endif
