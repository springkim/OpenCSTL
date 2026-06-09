# unordered_set

헤더 [`hashtable.h`](../opencstl/hashtable.h)에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
#define UNORDERED_SET(TYPE)             TYPE*
#define new_unordered_set(TYPE)         cstl_unordered_set(TYPE)
#define new_unordered_set(TYPE, hash)   cstl_unordered_set(TYPE, hash)
```

`UNORDERED_SET`은 **유일한(unique) 원소**를 **해시 버킷**에 저장하여 평균 **O(1)** 삽입, 삭제, 탐색을 제공하는 연관 컨테이너입니다.
C++ 표준 라이브러리의 [`std::unordered_set`](https://cppreference.com/cpp/container/unordered_set)에 대응되는 OpenCSTL의 구현체입니다.

`SET`과 달리 순회 시 정렬 순서가 보장되지 **않으며**, **버킷 순서**대로 방문됩니다. 단, OpenCSTL의 `UNORDERED_SET`은 일반적인 해시 컨테이너에서 잘 지원되지 않는 `rbegin`/`rend`/`prev`도 함께 제공하여 역방향 순회를 지원합니다(역방향 정렬이 아닌 역방향 버킷 순서).

내부 구조는 **flat 해시 테이블 + 개방 주소법(open addressing)** 으로, 핸들 자체가 버킷 배열의 첫 슬롯을 가리킵니다. 따라서 `h[i]`로 직접 버킷 슬롯 `i`에 접근할 수 있으며, `capacity`는 총 버킷 슬롯의 개수를 반환합니다.

`unordered_set`의 주요 연산 복잡도는 다음과 같습니다.

- 탐색, 삽입, 제거 — 평균 **O(1)**, 최악 **O(n)**
- 전체 순회 — **O(capacity)** *(빈 슬롯도 스캔하므로 `size`가 아니라 `capacity`에 비례)*

---

## 목차

1. [매크로 매개변수](#매크로-매개변수)
2. [멤버 타입](#멤버-타입)
3. [내부 구조: Flat Hash Table](#내부-구조-flat-hash-table)
4. [반복자 무효화](#반복자-무효화)
5. [멤버 함수](#멤버-함수)
   - [생성자 / 소멸자](#생성자--소멸자)
   - [원소 접근](#원소-접근)
   - [반복자](#반복자)
   - [용량(Capacity)](#용량capacity)
   - [수정자(Modifiers)](#수정자modifiers)
   - [탐색(Lookup)](#탐색lookup)
6. [set과의 비교](#set과의-비교)
7. [참고 사항](#참고-사항)
8. [예제](#예제)
9. [함수 요약표](#함수-요약표)
10. [관련 항목](#관련-항목)

---

## 매크로 매개변수

### `UNORDERED_SET(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 집합에 저장할 원소의 타입 |

**`TYPE*`으로 확장됩니다.** `SET`/`MAP`/`LIST`가 사용하는 `TYPE**`(노드 기반 핸들)과는 다른, `VECTOR`/`DEQUE`와 같은 평면 핸들 타입입니다. 이는 OpenCSTL의 unordered 컨테이너가 **노드 기반이 아닌 flat 테이블** 구현임을 의미합니다.

```c
UNORDERED_SET(int)   h;   // 확장 결과: int*  h;
UNORDERED_SET(char*) sh;  // 확장 결과: char** sh;
```

### `new_unordered_set(TYPE [, hash_fn])`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE` | 원소의 타입 |
| `hash_fn` *(선택)* | 해시 함수 `size_t (*)(const void*)`. 생략하거나 `NULL`을 전달하면 내장 기본 해시(xxHash 변형)가 사용됩니다. |

```c
UNORDERED_SET(int) h = new_unordered_set(int);          // 기본 해시
UNORDERED_SET(int) h = new_unordered_set(int, NULL);    // 동일
// ... 사용 ...
destroy(h);
```

---

## 멤버 타입

| 개념 (C++ 표준) | OpenCSTL 대응 |
|-----------------|--------------|
| `key_type` / `value_type` | `TYPE` |
| `hasher`                  | 생성자에 전달한 `size_t (*)(const void*)` 또는 기본 해시 |
| `reference` / `pointer`   | `TYPE&` / `TYPE*` |
| `iterator`                | `TYPE*` — 버킷 슬롯을 가리키는 포인터 |
| `reverse_iterator`        | `TYPE*` — `prev()`로 역방향 이동 |
| `size_type`               | `size_t` |

> **반복자의 본질**: 반복자는 버킷 배열의 한 슬롯(원소 데이터)을 가리키는 포인터입니다. `next()`/`prev()`는 다음/이전 **점유된 슬롯**까지 빈 슬롯을 건너뛰며 이동합니다. 따라서 빈 슬롯 위에서 `*it`을 수행하지 않도록 항상 반복자 인터페이스를 사용해야 합니다. **`it++` / `it--`는 빈 슬롯에서 멈추지 못하므로 사용 금지입니다.**

---

## 내부 구조: Flat Hash Table

`UNORDERED_SET`은 다음 특성을 가진 **flat (open-addressing) 해시 테이블**로 구현됩니다.

```
                    핸들 h ─┐
                           ↓
┌────────┬──────────┬──────┬──────┬──────┬──────┬──────┬──────┐
│ 헤더   │ control  │ s0   │ s1   │ s2   │ ...  │      │ sN-1 │
│ (메타) │ bytes    │      │      │      │      │      │      │
└────────┴──────────┴──────┴──────┴──────┴──────┴──────┴──────┘
                    ↑
                    h[0], h[1], ..., h[capacity-1]
```

- **Open addressing**: 충돌이 발생하면 같은 배열의 다음 슬롯으로 probe합니다. 별도의 chain 리스트가 없으므로 캐시 친화적입니다.
- **Control bytes**: 각 슬롯의 상태(빈 슬롯 / 점유 / 토움스톤)를 추적합니다. `HT_CTRL_EMPTY = 0`이 빈 슬롯을 의미합니다.
- **Power-of-2 capacity**: 슬롯 수는 항상 2의 거듭제곱이며, 최소값은 `HT_MIN_CAP = 8`입니다. 이로써 해시 값에서 슬롯 인덱스를 빠른 비트 마스킹(`hash & (capacity - 1)`)으로 계산할 수 있습니다.
- **해시 함수**: 기본 해시는 xxHash 계열의 `_cstl_hash32`/`_cstl_hash64`로 구현되어 있어 분포 품질이 좋습니다.

### Rehash 정책

- **로드 팩터 임계값**: `7/8 = 87.5%` (`HT_LOAD_NUM / HT_LOAD_DEN`)
- `size`가 `capacity * 7/8`을 넘으면 자동으로 **capacity를 두 배로** 늘리며 rehash가 발생합니다.
- rehash가 일어나면 **모든 반복자/포인터가 무효화**됩니다.

### 평균 O(1)의 의미

이상적인 해시 분포에서는 슬롯당 평균 probe 횟수가 상수에 가깝습니다. 그러나 **악의적인 키 집합**이나 **잘못 설계된 해시 함수**로 인해 충돌이 집중되면 한 슬롯에 가까운 곳까지 길게 probe를 해야 해서 최악 **O(n)** 까지 떨어집니다. 사용자 정의 타입을 사용하는 경우에는 분포가 고른 해시 함수를 직접 제공하는 것이 좋습니다.

---

## 반복자 무효화

`UNORDERED_SET`의 반복자는 버킷 배열 슬롯을 가리키는 포인터이므로, **rehash**가 발생하면 모든 반복자가 무효화됩니다.

| 연산 | 무효화되는 반복자 |
|------|------------------|
| 모든 읽기 전용 연산 | 없음 |
| `insert` (rehash 없음) | 없음 |
| `insert` (rehash 발생) | **모든 반복자** |
| `erase` | 제거된 원소만 |
| `clear` | 모든 반복자 |
| `destroy` | 모든 반복자 |

> **주의**: 큰 데이터를 적재할 것이 예상된다면, 삽입 도중 반복자를 보관하지 않거나, 미리 충분히 큰 capacity를 가진 컨테이너를 별도로 준비하세요. (현재 OpenCSTL은 `reserve()`를 제공하지 않으므로 이 경우 다른 방식이 필요합니다.)

---

## 멤버 함수

### 생성자 / 소멸자

#### `new_unordered_set`

```c
UNORDERED_SET(TYPE) new_unordered_set(TYPE);
UNORDERED_SET(TYPE) new_unordered_set(TYPE, hash_fn);
```

지정된 해시 함수를 사용하는 빈 unordered set을 생성합니다. 초기 capacity는 `HT_MIN_CAP = 8`입니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 해시 집합의 핸들 |
| **복잡도** | 상수 **O(1)** |

```c
UNORDERED_SET(int) h = new_unordered_set(int);
printf("%zu\n", size(h));   // 0
destroy(h);
```

---

#### `destroy`

```c
void destroy(UNORDERED_SET(TYPE) h);
```

unordered set을 파괴하고 모든 내부 저장소를 해제합니다. 단일 버퍼이므로 빠릅니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **반복자 무효화** | 모든 반복자 |

---

### 원소 접근

#### `operator[]`

```c
TYPE h[i];
```

버킷 슬롯 `i`에 직접 접근합니다. 슬롯 값이 `0`이면 빈 슬롯입니다. `capacity`로 총 슬롯 개수를 알 수 있습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `0 <= i < capacity(h)` |

> **이것은 저수준 디버깅 도구입니다.** 일반적인 원소 접근에는 반복자 또는 `find`를 사용하세요. `h[i]`는 빈 슬롯과 점유 슬롯을 구분하지 않으므로 단독으로 사용하면 잘못된 데이터를 읽을 수 있습니다.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
for (int i = 1; i < 10; i++) insert(h, i);

size_t cap = capacity(h);
for (int i = 0; i < (int)cap; i++) {
    int k = h[i];
    if (k == 0) printf("[---]");
    else        printf("[%3d]", k);
}
puts("");
destroy(h);
```

---

### 반복자

`UNORDERED_SET`의 반복자 타입은 `TYPE*`이지만, 노드 기반이 아닌 **점유된 슬롯들을 건너뛰며 순회**하므로 포인터 산술(`it++`)은 동작하지 않습니다. 반드시 [`next()`](#next), [`prev()`](#prev)를 사용해야 합니다.

#### `begin`

```c
TYPE* begin(UNORDERED_SET(TYPE) h);
```

버킷 순회 순서상 첫 번째 원소를 가리키는 반복자를 반환합니다. 비어 있으면 `end(h)`와 같습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 최악 **O(capacity)** *(첫 점유 슬롯까지 스캔)*, 일반적으로 **O(1)** 에 가까움 |

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 3); insert(h, 1); insert(h, 4);

for (int *it = begin(h); it != end(h); it = next(it))
    printf("[%d]", *it);   // 순서: 버킷 순회 순 (정렬 X)
puts("");
destroy(h);
```

---

#### `end`

```c
TYPE* end(UNORDERED_SET(TYPE) h);
```

순방향 순회 종료를 나타내는 센티넬 반복자(`NULL`)를 반환합니다. **역참조 금지.**

---

#### `rbegin`

```c
TYPE* rbegin(UNORDERED_SET(TYPE) h);
```

버킷 순회 순서상 **마지막** 원소를 가리키는 역방향 반복자를 반환합니다. `prev()`로 역방향으로 이동합니다.

> **주의**: 이는 *역정렬* 순서가 아니라 *역버킷* 순서입니다. 즉, 같은 원소들을 두 번 순회해도 `begin`→`next`...와 `rbegin`→`prev`...는 정반대의 슬롯 순서로 방문합니다.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
for (int i = 1; i <= 5; i++) insert(h, i);

for (int *it = rbegin(h); it != rend(h); it = prev(it))
    printf("[%d]", *it);
puts("");
destroy(h);
```

---

#### `rend`

```c
TYPE* rend(UNORDERED_SET(TYPE) h);
```

역방향 순회 종료를 나타내는 센티넬(`NULL`). **역참조 금지.**

---

#### `next`

```c
TYPE* next(TYPE* iter);
```

버킷 순회 순서상 다음 **점유된** 슬롯의 반복자를 반환합니다(빈 슬롯은 건너뜀).

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized** *(빈 슬롯이 연속될 수 있어 최악은 더 길지만 전체 순회가 O(capacity)) |

```c
for (int *it = begin(h); it != end(h); it = next(it))
    printf("[%d]", *it);
```

---

#### `prev`

```c
TYPE* prev(TYPE* iter);
```

버킷 순회 순서상 이전 **점유된** 슬롯의 반복자를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized** |

```c
for (int *it = rbegin(h); it != rend(h); it = prev(it))
    printf("[%d]", *it);
```

---

### 용량(Capacity)

#### `empty`

```c
bool empty(UNORDERED_SET(TYPE) h);
```

집합에 원소가 하나도 없으면 `true`를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `size`

```c
size_t size(UNORDERED_SET(TYPE) h);
```

현재 저장된 유일한 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 10);
insert(h, 20);
insert(h, 10);   // 중복 — 무시됨
printf("%zu\n", size(h));   // 2
destroy(h);
```

---

#### `capacity`

```c
size_t capacity(UNORDERED_SET(TYPE) h);
```

현재 할당된 해시 버킷 슬롯의 총 개수를 반환합니다. 항상 `>= size(h)`이며 항상 **2의 거듭제곱**입니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
UNORDERED_SET(int) h = new_unordered_set(int);
for (int i = 1; i < 10; i++) insert(h, i);

printf("size:     %zu\n", size(h));      // 9
printf("capacity: %zu\n", capacity(h));  // 16 (2의 거듭제곱)
destroy(h);
```

> **참고**: `capacity * 7/8`을 넘으면 rehash가 발생하여 capacity가 두 배가 됩니다.

---

### 수정자(Modifiers)

#### `insert`

```c
void insert(UNORDERED_SET(TYPE) h, TYPE value);
```

`value`가 집합에 없으면 삽입합니다. 이미 존재하면 **조용히 무시**됩니다. 충돌이 있으면 open addressing으로 다음 슬롯들을 probe합니다. 로드 팩터가 임계값을 넘으면 rehash가 발생합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 평균 **O(1)**, rehash 발생 시 **O(n)** |
| **반복자 무효화** | rehash 발생 시 모든 반복자 |

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 100);
insert(h, 200);
insert(h, 100);   // 중복 — 효과 없음
printf("%zu\n", size(h));   // 2
destroy(h);
```

---

#### `erase`

```c
void erase(UNORDERED_SET(TYPE) h, TYPE* iter);
```

반복자가 가리키는 원소를 제거합니다. open addressing 구조에서는 단순히 슬롯을 비우는 것이 아니라 **토움스톤(tombstone)** 으로 표시하여 이후의 probe 체인이 끊기지 않도록 합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 평균 **O(1)** |
| **반복자 무효화** | 제거된 반복자만 |

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 1);
insert(h, 2);
insert(h, 3);

