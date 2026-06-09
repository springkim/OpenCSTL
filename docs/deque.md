# deque

헤더 [`deque.h`](../opencstl/deque.h)에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
#define DEQUE(TYPE)      TYPE*
#define new_deque(TYPE)  cstl_deque(TYPE)
```

`DEQUE`(double-ended queue, 양방향 큐)는 **앞쪽과 뒤쪽 모두에서** 빠른 삽입과 삭제를 지원하는 시퀀스 컨테이너입니다.
C++ 표준 라이브러리의 [`std::deque`](https://cppreference.com/cpp/container/deque)에 대응되는 OpenCSTL의 구현체입니다.

`VECTOR`와 마찬가지로 원소가 **메모리상 연속(contiguous)** 으로 저장되므로 `[]` 첨자 접근과 표준 C 라이브러리 함수(`qsort`, `bsearch`)와의 호환이 모두 가능합니다. 그러나 `VECTOR`와는 달리 **앞쪽(front) 삽입/삭제 또한 분할 상환 O(1)** 입니다.

> **구현 노트 — OpenCSTL의 deque는 std::deque와 다릅니다**
>
> 표준 C++의 `std::deque`는 일반적으로 *고정 크기 청크들을 가리키는 포인터 배열*(chunked buffer)로 구현되어 메모리상 연속성이 청크 단위에서만 보장됩니다.
> 반면 OpenCSTL의 `DEQUE`는 **단일 연속 버퍼 + 앞쪽 여유 공간(leading gap)** 으로 구현되어, 전체 원소가 하나의 연속 영역에 놓입니다. 핸들 포인터(`*container`)는 버퍼 시작이 아닌 **첫 번째 원소**를 가리키며, `push_front`가 발생하면 핸들 포인터가 한 칸 뒤로 이동합니다. 앞쪽 gap이 소진되면 더 큰 버퍼로 재할당하면서 새로운 leading gap을 할당합니다.
>
> 이 설계의 효과:
> - **장점**: `[]` 접근이 진짜 O(1) 단일 인덱싱, `qsort`/`bsearch` 직접 호환
> - **트레이드오프**: 중간 삽입은 청크 deque보다 비쌀 수 있음

`deque`의 주요 연산 복잡도는 다음과 같습니다.

- 임의 접근 — 상수 **O(1)**
- 앞쪽/뒤쪽 삽입 및 제거 — 분할 상환 상수 **O(1) amortized**
- 중간 위치 삽입 및 제거 — 선형 **O(n)**

---

## 목차

1. [매크로 매개변수](#매크로-매개변수)
2. [멤버 타입](#멤버-타입)
3. [내부 메모리 레이아웃](#내부-메모리-레이아웃)
4. [반복자 무효화](#반복자-무효화)
5. [멤버 함수](#멤버-함수)
   - [생성자 / 소멸자](#생성자--소멸자)
   - [원소 접근](#원소-접근)
   - [반복자](#반복자)
   - [용량(Capacity)](#용량capacity)
   - [수정자(Modifiers)](#수정자modifiers)
   - [탐색(Lookup)](#탐색lookup)
   - [연산(Operations)](#연산operations)
6. [표준 C 라이브러리와의 호환성](#표준-c-라이브러리와의-호환성)
7. [vector / list와의 비교](#vector--list와의-비교)
8. [참고 사항](#참고-사항)
9. [예제](#예제)
10. [함수 요약표](#함수-요약표)
11. [관련 항목](#관련-항목)

---

## 매크로 매개변수

### `DEQUE(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 덱에 저장될 원소의 타입 |

`TYPE*`으로 확장됩니다. `VECTOR`와 동일한 핸들 타입입니다.

```c
DEQUE(int)   q;   // 확장 결과: int*  q;
DEQUE(char*) sq;  // 확장 결과: char** sq;
```

### `new_deque(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 덱에 저장될 원소의 타입 |

새로운 빈 덱을 생성하여 반환합니다. 반환된 포인터는 반드시 [`destroy`](#destroy)로 해제되어야 합니다. 초기 capacity는 **2**이며, 앞쪽 gap을 1로 두어 첫 번째 `push_front` 호출에서도 재할당이 일어나지 않도록 설계되어 있습니다.

```c
DEQUE(int) q = new_deque(int);
// ... 사용 ...
destroy(q);
```

---

## 멤버 타입

| 개념 (C++ 표준) | OpenCSTL 대응 |
|-----------------|--------------|
| `value_type`              | `TYPE` (매크로 인자) |
| `reference` / `pointer`   | `TYPE&` 대응: `q[i]`, 포인터: `TYPE*` |
| `iterator`                | `TYPE*` — 일반 포인터, 산술 연산 지원 |
| `reverse_iterator`        | `TYPE*` — 역방향은 `it--`로 진행 |
| `size_type`               | `size_t` |
| `difference_type`         | `ptrdiff_t` |
| `allocator_type`          | 내부 `calloc`/`realloc` 사용 (고정) |

> `vector`와 마찬가지로 반복자는 단순 포인터이며 `it + n`, `it - n`, `it1 - it2`, `q[i]` 등 모든 표준 C 포인터 연산을 지원합니다.

---

## 내부 메모리 레이아웃

OpenCSTL의 deque는 다음과 같은 단일 연속 버퍼로 구현됩니다.

```
                       핸들 *container ─┐
                                       ↓
┌──────────┬─────────┬────────────────────────────────────┬─────────┐
│  헤더    │  앞쪽   │           원소 영역                │  뒤쪽   │
│ (메타)   │  gap    │   [0][1][2] ... [length-1]         │  여유   │
└──────────┴─────────┴────────────────────────────────────┴─────────┘
                    ↑                                    ↑
                    *container                           end(q)
```

- **앞쪽 gap**: `push_front`를 위해 미리 비워둔 공간. 비어 있을 때 `push_front`가 호출되면 핸들 포인터가 한 칸 앞으로 당겨지며, 그 자리에 새 값이 들어갑니다.
- **뒤쪽 여유**: `push_back`을 위해 비워둔 공간. 같은 원리로 동작합니다.
- 양쪽 모두 소진되면 capacity를 두 배로 늘리며 재할당하고, 새 버퍼의 중앙(약 `capacity/4` 위치)으로 원소들을 옮기면서 양쪽 모두에 다시 gap을 확보합니다.

내부적으로 헤더 슬롯 `-1`에 **현재 핸들 포인터의 원래 할당 블록 시작 대비 음수 바이트 오프셋**(`-distance - 1`)이 저장됩니다. 이를 통해 OpenCSTL의 디스패처는 임의의 deque 핸들이 가리키는 위치에서 항상 원래 헤더의 절대 위치를 역계산할 수 있습니다.

---

## 반복자 무효화

| 연산 | 무효화되는 반복자 |
|------|------------------|
| 모든 읽기 전용 연산 | 없음 |
| `assign`, `clear` | 모든 반복자 (`assign`은 재할당, `clear`는 의미상 무효) |
| `push_back` | 재할당 시 모든 반복자, 아니면 `end(q)`만 |
| `push_front` | 재할당 시 모든 반복자, 아니면 `begin(q)`만 (핸들 포인터가 이동) |
| `pop_back` | 제거된 원소와 `end(q)` |
| `pop_front` | 제거된 원소와 `begin(q)` (핸들 포인터가 이동) |
| `insert` | 재할당 시 모든 반복자, 아니면 삽입 지점 ~ `end(q)` 범위 |
| `erase` | 제거된 원소와 그 이후 모든 반복자 |
| `resize` | 재할당 시 모든 반복자, 아니면 끝부분만 |
| `shrink_to_fit` | 재할당 시 모든 반복자 |
| `destroy` | 모든 반복자 |

> **주의**: `push_front`/`pop_front` 호출 후에는 핸들 포인터 자체가 변경되므로, 매크로를 통해 항상 최신 `q`로 접근해야 합니다. 보관해 둔 `begin(q)` 값은 무효가 될 수 있습니다.

---

## 멤버 함수

### 생성자 / 소멸자

#### `new_deque`

```c
DEQUE(TYPE) new_deque(TYPE);
```

빈 덱을 생성합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 덱의 핸들 |
| **복잡도** | 상수 **O(1)** |
| **예외** | 메모리 할당 실패 시 `verify()` 실패로 프로그램 중단 |

```c
DEQUE(int) q = new_deque(int);
printf("%zu\n", size(q));   // 0
destroy(q);
```

---

#### `destroy`

```c
void destroy(DEQUE(TYPE) q);
```

덱을 파괴하고 모든 할당 메모리를 해제합니다. `new_deque`로 생성된 모든 덱은 반드시 한 번 `destroy`되어야 합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(단일 버퍼 free)* |
| **반복자 무효화** | 모든 반복자 |

```c
DEQUE(int) q = new_deque(int);
push_back(q, 1);
destroy(q);
```

---

### 원소 접근

#### `operator[]`

```c
TYPE q[i];
```

인덱스 `i`(0-based) 위치의 원소에 접근합니다. 일반 C 배열처럼 읽기와 쓰기가 모두 가능합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 인덱스 `i` 위치의 원소에 대한 참조 |
| **복잡도** | 상수 **O(1)** *(진짜 단일 인덱싱)* |
| **사전 조건** | `0 <= i < size(q)` — 범위 벗어나면 **UB** |

```c
DEQUE(int) q = new_deque(int);
assign(q, 5, 0);
q[0] = 100;
q[4] = 999;
printf("%d %d\n", q[0], q[4]);   // 100 999
destroy(q);
```

> 핸들 포인터가 첫 원소를 가리키므로 `q[i]`는 정확히 `*(q + i)`가 되어 std::deque의 청크 인덱싱보다 훨씬 단순하고 빠릅니다.

---

#### `front`

```c
TYPE front(DEQUE(TYPE) q);
```

첫 번째 원소의 값을 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(q) == false` — 빈 덱에 호출 시 **UB** |

```c
DEQUE(int) q = new_deque(int);
push_back(q, 10);
push_front(q, 5);
printf("%d\n", front(q));   // 5
destroy(q);
```

---

#### `back`

```c
TYPE back(DEQUE(TYPE) q);
```

마지막 원소의 값을 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(q) == false` — 빈 덱에 호출 시 **UB** |

```c
DEQUE(int) q = new_deque(int);
push_back(q, 10);
push_front(q, 5);
printf("%d\n", back(q));   // 10
destroy(q);
```

---

### 반복자

`DEQUE`의 반복자 타입은 `TYPE*`이며, 원소가 연속 메모리에 있기 때문에 `vector`와 동일하게 표준 C 포인터 산술이 모두 동작합니다(`it++`, `it--`, `it + n`, `it - n`, `it1 - it2`).

#### `begin`

```c
TYPE* begin(DEQUE(TYPE) q);
```

첫 번째 원소를 가리키는 반복자를 반환합니다. 핸들 `q` 자체와 동일한 값입니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++)
    push_back(q, i * 10);

for (int *it = begin(q); it != end(q); it++)
    printf("[%3d]", *it);   // [  0][ 10][ 20][ 30][ 40]
puts("");
destroy(q);
```

---

#### `end`

```c
TYPE* end(DEQUE(TYPE) q);
```

마지막 원소 **다음** 위치를 가리키는 반복자를 반환합니다. **역참조해서는 안 됩니다.**

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `rbegin`

```c
TYPE* rbegin(DEQUE(TYPE) q);
```

마지막 원소를 가리키는 역방향 반복자를 반환합니다. 역방향 순회 시 `it--`로 이동합니다.

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++)
    push_back(q, i);

for (int *it = rbegin(q); it != rend(q); it--)
    printf("[%3d]", *it);   // [  4][  3][  2][  1][  0]
puts("");
destroy(q);
```

---

#### `rend`

```c
TYPE* rend(DEQUE(TYPE) q);
```

첫 번째 원소 **바로 앞** 위치를 가리키는 역방향 반복자를 반환합니다. **역참조해서는 안 됩니다.**

---

### 용량(Capacity)

#### `empty`

```c
bool empty(DEQUE(TYPE) q);
```

덱에 원소가 하나도 없으면 `true`를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `size`

```c
size_t size(DEQUE(TYPE) q);
```

현재 덱에 저장된 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `max_size`

```c
size_t max_size(DEQUE(TYPE) q);
```

덱이 시스템 제약상 보유할 수 있는 원소의 **이론적 최대 개수**를 반환합니다. 실제 가용량은 사용 가능한 RAM에 의해 제한됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
DEQUE(int) q = new_deque(int);
printf("max: %zu\n", max_size(q));
destroy(q);
```

---

#### `capacity`

```c
size_t capacity(DEQUE(TYPE) q);
```

재할당 없이 보유할 수 있는 원소의 총 개수를 반환합니다(앞쪽 gap + 현재 원소 영역 + 뒤쪽 여유 모두 포함).

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++) push_back(q, i);
printf("size:     %zu\n", size(q));      // 5
printf("capacity: %zu\n", capacity(q));  // >= 5
destroy(q);
```

> **참고**: OpenCSTL의 `deque`에는 `reserve()`가 없습니다. 앞쪽/뒤쪽 여유 공간을 자동으로 관리하는 자료구조의 특성상 단순한 사전 할당 의미를 정의하기 어렵기 때문입니다. 큰 데이터를 예상한다면 `assign(q, n)` 또는 `resize(q, n)`을 사용하세요.

---

#### `shrink_to_fit`

```c
void shrink_to_fit(DEQUE(TYPE) q);
```

남는 capacity(앞쪽 gap 포함)를 해제하여 `capacity(q) == size(q)`가 되도록 요청합니다. 원소들을 새 버퍼의 맨 앞으로 이동시키고 앞쪽 gap을 0으로 만듭니다(`size == 0`인 경우에는 capacity를 1로 유지).

| 항목 | 내용 |
|------|------|
| **복잡도** | 원소 수에 선형 **O(n)** |
| **반복자 무효화** | 재할당이 일어나면 모든 반복자/포인터 무효화 |

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 100; i++) push_back(q, i);
for (int i = 0; i < 50; i++)  pop_front(q);

