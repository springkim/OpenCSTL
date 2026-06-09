# map

헤더 [`map.h`](../opencstl/map.h) (내부적으로 [`rbtree.h`](../opencstl/rbtree.h))에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
#define MAP(KEY_TYPE)                               KEY_TYPE**
#define new_map(KEY_TYPE, VALUE_TYPE, comparator)   cstl_map(KEY_TYPE, VALUE_TYPE, comparator)
```

`MAP`은 **키-값 쌍(key-value pair)** 을 **키 정렬 순서**로 저장하는 연관 컨테이너입니다.
C++ 표준 라이브러리의 [`std::map`](https://cppreference.com/cpp/container/map)에 대응되는 OpenCSTL의 구현체이며, 각 키는 유일해야 합니다. 정렬 및 탐색은 생성 시 제공한 비교 함수에 의해 결정됩니다.

내부적으로 `MAP`은 [`SET`](./set.md)과 동일한 **Red-Black Tree** 구조로 구현됩니다. 노드 데이터 영역에 키와 값이 **연속해서** 저장된다는 점만 다릅니다. 따라서 모든 키 기반 연산이 **O(log n)** 시간에 수행됩니다. 노드 할당은 **arena allocator**(청크 단위 풀)에서 수행됩니다.

`MAP`은 `[]` 첨자 접근을 지원하지 **않습니다**(C++의 `operator[]` 대응 없음). 순회는 반복자와 [`next()`](#next)/[`prev()`](#prev)로, 값 접근은 [`first()`](#first)/[`second()`](#second)로 수행합니다.

`map`의 주요 연산 복잡도는 다음과 같습니다.

- 키 탐색 — **O(log n)**
- 삽입, 제거 — **O(log n)**
- 중위(in-order) 순회 — **O(n)**
- 최소/최대 키 접근 — **O(1)** *(begin / rbegin)*

---

## 목차

1. [매크로 매개변수](#매크로-매개변수)
2. [멤버 타입](#멤버-타입)
3. [노드 메모리 레이아웃](#노드-메모리-레이아웃)
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

### `MAP(KEY_TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `KEY_TYPE` | 맵의 키 타입 |

`KEY_TYPE**`으로 확장됩니다. **값 타입은 핸들 타입에 포함되지 않으며**, 생성 시점에 내부에 기록됩니다.

```c
MAP(int)   m;   // 확장 결과: int**  m;   (값은 임의 타입 가능)
MAP(char*) sm;  // 확장 결과: char*** sm;
```

> **주의**: 핸들 타입이 키 타입만을 반영하므로, 같은 키 타입을 가진 두 맵이 서로 다른 값 타입을 가질 수 있습니다. 이 경우 두 맵을 구별할 수 있도록 코드 컨텍스트에서 명확히 관리해야 합니다.

### `new_map(KEY_TYPE, VALUE_TYPE, comparator)`

| 매개변수 | 설명 |
|----------|-----|
| `KEY_TYPE` | 키 타입 |
| `VALUE_TYPE` | 키와 연관되는 값 타입 |
| `comparator` | 비교 함수 `int (*)(const void*, const void*)`. `qsort`와 동일 규약. `NULL`이면 기본 비교자가 사용됨. |

```c
MAP(int) m = new_map(int, char*, NULL);
MAP(int) m = new_map(int, char*, COMPARE(int));
```

비교 함수는 다음 규약을 따라야 합니다.

- `a < b` → 음수
- `a == b` → 0 (이미 같은 키로 간주되어 중복 삽입은 무시됨)
- `a > b` → 양수

---

## 멤버 타입

| 개념 (C++ 표준) | OpenCSTL 대응 |
|-----------------|--------------|
| `key_type`                | `KEY_TYPE` |
| `mapped_type`             | `VALUE_TYPE` (생성 시 캡처) |
| `value_type`              | C++의 `pair<KEY,VALUE>`에 대응; OpenCSTL은 노드에 키+값을 연속 저장 |
| `key_compare`             | 생성자에 전달한 `int (*)(const void*, const void*)` |
| `iterator`                | `KEY_TYPE*` — 노드 데이터(키) 시작을 가리키는 포인터 |
| `reverse_iterator`        | `KEY_TYPE*` — `prev()`로 역방향 이동 |
| `size_type`               | `size_t` |

> **반복자의 본질**: 반복자는 노드의 **키 시작점**을 가리키는 포인터입니다. 값은 키 바로 뒤에 연속 저장되어 있으므로 `second(iter, VALUE_TYPE)` 매크로가 그 위치를 읽어옵니다. **`it++` / `it--`는 사용하지 마세요.**

---

## 노드 메모리 레이아웃

`MAP`의 각 노드는 Red-Black Tree 헤더 뒤에 **키와 값을 연속해서** 저장합니다.

```
저주소 ────────────────────────────────────────────→ 고주소
┌───────┬────────┬───────────┬──────┬───────┬─────────┬─────────┐
│ color │ parent │ node_type │ left │ right │   key   │  value  │
└───────┴────────┴───────────┴──────┴───────┴─────────┴─────────┘
                                            ↑
                                            반복자가 가리키는 위치
                                            (KEY_TYPE*)
```

- `color`, `parent`, `left`, `right`, `node_type`: RB-tree 메타데이터 (음수 오프셋)
- `key`: 키 데이터 (`KEY_TYPE` 크기) — 반복자가 가리키는 시작점
- `value`: 값 데이터 (`VALUE_TYPE` 크기) — 키 바로 뒤

이 레이아웃 덕분에:

- `first(iter)`는 `*iter`로 단순 접근됩니다.
- `second(iter, VALUE_TYPE)`는 `*(VALUE_TYPE*)((char*)iter + sizeof(KEY_TYPE))`로 접근됩니다.
- 비교는 노드 데이터의 **키 부분만**(`key_size` 만큼) 사용하므로, 같은 키에 다른 값이 들어와도 중복으로 정확히 판정됩니다.

---

## 반복자 무효화

`MAP` 반복자는 노드 기반이므로 다른 노드의 삽입/삭제는 기존 반복자를 무효화하지 **않습니다**.

| 연산 | 무효화되는 반복자 |
|------|------------------|
| 모든 읽기 전용 연산 | 없음 |
| `insert` | 없음 |
| `erase` | 제거된 원소를 가리키는 반복자만 |
| `clear` | 모든 반복자 |
| `destroy` | 모든 반복자 |

---

## 멤버 함수

### 생성자 / 소멸자

#### `new_map`

```c
MAP(KEY_TYPE) new_map(KEY_TYPE, VALUE_TYPE, comparator);
```

빈 맵을 생성합니다. 값 타입은 생성 시점에 캡처되어 노드 메모리 레이아웃 결정에 사용됩니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 맵의 핸들 |
| **복잡도** | 상수 **O(1)** |

```c
MAP(int) m = new_map(int, char*, NULL);
printf("%zu\n", size(m));   // 0
destroy(m);
```

---

#### `destroy`

```c
void destroy(MAP(KEY_TYPE) m);
```

맵을 파괴하고 모든 노드 및 내부 자료구조를 해제합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | O(n) (arena chunk 일괄 해제) |
| **반복자 무효화** | 모든 반복자 |

---

### 원소 접근

#### `first`

```c
KEY_TYPE first(KEY_TYPE* iter);
```

반복자가 가리키는 키-값 쌍의 **키**를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 42, "hello");

