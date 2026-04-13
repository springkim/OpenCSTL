//
// Created by spring on 2026. 4. 11..
//
#pragma once
#ifndef _OPENCSTL_CROSSPLATFORM_H
#define _OPENCSTL_CROSSPLATFORM_H


// ░█████╗░░██████╗
// ██╔══██╗██╔════╝
// ██║░░██║╚█████╗░
// ██║░░██║░╚═══██╗
// ╚█████╔╝██████╔╝
// ░╚════╝░╚═════╝░
#if defined(_WIN32) || defined(_WIN64)
#define OCSTL_OS_STR "Windows"
#define OCSTL_OS_WINDOWS
#elif defined(__linux__)
#define OCSTL_OS_STR "Linux"
#define OCSTL_OS_LINUX
#elif defined(__APPLE__)
#define OCSTL_OS_STR "MacOS"
#define OCSTL_OS_MACOS
#endif

// ░█████╗░░█████╗░███╗░░░███╗██████╗░██╗██╗░░░░░███████╗██████
// ██╔══██╗██╔══██╗████╗░████║██╔══██╗██║██║░░░░░██╔════╝██╔══██╗
// ██║░░╚═╝██║░░██║██╔████╔██║██████╔╝██║██║░░░░░█████╗░░██████╔╝
// ██║░░██╗██║░░██║██║╚██╔╝██║██╔═══╝░██║██║░░░░░██╔══╝░░██╔══██╗
// ╚█████╔╝╚█████╔╝██║░╚═╝░██║██║░░░░░██║███████╗███████╗██║░░██║
// ░╚════╝░░╚════╝░╚═╝░░░░░╚═╝╚═╝░░░░░╚═╝╚══════╝╚══════╝╚═╝░░╚═╝
#if defined(__NVCC__)
#define OCSTL_CC_STR "NVCC"
#define OCSTL_CC_NVCC
#elif defined(__clang__)
#define OCSTL_CC_STR "Clang"
#define OCSTL_CC_CLANG
#elif defined(__TINYC__)
#define OCSTL_CC_STR "TCC"
#define OCSTL_CC_TCC
#elif defined(_MSC_VER)
#define OCSTL_CC_STR "MSVC"
#define OCSTL_CC_MSVC
#elif defined(__GNUC__)
#define OCSTL_CC_STR "GCC"
#define OCSTL_CC_GCC
#endif


// ░█████╗░░░░░░░██╗░░░██╗███████╗██████╗░░██████╗██╗░█████╗░███╗░░██╗
// ██╔══██╗░░░░░░██║░░░██║██╔════╝██╔══██╗██╔════╝██║██╔══██╗████╗░██║
// ██║░░╚═╝█████╗╚██╗░██╔╝█████╗░░██████╔╝╚█████╗░██║██║░░██║██╔██╗██║
// ██║░░██╗╚════╝░╚████╔╝░██╔══╝░░██╔══██╗░╚═══██╗██║██║░░██║██║╚████║
// ╚█████╔╝░░░░░░░░╚██╔╝░░███████╗██║░░██║██████╔╝██║╚█████╔╝██║░╚███║
// ░╚════╝░░░░░░░░░░╚═╝░░░╚══════╝╚═╝░░╚═╝╚═════╝░╚═╝░╚════╝░╚═╝░░╚══╝

#if defined(__STDC_VERSION__)
#   if __STDC_VERSION__ >= 202311L
#define OCSTL_C_VERSION_STR "C23"
#define OCSTL_C_VERSION_23
#   elif __STDC_VERSION__ >= 201710L
#define OCSTL_C_VERSION_STR "C17 (C18)"
#define OCSTL_C_VERSION_17
#   elif __STDC_VERSION__ >= 201112L
#define OCSTL_C_VERSION_STR "C11"
#define OCSTL_C_VERSION_11
#   elif __STDC_VERSION__ >= 199901L
#define OCSTL_C_VERSION_STR "C99"
#define OCSTL_C_VERSION_99
#   else
#define OCSTL_C_VERSION_STR "C94 (AMD1)"
#define OCSTL_C_VERSION_94
#   endif
#elif defined(__STDC__)
#define OCSTL_C_VERSION_STR "C89 (C90)"
#define OCSTL_C_VERSION_89
#else
#define OCSTL_C_VERSION_STR "pre-ANSI C (K&R)"
#define OCSTL_C_VERSION_PRE_ANSI
#endif


// ░█████╗░██████╗░████████╗██╗███╗░░░███╗██╗███████╗███████╗
// ██╔══██╗██╔══██╗╚══██╔══╝██║████╗░████║██║╚════██║██╔════╝
// ██║░░██║██████╔╝░░░██║░░░██║██╔████╔██║██║░░███╔═╝█████╗░░
// ██║░░██║██╔═══╝░░░░██║░░░██║██║╚██╔╝██║██║██╔══╝░░██╔══╝░░
// ╚█████╔╝██║░░░░░░░░██║░░░██║██║░╚═╝░██║██║███████╗███████╗
// ░╚════╝░╚═╝░░░░░░░░╚═╝░░░╚═╝╚═╝░░░░░╚═╝╚═╝╚══════╝╚══════╝

#if defined(_DEBUG) || !defined(NDEBUG)
#define OCSTL_DEBUG
#else
#define OCSTL_RELEASE
#endif
#endif //_OPENCSTL_CROSSPLATFORM_H
