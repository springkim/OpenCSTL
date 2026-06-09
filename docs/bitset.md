# bitset

헤더 [`bitset.h`](../opencstl/bitset.h)에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
typedef struct {
    size_type  nbits;
    ubyte_x   *bits;
    char      *str;
} BITSET;

#define new_bitset(N)  __cstl_bitset(N)
```

`BITSET`은 **고정 크기의 비트 시퀀스**를 저장 및 조작하는 컨테이너입니다.
C++ 표준 라이브러리의 [`std::bitset`](https://cppreference.com/cpp/utility/bitset)에 대응되는 OpenCSTL의 구현체이며, 다음과 같은 차이가 있습니다.

| 항목 | `std::bitset<N>` | OpenCSTL `BITSET` |
|------|------------------|-------------------|
| 크기 결정 시점 | **컴파일 타임** (템플릿 인자 `N`) | **런타임** (`new_bitset(n)`) |
| 인터페이스 | 멤버 함수 / 연산자 오버로딩 | **함수 테이블** (`bitset.set(b)`) |
| 핸들 타입 | 객체 자체 | **값 타입 구조체** `BITSET` |
| 범위 외 접근 | `at()`은 예외 던짐 | 검사 안 함 (UB) |
| 비트 연산자 (`&`, `|`, `^`) | 지원 | 미지원 (개별 비트 API만) |

내부 저장은 `ceil(nbits / 8)` 바이트(최소 8바이트)의 단일 버퍼이며, 비트 카운팅은 **하드웨어 popcount 명령어**(`__builtin_popcountll` / `__popcnt64`)를 사용해 가속됩니다.

`bitset`의 주요 연산 복잡도는 다음과 같습니다.

- 개별 비트 접근/설정 (`test`, `set_at`, `flip_at`) — 상수 **O(1)**
- 전체 비트 연산 (`set`, `reset`, `flip`) — **O(n/8)**
- 비트 카운팅 (`count`) — **O(n/64)** *(64비트 popcount 사용)*
- 전체 비트 질의 (`all`, `any`, `none`) — **O(n/8)**, 단축 평가됨
- 문자열 변환 (`to_string`) — **O(n)**

---

## 목차

1. [API 패턴: 함수 테이블](#api-패턴-함수-테이블)
2. [BITSET 구조체](#bitset-구조체)
3. [생성 / 소멸](#생성--소멸)
4. [멤버 함수](#멤버-함수)
   - [대량 비트 연산](#대량-비트-연산)
   - [개별 비트 연산](#개별-비트-연산)
   - [질의(Query)](#질의query)
   - [변환](#변환)
5. [std::bitset과의 차이점](#stdbitset과의-차이점)
6. [참고 사항](#참고-사항)
7. [예제](#예제)
8. [함수 요약표](#함수-요약표)
9. [관련 항목](#관련-항목)

---

## API 패턴: 함수 테이블

다른 OpenCSTL 컨테이너들이 매크로 디스패치(`push_back(v, x)`)를 사용하는 것과 달리, `BITSET`은 **전역 함수 테이블 `bitset`**을 통해 접근합니다.

```c
BITSET b = new_bitset(16);

bitset.set(b);              // 모든 비트를 1로
bitset.reset(b);            // 모든 비트를 0으로
bitset.set_at(b, 3, true);  // 3번 비트를 1로
bool x = bitset.test(b, 3); // 3번 비트가 1인지

destroy(b);
```

`bitset`은 다음과 같은 함수 포인터들을 담은 전역 구조체입니다.

```c
typedef struct {
    bitset_fn1 set;        // void set(BITSET)
    bitset_fn1 reset;      // void reset(BITSET)
    bitset_fn2 set_at;     // void set_at(BITSET, size_type, bool)
    bitset_fn3 count;      // size_type count(BITSET)
    bitset_fn4 all;        // bool all(BITSET)
    bitset_fn4 any;        // bool any(BITSET)
    bitset_fn4 none;       // bool none(BITSET)
    bitset_fn1 flip;       // void flip(BITSET)
    bitset_fn5 flip_at;    // void flip_at(BITSET, size_type)
    bitset_fn3 nbits;      // size_type nbits(BITSET)
    bitset_fn6 test;       // bool test(BITSET, size_type)
    bitset_fn7 to_string;  // char *to_string(BITSET)
} __BITSET;