int *it = begin(m);
printf("key: %d\n", first(it));   // key: 42
destroy(m);
```

---

#### `second`

```c
VALUE_TYPE second(KEY_TYPE* iter, VALUE_TYPE);
```

반복자가 가리키는 키-값 쌍의 **값**을 반환합니다. C에는 컴파일 타임 타입 추론이 없으므로 `VALUE_TYPE`을 두 번째 인자로 명시해야 합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 1, "apple");
insert(m, 2, "banana");

for (int *it = begin(m); it != end(m); it = next(it))
    printf("%d: %s\n", first(it), second(it, char*));

destroy(m);
```

> **주의**: `second`에 잘못된 `VALUE_TYPE`을 전달하면 노드 메모리를 잘못 해석하게 되어 **정의되지 않은 동작(UB)** 입니다. `new_map`에 전달했던 타입과 정확히 일치해야 합니다.

---

### 반복자

`MAP`의 반복자 타입은 `KEY_TYPE*`이지만, 노드 기반이므로 포인터 산술은 동작하지 않습니다. 반드시 [`next()`](#next), [`prev()`](#prev)를 사용해야 합니다.

#### `begin`

```c
KEY_TYPE* begin(MAP(KEY_TYPE) m);
```

가장 작은 키를 가진 항목의 반복자를 반환합니다.

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 3, "three");
insert(m, 1, "one");
insert(m, 2, "two");