printf("before: cap = %zu, size = %zu\n", capacity(q), size(q));
shrink_to_fit(q);
printf("after:  cap = %zu, size = %zu\n", capacity(q), size(q));
destroy(q);
```

---

### 수정자(Modifiers)

#### `assign`

```c
void assign(DEQUE(TYPE) q, size_t n);
void assign(DEQUE(TYPE) q, size_t n, TYPE value);
```

기존 내용을 모두 폐기하고 `n`개의 원소로 채웁니다. `value`가 주어지면 모든 원소가 그 값으로, 아니면 0으로 초기화됩니다. 호출 후 leading gap은 0으로 초기화됩니다.

| 오버로드 | 설명 |
|----------|-----|
| `assign(q, n)`        | `n`개의 0 초기화 원소 |
| `assign(q, n, value)` | `n`개의 원소를 모두 `value`로 채움 |

| 항목 | 내용 |
|------|------|
| **복잡도** | `n`에 선형 **O(n)** |
| **반복자 무효화** | 모든 반복자 (재할당 발생) |

```c
DEQUE(int) q = new_deque(int);
assign(q, 4, 99);   // [99][99][99][99]
destroy(q);
```

---

#### `push_back`

```c
void push_back(DEQUE(TYPE) q, TYPE value);
```

덱의 끝에 `value`를 추가합니다. 뒤쪽 여유가 부족하면 capacity를 두 배로 늘리며 재할당합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized** *(재할당 시 O(n))* |
| **반복자 무효화** | 재할당 시 모든 반복자, 아니면 `end(q)`만 |

---

#### `pop_back`

```c
void pop_back(DEQUE(TYPE) q);
```

마지막 원소를 제거합니다. 메모리는 해제되지 않으며, 그 자리는 뒤쪽 여유에 포함됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(q) == false` |
| **반복자 무효화** | 제거된 원소와 `end(q)` |