static __BITSET bitset = { /* ... */ };
```

> **왜 함수 테이블인가?**: `BITSET`은 다른 컨테이너처럼 헤더 메타데이터를 음수 오프셋에 숨겨두지 않고, **순수 값 타입 구조체**로 사용자에게 노출됩니다. 따라서 매크로 디스패치(`OPENCSTL_NIDX`)가 동작하지 않으며, 대신 명시적 함수 테이블 인터페이스를 채택했습니다.

---

## BITSET 구조체

```c
typedef struct {
    size_type  nbits;   // 비트의 총 개수 (생성 시 결정)
    ubyte_x   *bits;    // 비트들이 저장된 바이트 배열 (ceil(nbits/8) 바이트, 최소 8바이트)
    char      *str;     // to_string()이 사용하는 재사용 버퍼 (nbits+1 바이트)
} BITSET;
```

- **`nbits`**: 사용자가 요청한 비트 개수. `bitset.nbits(b)`로도 조회 가능.
- **`bits`**: 실제 비트들의 저장소. 비트 `i`는 `bits[i/8]`의 `(i % 8)`번째 비트(little-endian within byte)에 위치합니다.
- **`str`**: `to_string()` 호출 시 결과 문자열을 채워 반환하는 **재사용 버퍼**. 매 호출마다 새로 할당하지 않고 같은 버퍼를 덮어씁니다.

### 비트 ↔ 바이트 매핑

```
   비트 인덱스  : ... 15 14 13 12 11 10  9  8 | 7 6 5 4 3 2 1 0
                                              ↑       ↑       ↑
                                  byte_idx = 1│       │       │
                                              │       │  bit 0 of byte 0
                                              │       │
                                              │  bit 0 of byte 1
                                              │
                                  byte_idx = 1, bit 7 of byte 1

   bits[0] = [b7][b6][b5][b4][b3][b2][b1][b0]
   bits[1] = [b15][b14][b13][b12][b11][b10][b9][b8]
   ...
```

비트 `i`에 접근하려면:

- `byte_idx = i / 8`
- `bit_idx = i % 8`
- `(bits[byte_idx] >> bit_idx) & 1`

### 저장 공간

`new_bitset(N)`은 다음 두 버퍼를 calloc으로 할당합니다.

- 비트 버퍼: `max(ceil(N/8), 8)` 바이트
- 문자열 버퍼: `N + 1` 바이트

> **참고**: 최소 8바이트 보장은 popcount 루프가 64비트 단위로 안전하게 일괄 처리할 수 있도록 합니다.

---

## 생성 / 소멸

### `new_bitset`

```c
BITSET new_bitset(size_type N);
```

`N`개의 비트를 가진 새 bitset을 생성합니다. 모든 비트는 0으로 초기화됩니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 BITSET 구조체 (값 반환) |
| **복잡도** | 상수 **O(1)** *(calloc은 zero-init 비용을 시스템에 위임)* |

```c
BITSET b = new_bitset(16);
printf("size: %zu\n", bitset.nbits(b));   // 16
destroy(b);
```

> **주의**: `BITSET`은 **값 타입**입니다. 함수에 전달할 때 구조체가 복사되지만, 내부 `bits`/`str` 포인터는 공유됩니다. 따라서 같은 bitset의 별칭을 만들 수 있지만, **`destroy`는 한 번만** 호출해야 합니다.

---

### `destroy`

```c
void destroy(BITSET b);
```

bitset이 사용한 모든 메모리(`bits` 버퍼와 `str` 버퍼)를 해제합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
BITSET b = new_bitset(100);
// ... 사용 ...
destroy(b);
```

> **내부 동작**: 일반 컨테이너의 디스패치는 음수 오프셋 헤더를 통해 타입을 결정하지만, `BITSET`은 값 타입이라 음수 오프셋이 없습니다. 대신 `destroy` 매크로는 인자 크기가 `sizeof(BITSET)`과 같은지 확인한 뒤, `iveb`(인터벌 트리)에서 `b.bits` 주소를 조회해 `CT_BITSET`인지 확인하는 특수 경로를 사용합니다.

---

## 멤버 함수

### 대량 비트 연산

#### `bitset.set`

```c
void bitset.set(BITSET b);
```

모든 비트(`[0, nbits)`)를 **1**로 설정합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(n/8)** *(`memset`으로 처리)* |

