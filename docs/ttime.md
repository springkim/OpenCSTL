# ttime / chrono.now

헤더 [`ttime.h`](../opencstl/ttime.h)에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
double ttime(void);

typedef double (*ttime_func)(void);

typedef struct CHRONO {
    ttime_func now;
} CHRONO;

static CHRONO chrono = {
    ttime,
};
```

`ttime()`과 `chrono.now()`는 현재 타이머 값을 `double`로 반환하는 간단한 타이머 API입니다.
두 API 모두 반환 단위는 **밀리초(ms)** 이며, 실행 시간 측정은 보통 시작 시각과 종료 시각의 차이를 계산해 사용합니다.

```c
double t0 = ttime();

// 측정할 작업

double t1 = ttime();
printf("elapsed: %.3f ms\n", t1 - t0);
```

---

## 목차

1. [API 개요](#api-개요)
2. [ttime](#ttime)
3. [chrono.now](#chrononow)
4. [두 API의 차이](#두-api의-차이)
5. [플랫폼별 동작](#플랫폼별-동작)
6. [예제](#예제)
7. [참고 사항](#참고-사항)

---

## API 개요

| API | 형태 | 반환 타입 | 단위 | 용도 |
|-----|------|-----------|------|------|
| `ttime()` | 함수 | `double` | ms | 직접 타이머 호출 |
| `chrono.now()` | 함수 포인터 호출 | `double` | ms | 함수 테이블 스타일 타이머 호출 |

`chrono.now()`는 내부적으로 `ttime`을 가리킵니다.
따라서 현재 구현에서 두 API는 같은 값을 같은 단위로 반환합니다.

```c
double a = ttime();
double b = chrono.now();
```

---

## ttime

```c
double ttime(void);
```

현재 타이머 값을 밀리초 단위로 반환합니다.
반환값의 절대값 자체보다는, 같은 프로그램 안에서 두 번 호출한 값의 차이를 이용해 경과 시간을 구하는 용도로 사용합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 현재 타이머 값 |
| **반환 타입** | `double` |
| **단위** | 밀리초(ms) |
| **인자** | 없음 |
| **복잡도** | 상수 **O(1)** |

```c
double start = ttime();

for (int i = 0; i < 1000000; i++) {
    // workload
}

double end = ttime();
printf("duration: %.3f ms\n", end - start);
```

---

## chrono.now

호출 형태:

```c
double value = chrono.now();
```

`chrono.now()`는 `CHRONO` 구조체의 함수 포인터 `now`를 호출하는 형태입니다.
OpenCSTL의 일부 API가 사용하는 함수 테이블 스타일과 비슷하게, 타이머를 객체처럼 호출하고 싶을 때 사용할 수 있습니다.

```c
double start = chrono.now();

// 측정할 작업

double end = chrono.now();
printf("duration: %.3f ms\n", end - start);
```

구조는 다음과 같습니다.

```c
typedef struct CHRONO {
    ttime_func now;
} CHRONO;

static CHRONO chrono = {
    ttime,
};
```

즉, `chrono.now()`는 현재 구현에서 `ttime()`과 동일한 함수 호출입니다.

---

## 두 API의 차이

`ttime()`과 `chrono.now()`의 시간 단위 차이는 없습니다.
둘 다 `double` 타입의 밀리초(ms) 값을 반환합니다.

실질적인 차이는 호출 스타일입니다.

| 항목 | `ttime()` | `chrono.now()` |
|------|-----------|----------------|
| 호출 방식 | 직접 함수 호출 | `chrono` 구조체의 함수 포인터 호출 |
| 반환 단위 | ms | ms |
| 내부 구현 | 플랫폼별 타이머 호출 | `ttime`을 가리키는 래퍼 |
| 권장 사용처 | 간단한 측정 코드 | 함수 테이블 스타일을 선호하는 코드 |

경과 시간은 두 API 중 하나를 일관되게 사용해 아래처럼 계산합니다.

```c
double elapsed_ms = end - start;
```

`elapsed_ms`는 **밀리초(ms)** 단위입니다.

---

## 플랫폼별 동작

OpenCSTL은 플랫폼에 따라 다른 시스템 타이머를 사용합니다.

| 플랫폼 | 내부 API | 반환값 계산 |
|--------|----------|-------------|
| Windows | `QueryPerformanceCounter` / `QueryPerformanceFrequency` | 카운터 값을 ms로 변환 |
| POSIX 계열 | `gettimeofday` | 초와 마이크로초를 ms로 변환 |

Windows에서는 고해상도 성능 카운터 값을 사용하므로 반환값의 기준점은 플랫폼이 정한 임의의 시점입니다.
POSIX 계열에서는 `gettimeofday` 기반으로 초(`tv_sec`)와 마이크로초(`tv_usec`)를 밀리초로 변환합니다.

따라서 반환값의 절대값에 의미를 두기보다, 같은 실행 흐름 안에서 얻은 두 값의 차이를 경과 시간으로 사용하는 것이 안전합니다.

---

## 예제

### ttime으로 측정

```c
#include "opencstl.h"

#include <stdio.h>

int main(void) {
    double t0 = ttime();

    for (int i = 0; i < 1000000; i++) {
        // workload
    }

    double t1 = ttime();
    printf("elapsed: %.3f ms\n", t1 - t0);
    return 0;
}
```

### chrono.now로 측정

```c
#include "opencstl.h"

#include <stdio.h>

int main(void) {
    double t0 = chrono.now();

    for (int i = 0; i < 1000000; i++) {
        // workload
    }

    double t1 = chrono.now();
    printf("elapsed: %.3f ms\n", t1 - t0);
    return 0;
}
```

---

## 참고 사항

- `ttime()`과 `chrono.now()`는 메모리를 할당하지 않으며 `destroy`가 필요 없습니다.
- 반환값은 `double`이므로 출력할 때는 `%f`, `%.2f`, `%.3f` 등을 사용합니다.
- 실행 시간 측정에서는 `start`와 `end`를 같은 API로 받는 것이 가장 명확합니다.
- POSIX 계열 구현은 `gettimeofday` 기반이므로 시스템 시간이 조정되면 측정값에 영향을 받을 수 있습니다.