---

#### `push_front`

```c
void push_front(DEQUE(TYPE) q, TYPE value);
```

덱의 맨 앞에 `value`를 삽입합니다. **핸들 포인터가 한 칸 앞으로 이동**합니다(매크로가 이를 투명하게 처리). 앞쪽 gap이 비어 있으면 capacity를 두 배로 늘리며 재할당합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized** |
| **반복자 무효화** | 재할당 시 모든 반복자, 아니면 `begin(q)`만(핸들 자체가 이동) |

```c
DEQUE(int) q = new_deque(int);
push_back(q, 2);
push_front(q, 1);
push_front(q, 0);   // [0][1][2]
for (int i = 0; i < (int)size(q); i++)
    printf("[%d]", q[i]);
puts("");
destroy(q);
```

---

#### `pop_front`

```c
void pop_front(DEQUE(TYPE) q);
```

첫 번째 원소를 제거합니다. **핸들 포인터가 한 칸 뒤로 이동**하며, 비워진 자리는 앞쪽 gap의 일부가 됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(q) == false` |
| **반복자 무효화** | 제거된 원소와 `begin(q)` |

```c
DEQUE(int) q = new_deque(int);
push_back(q, 10);
push_back(q, 20);
push_back(q, 30);
pop_front(q);
printf("%d\n", front(q));   // 20
destroy(q);
```

---

#### `insert`

```c
void insert(DEQUE(TYPE) q, TYPE* pos, TYPE value);
void insert(DEQUE(TYPE) q, TYPE* pos, size_t n, TYPE value);
```

`pos`가 가리키는 원소 **앞**에 원소(들)를 삽입합니다. `pos`는 보통 `q + index` 형태로 지정합니다.

| 오버로드 | 설명 |
|----------|-----|
| `insert(q, pos, value)`    | 단일 `value`를 `pos` 앞에 삽입 |
| `insert(q, pos, n, value)` | `value`를 `n`개 복제하여 `pos` 앞에 삽입 |

| 항목 | 내용 |
|------|------|
| **복잡도** | `pos`에서 끝까지의 거리 + 삽입 개수에 선형 **O(n)** |
| **반복자 무효화** | 재할당 시 모든 반복자, 아니면 삽입 지점 ~ `end(q)` |

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++)
    push_back(q, i * 10);       // [0][10][20][30][40]

insert(q, q + 5, 777);          // [0][10][20][30][40][777]
insert(q, q + 4, 3, 999);       // [0][10][20][30][999][999][999][40][777]

for (int i = 0; i < (int)size(q); i++)
    printf("[%3d]", q[i]);
puts("");
destroy(q);
```

