# unordered_map

헤더 [`hashtable.h`](../opencstl/hashtable.h)에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
#define UNORDERED_MAP(KEY_TYPE)                            KEY_TYPE*
#define new_unordered_map(KEY_TYPE, VALUE_TYPE)            cstl_unordered_map(KEY_TYPE, VALUE_TYPE)
#define new_unordered_map(KEY_TYPE, VALUE_TYPE, hash_fn)   cstl_unordered_map(KEY_TYPE, VALUE_TYPE, hash_fn)
```

`UNORDERED_MAP`은 **키-값 쌍(key-value pair)** 을 **해시 버킷**에 저장하여 평균 **O(1)** 삽입, 삭제, 탐색을 제공하는 연관 컨테이너입니다.
C++ 표준 라이브러리의 [`std::unordered_map`](https://cppreference.com/cpp/container/unordered_map)에 대응되는 OpenCSTL의 구현체입니다.

`MAP`과 달리 순회 시 키 정렬 순서가 보장되지 **않으며**, **버킷 순서**대로 방문됩니다. OpenCSTL의 `UNORDERED_MAP`은 일반적인 해시 컨테이너에서 잘 지원되지 않는 `rbegin`/`rend`/`prev`도 함께 제공하여 역방향 순회를 지원합니다(역방향 정렬이 아닌 역방향 버킷 순서).

내부 구조는 [`UNORDERED_SET`](./unordered_set.md)과 동일한 **flat 해시 테이블 + 개방 주소법(open addressing)** 으로, 각 슬롯에 키와 값을 **연속해서** 저장합니다.

`unordered_map`의 주요 연산 복잡도는 다음과 같습니다.

- 키 탐색 — 평균 **O(1)**, 최악 **O(n)**
- 삽입, 제거 — 평균 **O(1)**, 최악 **O(n)**
- 전체 순회 — **O(capacity)** *(빈 슬롯도 스캔)*

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
6. [map과의 비교](#map과의-비교)
7. [참고 사항](#참고-사항)
8. [예제](#예제)
9. [함수 요약표](#함수-요약표)
10. [관련 항목](#관련-항목)

---

## 매크로 매개변수

### `UNORDERED_MAP(KEY_TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `KEY_TYPE` | 맵의 키 타입 |

**`KEY_TYPE*`으로 확장됩니다.** `MAP`이 사용하는 `KEY_TYPE**`(노드 기반 핸들)과는 다른, `VECTOR`/`DEQUE`와 같은 평면 핸들 타입입니다. 이는 OpenCSTL의 unordered 컨테이너가 **flat 테이블** 구현임을 의미합니다.

값 타입은 핸들 타입에 노출되지 않으며, 생성 시점에 내부에 기록됩니다.

```c
UNORDERED_MAP(int)   m;   // 확장 결과: int*  m;   (값은 임의 타입 가능)
UNORDERED_MAP(char*) sm;  // 확장 결과: char** sm;
```

### `new_unordered_map(KEY_TYPE, VALUE_TYPE [, hash_fn])`

| 매개변수 | 설명 |
|----------|-----|
| `KEY_TYPE` | 키 타입 |
| `VALUE_TYPE` | 키와 연관되는 값 타입 |
| `hash_fn` *(선택)* | 해시 함수 `size_t (*)(const void*)`. 생략하거나 `NULL`을 전달하면 내장 기본 해시(xxHash 변형)가 사용됩니다. |

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);         // 기본 해시
UNORDERED_MAP(int) m = new_unordered_map(int, char*, NULL);   // 동일
// ... 사용 ...
destroy(m);
```

---

## 멤버 타입

| 개념 (C++ 표준) | OpenCSTL 대응 |
|-----------------|--------------|
| `key_type`                | `KEY_TYPE` |
| `mapped_type`             | `VALUE_TYPE` (생성 시 캡처) |
| `value_type`              | C++의 `pair<KEY,VALUE>`에 대응; OpenCSTL은 슬롯에 키+값 연속 저장 |
| `hasher`                  | 생성자에 전달한 `size_t (*)(const void*)` 또는 기본 해시 |
| `iterator`                | `KEY_TYPE*` — 점유된 슬롯의 키 시작점을 가리키는 포인터 |
| `reverse_iterator`        | `KEY_TYPE*` — `prev()`로 역방향 이동 |
| `size_type`               | `size_t` |

> **반복자의 본질**: 반복자는 슬롯의 **키 시작점**을 가리키는 포인터입니다. 값은 키 바로 뒤에 연속 저장되어 있으므로 `second(iter, VALUE_TYPE)`가 그 위치를 읽어옵니다. **`it++` / `it--`는 빈 슬롯을 건너뛰지 않으므로 사용 금지.**

---

## 내부 구조: Flat Hash Table

`UNORDERED_MAP`은 [`UNORDERED_SET`](./unordered_set.md#내부-구조-flat-hash-table)과 동일한 flat 해시 테이블을 사용하지만, 각 슬롯의 데이터 영역에 **키와 값을 연속해서** 저장합니다.

```
                    핸들 m ─┐
                           ↓
