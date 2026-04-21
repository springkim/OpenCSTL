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
#ifndef OPENCSTL_PTHREAD_H
#define OPENCSTL_PTHREAD_H
#ifdef __cplusplus
extern "C" {



#endif
#include "crossplatform.h"
#if defined(OCSTL_OS_WINDOWS)

#include<windows.h>
#include<process.h>
#include<errno.h>
#include<assert.h>
#if !defined(sleep)
#define sleep(num) Sleep(1000*(num))
#endif
typedef struct pthread_tag {
    HANDLE handle;
} pthread_t;

typedef struct pthread_mutex_tag {
    HANDLE handle;
} pthread_mutex_t;

typedef struct pthread_attr_tag {
    int attr;
} pthread_attr_t;

typedef struct pthread_mutexattr_tag {
    int attr;
} pthread_mutexattr_t;

typedef DWORD pthread_key_t;

typedef struct _pthread_cleanup_stack {
#define _PTHREAD_CLEANUP_STACK_MAX 128
    HANDLE thread;

    void (*routine[_PTHREAD_CLEANUP_STACK_MAX])(void *);

    void *arg[_PTHREAD_CLEANUP_STACK_MAX];
    size_t index;
    struct _pthread_cleanup_stack *next;
} _pthread_cleanup_stack;

__declspec(selectany) _pthread_cleanup_stack *_pthread_cleanup_stack_head= NULL;
__declspec(selectany) HANDLE _pthread_cleanup_mutex= NULL;

inline int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
    DWORD dwThreadId = 1;
    HANDLE handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) start_routine, arg, 0, &dwThreadId);
    thread->handle = (HANDLE) handle;
    return thread->handle == (HANDLE) NULL;
}

inline void pthread_exit(void *value_ptr) {
    if (!_pthread_cleanup_mutex)
        if (!_pthread_cleanup_mutex)
            _pthread_cleanup_mutex = CreateMutexA(NULL, FALSE, NULL);
    WaitForSingleObject(_pthread_cleanup_mutex, INFINITE);
    HANDLE thread = GetCurrentThread();
    _pthread_cleanup_stack **head = &_pthread_cleanup_stack_head;
    _pthread_cleanup_stack *rts = NULL;
    while (*head) {
        _pthread_cleanup_stack *tmp = *head;
        int b = (*head)->thread == thread;
        if (b) *head = (*head)->next;
        if (*head)head = &(*head)->next;
        if (b)rts = tmp;
    }
    if (rts) {
        while (rts->index--) {
            rts->routine[rts->index](rts->arg[rts->index]);
        }
        free(rts);
    }
    TerminateThread(GetCurrentThread(), *(int *) value_ptr);
    ReleaseMutex(_pthread_cleanup_mutex);
}

inline int pthread_join(pthread_t thread, void **value_ptr) {
    DWORD r = WaitForSingleObject(thread.handle, INFINITE);
    CloseHandle(thread.handle);
    return r == WAIT_OBJECT_0 ? 0 : EINVAL;
}

inline pthread_t pthread_self(void) {
    pthread_t pt;
    pt.handle = GetCurrentThread();
    return pt;
}

inline int pthread_detach(pthread_t thread) {
    CloseHandle(thread.handle);
}

inline int pthread_cancel(pthread_t thread) {
    TerminateThread(thread.handle, 1);
    return 0;
}

inline void pthread_cleanup_push(void (*routine)(void *), void *arg) {
    if (!_pthread_cleanup_mutex)
        if (!_pthread_cleanup_mutex)
            _pthread_cleanup_mutex = CreateMutexA(NULL, FALSE, NULL);
    WaitForSingleObject(_pthread_cleanup_mutex, INFINITE);
    HANDLE thread = GetCurrentThread();
    _pthread_cleanup_stack **head = &_pthread_cleanup_stack_head;
    while (*head) {
        if ((*head)->thread == thread) break;
        head = &(*head)->next;
    }
    if (!*head) {
        *head = (_pthread_cleanup_stack *) malloc(sizeof(_pthread_cleanup_stack));
        (*head)->index = 0;
        (*head)->next = NULL;
        (*head)->thread = thread;
    }
    (*head)->routine[(*head)->index] = routine;
    (*head)->arg[(*head)->index] = arg;
    (*head)->index++;
    assert((*head)->index <= _PTHREAD_CLEANUP_STACK_MAX);
    ReleaseMutex(_pthread_cleanup_mutex);
}

static void pthread_cleanup_pop(int execute) {
    if (!_pthread_cleanup_mutex)
        if (!_pthread_cleanup_mutex)
            _pthread_cleanup_mutex = CreateMutexA(NULL, FALSE, NULL);
    WaitForSingleObject(_pthread_cleanup_mutex, INFINITE);
    HANDLE thread = GetCurrentThread();
    _pthread_cleanup_stack **head = &_pthread_cleanup_stack_head;
    while (*head) {
        _pthread_cleanup_stack *tmp = *head;
        if ((*head)->thread == thread) {
            --(*head)->index;
            assert((*head)->index >= 0);
            if ((*head)->index == 0) {
                *head = (*head)->next;
                while (execute && tmp->index--) {
                    tmp->routine[tmp->index](tmp->arg[tmp->index]);
                }
                free(tmp);
            }
            break;
        } else {
            head = &(*head)->next;
        }
    }
    ReleaseMutex(_pthread_cleanup_mutex);
}

inline int pthread_mutexattr_destroy(pthread_mutexattr_t *attr) {
    return 0;
}

inline int pthread_mutexattr_init(pthread_mutexattr_t *attr) {
    return 0;
}

inline int pthread_mutex_destroy(pthread_mutex_t *mutex) {
    return !CloseHandle(mutex->handle);
}

inline int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr) {
    HANDLE handle = CreateMutexA(NULL, FALSE, NULL);
    if (handle != NULL) {
        mutex->handle = handle;
        return 0;
    } else {
        return 1;
    }
}

inline int pthread_mutex_lock(pthread_mutex_t *mutex) {
    return (WaitForSingleObject(mutex->handle, INFINITE) == WAIT_OBJECT_0) ? 0 : EINVAL;
}

inline int pthread_mutex_trylock(pthread_mutex_t *mutex) {
    DWORD retvalue = WaitForSingleObject(mutex->handle, 0);
    switch (WaitForSingleObject(mutex->handle, 0)) {
        case WAIT_OBJECT_0: return 0;
        case WAIT_TIMEOUT: return EBUSY;
        default: return EINVAL;
    }
}

inline int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    return !ReleaseMutex(mutex->handle);
}

inline int pthread_key_create(pthread_key_t *key, void (*destr_function)(void *)) {
    DWORD dkey = TlsAlloc();
    if (dkey != 0xFFFFFFFF) {
        *key = dkey;
        return 0;
    } else {
        return EAGAIN;
    }
}

inline int pthread_key_delete(pthread_key_t key) {
    return TlsFree(key) ? 0 : EINVAL;
}

inline int pthread_setspecific(pthread_key_t key, const void *pointer) {
    return TlsSetValue(key, (LPVOID) pointer) ? 0 : EINVAL;
}

inline void *pthread_getspecific(pthread_key_t key) {
    return TlsGetValue(key);
}
#else
#include <pthread.h>
#endif
#ifdef __cplusplus
}
#endif

#endif //OPENCSTL_PTHREAD_H