---

#### `erase`

```c
void erase(DEQUE(TYPE) q, TYPE* pos);
void erase(DEQUE(TYPE) q, TYPE* first, TYPE* last);
```

원소(들)를 제거합니다. 제거된 위치 이후의 모든 반복자는 무효화됩니다.

| 오버로드 | 설명 |
|----------|-----|
| `erase(q, pos)`         | `pos` 위치의 원소 제거 |
| `erase(q, first, last)` | 반열림 구간 `[first, last)` 제거 |

| 항목 | 내용 |
|------|------|
| **복잡도** | 제거 위치 이후 원소 수에 선형 **O(n)** |

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++)
    push_back(q, i * 10);   // [0][10][20][30][40]

erase(q, q + 1);             // [0][20][30][40]
erase(q, q + 1, q + 3);      // [0][40]

for (int i = 0; i < (int)size(q); i++)
    printf("[%3d]", q[i]);
puts("");
destroy(q);
```

**출력:**
```
[  0][ 40]
```

---

#### `resize`

```c
void resize(DEQUE(TYPE) q, size_t n);
void resize(DEQUE(TYPE) q, size_t n, TYPE value);
```

덱의 크기가 정확히 `n`이 되도록 조정합니다.

- `n > size(q)`인 경우: 새 원소가 끝에 추가됩니다 (0 또는 `value`).
- `n < size(q)`인 경우: 인덱스 `n` 이후의 원소가 끝에서부터 제거됩니다.
- `n == size(q)`인 경우: 아무 일도 일어나지 않습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | `|n - size(q)|`에 선형 **O(n)** |
| **반복자 무효화** | 재할당 시 모든 반복자, 아니면 끝부분의 변경 영역만 |

---

#### `clear`

```c
void clear(DEQUE(TYPE) q);
```

모든 원소를 제거합니다. `size(q)`는 `0`이 되며 capacity와 leading gap은 유지됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **반복자 무효화** | 모든 반복자 |

---

### 탐색(Lookup)

#### `find`

```c
TYPE* find(DEQUE(TYPE) q, TYPE value);
TYPE* find(DEQUE(TYPE) q, TYPE* start, TYPE value);
```

`value`를 선형 탐색하여 일치하는 첫 원소의 반복자를 반환합니다. 찾지 못하면 `end(q)`를 반환합니다.

| 오버로드 | 설명 |
|----------|-----|
| `find(q, value)`        | 처음부터 덱 전체 탐색 |
| `find(q, start, value)` | 반복자 `start`부터 탐색 |

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |

정렬된 덱에서는 [`lower_bound`](#lower_bound)/[`upper_bound`](#upper_bound) 또는 표준 C 라이브러리의 `bsearch`를 사용해 **O(log n)** 으로 탐색할 수 있습니다.

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++) push_back(q, i * 10);

int *it = find(q, 20);
if (it != end(q))
    printf("found %d at index %td\n", *it, it - q);   // found 20 at index 2
destroy(q);
```