┌────────┬──────────┬────────────────┬────────────────┬─────┐
│ 헤더   │ control  │ slot[0]        │ slot[1]        │ ... │
│ (메타) │ bytes    │ [key][value]   │ [key][value]   │     │
└────────┴──────────┴────────────────┴────────────────┴─────┘
                    ↑
                    m[0], m[1], ... (단, m[i]는 키만 반환)
```

- **슬롯 크기**: `sizeof(KEY) + sizeof(VALUE)` (정렬 패딩 포함)
- **Open addressing**: 충돌 시 다음 슬롯으로 probe
- **Control bytes**: 슬롯의 빈 여부와 토움스톤 상태 추적
- **Power-of-2 capacity**: 최소 8, 2배씩 성장
- **해시 함수**: 기본은 xxHash 계열

### Rehash 정책

- **로드 팩터 임계값**: `7/8 = 87.5%`
- `size`가 `capacity * 7/8`을 초과하면 자동으로 **capacity가 두 배가 되며 rehash 발생**
- rehash가 일어나면 **모든 반복자/포인터가 무효화**

### 평균 O(1)의 의미

이상적인 해시 분포에서는 평균 probe 횟수가 상수이지만, 충돌이 집중되면 최악 **O(n)** 까지 떨어집니다. 사용자 정의 키 타입은 분포가 고른 해시 함수를 직접 제공하는 것이 좋습니다.

---

## 반복자 무효화

| 연산 | 무효화되는 반복자 |
|------|------------------|
| 모든 읽기 전용 연산 | 없음 |
| `insert` (rehash 없음) | 없음 |
| `insert` (rehash 발생) | **모든 반복자** |
| `erase` | 제거된 원소만 |
| `clear` | 모든 반복자 |
| `destroy` | 모든 반복자 |

---

## 멤버 함수

### 생성자 / 소멸자

#### `new_unordered_map`

```c
UNORDERED_MAP(KEY_TYPE) new_unordered_map(KEY_TYPE, VALUE_TYPE);
UNORDERED_MAP(KEY_TYPE) new_unordered_map(KEY_TYPE, VALUE_TYPE, hash_fn);
```

빈 unordered map을 생성합니다. 값 타입은 생성 시점에 캡처되어 슬롯 메모리 레이아웃 결정에 사용됩니다. 초기 capacity는 `HT_MIN_CAP = 8`입니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 해시 맵의 핸들 |
| **복잡도** | 상수 **O(1)** |

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
printf("%zu\n", size(m));   // 0
destroy(m);
```

---

#### `destroy`

```c
void destroy(UNORDERED_MAP(KEY_TYPE) m);
```

unordered map을 파괴하고 모든 내부 저장소를 해제합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
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
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
insert(m, 1, "hello");

int *it = begin(m);
printf("key: %d\n", first(it));   // key: 1
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
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
insert(m, 1, "apple");
insert(m, 2, "banana");

for (int *it = begin(m); it != end(m); it = next(it))
    printf("%d -> %s\n", first(it), second(it, char*));