for (int *it = begin(m); it != end(m); it = next(it))
    printf("%d: %s\n", first(it), second(it, char*));
// 1: one
// 2: two
// 3: three
destroy(m);
```

---

#### `end`

```c
KEY_TYPE* end(MAP(KEY_TYPE) m);
```

가장 큰 키 **다음** 위치의 센티넬 반복자를 반환합니다(`NULL`). 역참조 금지.

---

#### `rbegin`

```c
KEY_TYPE* rbegin(MAP(KEY_TYPE) m);
```

가장 큰 키를 가진 항목의 반복자를 반환합니다.

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 1, "one");
insert(m, 2, "two");
insert(m, 3, "three");

for (int *it = rbegin(m); it != rend(m); it = prev(it))
    printf("%d: %s\n", first(it), second(it, char*));
// 3: three
// 2: two
// 1: one
destroy(m);
```

---

#### `rend`

```c
KEY_TYPE* rend(MAP(KEY_TYPE) m);
```

가장 작은 키 **이전** 위치의 역방향 센티넬 반복자(`NULL`). 역참조 금지.

---

#### `next`

```c
KEY_TYPE* next(KEY_TYPE* iter);
```

키 오름차순 다음 원소로 진행합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized**, 최악 **O(log n)** |

---

#### `prev`

```c
KEY_TYPE* prev(KEY_TYPE* iter);
```

키 오름차순 이전 원소로(즉 내림차순 기준 다음 원소로) 이동합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized**, 최악 **O(log n)** |

---

### 용량(Capacity)

#### `empty`

```c
bool empty(MAP(KEY_TYPE) m);
```

맵에 키-값 쌍이 하나도 없으면 `true`를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
MAP(int) m = new_map(int, int, NULL);
printf("%d\n", empty(m));   // 1
insert(m, 1, 100);
printf("%d\n", empty(m));   // 0
destroy(m);
```

---

#### `size`

```c
size_t size(MAP(KEY_TYPE) m);
```

맵에 현재 저장된 키-값 쌍의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(내부 카운터 유지)* |

```c
MAP(int) m = new_map(int, int, NULL);
insert(m, 1, 10);
insert(m, 2, 20);
printf("%zu\n", size(m));   // 2
destroy(m);
```

---

### 수정자(Modifiers)

#### `insert`

```c
void insert(MAP(KEY_TYPE) m, KEY_TYPE key, VALUE_TYPE value);
```

키-값 쌍을 삽입합니다. **이미 같은 키가 존재하면 기존 항목은 변경되지 않고 삽입 자체가 무시됩니다**(no overwrite).

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(log n)** |
| **반복자 무효화** | 없음 |

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 0, "zero");
insert(m, 1, "one");
insert(m, 0, "ZERO");   // 중복 키 — 무시됨

for (int *it = begin(m); it != end(m); it = next(it))
    printf("%d: %s\n", first(it), second(it, char*));
// 0: zero    (덮어쓰지 않음)
// 1: one
destroy(m);
```

> **값을 덮어쓰려면**(C++의 `insert_or_assign`에 해당하는 동작): 현재 OpenCSTL은 별도 함수를 제공하지 않으므로 다음 패턴을 사용합니다.
>
> ```c
> int *it = find(m, key);
> if (it != end(m)) erase(m, it);
> insert(m, key, new_value);
> ```

---

#### `erase`

```c
void erase(MAP(KEY_TYPE) m, KEY_TYPE* iter);
```

반복자가 가리키는 키-값 쌍을 제거합니다. RB-tree 속성 유지를 위해 `erase_fixup`이 호출됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(log n)** |
| **반복자 무효화** | 제거된 반복자만 |