---

#### `count`

```c
size_t count(DEQUE(TYPE) q, TYPE value);
```

덱 내에서 `value`와 같은 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |

```c
DEQUE(int) q = new_deque(int);
push_back(q, 1); push_back(q, 2); push_back(q, 2);
push_back(q, 3); push_back(q, 2);

printf("count of 2: %zu\n", count(q, 2));   // 3
destroy(q);
```

---

#### `count_if`

```c
size_t count_if(DEQUE(TYPE) q, bool (*pred)(void*));
```

`pred(원소)`가 `true`를 반환하는 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |

```c
bool is_positive(void *p) { return (*(int*)p) > 0; }

DEQUE(int) q = new_deque(int);
int xs[] = {-2, -1, 0, 1, 2, 3};
for (int i = 0; i < 6; i++) push_back(q, xs[i]);

printf("positives: %zu\n", count_if(q, is_positive));   // 3
destroy(q);
```

---

#### `lower_bound`

```c
TYPE* lower_bound(DEQUE(TYPE) q, TYPE value, int (*cmp)(void*, void*));
```

**정렬된** 덱에서 `value`보다 작지 않은(`!(x < value)`) 첫 원소를 가리키는 반복자를 이진 탐색으로 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 조건을 만족하는 첫 원소의 반복자, 없으면 `NULL` |
| **복잡도** | **O(log n)** |
| **사전 조건** | 덱이 `cmp` 기준으로 정렬되어 있어야 함 |