destroy(m);
```

> **주의**: `second`에 잘못된 `VALUE_TYPE`을 전달하면 슬롯 메모리를 잘못 해석하여 **정의되지 않은 동작(UB)** 입니다. `new_unordered_map`에 전달했던 타입과 정확히 일치해야 합니다.

---

### 반복자

`UNORDERED_MAP`의 반복자 타입은 `KEY_TYPE*`이지만, 빈 슬롯을 건너뛰며 순회하므로 포인터 산술(`it++`)은 동작하지 않습니다. 반드시 [`next()`](#next), [`prev()`](#prev)를 사용해야 합니다.

#### `begin`

```c
KEY_TYPE* begin(UNORDERED_MAP(KEY_TYPE) m);
```

버킷 순회 순서상 첫 번째 항목의 반복자를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 최악 **O(capacity)** |

---

#### `end`

```c
KEY_TYPE* end(UNORDERED_MAP(KEY_TYPE) m);
```

순방향 순회 종료를 나타내는 센티넬(`NULL`). **역참조 금지.**

---

#### `rbegin`

```c
KEY_TYPE* rbegin(UNORDERED_MAP(KEY_TYPE) m);
```

버킷 순회 순서상 마지막 항목의 역방향 반복자를 반환합니다.

> **주의**: *역정렬* 순서가 아니라 *역버킷* 순서입니다.

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
insert(m, 1, "one");
insert(m, 2, "two");
insert(m, 3, "three");

for (int *it = rbegin(m); it != rend(m); it = prev(it))
    printf("%d -> %s\n", first(it), second(it, char*));
destroy(m);
```

---

#### `rend`

```c
KEY_TYPE* rend(UNORDERED_MAP(KEY_TYPE) m);
```

역방향 순회 종료를 나타내는 센티넬(`NULL`). **역참조 금지.**

---

#### `next`

```c
KEY_TYPE* next(KEY_TYPE* iter);
```

버킷 순회 순서상 다음 **점유된** 슬롯의 반복자를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized** |

---

#### `prev`

```c
KEY_TYPE* prev(KEY_TYPE* iter);
```

버킷 순회 순서상 이전 **점유된** 슬롯의 반복자를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 분할 상환 상수 **O(1) amortized** |

---

### 용량(Capacity)

#### `empty`

```c
bool empty(UNORDERED_MAP(KEY_TYPE) m);
```

맵에 키-값 쌍이 하나도 없으면 `true`를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `size`

```c
size_t size(UNORDERED_MAP(KEY_TYPE) m);
```

맵에 저장된 키-값 쌍의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `capacity`

```c
size_t capacity(UNORDERED_MAP(KEY_TYPE) m);
```

현재 할당된 해시 버킷 슬롯의 총 개수를 반환합니다. 항상 `>= size(m)`이며 **2의 거듭제곱**입니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

### 수정자(Modifiers)

#### `insert`

```c
void insert(UNORDERED_MAP(KEY_TYPE) m, KEY_TYPE key, VALUE_TYPE value);
```

키-값 쌍을 삽입합니다. **이미 같은 키가 존재하면 기존 항목은 변경되지 않고 삽입 자체가 무시됩니다**(no overwrite). 로드 팩터가 임계값을 넘으면 rehash가 발생합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 평균 **O(1)**, rehash 시 **O(n)** |
| **반복자 무효화** | rehash 발생 시 모든 반복자 |

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
insert(m, 0, "zero");
insert(m, 1, "one");
insert(m, 0, "ZERO");   // 중복 키 — 무시됨

int *it = find(m, 0);
printf("%s\n", second(it, char*));   // zero (변경되지 않음)
destroy(m);
```

> **값을 덮어쓰려면**(C++의 `insert_or_assign`/`operator[]=`에 해당하는 동작): 현재 OpenCSTL은 별도 함수를 제공하지 않으므로 다음 패턴을 사용하세요.
>
> ```c
> int *it = find(m, key);
> if (it != end(m)) erase(m, it);
> insert(m, key, new_value);
> ```

---

#### `erase`

```c
void erase(UNORDERED_MAP(KEY_TYPE) m, KEY_TYPE* iter);
```

반복자가 가리키는 키-값 쌍을 제거합니다. open addressing에서는 슬롯을 토움스톤으로 표시하여 이후의 probe 체인이 끊기지 않도록 합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 평균 **O(1)** |
| **반복자 무효화** | 제거된 반복자만 |

```c
UNORDERED_MAP(int) m = new_unordered_map(int, int);
insert(m, 1, 10);
insert(m, 2, 20);
insert(m, 3, 30);

