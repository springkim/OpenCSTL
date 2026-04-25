//
// Created by spring on 4/24/2026.
//

#ifndef OPENCSTL_THREADING_CC_H
#define OPENCSTL_THREADING_CC_H
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

int cpu_count(void) {
#if defined(_WIN32)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (int) si.dwNumberOfProcessors;

#elif defined(__APPLE__)
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
#endif //OPENCSTL_THREADING_CC_H