int *it = find(h, 2);
if (it != end(h))
    erase(h, it);

printf("size: %zu\n", size(h));   // 2
destroy(h);
```

---

#### `clear`

```c
void clear(UNORDERED_SET(TYPE) h);
```

모든 원소를 제거합니다. `size(h)`는 `0`이 되며, capacity는 유지됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(capacity)** *(control bytes 일괄 초기화)* |
| **반복자 무효화** | 모든 반복자 |

---

### 탐색(Lookup)

#### `find`

```c
TYPE* find(UNORDERED_SET(TYPE) h, TYPE value);
```

해시 함수로 `value`를 탐색합니다. 일치하는 원소의 반복자를 반환하며, 없으면 `end(h)`(즉 `NULL`)를 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 일치 원소의 반복자, 없으면 `end(h)` |
| **복잡도** | 평균 **O(1)**, 최악 **O(n)** |

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 10);
insert(h, 20);

int *it = find(h, 10);
if (it != end(h))
    printf("found: %d\n", *it);   // found: 10

it = find(h, 99);
if (it == end(h))
    printf("not found\n");

destroy(h);
```

---

## set과의 비교

| 항목 | `SET` | `UNORDERED_SET` |
|------|-------|-----------------|
| **내부 구조** | Red-Black Tree (노드 기반) | Flat hash table (open addressing) |
| **핸들 타입** | `TYPE**` | `TYPE*` |
| **정렬** | 키 오름차순 | 버킷 순회 순 (정렬 없음) |
| **탐색** | O(log n) | 평균 O(1), 최악 O(n) |
| **삽입/삭제** | O(log n) | 평균 O(1) |
| **반복자 안정성 (insert 시)** | 항상 안정 | rehash 시 무효화 |
| **`rbegin`/`rend`/`prev`** | 지원 (정렬 역순) | 지원 (버킷 역순) |
| **`capacity` / `operator[]`** | 미지원 | 지원 (버킷 직접 검사) |
| **최솟값/최댓값 O(1)** | `*begin` / `*rbegin` | 미지원 |
| **순회 비용** | O(n) (실원소 수) | O(capacity) (빈 슬롯 포함) |
| **메모리 효율** | 노드 오버헤드 큼 | 로드 팩터에 의해 결정 (최대 87.5%) |
| **캐시 친화성** | 나쁨 (포인터 추적) | 매우 좋음 (연속 배열) |
| **적합한 용도** | 정렬 순회, 범위 질의, 최소/최대 | 빠른 membership 테스트 |