> **구현 노트**: 비트 버퍼가 8바이트 단위로 정렬되어 있어 마지막 부분 바이트의 사용되지 않는 비트들이 1로 설정될 수 있으나, OpenCSTL은 마지막 바이트에 정확한 마스크 `(1 << rem) - 1`를 적용하여 **유효 범위 밖의 패딩 비트는 0으로 유지**합니다. 이는 `count()` 같은 함수가 패딩 비트를 잘못 계산하는 것을 방지합니다.

```c
BITSET b = new_bitset(10);
bitset.set(b);
printf("%s\n", bitset.to_string(b));   // 1111111111
destroy(b);
```

---

#### `bitset.reset`

```c
void bitset.reset(BITSET b);
```

모든 비트를 **0**으로 설정합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(n/8)** *(`memset`으로 처리)* |

```c
BITSET b = new_bitset(10);
bitset.set(b);     // 모두 1
bitset.reset(b);   // 모두 0
printf("%s\n", bitset.to_string(b));   // 0000000000
destroy(b);
```

---

#### `bitset.flip`

```c
void bitset.flip(BITSET b);
```

모든 비트를 **반전**합니다(0→1, 1→0).

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(n/8)** |

마지막 부분 바이트는 유효 비트 영역에만 XOR 마스크를 적용하므로 패딩 비트는 0으로 유지됩니다.

```c
BITSET b = new_bitset(8);
bitset.set_at(b, 0, true);
bitset.set_at(b, 3, true);
// 00001001
bitset.flip(b);
printf("%s\n", bitset.to_string(b));   // 11110110
destroy(b);
```

---

### 개별 비트 연산

#### `bitset.set_at`

```c
void bitset.set_at(BITSET b, size_type idx, bool val);
```

인덱스 `idx` 위치의 비트를 `val`로 설정합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `0 <= idx < nbits` — 범위 검사 없음, 벗어나면 **UB** |

```c
BITSET b = new_bitset(8);
bitset.set_at(b, 0, true);   // 00000001
bitset.set_at(b, 3, true);   // 00001001
bitset.set_at(b, 7, true);   // 10001001
printf("%s\n", bitset.to_string(b));   // 10001001
destroy(b);
```

> **내부 동작**:
> - `val == true`인 경우: `bits[idx/8] |= (1 << (idx % 8))`
> - `val == false`인 경우: `bits[idx/8] &= ~(1 << (idx % 8))`

---

#### `bitset.flip_at`

```c
void bitset.flip_at(BITSET b, size_type idx);
```

인덱스 `idx` 위치의 비트만 반전합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `0 <= idx < nbits` — 범위 검사 없음 |

```c
BITSET b = new_bitset(8);
bitset.set_at(b, 2, true);   // 00000100
bitset.flip_at(b, 2);        // 00000000
bitset.flip_at(b, 5);        // 00100000
destroy(b);
```

> **내부 동작**: `bits[idx/8] ^= (1 << (idx % 8))`

---

#### `bitset.test`

```c
bool bitset.test(BITSET b, size_type idx);
```

인덱스 `idx` 위치의 비트가 **1인지** 검사합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 비트가 1이면 `true`, 0이면 `false` |
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `0 <= idx < nbits` — 범위 검사 없음 |

```c
BITSET b = new_bitset(8);
bitset.set_at(b, 3, true);

printf("%d\n", bitset.test(b, 3));   // 1
printf("%d\n", bitset.test(b, 4));   // 0
destroy(b);
```

> **내부 동작**: `(bits[idx/8] >> (idx % 8)) & 1`

---

### 질의(Query)

#### `bitset.count`

```c
size_type bitset.count(BITSET b);
```

**1로 설정된 비트의 총 개수**를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(n/64)** *(하드웨어 popcount 명령어 사용)* |

내부 구현은 다음과 같은 3단계 루프를 사용합니다.

1. 8바이트(64비트) 단위로 `__popcnt64` 적용
2. 남은 영역을 4바이트 단위로 `__popcnt` 적용
3. 마지막 부분 바이트를 1바이트씩 처리

플랫폼/컴파일러에 따라 적절한 인트린식을 자동 선택합니다.