```c
int int_cmp(void *a, void *b) { return *(int*)a - *(int*)b; }

DEQUE(int) q = new_deque(int);
int xs[] = {1, 2, 4, 4, 4, 7, 9};
for (int i = 0; i < 7; i++) push_back(q, xs[i]);

int *it = lower_bound(q, 4, int_cmp);
printf("index: %td\n", it - q);   // 2
destroy(q);
```

---

#### `upper_bound`

```c
TYPE* upper_bound(DEQUE(TYPE) q, TYPE value, int (*cmp)(void*, void*));
```

**정렬된** 덱에서 `value`보다 큰(`value < x`) 첫 원소를 가리키는 반복자를 이진 탐색으로 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 조건을 만족하는 첫 원소의 반복자, 없으면 `NULL` |
| **복잡도** | **O(log n)** |
| **사전 조건** | 덱이 `cmp` 기준으로 정렬되어 있어야 함 |

`lower_bound`와 `upper_bound`를 함께 사용하면 동일 값의 구간 `[lower, upper)`를 얻을 수 있습니다.

```c
int *lo = lower_bound(q, 4, int_cmp);
int *hi = upper_bound(q, 4, int_cmp);
printf("count of 4: %td\n", hi - lo);   // 3
```

---

### 연산(Operations)

#### `reverse`

```c
void reverse(DEQUE(TYPE) q);
```

덱 내 원소들의 순서를 **제자리(in-place)** 에서 뒤집습니다. 작은 타입은 스택 버퍼(128바이트 이하)를, 큰 타입은 힙 버퍼를 임시 스왑 공간으로 사용합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |
| **반복자 무효화** | 반복자는 유효, 단 가리키는 값이 변경됨 |

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++) push_back(q, i);   // [0][1][2][3][4]

reverse(q);                                     // [4][3][2][1][0]
for (int i = 0; i < (int)size(q); i++)
    printf("[%3d]", q[i]);
puts("");
destroy(q);
```

**출력:**
```
[  4][  3][  2][  1][  0]
```

---

## 표준 C 라이브러리와의 호환성

`DEQUE`는 원소를 연속된 메모리에 저장하므로 `qsort`, `bsearch`를 비롯한 모든 표준 C 라이브러리 함수와 **완전히 호환**됩니다.

```c
#include "opencstl.h"