---

## 참고 사항

- 핸들 타입은 `TYPE*`이지만, **포인터 산술(`h++`, `h+1`)을 직접 수행해서는 안 됩니다.** 이는 빈 슬롯을 무시하지 않으며 컨테이너 자체의 정체성을 깨뜨립니다. `next(it)`/`prev(it)`를 사용하세요.
- `h[i]`는 **저수준 슬롯 검사 도구**입니다. 빈 슬롯(`0`)과 토움스톤을 일반 원소와 구분하지 못하므로 디버깅 목적 외에는 사용하지 마세요.
- 해시 함수 인자는 **선택사항**입니다. `new_unordered_set(int)`만으로 동작합니다.
- 중복 삽입은 조용히 무시됩니다.
- `UNORDERED_SET`은 `front`/`back`을 제공하지 않으며, **최솟값/최댓값을 O(1)에 구하는 기능이 없습니다.** 필요하면 `SET`을 사용하세요.
- 사용자 정의 타입을 키로 쓸 때는 **분포가 고른 해시 함수**를 직접 제공하는 것이 강력히 권장됩니다. 잘못된 해시는 최악 O(n)을 유발할 수 있습니다.

### 사용자 정의 해시 함수

```c
size_t my_hash(const void *p) {
    int v = *(const int*)p;
    return (size_t)(v * 2654435761u);   // Knuth's multiplicative hash
}

UNORDERED_SET(int) h = new_unordered_set(int, my_hash);
```