| 플랫폼 | 인트린식 |
|--------|----------|
| Linux/GCC, Clang | `__builtin_popcount`, `__builtin_popcountll` |
| macOS/Clang | `__builtin_popcount`, `__builtin_popcountll` |
| Windows/MSVC | `<intrin.h>`의 `__popcnt`/`__popcnt64` |
| TCC, COMPCERT, POCC | 소프트웨어 popcount (`__popcnt_sw`) |

```c
BITSET b = new_bitset(100);
for (size_type i = 0; i < 100; i++) {
    if (i % 3 == 0) bitset.set_at(b, i, true);
}
printf("count: %zu\n", bitset.count(b));   // 34 (0,3,6,...,99)
destroy(b);
```

---

#### `bitset.all`

```c
bool bitset.all(BITSET b);
```

**모든** 비트가 1인지 검사합니다. `nbits == 0`인 빈 bitset은 `true`를 반환합니다 (공허 참, vacuous truth).

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(n/8)** — 0인 비트를 만나면 **즉시 단축 반환** |

```c
BITSET b = new_bitset(8);
bitset.set(b);
printf("%d\n", bitset.all(b));   // 1

bitset.set_at(b, 3, false);
printf("%d\n", bitset.all(b));   // 0
destroy(b);
```

---

#### `bitset.any`

```c
bool bitset.any(BITSET b);
```

**하나라도** 1인 비트가 있는지 검사합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(n/8)** — 1인 비트를 만나면 **즉시 단축 반환** |

```c
BITSET b = new_bitset(8);
printf("%d\n", bitset.any(b));   // 0

bitset.set_at(b, 5, true);
printf("%d\n", bitset.any(b));   // 1
destroy(b);
```

---

#### `bitset.none`

```c
bool bitset.none(BITSET b);
```

**모든** 비트가 0인지 검사합니다. `!bitset.any(b)`와 등가입니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(n/8)** — 1인 비트를 만나면 즉시 단축 반환 후 `!` |

```c
BITSET b = new_bitset(8);
printf("%d\n", bitset.none(b));   // 1

bitset.set_at(b, 5, true);
printf("%d\n", bitset.none(b));   // 0
destroy(b);
```

---

#### `bitset.nbits`

```c
size_type bitset.nbits(BITSET b);
```

