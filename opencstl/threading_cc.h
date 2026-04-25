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

#ifndef OPENCSTL_THREADING_CC_H
#define OPENCSTL_THREADING_CC_H
#include <stdint.h>
#include "crossplatform.h"
#if defined(OCSTL_OS_WINDOWS)
#include <windows.h>
#elif defined(OCSTL_OS_MACOS)
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

static int cpu_count(void) {
#if defined(OCSTL_OS_WINDOWS)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (int) si.dwNumberOfProcessors;

#elif defined(OCSTL_OS_MACOS)
    int count = 0;
    size_t size = sizeof(count);
    // hw.logicalcpu: 하이퍼스레딩 포함 논리 코어
    // hw.physicalcpu: 물리 코어만
    sysctlbyname("hw.logicalcpu", &count, &size, NULL, 0);
    return count;

#else
    int cpu_threads = 0;
    int siblings = 0;
    int cpu_cores = 0;
    int got_sib = 0;
    int got_cor = 0;

    FILE *f = fopen("/proc/cpuinfo", "r");
    if (f) {
        char line[128];
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "processor", 9) == 0) {
                cpu_threads++;
            }
            // siblings, cpu cores는 첫 번째 값만 읽으면 충분
            if (!got_sib && strncmp(line, "siblings", 8) == 0) {
                char *colon = strchr(line, ':');
                if (colon) {
                    siblings = atoi(colon + 1);
                    got_sib = 1;
                }
            }
            if (!got_cor && strncmp(line, "cpu cores", 9) == 0) {
                char *colon = strchr(line, ':');
                if (colon) {
                    cpu_cores = atoi(colon + 1);
                    got_cor = 1;
                }
            }
        }
        fclose(f);
    }

    int hyper_threading = (got_sib && got_cor && cpu_cores > 0)
                              ? (siblings == cpu_cores * 2)
                              : 0;
    if (hyper_threading) {
        cpu_threads *= 2;
    }
    // fallback
    if (cpu_threads <= 0) {
        long n = sysconf(_SC_NPROCESSORS_ONLN);
        cpu_threads = n > 0 ? (int) n : 1;
    }

    return cpu_threads;
#endif
}

#if defined(OCSTL_OS_WINDOWS)
#include <windows.h>
#elif defined(OCSTL_OS_LINUX)
#undef GNU_SOURCE
#define _GNU_SOURCE
#include <sched.h>
#elif defined(OCSTL_OS_MACOS)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach/thread_policy.h>
#endif

static void cpu_pin(void) {
#if defined(OCSTL_OS_WINDOWS)
    SetThreadAffinityMask(GetCurrentThread(), (DWORD_PTR) 1 << 0);

#elif defined(OCSTL_OS_LINUX)
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

#elif defined(OCSTL_OS_MACOS)
    // 코어 친화성 힌트
    thread_affinity_policy_data_t affinity = {1};
    thread_policy_set(
        mach_thread_self(),
        THREAD_AFFINITY_POLICY,
        (thread_policy_t) & affinity,
        THREAD_AFFINITY_POLICY_COUNT
    );

    mach_timebase_info_data_t tb;
    mach_timebase_info(&tb);

#define NS_TO_MACH(ns) ((uint64_t)(ns) * tb.denom / tb.numer)

    thread_time_constraint_policy_data_t rt;
    rt.period = 0; // 비주기적 — 벤치마크는 한 번 길게 돌림
    rt.computation = NS_TO_MACH(500000000); // 50ms — 정렬 한 회 실행 예산
    rt.constraint = NS_TO_MACH(500000000); // computation과 동일하게
    rt.preemptible = 0; // 비선점 — 중간에 끊기지 않도록

    thread_policy_set(
        mach_thread_self(),
        THREAD_TIME_CONSTRAINT_POLICY,
        (thread_policy_t) & rt,
        THREAD_TIME_CONSTRAINT_POLICY_COUNT
    );

#undef NS_TO_MACH
#endif
}
#endif //OPENCSTL_THREADING_CC_H