int *it = find(m, 2);
if (it != end(m))
    erase(m, it);

printf("size: %zu\n", size(m));   // 2
destroy(m);
```

---

#### `clear`

```c
void clear(UNORDERED_MAP(KEY_TYPE) m);
```

모든 키-값 쌍을 제거합니다. `size(m)`는 `0`이 되며 capacity는 유지됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | **O(capacity)** |
| **반복자 무효화** | 모든 반복자 |

---

### 탐색(Lookup)

#### `find`

```c
KEY_TYPE* find(UNORDERED_MAP(KEY_TYPE) m, KEY_TYPE key);
```

해시 함수로 `key`를 탐색합니다. 일치하는 항목의 반복자를 반환하며, 없으면 `end(m)`(즉 `NULL`)을 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 일치 항목의 반복자, 없으면 `end(m)` |
| **복잡도** | 평균 **O(1)**, 최악 **O(n)** |

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
insert(m, 1, "apple");
insert(m, 2, "banana");

int *it = find(m, 1);
if (it != end(m))
    printf("%d -> %s\n", first(it), second(it, char*));   // 1 -> apple

it = find(m, 99);
if (it == end(m))
    printf("key 99 not found\n");

destroy(m);
```

---

## map과의 비교

| 항목 | `MAP` | `UNORDERED_MAP` |
|------|-------|-----------------|
| **내부 구조** | Red-Black Tree (노드 기반) | Flat hash table (open addressing) |
| **핸들 타입** | `KEY**` | `KEY*` |
| **정렬** | 키 오름차순 | 버킷 순회 순 (정렬 없음) |
| **탐색** | O(log n) | 평균 O(1), 최악 O(n) |
| **삽입/삭제** | O(log n) | 평균 O(1) |
| **반복자 안정성 (insert 시)** | 항상 안정 | rehash 시 무효화 |
| **`rbegin`/`rend`/`prev`** | 지원 (정렬 역순) | 지원 (버킷 역순) |
| **`capacity`** | 미지원 | 지원 |
| **최소/최대 키 O(1)** | `*begin` / `*rbegin` | 미지원 |
| **순회 비용** | O(n) (실원소 수) | O(capacity) (빈 슬롯 포함) |
| **메모리 효율** | 노드 헤더 오버헤드 큼 | 로드 팩터에 의해 결정 |
| **캐시 친화성** | 나쁨 (포인터 추적) | 매우 좋음 (연속 배열) |
| **적합한 용도** | 정렬 순회, 범위 질의 | 빠른 키 기반 lookup |

---

## 참고 사항

- 핸들 타입은 `KEY*`이지만, **포인터 산술(`m++`)을 직접 수행해서는 안 됩니다.** 컨테이너 정체성을 깨뜨리며, 빈 슬롯도 무시하지 않습니다.
- `m[i]`로 접근하면 **슬롯 i의 키 부분만** 얻을 수 있습니다(값 영역은 키 바로 뒤 메모리에 있습니다). 일반적인 키-값 접근에는 반복자와 `first`/`second`를 사용하세요.
- 해시 함수 인자는 **선택사항**입니다. `new_unordered_map(int, char*)`만으로 동작합니다.
- 중복 키 삽입은 조용히 무시됩니다. `insert_or_assign` 동작이 필요하면 `find` + `erase` + `insert`를 조합하세요.
- 같은 맵 인스턴스에서 모든 연산에 **동일한 `VALUE_TYPE`**을 사용해야 합니다. 다른 타입을 섞으면 UB입니다.
- 사용자 정의 키 타입은 **분포가 고른 해시 함수**를 직접 제공하는 것이 강력히 권장됩니다.

### 사용자 정의 키와 해시 함수

```c
typedef struct { int x, y; } Point;

size_t point_hash(const void *p) {
    const Point *pt = (const Point*)p;
    return (size_t)(pt->x * 73856093u) ^ (size_t)(pt->y * 19349663u);
}

UNORDERED_MAP(Point) grid = new_unordered_map(Point, int, point_hash);
insert(grid, (Point){3, 5}, 42);
```

### 값 업데이트 패턴

```c
void umap_set(UNORDERED_MAP(int) m, int key, int new_val) {
    int *it = find(m, key);
    if (it != end(m)) erase(m, it);
    insert(m, key, new_val);
}
```