int cmp_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main() {
    DEQUE(int) q = new_deque(int);

    push_back(q, 38);
    push_back(q, 26);
    push_back(q, 13);
    push_back(q, 83);
    push_back(q, 19);

    insert(q, q + 2, 777);    // 인덱스 2에 777 삽입
    insert(q, q + 4, 3, 999); // 인덱스 4에 999를 세 개 삽입

    for (int i = 0; i < (int)size(q); i++)
        printf("[%3d]", q[i]);
    puts("");

    qsort(q, size(q), sizeof(int), cmp_int);

    for (int i = 0; i < (int)size(q); i++)
        printf("[%3d]", q[i]);
    puts("");

    int key = 83;
    void *r = bsearch(&key, q, size(q), sizeof(int), cmp_int);
    if (r) printf("found %d at index %td\n", *(int*)r, (int*)r - q);

    destroy(q);
    return 0;
}
```

다음과 같은 함수들도 그대로 사용 가능합니다.

| 함수 | 용도 |
|------|------|
| `memcpy(dst, q, sizeof(T)*size(q))` | 덱 내용 복사 |
| `memset(q, 0, sizeof(T)*size(q))`   | 영역 초기화 |
| `fwrite(q, sizeof(T), size(q), fp)` | 파일로 일괄 쓰기 |
| `fread(q, sizeof(T), n, fp)`        | 파일에서 일괄 읽기 |

> **참고**: `qsort`는 현재 원소 영역만 다루기 때문에 leading gap이 있는 상태에서도 안전합니다(핸들이 첫 원소를 가리키므로).

---

## vector / list와의 비교

| 항목 | `VECTOR` | `DEQUE` | `LIST` |
|------|---------|--------|--------|
| **메모리 배치** | 단일 연속 버퍼 | 단일 연속 버퍼 + 앞/뒤 gap | 분산 노드 |
| **핸들 타입** | `TYPE*` | `TYPE*` | `TYPE**` |
| **`[]` 임의 접근** | O(1) | O(1) | 미지원 |
| **앞쪽 push/pop** | O(n) | **분할 상환 O(1)** | O(1) |
| **뒤쪽 push/pop** | 분할 상환 O(1) | 분할 상환 O(1) | O(1) |
| **중간 삽입/삭제** | O(n) | O(n) | O(1) *(반복자 주어진 경우)* |
| **반복자 무효화 (read)** | 없음 | 없음 | 없음 |
| **`push_front` 후 begin 무효화** | (해당 없음) | **있음 (핸들 이동)** | 없음 |
| **`reserve`/`capacity`** | 둘 다 지원 | `capacity`만 | 둘 다 미지원 |
| **stdlib 호환** | 가능 | 가능 | 불가 |
| **캐시 친화성** | 매우 좋음 | 매우 좋음 | 나쁨 |

**언제 `deque`를 선택하나?**
- 양 끝에서 빈번한 삽입/삭제가 필요한 경우 (작업 큐, 슬라이딩 윈도우 등)
- 임의 접근(`[]`)이 필요하면서도 앞쪽 삽입이 자주 일어나는 경우
- BFS, deque-기반 알고리즘 (Monotonic Deque 등)

---

## 참고 사항

- `DEQUE`와 `VECTOR`는 같은 핸들 타입(`TYPE*`)과 동일한 반복자 의미를 공유합니다.
- `push_front`/`pop_front`는 `DEQUE`에서만 사용 가능하며, `VECTOR`에서는 지원되지 않습니다.
- `push_front`/`pop_front` 호출 후에는 핸들 포인터가 변경될 수 있으므로 **항상 매크로 변수 `q`를 통해 접근**해야 합니다. `begin(q)` 결과를 별도로 보관해 두면 무효화될 수 있습니다.
- `float`는 직접 지원되지만, `typedef`로 별칭된 부동소수 타입은 인식되지 않습니다. `float` 키워드를 직접 사용하세요.
- `DEQUE`는 `reserve()`를 지원하지 않습니다. 큰 데이터를 미리 적재해야 한다면 `assign(q, n)` 또는 `resize(q, n)`을 사용하세요.

### 메모리 추적 및 디스패치

`DEQUE`는 내부적으로 **van Emde Boas tree** 기반의 인터벌 트리(`iveb`)에 자신의 주소 범위를 등록합니다. 또한 헤더 슬롯 `-1`에 저장된 `distance` 값을 활용하여, 핸들 포인터가 leading gap 때문에 원래 할당 블록 시작에서 떨어져 있어도 정확한 헤더 위치를 역계산할 수 있습니다.

이 덕분에 OpenCSTL의 일반 디스패치 매크로(`size`, `push_back`, ...)는 `VECTOR`인지 `DEQUE`인지를 런타임에 구분하여 적절한 구현을 호출할 수 있습니다.

---

## 예제

```c
#include "opencstl.h"

int main() {
    DEQUE(int) q = new_deque(int);

    for (int i = 1; i <= 5; i++)
        push_back(q, i);      // [1][2][3][4][5]

    push_front(q, 0);         // [0][1][2][3][4][5]
    pop_back(q);              // [0][1][2][3][4]

    insert(q, q + 2, 99);     // [0][1][99][2][3][4]

    for (int *it = begin(q); it != end(q); it++)
        printf("[%3d]", *it);
    puts("");

    printf("front: %d\n", front(q));
    printf("back:  %d\n", back(q));
    printf("size:  %zu\n", size(q));

    destroy(q);
    return 0;
}
```

**출력:**
```
[  0][  1][ 99][  2][  3][  4]
front: 0
back:  4
size:  6
```

### 슬라이딩 윈도우 최댓값 (Monotonic Deque)

`deque`의 대표적인 활용 예입니다. 윈도우 안에서의 최댓값을 O(n)에 구합니다.

```c
DEQUE(int) idx = new_deque(int);
int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
int n = 8, k = 3;