bitset의 총 비트 수를 반환합니다. 이는 `b.nbits` 필드와 같으며 변경 불가능합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
BITSET b = new_bitset(64);
printf("%zu\n", bitset.nbits(b));   // 64
destroy(b);
```

---

### 변환

#### `bitset.to_string`

```c
char *bitset.to_string(BITSET b);
```

bitset을 **MSB-first**(최상위 비트가 앞) 문자열로 변환하여 반환합니다. C++의 `std::bitset::to_string()`과 동일한 순서입니다.

| 항목 | 내용 |
|------|------|
| **반환값** | `nbits` 길이의 `'\0'`-종결 문자열 |
| **복잡도** | 선형 **O(n)** |

```c
BITSET b = new_bitset(8);
bitset.set_at(b, 0, true);   // 인덱스 0 = 비트값 1
bitset.set_at(b, 1, true);
bitset.set_at(b, 7, true);   // 인덱스 7 = 최상위
// 내부: 10000011
// to_string은 MSB부터: "10000011"
printf("%s\n", bitset.to_string(b));   // 10000011
destroy(b);
```

> **주의: 반환된 포인터의 수명**
>
> `to_string`이 반환하는 문자열은 BITSET 구조체 내부의 `str` 버퍼를 가리킵니다. 다음 호출마다 같은 버퍼가 덮어쓰여지므로:
>
> ```c
> char *s1 = bitset.to_string(b);
> // ... b를 변경 ...
> char *s2 = bitset.to_string(b);
> // 이 시점에 s1과 s2는 같은 버퍼를 가리키며, 모두 최신 값을 반영
> ```
>
> 영구 보관하려면 `strdup` 등으로 복사하세요.

---

## std::bitset과의 차이점

| 기능 | `std::bitset<N>` | OpenCSTL `BITSET` | 비고 |
|------|------------------|-------------------|------|
| 크기 결정 | 컴파일 타임 | 런타임 | OpenCSTL이 더 유연 |
| 생성 | `bitset<N> b;` | `BITSET b = new_bitset(N);` | |
| 해제 | 자동 (소멸자) | `destroy(b)` | 수동 해제 필요 |
| 비트 접근 (읽기) | `b[i]` / `b.test(i)` | `bitset.test(b, i)` | `[]` 미지원 |
| 비트 접근 (쓰기) | `b[i] = true` | `bitset.set_at(b, i, true)` | |
| 모든 비트 1 | `b.set()` | `bitset.set(b)` | |
| 모든 비트 0 | `b.reset()` | `bitset.reset(b)` | |
| 전체 반전 | `b.flip()` | `bitset.flip(b)` | |
| `&`, `|`, `^` 연산자 | 지원 | **미지원** | 직접 루프 작성 필요 |
| 시프트 (`<<`, `>>`) | 지원 | **미지원** | |
| 비교 (`==`) | 지원 | **미지원** | `count`/`to_string` 비교로 우회 |
| `to_ulong` / `to_ullong` | 지원 | **미지원** | `bits[]` 직접 읽기로 우회 |
| 범위 검사 | `at()`은 예외, `[]`은 UB | 모두 검사 안 함 (UB) | |
| 빈 bitset (`N=0`)의 `all()` | `true` | `true` (vacuous truth) | 동일 |

---

## 참고 사항

- **값 타입**: `BITSET`은 포인터가 아니라 구조체 값입니다. 함수에 전달할 때 구조체 자체는 복사되지만 **내부 `bits`/`str` 포인터는 공유**되므로 한 곳에서 변경하면 다른 별칭에도 반영됩니다. **`destroy`는 한 번만** 호출하세요.
- **범위 검사 없음**: `set_at`/`flip_at`/`test`는 `idx`의 범위를 검사하지 않습니다. 잘못된 인덱스 사용 시 정의되지 않은 동작입니다. 사용자가 직접 보장해야 합니다.
- **패딩 비트**: `nbits`가 8의 배수가 아닌 경우 마지막 바이트의 사용되지 않는 비트들은 항상 0으로 유지됩니다(`set`/`flip`이 적절한 마스크를 사용). 이는 `count`, `all`, `any` 등이 잘못된 결과를 내지 않도록 보장합니다.
- **`to_string` 버퍼 재사용**: 매번 새 메모리를 할당하지 않으므로 출력만 할 때는 매우 효율적입니다. 단, 영구 보관 시 `strdup` 필요.
- **하드웨어 가속**: `count`는 컴파일러/플랫폼 인트린식을 자동 선택합니다. TCC 같은 인트린식 미지원 컴파일러에서는 자동으로 비트 트릭 기반 소프트웨어 popcount로 대체됩니다.

### 비트 연산 우회 패턴

`&`, `|`, `^` 연산자가 없으므로 두 bitset을 AND하려면 직접 루프를 작성해야 합니다.

```c
// AND
BITSET a = new_bitset(64);
BITSET b = new_bitset(64);
BITSET r = new_bitset(64);

// ... a, b 채우기 ...

for (size_type i = 0; i < bitset.nbits(a); i++) {
    bool av = bitset.test(a, i);
    bool bv = bitset.test(b, i);
    bitset.set_at(r, i, av && bv);
}

destroy(a); destroy(b); destroy(r);
```

> **성능 팁**: 위 코드는 `O(n)`이지만 비트 단위로 처리합니다. 더 빠른 일괄 처리가 필요하면 `b.bits` 배열에 직접 접근해 바이트 단위로 `&`을 수행하면 **O(n/8)** 으로 줄일 수 있습니다.
>
> ```c
> size_type cap = (bitset.nbits(a) + 7) / 8;
> for (size_type i = 0; i < cap; i++) {
>     r.bits[i] = a.bits[i] & b.bits[i];
> }
> ```

---

## 예제

### 기본 사용

```c
#include "opencstl.h"