```c
MAP(int) m = new_map(int, int, NULL);
insert(m, 1, 10);
insert(m, 2, 20);
insert(m, 3, 30);

int *it = find(m, 2);
if (it != end(m))
    erase(m, it);

for (int *p = begin(m); p != end(m); p = next(p))
    printf("%d: %d\n", first(p), second(p, int));
// 1: 10
// 3: 30
destroy(m);
```

---

#### `clear`

```c
void clear(MAP(KEY_TYPE) m);
```

모든 키-값 쌍을 제거합니다. `size(m)`는 `0`이 됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 원소 수에 선형 **O(n)** |
| **반복자 무효화** | 모든 반복자 |

---

### 탐색(Lookup)

#### `find`

```c
KEY_TYPE* find(MAP(KEY_TYPE) m, KEY_TYPE key);
```

트리 구조를 이용해 `key`를 탐색합니다. 일치하는 항목의 반복자를 반환하며, 없으면 `end(m)`을 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 일치하는 항목의 반복자, 없으면 `end(m)`(NULL) |
| **복잡도** | **O(log n)** |

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 1, "apple");
insert(m, 2, "banana");

int *it = find(m, 1);
if (it != end(m))
    printf("found: %d -> %s\n", first(it), second(it, char*));
// found: 1 -> apple

it = find(m, 99);
if (it == end(m))
    printf("key 99 not found\n");

destroy(m);
```

---

## set과의 비교

| 항목 | `SET` | `MAP` |
|------|-------|-------|
| **저장 단위** | 키만 | 키 + 값 |
| **핸들 타입** | `TYPE**` | `KEY_TYPE**` |
| **내부 구조** | Red-Black Tree | Red-Black Tree (동일) |
| **노드 데이터 영역** | `[key]` | `[key][value]` (연속) |
| **삽입** | `insert(s, val)` | `insert(m, key, val)` |
| **반복자 역참조** | `*it`로 값 직접 접근 | `first(it)`, `second(it, VALUE_TYPE)` |
| **삽입/삭제/탐색 복잡도** | O(log n) | O(log n) |
| **순회 순서** | 키 오름차순 | 키 오름차순 |

> **재사용된 구조**: `SET`과 `MAP`은 사실상 같은 코드(`__cstl_tree_*` 시리즈)를 공유합니다. 차이는 노드 데이터 영역에 값(value)을 추가로 저장하는지 여부뿐입니다. 이 덕분에 두 컨테이너의 메모리 효율과 성능 특성이 거의 동일합니다.

---

## 참고 사항

- `MAP`의 핸들 타입은 `KEY_TYPE**`(노드 기반)입니다. 반복자에 **`it++` / `it--`를 사용하지 마세요.** 반드시 `next(it)`, `prev(it)`를 사용합니다.
- 값은 핸들 타입에 노출되지 않으므로 `second(iter, VALUE_TYPE)`를 통해 명시적으로 접근해야 합니다.
- **중복 키 삽입은 조용히 무시됩니다.** `insert_or_assign` 동작이 필요하면 `find` + `erase` + `insert`를 조합하세요.
- 키는 항상 **오름차순**으로 순회됩니다(`begin` → `end`). 내림차순은 `rbegin` → `rend`.
- 같은 맵 인스턴스에서 모든 연산에 **동일한 `VALUE_TYPE`**을 사용해야 합니다. 다른 타입을 섞으면 UB입니다.
- 비교자가 `NULL`이면 기본 비교자가 사용됩니다. 문자열 키는 `strcmp` 기반 비교자가 자동 선택됩니다.

### 사용자 정의 키와 비교 함수

```c
typedef struct { int year, month, day; } Date;

int date_cmp(const void *a, const void *b) {
    const Date *da = (const Date*)a;
    const Date *db = (const Date*)b;
    if (da->year  != db->year)  return da->year  - db->year;
    if (da->month != db->month) return da->month - db->month;
    return da->day - db->day;
}