for (int i = 0; i < n; i++) {
    while (!empty(idx) && back(idx) <= arr[i])
        pop_back(idx);                        // 뒤에서 작은 후보 제거
    push_back(idx, arr[i]);
    if (i >= k && front(idx) == arr[i - k])
        pop_front(idx);                       // 윈도우 밖으로 나간 값 제거
    if (i >= k - 1)
        printf("%d ", front(idx));            // 현재 윈도우 최댓값
}
puts("");
destroy(idx);
```

---

## 함수 요약표

### 생성자 / 소멸자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_deque` | `DEQUE(T) new_deque(T)` | O(1) | 빈 덱 생성 |
| `destroy`   | `void destroy(q)`       | O(1) | 덱 파괴 및 메모리 해제 |

### 원소 접근

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `operator[]` | `q[i]`           | O(1) | 인덱스 `i` 위치 원소 접근 |
| `front`      | `T front(q)`     | O(1) | 첫 번째 원소 |
| `back`       | `T back(q)`      | O(1) | 마지막 원소 |

### 반복자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `begin`  | `T* begin(q)`   | O(1) | 첫 원소 반복자 |
| `end`    | `T* end(q)`     | O(1) | 마지막 원소 다음 위치 |
| `rbegin` | `T* rbegin(q)`  | O(1) | 역방향: 마지막 원소 |
| `rend`   | `T* rend(q)`    | O(1) | 역방향: 첫 원소 이전 위치 |

### 용량(Capacity)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `empty`         | `bool empty(q)`         | O(1) | 비었는지 검사 |
| `size`          | `size_t size(q)`        | O(1) | 원소 개수 |
| `max_size`      | `size_t max_size(q)`    | O(1) | 보유 가능한 이론적 최대 원소 수 |
| `capacity`      | `size_t capacity(q)`    | O(1) | 현재 할당된 저장 용량 |
| `shrink_to_fit` | `void shrink_to_fit(q)` | O(n) | capacity를 size에 맞춤 (leading gap 제거) |

### 수정자(Modifiers)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `assign`      | `assign(q, n)` / `assign(q, n, val)`           | O(n) | 내용을 `n`개 원소로 치환 |
| `push_back`   | `void push_back(q, val)`                       | O(1)* | 끝에 원소 추가 (*분할 상환) |
| `pop_back`    | `void pop_back(q)`                             | O(1) | 마지막 원소 제거 |
| `push_front`  | `void push_front(q, val)`                      | O(1)* | 앞에 원소 추가 (*분할 상환) |
| `pop_front`   | `void pop_front(q)`                            | O(1) | 첫 원소 제거 |
| `insert`      | `insert(q, pos, val)` / `insert(q, pos, n, val)` | O(n) | `pos` 앞에 삽입 |
| `erase`       | `erase(q, pos)` / `erase(q, first, last)`      | O(n) | 원소 제거 |
| `resize`      | `resize(q, n)` / `resize(q, n, val)`           | O(n) | 크기 변경 |
| `clear`       | `void clear(q)`                                | O(1) | 모든 원소 제거 (capacity 유지) |

### 탐색(Lookup)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `find`         | `T* find(q, val)` / `find(q, start, val)` | O(n) | 선형 탐색 |
| `count`        | `size_t count(q, val)`                    | O(n) | 값이 일치하는 원소 개수 |
| `count_if`     | `size_t count_if(q, pred)`                | O(n) | 술어가 참인 원소 개수 |
| `lower_bound`  | `T* lower_bound(q, val, cmp)`             | O(log n) | 정렬된 덱에서 하한 |
| `upper_bound`  | `T* upper_bound(q, val, cmp)`             | O(log n) | 정렬된 덱에서 상한 |

### 연산(Operations)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `reverse` | `void reverse(q)` | O(n) | 원소 순서를 제자리에서 뒤집음 |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`VECTOR`](./vector.md)                     | 동적 배열. 뒤에서만 O(1), 앞은 O(n) |
| [`LIST`](./list.md)                         | 이중 연결 리스트. 어디서든 O(1) 삽입/삭제, `[]` 불가 |
| [`QUEUE`](./__queue.md)                     | FIFO 어댑터 (보통 deque로 백킹) |
| [`STACK`](./__stack.md)                     | LIFO 어댑터 |
| [`PRIORITY_QUEUE`](./__priority_queue.md)   | 최대 힙 어댑터 |