int main() {
    BITSET b = new_bitset(16);

    // 비트 1, 4, 7, 15 설정
    bitset.set_at(b, 1, true);
    bitset.set_at(b, 4, true);
    bitset.set_at(b, 7, true);
    bitset.set_at(b, 15, true);

    printf("bits:    %s\n", bitset.to_string(b));   // MSB-first
    printf("count:   %zu\n", bitset.count(b));      // 4
    printf("nbits:   %zu\n", bitset.nbits(b));      // 16
    printf("test(7): %d\n", bitset.test(b, 7));     // 1
    printf("test(8): %d\n", bitset.test(b, 8));     // 0
    printf("any:     %d\n", bitset.any(b));         // 1
    printf("all:     %d\n", bitset.all(b));         // 0
    printf("none:    %d\n", bitset.none(b));        // 0

    bitset.flip(b);
    printf("flipped: %s\n", bitset.to_string(b));
    printf("count:   %zu\n", bitset.count(b));      // 12

    destroy(b);
    return 0;
}
```

**출력:**
```
bits:    1000000010010010
count:   4
nbits:   16
test(7): 1
test(8): 0
any:     1
all:     0
none:    0
flipped: 0111111101101101
count:   12
```

### 에라토스테네스의 체

```c
#include "opencstl.h"

int main() {
    const size_type N = 100;
    BITSET is_prime = new_bitset(N + 1);
    bitset.set(is_prime);              // 모두 1로 시작
    bitset.set_at(is_prime, 0, false); // 0은 소수 아님
    bitset.set_at(is_prime, 1, false); // 1은 소수 아님

    for (size_type i = 2; i * i <= N; i++) {
        if (bitset.test(is_prime, i)) {
            for (size_type j = i * i; j <= N; j += i)
                bitset.set_at(is_prime, j, false);
        }
    }

    printf("primes up to %zu:\n", N);
    for (size_type i = 2; i <= N; i++) {
        if (bitset.test(is_prime, i)) printf("%zu ", i);
    }
    puts("");
    printf("total: %zu primes\n", bitset.count(is_prime) - 0);

    destroy(is_prime);
    return 0;
}
```

### 집합 멤버십 (Bloom-filter 풍 사용)

```c
const size_type N = 1024;
BITSET seen = new_bitset(N);

// 어떤 키를 해시 -> 비트 위치
for (int i = 0; i < input_count; i++) {
    size_type h = hash(input[i]) % N;
    bitset.set_at(seen, h, true);
}

// 빠른 멤버십 추정
if (bitset.test(seen, hash(query) % N)) {
    // 본 적 있을 가능성 (false positive 가능)
} else {
    // 절대 본 적 없음 (확정)
}

destroy(seen);
```

---

## 함수 요약표

### 생성 / 소멸

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_bitset` | `BITSET new_bitset(N)` | O(1) | N비트 bitset 생성 (모두 0) |
| `destroy`    | `void destroy(b)`      | O(1) | bitset 메모리 해제 |

### 대량 비트 연산

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `bitset.set`   | `void set(BITSET)`   | O(n/8) | 모든 비트를 1로 |
| `bitset.reset` | `void reset(BITSET)` | O(n/8) | 모든 비트를 0으로 |
| `bitset.flip`  | `void flip(BITSET)`  | O(n/8) | 모든 비트 반전 |

### 개별 비트 연산

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `bitset.set_at`  | `void set_at(BITSET, idx, val)`  | O(1) | idx번 비트를 val로 설정 |
| `bitset.flip_at` | `void flip_at(BITSET, idx)`      | O(1) | idx번 비트 반전 |
| `bitset.test`    | `bool test(BITSET, idx)`         | O(1) | idx번 비트가 1인지 검사 |

### 질의(Query)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `bitset.count` | `size_type count(BITSET)` | O(n/64) | 1로 설정된 비트의 개수 |
| `bitset.all`   | `bool all(BITSET)`        | O(n/8)  | 모든 비트가 1인지 |
| `bitset.any`   | `bool any(BITSET)`        | O(n/8)  | 하나라도 1이 있는지 |
| `bitset.none`  | `bool none(BITSET)`       | O(n/8)  | 모든 비트가 0인지 |
| `bitset.nbits` | `size_type nbits(BITSET)` | O(1)    | 총 비트 수 |

### 변환

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `bitset.to_string` | `char* to_string(BITSET)` | O(n) | MSB-first 이진 문자열 (재사용 버퍼) |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`VECTOR(bool)`](./vector.md)               | 동적 크기, 바이트당 1비트가 아니라 1바이트(bool) 사용 — 메모리 효율은 낮지만 임의 접근/삽입/삭제가 자유로움 |
| [`UNORDERED_SET`](./unordered_set.md)       | 임의 키 집합. 비트 표현 불가능한 키나 큰 도메인에 적합 |
| [`SET`](./set.md)                           | 정렬된 키 집합 |
