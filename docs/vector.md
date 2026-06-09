# vector

헤더 [`vector.h`](../opencstl/vector.h)에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
#define VECTOR(TYPE)      TYPE*
#define new_vector(TYPE)  cstl_vector(TYPE)
```

`VECTOR`는 **동적 크기 배열**을 캡슐화한 시퀀스 컨테이너입니다.
C++ 표준 라이브러리의 [`std::vector`](https://cppreference.com/cpp/container/vector)에 대응되는 OpenCSTL의 구현체이며, C 언어에서 매크로 기반으로 제네릭 동작을 제공합니다.

원소들은 **메모리상 연속(contiguous)** 으로 저장됩니다. 따라서 다음과 같은 방식으로 원소에 접근할 수 있습니다.

- 반복자(iterator)
- 원소에 대한 일반 포인터의 오프셋 연산
- `[]` 첨자 연산자 — 일반 C 배열과 동일한 인터페이스

또한 `VECTOR`의 원소를 가리키는 포인터는, 배열 포인터를 인자로 받는 모든 C 표준 라이브러리 함수(`qsort`, `bsearch`, `memcpy`, `fwrite` 등)에 그대로 전달할 수 있습니다.

벡터의 저장 공간은 자동으로 관리되며 필요에 따라 확장됩니다. 벡터는 일반적으로 정적 배열보다 더 많은 공간을 차지하는데, 이는 향후 원소 추가에 대비해 여유 메모리를 미리 확보해 두기 때문입니다. 이러한 전략 덕분에 벡터는 원소를 삽입할 때마다 재할당할 필요가 없으며, 여분의 메모리가 소진된 경우에만 재할당이 일어납니다. 현재 할당되어 있는 총 메모리량은 [`capacity()`](#capacity) 함수로 조회할 수 있습니다.

벡터의 흔한 연산들의 복잡도는 다음과 같습니다.

- 임의 접근 — 상수 **O(1)**
- 끝에서의 삽입 및 제거 — 분할 상환 상수 **O(1)** *(amortized constant)*
- 임의 위치에서의 삽입 및 제거 — 끝까지의 거리에 선형 **O(n)**

---

## 목차

1. [매크로 매개변수](#매크로-매개변수)
2. [멤버 타입](#멤버-타입)
3. [반복자 무효화](#반복자-무효화)
4. [멤버 함수](#멤버-함수)
   - [생성자 / 소멸자](#생성자--소멸자)
   - [원소 접근](#원소-접근)
   - [반복자](#반복자)
   - [용량(Capacity)](#용량capacity)
   - [수정자(Modifiers)](#수정자modifiers)
   - [탐색(Lookup)](#탐색lookup)
   - [연산(Operations)](#연산operations)
5. [표준 C 라이브러리와의 호환성](#표준-c-라이브러리와의-호환성)
6. [참고 사항](#참고-사항)
7. [예제](#예제)
8. [함수 요약표](#함수-요약표)
9. [관련 항목](#관련-항목)

---

## 매크로 매개변수

### `VECTOR(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 동적 배열에 저장될 원소의 타입 |

`TYPE*`으로 확장됩니다. **변수 선언 전용**이며 초기화는 별도로 [`new_vector`](#new_vector)를 호출해 수행해야 합니다.

```c
VECTOR(int)   v;   // 확장 결과: int*  v;
VECTOR(char*) sv;  // 확장 결과: char** sv;
```

### `new_vector(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 동적 배열에 저장될 원소의 타입 |

새로운 빈 벡터를 생성하여 반환합니다. 반환된 포인터는 더 이상 사용하지 않을 때 반드시 [`destroy`](#destroy)를 통해 해제되어야 합니다.

```c
VECTOR(int) v = new_vector(int);
// ... 사용 ...
destroy(v);
```

---

## 멤버 타입

OpenCSTL의 `VECTOR`는 C 매크로 기반 구현이므로 C++의 typedef 멤버 타입과 1:1 대응되는 형태는 아니지만, 동일한 개념이 다음과 같이 매핑됩니다.

| 개념 (C++ 표준) | OpenCSTL 대응 |
|-----------------|--------------|
| `value_type`              | `TYPE` (매크로 인자) |
| `reference` / `pointer`   | `TYPE&` 대응: 첨자 표현식 `v[i]`, 포인터: `TYPE*` |
| `const_reference`         | 상수 컨텍스트의 `v[i]` |
| `iterator`                | `TYPE*` — 일반 포인터 |
| `reverse_iterator`        | `TYPE*` — 역방향은 포인터 감소(`it--`) 형태 |
| `size_type`               | `size_t` |
| `difference_type`         | `ptrdiff_t` |
| `allocator_type`          | 내부 `calloc`/`realloc` 사용 (고정) |

> **참고**: 반복자는 단순한 포인터이므로 `it + n`, `it - n`, `it1 - it2`, `*it`, `it[k]` 등 표준 C 포인터 연산을 모두 지원합니다.

---

## 반복자 무효화

| 연산 | 무효화되는 반복자 |
|------|------------------|
| 모든 읽기 전용 연산 | 없음 |
| `assign`, `clear` | 모든 반복자 |
| `reserve`, `shrink_to_fit` | 재할당이 발생하면 모든 반복자 |
| `push_back` | 재할당 시 모든 반복자, 아니면 `end(v)`만 |
| `pop_back` | 제거된 원소와 `end(v)` |
| `insert` | 재할당 시 모든 반복자, 아니면 삽입 지점부터 `end(v)`까지 |
| `erase` | 제거된 원소와 그 이후의 모든 반복자 (`end(v)` 포함) |
| `resize` | 재할당 시 모든 반복자, 아니면 `end(v)`와 제거되는 원소들 |
| `destroy` | 모든 반복자 |

> 컨테이너 내부 버퍼에 대한 포인터 또한 동일한 규칙으로 무효화됩니다. 재할당이 일어나면 그 이전에 보관해 둔 `T*` 포인터들은 모두 댕글링(dangling) 상태가 됩니다.

---

## 멤버 함수

### 생성자 / 소멸자

#### `new_vector`

```c
VECTOR(TYPE) new_vector(TYPE);
```

원소가 없는 빈 벡터를 생성합니다. 초기 내부 저장공간이 할당됩니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 벡터에 대한 포인터 |
| **복잡도** | 상수 **O(1)** |
| **예외** | 메모리 할당 실패 시 `fault()` 호출 |

```c
VECTOR(int) v = new_vector(int);
printf("%zu\n", size(v));   // 0
destroy(v);
```

---

#### `destroy`

```c
void destroy(VECTOR(TYPE) v);
```

벡터를 파괴하고 할당된 모든 메모리를 해제합니다.
`new_vector`로 생성된 모든 벡터는 반드시 한 번 `destroy`되어야 합니다. 호출 이후 포인터 `v`는 무효 상태가 됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 원소 개수에 선형 **O(n)** *(원소가 단순 POD인 경우 상수)* |

```c
VECTOR(int) v = new_vector(int);
push_back(v, 1);
push_back(v, 2);
destroy(v);
// v 는 더 이상 사용 불가
```

> **주의**: 중첩된 컨테이너(예: `VECTOR(int*)`로 표현한 2차원 배열)의 경우, 내부 벡터들을 먼저 `destroy`한 뒤 외부 벡터를 `destroy`해야 합니다. [참고 사항](#참고-사항)의 2D 배열 패턴을 확인하세요.

---

### 원소 접근

#### `operator[]`

```c
TYPE v[i];
```

지정된 인덱스 `i` 위치의 원소(0-based)에 접근합니다. 일반 C 배열처럼 좌변값(lvalue)으로 사용 가능하여 읽기와 쓰기가 모두 가능합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 인덱스 `i` 위치의 원소에 대한 참조 |
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `0 <= i < size(v)` — 범위를 벗어나면 **정의되지 않은 동작(UB)** |

`VECTOR`와 `DEQUE`에서만 사용 가능합니다. `LIST`에서는 지원하지 않습니다.

```c
VECTOR(int) v = new_vector(int);
assign(v, 5, 0);
v[0] = 10;
v[4] = 99;
printf("%d %d\n", v[0], v[4]);   // 10 99
destroy(v);
```

---

#### `front`

```c
TYPE front(VECTOR(TYPE) v);
```

첫 번째 원소의 값을 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | `v[0]`의 값 |
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(v) == false` — 빈 벡터에 호출 시 **UB** |

```c
VECTOR(int) v = new_vector(int);
push_back(v, 10);
push_back(v, 20);
push_back(v, 30);
printf("%d\n", front(v));   // 10
destroy(v);
```

---

#### `back`

```c
TYPE back(VECTOR(TYPE) v);
```

마지막 원소의 값을 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | `v[size(v) - 1]`의 값 |
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(v) == false` — 빈 벡터에 호출 시 **UB** |

```c
VECTOR(int) v = new_vector(int);
push_back(v, 10);
push_back(v, 20);
push_back(v, 30);
printf("%d\n", back(v));   // 30
destroy(v);
```

---

### 반복자

`VECTOR`의 반복자 타입은 `TYPE*`입니다. 원소가 연속 메모리에 저장되기 때문에 반복자 산술 연산(`it++`, `it--`, `it + n`, `it - n`, `it1 - it2`)은 일반 C 포인터 산술과 정확히 동일하게 동작합니다.

#### `begin`

```c
TYPE* begin(VECTOR(TYPE) v);
```

첫 번째 원소를 가리키는 반복자(포인터)를 반환합니다. `v` 자체와 동일한 값입니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i * 10);

for (int *it = begin(v); it != end(v); it++)
    printf("[%3d]", *it);   // [  0][ 10][ 20][ 30][ 40]
puts("");
destroy(v);
```

---

#### `end`

```c
TYPE* end(VECTOR(TYPE) v);
```

마지막 원소의 **다음** 위치를 가리키는 반복자를 반환합니다. **역참조해서는 안 됩니다.** 순방향 순회의 종료 조건으로 사용됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `rbegin`

```c
TYPE* rbegin(VECTOR(TYPE) v);
```

마지막 원소를 가리키는 역방향 반복자를 반환합니다. 역방향 순회 시 `it--`로 다음(앞쪽) 원소로 이동합니다.

> C++ 표준의 `reverse_iterator`와 달리 OpenCSTL은 단순 포인터를 사용하므로, **`++`가 아닌 `--`**로 이동해야 함에 유의하세요.

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i);

for (int *it = rbegin(v); it != rend(v); it--)
    printf("[%3d]", *it);   // [  4][  3][  2][  1][  0]
puts("");
destroy(v);
```

---

#### `rend`

```c
TYPE* rend(VECTOR(TYPE) v);
```

첫 번째 원소 **바로 앞** 위치를 가리키는 역방향 반복자를 반환합니다. **역참조해서는 안 됩니다.** 역방향 순회의 종료 조건으로 사용됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

### 용량(Capacity)

#### `empty`

```c
bool empty(VECTOR(TYPE) v);
```

벡터에 원소가 하나도 없으면 `true`, 그렇지 않으면 `false`를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
VECTOR(int) v = new_vector(int);
printf("%d\n", empty(v));   // 1
push_back(v, 42);
printf("%d\n", empty(v));   // 0
destroy(v);
```

---

#### `size`

```c
size_t size(VECTOR(TYPE) v);
```

현재 벡터에 저장된 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i);
printf("%zu\n", size(v));   // 5
destroy(v);
```

---

#### `max_size`

```c
size_t max_size(VECTOR(TYPE) v);
```

벡터가 시스템 제약상 보유할 수 있는 원소의 **이론적 최대 개수**를 반환합니다. 실제 가용량은 사용 가능한 RAM 크기에 의해 제한됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
VECTOR(int) v = new_vector(int);
printf("max: %zu\n", max_size(v));
destroy(v);
```

---

#### `reserve`

```c
void reserve(VECTOR(TYPE) v, size_t n);
```

벡터의 용량(capacity)이 최소 `n`이 되도록 저장공간을 사전 할당합니다. `n`이 현재 `capacity(v)`보다 작거나 같으면 아무 일도 일어나지 않습니다. `size(v)`는 변경되지 않습니다.

`reserve`는 사전에 큰 데이터를 적재할 것이 예상될 때 **여러 번의 재할당을 한 번으로 줄이는** 최적화 도구입니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 재할당이 발생하면 원소 수에 선형 **O(n)**, 아니면 **O(1)** |
| **반복자 무효화** | 재할당 시 모든 반복자/포인터 무효화 |

```c
VECTOR(int) v = new_vector(int);
reserve(v, 1000);   // 미리 1000개 분량 확보
printf("size:     %zu\n", size(v));      // 0
printf("capacity: %zu\n", capacity(v));  // >= 1000

for (int i = 0; i < 1000; i++)
    push_back(v, i);   // 재할당 없이 추가됨
destroy(v);
```

---

#### `capacity`

```c
size_t capacity(VECTOR(TYPE) v);
```

재할당 없이 보유할 수 있는 원소의 개수를 반환합니다. 항상 `capacity(v) >= size(v)`가 성립합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i);
printf("size:     %zu\n", size(v));      // 5
printf("capacity: %zu\n", capacity(v));  // >= 5 (구현마다 다름)
destroy(v);
```

---

#### `shrink_to_fit`

```c
void shrink_to_fit(VECTOR(TYPE) v);
```

남는 capacity를 해제해 `capacity(v) == size(v)`가 되도록 요청합니다. 메모리를 회수하기 위한 비구속적(non-binding) 요청이지만, OpenCSTL 구현은 항상 size에 맞추어 재할당을 수행합니다(단, `size == 0`인 경우 capacity는 1로 유지).

| 항목 | 내용 |
|------|------|
| **복잡도** | 원소 수에 선형 **O(n)** |
| **반복자 무효화** | 재할당이 일어나면 모든 반복자/포인터 무효화 |

```c
VECTOR(int) v = new_vector(int);
reserve(v, 1000);
push_back(v, 1);
push_back(v, 2);

printf("before: cap = %zu\n", capacity(v));  // 1000
shrink_to_fit(v);
printf("after:  cap = %zu\n", capacity(v));  // 2
destroy(v);
```

---

### 수정자(Modifiers)

#### `assign`

```c
void assign(VECTOR(TYPE) v, size_t n);
void assign(VECTOR(TYPE) v, size_t n, TYPE value);
```

기존 내용을 모두 폐기하고 `n`개의 원소로 채웁니다. `value`가 주어지면 모든 원소가 그 값으로 초기화되고, 그렇지 않으면 0으로 초기화됩니다.

| 오버로드 | 설명 |
|----------|-----|
| `assign(v, n)`        | `n`개의 0 초기화 원소를 할당 |
| `assign(v, n, value)` | `n`개의 원소를 모두 `value`로 채움 |

| 항목 | 내용 |
|------|------|
| **복잡도** | `n`에 선형 **O(n)** |
| **반복자 무효화** | 항상 모든 반복자 |

```c
VECTOR(int) v = new_vector(int);
assign(v, 5);        // [0][0][0][0][0]
assign(v, 5, 7);     // [7][7][7][7][7]
destroy(v);
```

---

#### `push_back`

```c
void push_back(VECTOR(TYPE) v, TYPE value);
```

벡터의 끝에 `value`를 추가합니다. 내부 버퍼가 가득 차 있다면 자동으로 재할당이 일어납니다(이 경우 모든 반복자/포인터가 무효화됩니다).

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized** *(재할당 시 O(n))* |
| **반복자 무효화** | 재할당 시 모든 반복자, 아니면 `end(v)`만 |

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i);   // [0][1][2][3][4]
destroy(v);
```

---

#### `pop_back`

```c
void pop_back(VECTOR(TYPE) v);
```

벡터의 마지막 원소를 제거합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(v) == false` — 빈 벡터에 호출 시 **UB** |
| **반복자 무효화** | 제거된 원소와 `end(v)` |

```c
VECTOR(int) v = new_vector(int);
push_back(v, 10);
push_back(v, 20);
push_back(v, 30);
pop_back(v);
printf("%d\n", back(v));   // 20
printf("%zu\n", size(v));  // 2
destroy(v);
```

---

#### `insert`

```c
void insert(VECTOR(TYPE) v, TYPE* pos, TYPE value);
void insert(VECTOR(TYPE) v, TYPE* pos, size_t n, TYPE value);
```

`pos`가 가리키는 원소 **앞**에 원소를 삽입합니다. `pos`는 보통 `v + index` 형태로 지정합니다.

| 오버로드 | 설명 |
|----------|-----|
| `insert(v, pos, value)`    | 단일 `value`를 `pos` 앞에 삽입 |
| `insert(v, pos, n, value)` | `value`를 `n`개 복제하여 `pos` 앞에 삽입 |

| 항목 | 내용 |
|------|------|
| **복잡도** | `pos`에서 끝까지 거리 + 삽입 개수에 선형 **O(n)** |
| **반복자 무효화** | 재할당 시 모든 반복자, 아니면 삽입 지점 ~ `end(v)` 범위 |

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i * 10);    // [0][10][20][30][40]

insert(v, v + 2, 99);        // [0][10][99][20][30][40]
insert(v, v + 4, 3, 77);     // [0][10][99][20][77][77][77][30][40]

for (int i = 0; i < (int)size(v); i++)
    printf("[%3d]", v[i]);
puts("");
destroy(v);
```

**출력:**
```
[  0][ 10][ 99][ 20][ 77][ 77][ 77][ 30][ 40]
```

---

#### `erase`

```c
void erase(VECTOR(TYPE) v, TYPE* pos);
void erase(VECTOR(TYPE) v, TYPE* first, TYPE* last);
```

벡터에서 원소(들)를 제거합니다. 제거된 위치 이후의 모든 반복자(및 `end(v)`)는 무효화됩니다.

| 오버로드 | 설명 |
|----------|-----|
| `erase(v, pos)`           | `pos` 위치의 원소 하나 제거 |
| `erase(v, first, last)`   | 반열림 구간 `[first, last)`의 원소를 모두 제거 |

| 항목 | 내용 |
|------|------|
| **복잡도** | 제거 위치 이후 원소 수에 선형 **O(n)** |

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i * 10);   // [0][10][20][30][40]

erase(v, v + 1);             // [0][20][30][40]
erase(v, v + 1, v + 3);      // [0][40]

for (int i = 0; i < (int)size(v); i++)
    printf("[%3d]", v[i]);
puts("");
destroy(v);
```

**출력:**
```
[  0][ 40]
```

---

#### `resize`

```c
void resize(VECTOR(TYPE) v, size_t n);
void resize(VECTOR(TYPE) v, size_t n, TYPE value);
```

벡터의 크기가 정확히 `n`이 되도록 조정합니다.

- `n > size(v)`인 경우: 새로운 원소가 끝에 추가됩니다. `value`가 제공되면 그 값으로, 아니면 0으로 초기화됩니다.
- `n < size(v)`인 경우: 인덱스 `n` 이후의 원소가 제거됩니다.
- `n == size(v)`인 경우: 아무 일도 일어나지 않습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | `|n - size(v)|`에 선형 **O(n)** |
| **반복자 무효화** | 재할당 시 모든 반복자, 아니면 `end(v)`와 제거되는 원소들 |

```c
VECTOR(int) v = new_vector(int);
push_back(v, 1);
push_back(v, 2);
push_back(v, 3);

resize(v, 6, 99);   // [1][2][3][99][99][99]
for (int i = 0; i < (int)size(v); i++)
    printf("[%3d]", v[i]);
puts("");

resize(v, 2);       // [1][2]
printf("size: %zu\n", size(v));
destroy(v);
```

**출력:**
```
[  1][  2][  3][ 99][ 99][ 99]
size: 2
```

---

#### `clear`

```c
void clear(VECTOR(TYPE) v);
```

벡터의 모든 원소를 제거합니다. 호출 후 `size(v) == 0`이 됩니다. **내부 버퍼는 유지**되므로 `capacity(v)`는 변경되지 않으며, 동일한 벡터를 효율적으로 재사용할 수 있습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(원소 소멸자가 따로 없으므로)* |
| **반복자 무효화** | 모든 반복자 |

```c
VECTOR(int) v = new_vector(int);
push_back(v, 1);
push_back(v, 2);
push_back(v, 3);

clear(v);
printf("size: %zu\n", size(v));   // 0

push_back(v, 99);
printf("size: %zu\n", size(v));   // 1
destroy(v);
```

---

### 탐색(Lookup)

#### `find`

```c
TYPE* find(VECTOR(TYPE) v, TYPE value);
TYPE* find(VECTOR(TYPE) v, TYPE* start, TYPE value);
```

벡터 내에서 `value`를 선형 탐색합니다.

| 오버로드 | 설명 |
|----------|-----|
| `find(v, value)`        | 처음부터 벡터 전체를 탐색 |
| `find(v, start, value)` | 반복자 `start` 위치부터 탐색 |

| 항목 | 내용 |
|------|------|
| **반환값** | 일치하는 첫 원소를 가리키는 반복자(포인터). 찾지 못한 경우 `end(v)` |
| **복잡도** | 선형 **O(n)** |

정렬된 벡터에서는 표준 C 라이브러리의 `bsearch`(또는 [`lower_bound`](#lower_bound)/[`upper_bound`](#upper_bound))를 사용해 **O(log n)** 으로 탐색할 수 있습니다.

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i * 10);   // [0][10][20][30][40]

int *it = find(v, 20);
if (it != end(v))
    printf("found %d at index %td\n", *it, it - v);   // found 20 at index 2

int *it2 = find(v, v + 3, 30);
if (it2 != end(v))
    printf("found %d at index %td\n", *it2, it2 - v); // found 30 at index 3

destroy(v);
```

---

#### `count`

```c
size_t count(VECTOR(TYPE) v, TYPE value);
```

벡터 내에서 `value`와 같은 원소의 개수를 반환합니다. 비교는 타입에 따라 적절한 동치 함수가 사용됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |

```c
VECTOR(int) v = new_vector(int);
push_back(v, 1); push_back(v, 2); push_back(v, 2);
push_back(v, 3); push_back(v, 2);

printf("count of 2: %zu\n", count(v, 2));   // 3
destroy(v);
```

---

#### `count_if`

```c
size_t count_if(VECTOR(TYPE) v, bool (*pred)(void*));
```

`pred(원소)`가 `true`를 반환하는 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |

```c
bool is_even(void *p) { return (*(int*)p) % 2 == 0; }

VECTOR(int) v = new_vector(int);
for (int i = 1; i <= 10; i++) push_back(v, i);

printf("evens: %zu\n", count_if(v, is_even));   // 5
destroy(v);
```

---

#### `lower_bound`

```c
TYPE* lower_bound(VECTOR(TYPE) v, TYPE value, int (*cmp)(void*, void*));
```

**정렬된** 벡터에서 `value`보다 작지 않은(`!(x < value)`) 첫 원소를 가리키는 반복자를 이진 탐색으로 찾아 반환합니다. 비교 함수 `cmp(a, b)`는 `a < b`이면 음수, `a == b`이면 0, `a > b`이면 양수를 반환해야 합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 조건을 만족하는 첫 원소의 반복자, 없으면 `NULL` |
| **복잡도** | **O(log n)** |
| **사전 조건** | 벡터가 `cmp` 기준으로 정렬되어 있어야 함 |

```c
int int_cmp(void *a, void *b) { return *(int*)a - *(int*)b; }

VECTOR(int) v = new_vector(int);
int xs[] = {1, 2, 4, 4, 4, 7, 9};
for (int i = 0; i < 7; i++) push_back(v, xs[i]);

int *it = lower_bound(v, 4, int_cmp);
printf("index: %td\n", it - v);   // 2 (가장 처음 등장하는 4의 위치)
destroy(v);
```

---

#### `upper_bound`

```c
TYPE* upper_bound(VECTOR(TYPE) v, TYPE value, int (*cmp)(void*, void*));
```

**정렬된** 벡터에서 `value`보다 큰(`value < x`) 첫 원소를 가리키는 반복자를 이진 탐색으로 찾아 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 조건을 만족하는 첫 원소의 반복자, 없으면 `NULL` |
| **복잡도** | **O(log n)** |
| **사전 조건** | 벡터가 `cmp` 기준으로 정렬되어 있어야 함 |

`lower_bound`와 `upper_bound`를 함께 사용하면 동일 값의 구간 `[lower, upper)`를 얻을 수 있습니다.

```c
int int_cmp(void *a, void *b) { return *(int*)a - *(int*)b; }

VECTOR(int) v = new_vector(int);
int xs[] = {1, 2, 4, 4, 4, 7, 9};
for (int i = 0; i < 7; i++) push_back(v, xs[i]);

int *lo = lower_bound(v, 4, int_cmp);
int *hi = upper_bound(v, 4, int_cmp);
printf("count of 4: %td\n", hi - lo);   // 3
destroy(v);
```

---

### 연산(Operations)

#### `reverse`

```c
void reverse(VECTOR(TYPE) v);
```

벡터 내 원소들의 순서를 **제자리(in-place)** 에서 뒤집습니다. OpenCSTL의 확장 기능으로, C++ 표준 `std::vector`에는 없지만 `std::reverse(v.begin(), v.end())`와 동등한 동작을 수행합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |
| **반복자 무효화** | 반복자는 유효, 단 가리키는 값이 변경됨 |

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++) push_back(v, i);   // [0][1][2][3][4]

reverse(v);                                      // [4][3][2][1][0]
for (int i = 0; i < (int)size(v); i++)
    printf("[%3d]", v[i]);
puts("");
destroy(v);
```

**출력:**
```
[  4][  3][  2][  1][  0]
```

---

## 표준 C 라이브러리와의 호환성

`VECTOR`는 연속된 메모리에 원소를 저장하므로, `qsort`, `bsearch`를 포함한 모든 표준 C 라이브러리 함수와 **완전히 호환**됩니다.

```c
#include "opencstl.h"

int StringCmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    VECTOR(char*) v = new_vector(char*);
    push_back(v, "banana");
    push_back(v, "apple");
    push_back(v, "cherry");
    push_back(v, "spring");

    qsort(v, size(v), sizeof(char *), StringCmp);

    for (int i = 0; i < (int)size(v); i++)
        puts(v[i]);

    char *key = "spring";
    void *result = bsearch(&key, v, size(v), sizeof(char *), StringCmp);
    if (result) {
        int idx = (char **)result - (char **)v;
        printf("bsearch: %s (index %d)\n", *(char **)result, idx);
    }

    destroy(v);
    return 0;
}
```

**출력:**
```
apple
banana
cherry
spring
bsearch: spring (index 3)
```

이 외에도 다음과 같은 함수들과 그대로 호환됩니다.

| 함수 | 용도 |
|------|------|
| `memcpy(dst, v, sizeof(T)*size(v))` | 벡터 내용 복사 |
| `memset(v, 0, sizeof(T)*size(v))` | 벡터 영역 초기화 |
| `fwrite(v, sizeof(T), size(v), fp)` | 파일로 일괄 쓰기 |
| `fread(v, sizeof(T), n, fp)` | 파일에서 일괄 읽기 (사전에 `resize(v, n)` 필요) |

---

## 참고 사항

- `VECTOR`의 핸들 타입은 `TYPE*`입니다(연속 시퀀스 컨테이너).
  반면 노드 기반 컨테이너(`LIST`, `SET`, `MAP`)는 핸들 타입으로 `TYPE**`을 사용합니다.
- `float`는 직접 지원되지만, `typedef`로 별칭된 부동소수 타입은 인식되지 않습니다. `float` 키워드를 직접 사용하세요.
- `VECTOR(int*)`(벡터의 벡터, 즉 포인터 벡터)의 경우, 각 내부 벡터는 외부 벡터를 `destroy`하기 **전에** 개별적으로 `destroy`되어야 합니다.

### 2D 배열 (중첩 벡터) 패턴

```c
VECTOR(int*) matrix = new_vector(int*);
const size_t sz = 4;

assign(matrix, sz);
for (int i = 0; i < (int)size(matrix); i++) {
    matrix[i] = new_vector(int);
    assign(matrix[i], sz, 0);
}

for (int i = 0; i < (int)size(matrix); i++)
    for (int j = 0; j < (int)size(matrix[i]); j++)
        matrix[i][j] = i * j;

for (int i = 0; i < (int)size(matrix); i++) {
    for (int j = 0; j < (int)size(matrix[i]); j++)
        printf("[%3d]", matrix[i][j]);
    puts("");
}

// 내부 벡터를 먼저, 외부 벡터를 나중에 destroy
for (int i = 0; i < (int)size(matrix); i++)
    destroy(matrix[i]);
destroy(matrix);
```

**출력:**
```
[  0][  0][  0][  0]
[  0][  1][  2][  3]
[  0][  2][  4][  6]
[  0][  3][  6][  9]
```

### 성장 전략

OpenCSTL의 `VECTOR`는 capacity가 부족할 때 내부적으로 정의된 성장 정책(`get_new_capacity`)에 따라 새 capacity를 정합니다. 일반적으로 1.5배~2배 사이의 기하급수적 성장 전략을 따르며, 이를 통해 `push_back`의 **분할 상환 상수 시간** 보장이 성립합니다.

### 메모리 추적

`VECTOR`는 내부적으로 **van Emde Boas tree** 기반의 인터벌 트리(`iveb`)에 자신의 주소 범위를 등록합니다. 이는 일부 OpenCSTL의 알고리즘(예: 자동 타입 추론, 반복자 검증)이 임의의 포인터가 어느 컨테이너에 속하는지를 빠르게 결정하기 위함입니다. `destroy` 호출 시 이 등록도 해제됩니다.

---

## 예제

```c
#include "opencstl.h"

int main() {
    VECTOR(int) v = new_vector(int);

    // 끝에 원소 추가
    for (int i = 0; i < 5; i++)
        push_back(v, i);           // [0][1][2][3][4]

    // 첨자 쓰기
    v[2] = -1;                     // [0][1][-1][3][4]

    // 인덱스 1에 삽입
    insert(v, v + 1, 99);          // [0][99][1][-1][3][4]

    // 인덱스 3 삭제
    erase(v, v + 3);               // [0][99][1][3][4]

    // 순방향 순회
    for (int *it = begin(v); it != end(v); it++)
        printf("[%3d]", *it);
    puts("");

    // 역방향 순회
    for (int *it = rbegin(v); it != rend(v); it--)
        printf("[%3d]", *it);
    puts("");

    printf("size:     %zu\n", size(v));
    printf("capacity: %zu\n", capacity(v));
    printf("front:    %d\n",  front(v));
    printf("back:     %d\n",  back(v));

    destroy(v);
    return 0;
}
```

**출력:**
```
[  0][ 99][  1][  3][  4]
[  4][  3][  1][ 99][  0]
size:     5
capacity: 8
front:    0
back:     4
```

---

## 함수 요약표

### 생성자 / 소멸자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_vector` | `VECTOR(T) new_vector(T)` | O(1) | 빈 벡터를 생성 |
| `destroy`    | `void destroy(v)`         | O(n) | 벡터를 파괴하고 메모리 해제 |

### 원소 접근

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `operator[]` | `v[i]`           | O(1) | 인덱스 `i` 위치 원소 접근 |
| `front`      | `T front(v)`     | O(1) | 첫 번째 원소 |
| `back`       | `T back(v)`      | O(1) | 마지막 원소 |

### 반복자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `begin`  | `T* begin(v)`   | O(1) | 첫 원소를 가리키는 반복자 |
| `end`    | `T* end(v)`     | O(1) | 마지막 원소 다음 위치 |
| `rbegin` | `T* rbegin(v)`  | O(1) | 역방향: 마지막 원소 |
| `rend`   | `T* rend(v)`    | O(1) | 역방향: 첫 원소 이전 위치 |

### 용량(Capacity)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `empty`          | `bool empty(v)`         | O(1) | 비었는지 검사 |
| `size`           | `size_t size(v)`        | O(1) | 원소 개수 |
| `max_size`       | `size_t max_size(v)`    | O(1) | 보유 가능한 이론적 최대 원소 수 |
| `reserve`        | `void reserve(v, n)`    | O(n) | capacity ≥ `n`이 되도록 사전 할당 |
| `capacity`       | `size_t capacity(v)`    | O(1) | 현재 할당된 저장 용량 |
| `shrink_to_fit`  | `void shrink_to_fit(v)` | O(n) | capacity를 size에 맞춤 |

### 수정자(Modifiers)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `assign`     | `assign(v, n)` / `assign(v, n, val)`             | O(n) | 내용을 `n`개 원소로 치환 |
| `push_back`  | `void push_back(v, val)`                         | O(1)* | 끝에 원소 추가 (*분할 상환) |
| `pop_back`   | `void pop_back(v)`                               | O(1) | 마지막 원소 제거 |
| `insert`     | `insert(v, pos, val)` / `insert(v, pos, n, val)` | O(n) | `pos` 앞에 삽입 |
| `erase`      | `erase(v, pos)` / `erase(v, first, last)`        | O(n) | 원소 제거 |
| `resize`     | `resize(v, n)` / `resize(v, n, val)`             | O(n) | 크기를 `n`으로 변경 |
| `clear`      | `void clear(v)`                                  | O(1) | 모든 원소 제거 (capacity 유지) |

### 탐색(Lookup)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `find`         | `T* find(v, val)` / `find(v, start, val)` | O(n) | 선형 탐색 |
| `count`        | `size_t count(v, val)`                    | O(n) | 값이 일치하는 원소 개수 |
| `count_if`     | `size_t count_if(v, pred)`                | O(n) | 술어가 참인 원소 개수 |
| `lower_bound`  | `T* lower_bound(v, val, cmp)`             | O(log n) | 정렬된 벡터에서 하한 |
| `upper_bound`  | `T* upper_bound(v, val, cmp)`             | O(log n) | 정렬된 벡터에서 상한 |

### 연산(Operations)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `reverse` | `void reverse(v)` | O(n) | 원소 순서를 제자리에서 뒤집음 |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`LIST`](./__list.md)                       | 이중 연결 리스트. 반복자를 통한 임의 위치 삽입/삭제가 O(1), `[]` 접근 불가 |
| [`DEQUE`](./__deque.md)                     | 양방향 큐. `[]` 접근 지원, 양 끝에서의 삽입/삭제가 O(1) |
| [`SET`](./__set.md)                         | 정렬된 유일 키 집합. 삽입/삭제/검색 O(log n) |
| [`MAP`](./__map.md)                         | 정렬된 키-값 저장소. 모든 연산이 O(log n) |
| [`STACK`](./__stack.md)                     | LIFO 어댑터 |
| [`QUEUE`](./__queue.md)                     | FIFO 어댑터 |
| [`PRIORITY_QUEUE`](./__priority_queue.md)   | 최대 힙(max-heap) 어댑터 |
| [`UNORDERED_SET`](./__unordered_set.md)     | 해시 기반 유일 키 집합. 평균 O(1) |
| [`UNORDERED_MAP`](./__unordered_map.md)     | 해시 기반 키-값 저장소. 평균 O(1) |