---

## 예제

```c
#include "opencstl.h"

int main() {
    UNORDERED_SET(int) h = new_unordered_set(int);

    for (int i = 1; i < 100; i++)
        insert(h, i);

    // 내부 버킷 레이아웃 검사
    size_t cap = capacity(h);
    int occupied = 0;
    for (int i = 0; i < (int)cap; i++) {
        int k = h[i];
        if (k == 0) printf("[---]");
        else        { printf("[%3d]", k); occupied++; }
    }
    puts("");

    printf("size:        %zu\n", size(h));
    printf("capacity:    %zu\n", cap);
    printf("load factor: %.2f\n", (double)size(h) / cap);

    // 역방향 순회 (버킷 역순)
    printf("Reverse:\n");
    for (int *it = rbegin(h); it != rend(h); it = prev(it))
        printf("[%3d]", *it);
    puts("");

    // membership 테스트
    int *it = find(h, 42);
    printf("contains 42: %s\n", (it != end(h)) ? "yes" : "no");

    // 한 원소 제거
    it = find(h, 50);
    if (it != end(h)) erase(h, it);
    printf("size after erase: %zu\n", size(h));

    destroy(h);
    return 0;
}
```

---

## 함수 요약표

### 생성자 / 소멸자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_unordered_set` | `UNORDERED_SET(T) new_unordered_set(T [, hash])` | O(1) | 빈 해시 집합 생성 |
| `destroy`           | `void destroy(h)`                                | O(1) | 해시 집합 파괴 |