MAP(Date) calendar = new_map(Date, char*, date_cmp);
insert(calendar, (Date){2026, 6, 9}, "today");
```

### 값 업데이트 패턴

```c
void map_set(MAP(int) m, int key, int new_val) {
    int *it = find(m, key);
    if (it != end(m)) erase(m, it);
    insert(m, key, new_val);
}
```

이는 `find` + 조건부 `erase` + `insert`로 평균 **O(log n)** 비용으로 동작합니다.

---

## 예제

```c
#include "opencstl.h"

int main() {
    char *containers[] = {
        "vector", "list", "set", "map", "stack",
        "queue", "priority_queue", "unordered_map", "unordered_set"
    };
    size_t len = sizeof(containers) / sizeof(containers[0]);

    MAP(int) m = new_map(int, char*, NULL);

    for (int i = 0; i < (int)len; i++)
        insert(m, i, containers[i]);

    printf("--- Ascending (begin -> end) ---\n");
    for (int *it = begin(m); it != end(m); it = next(it))
        printf("[%2d: %s]\n", first(it), second(it, char*));

    printf("--- Descending (rbegin -> rend) ---\n");
    for (int *it = rbegin(m); it != rend(m); it = prev(it))
        printf("[%2d: %s]\n", first(it), second(it, char*));

    printf("--- Find and erase key 4 ---\n");
    int *it = find(m, 4);
    if (it != end(m)) {
        printf("erasing: %d -> %s\n", first(it), second(it, char*));
        erase(m, it);
    }
    printf("size after erase: %zu\n", size(m));

    destroy(m);
    return 0;
}
```

**출력:**
```
--- Ascending (begin -> end) ---
[ 0: vector]
[ 1: list]
[ 2: set]
[ 3: map]
[ 4: stack]
[ 5: queue]
[ 6: priority_queue]
[ 7: unordered_map]
[ 8: unordered_set]
--- Descending (rbegin -> rend) ---
[ 8: unordered_set]
...
[ 0: vector]
--- Find and erase key 4 ---
erasing: 4 -> stack
size after erase: 8
```

---

## 함수 요약표

### 생성자 / 소멸자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_map` | `MAP(K) new_map(K, V, comp)` | O(1)  | 빈 맵 생성 |
| `destroy` | `void destroy(m)`             | O(n)  | 맵 파괴 및 메모리 해제 |

### 원소 접근

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `first`  | `K first(it)`        | O(1) | 반복자가 가리키는 키 |
| `second` | `V second(it, V)`    | O(1) | 반복자가 가리키는 값 |

### 반복자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `begin`  | `K* begin(m)`   | O(1)        | 최소 키 반복자 |
| `end`    | `K* end(m)`     | O(1)        | 끝 센티넬 (NULL) |
| `rbegin` | `K* rbegin(m)`  | O(1)        | 최대 키 반복자 |
| `rend`   | `K* rend(m)`    | O(1)        | 역방향 끝 센티넬 (NULL) |
| `next`   | `K* next(it)`   | O(1)*       | 키 오름차순 다음 |
| `prev`   | `K* prev(it)`   | O(1)*       | 키 오름차순 이전 |

\* 분할 상환 상수, 최악 O(log n)

### 용량(Capacity)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `empty` | `bool empty(m)`     | O(1) | 비었는지 검사 |
| `size`  | `size_t size(m)`    | O(1) | 키-값 쌍 개수 |

### 수정자(Modifiers)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `insert` | `void insert(m, key, val)` | O(log n) | 키-값 삽입 (중복 키는 무시) |
| `erase`  | `void erase(m, it)`        | O(log n) | 반복자가 가리키는 항목 제거 |
| `clear`  | `void clear(m)`            | O(n)     | 모든 항목 제거 |

### 탐색(Lookup)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `find` | `K* find(m, key)` | O(log n) | 키로 탐색, 없으면 `end(m)` |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`SET`](./set.md)                           | 정렬된 유일 키 집합. `MAP`과 동일한 RB-tree 구조 사용 |
| [`UNORDERED_MAP`](./__unordered_map.md)     | 해시 기반 키-값 저장소. 평균 O(1), 정렬 없음 |
| [`UNORDERED_SET`](./__unordered_set.md)     | 해시 기반 유일 키 집합. 평균 O(1) |
| [`VECTOR`](./vector.md)                     | 연속 시퀀스. 인덱스를 키로 보는 패턴에 적합 |
