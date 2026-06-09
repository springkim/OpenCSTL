# set

헤더 [`set.h`](../opencstl/set.h) (내부적으로 [`rbtree.h`](../opencstl/rbtree.h))에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
#define SET(TYPE)                  TYPE**
#define new_set(TYPE, comparator)  cstl_set(TYPE, comparator)
```

`SET`은 **유일한(unique) 원소**를 **정렬된 순서**로 저장하는 연관 컨테이너입니다.
C++ 표준 라이브러리의 [`std::set`](https://cppreference.com/cpp/container/set)에 대응되는 OpenCSTL의 구현체이며, 정렬 기준은 생성 시 제공하는 비교 함수(comparator)에 의해 결정됩니다. 동일한 비교 기준으로 같다고 판정되는 두 원소는 동시에 존재할 수 없습니다.

내부적으로 `SET`은 **자가 균형 이진 탐색 트리(Red-Black Tree)** 로 구현됩니다. 따라서 삽입, 삭제, 탐색이 모두 **O(log n)** 시간에 수행되며, 순회는 항상 정렬된 오름차순으로 진행됩니다. 노드 할당은 효율을 위해 **arena allocator**(청크 단위 풀)에서 수행됩니다.

`SET`은 `[]` 첨자 접근을 지원하지 **않습니다**. 순회는 반드시 반복자와 [`next()`](#next)/[`prev()`](#prev) 함수로 수행해야 합니다.

`set`의 주요 연산 복잡도는 다음과 같습니다.

- 탐색, 삽입, 제거 — **O(log n)**
- 중위(in-order) 순회 — **O(n)**
- 최솟값/최댓값 접근 — **O(1)** *(begin / rbegin)*

---

## 목차

1. [매크로 매개변수](#매크로-매개변수)
2. [멤버 타입](#멤버-타입)
3. [내부 구조: Red-Black Tree](#내부-구조-red-black-tree)
4. [반복자 무효화](#반복자-무효화)
5. [멤버 함수](#멤버-함수)
   - [생성자 / 소멸자](#생성자--소멸자)
   - [반복자](#반복자)
   - [용량(Capacity)](#용량capacity)
   - [수정자(Modifiers)](#수정자modifiers)
   - [탐색(Lookup)](#탐색lookup)
6. [참고 사항](#참고-사항)
7. [예제](#예제)
8. [함수 요약표](#함수-요약표)
9. [관련 항목](#관련-항목)

---

## 매크로 매개변수

### `SET(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 집합에 저장할 원소의 타입 |

`TYPE**`으로 확장됩니다. `LIST`, `MAP`과 같이 **노드 기반 컨테이너의 공통 핸들 타입**입니다.

```c
SET(int)   s;   // 확장 결과: int**  s;
SET(char*) ss;  // 확장 결과: char*** ss;
```

### `new_set(TYPE, comparator)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE` | 원소의 타입 |
| `comparator` | 비교 함수 `int (*)(const void*, const void*)`. `qsort`와 동일한 규약. `NULL`이면 해당 타입의 기본 비교자가 사용됩니다. |

```c
SET(int) s = new_set(int, NULL);          // 기본 비교자 (오름차순)
SET(int) s = new_set(int, COMPARE(int));  // 명시적 오름차순 비교자
```

비교 함수는 다음 규약을 따라야 합니다.

- `a < b` → 음수
- `a == b` → 0 (이 경우 두 원소는 "같다"고 간주되며 중복으로 처리됨)
- `a > b` → 양수

---

## 멤버 타입

| 개념 (C++ 표준) | OpenCSTL 대응 |
|-----------------|--------------|
| `key_type` / `value_type` | `TYPE` |
| `key_compare`             | 생성자에 전달한 `int (*)(const void*, const void*)` |
| `reference` / `pointer`   | 노드 데이터에 대한 `TYPE*` |
| `iterator`                | `TYPE*` — 노드 데이터 시작을 가리키는 포인터 |
| `reverse_iterator`        | `TYPE*` — `prev()`로 역방향 이동 |
| `size_type`               | `size_t` |

> **반복자의 본질**: 반복자는 노드의 데이터 시작점을 가리키는 포인터입니다. 노드 헤더(`color`, `parent`, `left`, `right`, `node_type`)는 데이터 앞쪽 음수 오프셋에 저장되어 있으며, `next()`/`prev()`가 트리 구조를 따라 in-order 다음/이전 노드로 이동합니다. **`it++` / `it--`는 절대 사용하지 마세요.**

---

## 내부 구조: Red-Black Tree

`SET`은 다음 5가지 속성을 만족하는 Red-Black Tree로 구현됩니다.

1. 모든 노드는 빨강 또는 검정 중 하나입니다.
2. 루트는 항상 검정입니다.
3. 모든 NIL 리프(센티넬)는 검정입니다.
4. 빨강 노드의 자식은 모두 검정입니다.
5. 임의의 노드에서 자손 NIL까지의 모든 경로에서 검정 노드의 개수는 같습니다.

이 속성들에 의해 트리의 높이는 항상 `O(log n)`으로 유지됩니다.

각 노드의 메모리 레이아웃은 다음과 같습니다.

```
저주소 ─────────────────────────────────────→ 고주소
┌───────┬────────┬───────────┬──────┬───────┬─────────┐
│ color │ parent │ node_type │ left │ right │  data   │
└───────┴────────┴───────────┴──────┴───────┴─────────┘
                                            ↑
                                            반복자가 가리키는 위치
```

- `color`: RED(1) 또는 BLACK(0)
- `parent`, `left`, `right`: 인접 노드의 데이터 시작점 포인터
- `node_type`: 컨테이너 종류 식별자 (`OPENCSTL_SET` 등)
- `data`: 실제 원소 데이터 (`TYPE` 크기)

### Arena Allocator

노드 할당은 한 번에 한 노드를 `malloc`하는 것이 아니라, **`CSTL_ARENA_CHUNK_SIZE`(=256)** 개의 노드를 한 번에 받아오는 **arena chunk** 단위로 이루어집니다. 이로써:

- `malloc` 호출 횟수가 `n` → `n/256`으로 줄어듭니다.
- 노드들이 메모리상 인접해 캐시 친화성이 향상됩니다.
- `destroy` 시 모든 chunk를 일괄 해제할 수 있어 빠릅니다.

---

## 반복자 무효화

`SET`은 노드 기반 컨테이너이므로, 다른 노드의 삽입과 삭제는 기존 반복자를 무효화하지 **않습니다**.

| 연산 | 무효화되는 반복자 |
|------|------------------|
| 모든 읽기 전용 연산 | 없음 |
| `insert` | 없음 (기존 반복자 모두 유효) |
| `erase` | 제거된 원소를 가리키는 반복자만 |
| `clear` | 모든 반복자 |
| `destroy` | 모든 반복자 |

> **활용**: 큰 트리에서 특정 원소를 가리키는 반복자를 외부에 보관해 두고, 다른 원소들을 자유롭게 삽입/삭제하는 패턴이 안전합니다.

---

## 멤버 함수

### 생성자 / 소멸자

#### `new_set`

```c
SET(TYPE) new_set(TYPE, comparator);
```

지정된 비교 함수를 사용하는 빈 집합을 생성합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 집합의 핸들 |
| **복잡도** | 상수 **O(1)** |

```c
SET(int) s = new_set(int, NULL);
printf("%zu\n", size(s));   // 0
destroy(s);
```

---

#### `destroy`

```c
void destroy(SET(TYPE) s);
```

집합을 파괴하고 모든 노드 및 내부 자료구조를 해제합니다. arena chunk 전체를 일괄 free하므로 빠릅니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | arena chunk 수에 선형 **O(n/256)** ≈ O(n) (개념적으로는 선형) |
| **반복자 무효화** | 모든 반복자 |

---

### 반복자

`SET`의 반복자 타입은 `TYPE*`이지만, 노드 기반 자료구조이므로 포인터 산술(`it++`, `it--`)은 **동작하지 않습니다**. 반드시 [`next()`](#next), [`prev()`](#prev)를 사용해야 합니다.

#### `begin`

```c
TYPE* begin(SET(TYPE) s);
```

가장 작은 원소(in-order 첫 노드)를 가리키는 반복자를 반환합니다. 비어 있으면 `end(s)`와 같습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(트리 시작 노드 포인터를 캐싱)* |

```c
SET(int) s = new_set(int, NULL);
insert(s, 5);
insert(s, 2);
insert(s, 8);

for (int *it = begin(s); it != end(s); it = next(it))
    printf("[%3d]", *it);   // [  2][  5][  8]
puts("");
destroy(s);
```

---

#### `end`

```c
TYPE* end(SET(TYPE) s);
```

가장 큰 원소 **다음** 위치를 나타내는 센티넬 반복자를 반환합니다. OpenCSTL에서는 `NULL`로 정의됩니다. **역참조해서는 안 됩니다.**

---

#### `rbegin`

```c
TYPE* rbegin(SET(TYPE) s);
```

가장 큰 원소(in-order 마지막 노드)를 가리키는 역방향 반복자를 반환합니다.

```c
SET(int) s = new_set(int, NULL);
insert(s, 5);
insert(s, 2);
insert(s, 8);

for (int *it = rbegin(s); it != rend(s); it = prev(it))
    printf("[%3d]", *it);   // [  8][  5][  2]
puts("");
destroy(s);
```

---

#### `rend`

```c
TYPE* rend(SET(TYPE) s);
```

가장 작은 원소 **이전** 위치의 역방향 센티넬 반복자를 반환합니다. **역참조해서는 안 됩니다.**

---

#### `next`

```c
TYPE* next(TYPE* iter);
```

정렬 순서상 다음 원소로 진행한 반복자를 반환합니다. 트리 구조를 따라 in-order 후속 노드로 이동합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized**, 최악 **O(log n)** |

> **내부 동작**: 현재 노드의 오른쪽 서브트리가 있으면 그 서브트리의 최좌측 노드로, 없으면 부모 체인을 따라 올라가다가 "올라온 방향이 왼쪽 자식인" 가장 가까운 조상으로 이동합니다.

```c
for (int *it = begin(s); it != end(s); it = next(it))
    printf("%d\n", *it);
```

---

#### `prev`

```c
TYPE* prev(TYPE* iter);
```

정렬 순서상 이전 원소로 이동한 반복자를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized**, 최악 **O(log n)** |

```c
for (int *it = rbegin(s); it != rend(s); it = prev(it))
    printf("%d\n", *it);
```

---

### 용량(Capacity)

#### `empty`

```c
bool empty(SET(TYPE) s);
```

집합에 원소가 하나도 없으면 `true`를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
SET(int) s = new_set(int, NULL);
printf("%d\n", empty(s));   // 1
insert(s, 42);
printf("%d\n", empty(s));   // 0
destroy(s);
```

---

#### `size`

```c
size_t size(SET(TYPE) s);
```

집합에 저장된 유일한 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(내부 카운터 유지)* |

```c
SET(int) s = new_set(int, NULL);
insert(s, 1);
insert(s, 2);
insert(s, 2);   // 중복 — 무시됨
printf("%zu\n", size(s));   // 2
destroy(s);
```

---

### 수정자(Modifiers)

#### `insert`

```c
void insert(SET(TYPE) s, TYPE value);
```

`value`가 집합에 없을 때만 삽입합니다. 이미 같은 값(비교 함수 기준 0을 반환)이 존재하면 **조용히 무시**되어 기존 원소는 그대로 유지됩니다. 삽입 후 RB-tree 속성을 유지하기 위해 색깔 변경과 회전(`left_rotate`/`right_rotate`)이 수행됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(log n)** |
| **반복자 무효화** | 없음 |

```c
SET(int) s = new_set(int, NULL);
insert(s, 3);
insert(s, 1);
insert(s, 4);
insert(s, 1);   // 중복 — 무시됨
insert(s, 5);

for (int *it = begin(s); it != end(s); it = next(it))
    printf("[%3d]", *it);   // [  1][  3][  4][  5]
puts("");
destroy(s);
```

---

#### `erase`

```c
void erase(SET(TYPE) s, TYPE* iter);
```

`iter`가 가리키는 원소를 제거합니다. 삭제 후 RB-tree 속성을 유지하기 위해 `erase_fixup`이 호출되어 회전과 색깔 조정이 수행됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(log n)** |
| **반복자 무효화** | 제거된 반복자만 |

```c
SET(int) s = new_set(int, NULL);
insert(s, 1);
insert(s, 2);
insert(s, 3);

int *it = find(s, 2);
if (it != end(s))
    erase(s, it);   // 2 제거

for (int *p = begin(s); p != end(s); p = next(p))
    printf("[%3d]", *p);   // [  1][  3]
puts("");
destroy(s);
```

> **팁**: 키 값으로 바로 지우려면 `find` + `erase`를 조합하세요. 키가 없을 수 있는 경우 항상 `it != end(s)`를 확인해야 합니다.

---

#### `clear`

```c
void clear(SET(TYPE) s);
```

모든 원소를 제거합니다. `size(s)`는 `0`이 됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 원소 수에 선형 **O(n)** |
| **반복자 무효화** | 모든 반복자 |

---

### 탐색(Lookup)

#### `find`

```c
TYPE* find(SET(TYPE) s, TYPE value);
```

트리 구조를 이용해 `value`를 탐색합니다. 일치하는 원소의 반복자를 반환하며, 없으면 `end(s)`(즉 `NULL`)를 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 일치하는 원소의 반복자, 없으면 `end(s)` |
| **복잡도** | **O(log n)** |

```c
SET(int) s = new_set(int, NULL);
insert(s, 10);
insert(s, 20);
insert(s, 30);

int *it = find(s, 20);
if (it != end(s))
    printf("found: %d\n", *it);   // found: 20

it = find(s, 99);
if (it == end(s))
    printf("not found\n");         // not found

destroy(s);
```

---

## 참고 사항

- `SET`의 핸들 타입은 `TYPE**`(노드 기반)입니다. `LIST`/`MAP`과 동일합니다.
- 반복자에 **`it++` / `it--`를 사용하지 마세요.** 반드시 `next(it)`, `prev(it)`를 사용합니다.
- **중복 삽입은 조용히 무시됩니다.** 중복 허용이 필요하면 `multiset`이 별도로 존재하는 C++과 달리 OpenCSTL은 `multiset`을 제공하지 않으므로, 카운트가 필요하면 `MAP(KEY, int)`로 대체할 수 있습니다.
- 원소는 항상 **오름차순**으로 순회됩니다(`begin` → `end`). 내림차순은 `rbegin` → `rend`를 사용합니다.
- `SET`은 `front`/`back`을 제공하지 않습니다. 최솟값과 최댓값은 `*begin(s)`, `*rbegin(s)`로 접근하세요.
- 비교자가 `NULL`이면 기본 비교자 `COMPARE(TYPE)`이 사용됩니다. 문자열의 경우 `strcmp` 기반 비교자가 자동 선택됩니다.
- 사용자 정의 구조체 타입의 경우 반드시 명시적 `comparator`를 전달해야 합니다.

### 사용자 정의 비교 함수

```c
typedef struct { int x, y; } Point;

int point_cmp(const void *a, const void *b) {
    const Point *pa = (const Point*)a;
    const Point *pb = (const Point*)b;
    if (pa->x != pb->x) return pa->x - pb->x;
    return pa->y - pb->y;
}

SET(Point) s = new_set(Point, point_cmp);
```

---

## 예제

```c
#include "opencstl.h"

int main() {
    SET(int) s = new_set(int, NULL);

    // 중복 포함 삽입 — 중복은 자동으로 무시됨
    srand(42);
    for (int i = 0; i < 10; i++)
        insert(s, rand() % 100);

    printf("size: %zu\n", size(s));

    // 오름차순 순회
    printf("ascending:\n");
    for (int *it = begin(s); it != end(s); it = next(it))
        printf("[%3d]", *it);
    puts("");

    // 내림차순 순회
    printf("descending:\n");
    for (int *it = rbegin(s); it != rend(s); it = prev(it))
        printf("[%3d]", *it);
    puts("");

    // 최솟값 탐색 후 제거
    int *it = find(s, *begin(s));
    if (it != end(s)) {
        printf("erasing min: %d\n", *it);
        erase(s, it);
    }

    printf("size after erase: %zu\n", size(s));

    destroy(s);
    return 0;
}
```

---

## 함수 요약표

### 생성자 / 소멸자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_set` | `SET(T) new_set(T, comp)` | O(1)  | 빈 집합 생성 |
| `destroy` | `void destroy(s)`          | O(n)  | 집합 파괴 및 메모리 해제 |

### 반복자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `begin`  | `T* begin(s)`   | O(1)        | 최솟값 반복자 |
| `end`    | `T* end(s)`     | O(1)        | 끝 센티넬 (NULL) |
| `rbegin` | `T* rbegin(s)`  | O(1)        | 최댓값 반복자 |
| `rend`   | `T* rend(s)`    | O(1)        | 역방향 끝 센티넬 (NULL) |
| `next`   | `T* next(it)`   | O(1)*       | in-order 다음 원소 |
| `prev`   | `T* prev(it)`   | O(1)*       | in-order 이전 원소 |

\* 분할 상환 상수, 최악 O(log n)

### 용량(Capacity)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `empty` | `bool empty(s)`     | O(1) | 비었는지 검사 |
| `size`  | `size_t size(s)`    | O(1) | 원소 개수 |

### 수정자(Modifiers)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `insert` | `void insert(s, val)`     | O(log n) | 원소 삽입 (중복은 무시) |
| `erase`  | `void erase(s, it)`       | O(log n) | 반복자가 가리키는 원소 제거 |
| `clear`  | `void clear(s)`           | O(n)     | 모든 원소 제거 |

### 탐색(Lookup)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `find` | `T* find(s, val)` | O(log n) | 원소 탐색, 없으면 `end(s)` |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`MAP`](./map.md)                           | 정렬된 키-값 저장소. `SET`과 동일한 RB-tree 구조 사용 |
| [`UNORDERED_SET`](./__unordered_set.md)     | 해시 기반 유일 키 집합. 평균 O(1), 정렬 없음 |
| [`UNORDERED_MAP`](./__unordered_map.md)     | 해시 기반 키-값 저장소. 평균 O(1) |
| [`VECTOR`](./vector.md)                     | 정렬되지 않은 시퀀스. `qsort`+`bsearch` 조합으로 정렬 접근 가능 |
| [`PRIORITY_QUEUE`](./__priority_queue.md)   | 힙. O(1) 최댓값 접근, 전체 순회 불가 |