이는 평균 **O(1)** 비용으로 동작합니다.

### Counting 패턴 (단어 빈도 등)

```c
UNORDERED_MAP(char*) freq = new_unordered_map(char*, int);

for (int i = 0; i < n; i++) {
    char *w = words[i];
    int *it = find(freq, w);
    if (it == end(freq)) {
        insert(freq, w, 1);
    } else {
        int cur = second(it, int);
        erase(freq, it);
        insert(freq, w, cur + 1);
    }
}
```

---

## 예제

```c
#include "opencstl.h"

int main() {
    UNORDERED_MAP(int) m = new_unordered_map(int, char*);

    char *names[] = {"Alice", "Bob", "Carol", "Dave", "Eve"};
    for (int i = 0; i < 5; i++)
        insert(m, i, names[i]);

    printf("size: %zu\n", size(m));   // 5

    // 순방향 순회 (버킷 순서)
    printf("Forward:\n");
    for (int *it = begin(m); it != end(m); it = next(it))
        printf("  %d -> %s\n", first(it), second(it, char*));

    // 역방향 순회 (역버킷 순서)
    printf("Reverse:\n");
    for (int *it = rbegin(m); it != rend(m); it = prev(it))
        printf("  %d -> %s\n", first(it), second(it, char*));

    // 키 탐색
    int *it = find(m, 2);
    if (it != end(m))
        printf("key 2 -> %s\n", second(it, char*));   // key 2 -> Carol

    // 한 항목 제거
    it = find(m, 1);
    if (it != end(m)) erase(m, it);
    printf("size after erase: %zu\n", size(m));   // 4

    destroy(m);
    return 0;
}
```

---

## 함수 요약표

### 생성자 / 소멸자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_unordered_map` | `UNORDERED_MAP(K) new_unordered_map(K, V [, hash])` | O(1) | 빈 해시 맵 생성 |
| `destroy`           | `void destroy(m)`                                   | O(1) | 해시 맵 파괴 |

### 원소 접근

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `first`  | `K first(it)`        | O(1) | 반복자가 가리키는 키 |
| `second` | `V second(it, V)`    | O(1) | 반복자가 가리키는 값 |

### 반복자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `begin`  | `K* begin(m)`   | O(capacity) 최악 | 버킷 순서상 첫 항목 |
| `end`    | `K* end(m)`     | O(1)  | 끝 센티넬 (NULL) |
| `rbegin` | `K* rbegin(m)`  | O(capacity) 최악 | 버킷 순서상 마지막 항목 |
| `rend`   | `K* rend(m)`    | O(1)  | 역방향 끝 센티넬 (NULL) |
| `next`   | `K* next(it)`   | O(1)* | 다음 점유 슬롯 |
| `prev`   | `K* prev(it)`   | O(1)* | 이전 점유 슬롯 |

\* 분할 상환 상수

### 용량(Capacity)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `empty`    | `bool empty(m)`      | O(1) | 비었는지 검사 |
| `size`     | `size_t size(m)`     | O(1) | 키-값 쌍 개수 |
| `capacity` | `size_t capacity(m)` | O(1) | 버킷 슬롯 총 개수 (2의 거듭제곱) |

### 수정자(Modifiers)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `insert` | `void insert(m, key, val)` | O(1) 평균 | 키-값 삽입 (중복 키 무시) |
| `erase`  | `void erase(m, it)`        | O(1) 평균 | 반복자가 가리키는 항목 제거 |
| `clear`  | `void clear(m)`            | O(capacity) | 모든 항목 제거 (capacity 유지) |

### 탐색(Lookup)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `find` | `K* find(m, key)` | O(1) 평균, O(n) 최악 | 키로 탐색, 없으면 `end(m)` |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`MAP`](./map.md)                           | 정렬된 키-값 저장소. O(log n) 연산, 키 오름차순 순회 |
| [`UNORDERED_SET`](./unordered_set.md)       | 해시 기반 유일 키 집합 (값 없음) |
| [`SET`](./set.md)                           | 정렬된 유일 키 집합 |
| [`VECTOR`](./vector.md)                     | 연속 시퀀스. 인덱스를 키로 보는 패턴에 적합 |