### 원소 접근

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `operator[]` | `h[i]` | O(1) | 버킷 슬롯 `i` 직접 접근 (저수준) |

### 반복자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `begin`  | `T* begin(h)`   | O(capacity) 최악 | 버킷 순서상 첫 원소 |
| `end`    | `T* end(h)`     | O(1)   | 끝 센티넬 (NULL) |
| `rbegin` | `T* rbegin(h)`  | O(capacity) 최악 | 버킷 순서상 마지막 원소 |
| `rend`   | `T* rend(h)`    | O(1)   | 역방향 끝 센티넬 (NULL) |
| `next`   | `T* next(it)`   | O(1)*  | 다음 점유 슬롯 |
| `prev`   | `T* prev(it)`   | O(1)*  | 이전 점유 슬롯 |

\* 분할 상환 상수

### 용량(Capacity)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `empty`    | `bool empty(h)`      | O(1) | 비었는지 검사 |
| `size`     | `size_t size(h)`     | O(1) | 원소 개수 |
| `capacity` | `size_t capacity(h)` | O(1) | 버킷 슬롯 총 개수 (2의 거듭제곱) |

### 수정자(Modifiers)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `insert` | `void insert(h, val)` | O(1) 평균 | 원소 삽입 (중복 무시) |
| `erase`  | `void erase(h, it)`   | O(1) 평균 | 반복자가 가리키는 원소 제거 |
| `clear`  | `void clear(h)`       | O(capacity) | 모든 원소 제거 (capacity 유지) |

### 탐색(Lookup)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `find` | `T* find(h, val)` | O(1) 평균, O(n) 최악 | 원소 탐색, 없으면 `end(h)` |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`SET`](./set.md)                           | 정렬된 유일 키 집합. O(log n) 연산, 정렬 순회 |
| [`UNORDERED_MAP`](./unordered_map.md)       | 해시 기반 키-값 저장소 |
| [`MAP`](./map.md)                           | 정렬된 키-값 저장소 |
| [`VECTOR`](./vector.md)                     | 연속 시퀀스. `qsort`로 직접 정렬 가능 |
