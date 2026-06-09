# list

헤더 [`list.h`](../opencstl/list.h)에 정의되어 있습니다.
*(`#include "opencstl.h"`를 통해 함께 포함됩니다.)*

```c
#define LIST(TYPE)     TYPE**
#define new_list(TYPE) cstl_list(TYPE)
```

`LIST`는 컨테이너의 **임의의 위치**에서 상수 시간(constant time)에 원소를 삽입 및 삭제할 수 있도록 지원하는 시퀀스 컨테이너입니다.
C++ 표준 라이브러리의 [`std::list`](https://cppreference.com/cpp/container/list)에 대응되는 OpenCSTL의 구현체이며, **이중 연결 리스트(doubly-linked list)** 로 구현됩니다.

각 원소는 별도의 동적 할당 노드에 저장되며, 선행 노드(prev)와 후속 노드(next)에 대한 포인터로 서로 연결됩니다. 노드 메모리 구조는 다음과 같습니다.

```
[node_type][prev][next] ↘ [data]
```

원소가 연속된 메모리에 있지 않으므로 **빠른 임의 접근은 지원되지 않습니다.**
즉, `[]` 첨자 연산자나 표준 C 라이브러리 함수(`qsort`, `bsearch` 등)는 사용할 수 없습니다. 순회는 반드시 [`next()`](#next), [`prev()`](#prev) 반복자 함수로 수행해야 합니다.

`list`의 주요 연산 복잡도는 다음과 같습니다.

- 첫/마지막 원소 접근 — 상수 **O(1)**
- 반복자가 주어졌을 때의 삽입/삭제 — 상수 **O(1)**
- 선형 탐색 — **O(n)**
- 임의 접근 — **지원하지 않음**

`vector`와 달리 노드는 독립적으로 힙에 존재하므로, 삽입과 삭제는 다른 반복자/포인터를 무효화하지 **않습니다**. 이는 long-lived 반복자를 유지해야 하는 워크로드에서 큰 장점입니다.

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
5. [vector와의 비교](#vector와의-비교)
6. [참고 사항](#참고-사항)
7. [예제](#예제)
8. [함수 요약표](#함수-요약표)
9. [관련 항목](#관련-항목)

---

## 매크로 매개변수

### `LIST(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 리스트에 저장될 원소의 타입 |

`TYPE**`으로 확장됩니다. 노드가 각각 힙에 할당되어 연결되기 때문에, 핸들은 포인터에 대한 포인터입니다. 이는 `VECTOR`(`TYPE*`)와의 가장 큰 구조적 차이입니다.

```c
LIST(int)   lst;   // 확장 결과: int**  lst;
LIST(char*) slst;  // 확장 결과: char*** slst;
```

> **왜 `TYPE**`인가?**: `LIST`의 핸들은 "현재 head 포인터를 가리키는 포인터"의 의미를 갖습니다. 한 단계 더 간접 참조함으로써 OpenCSTL은 컨테이너 자체의 식별성과 노드의 위치를 분리할 수 있습니다.

### `new_list(TYPE)`

| 매개변수 | 설명 |
|----------|-----|
| `TYPE`   | 리스트에 저장될 원소의 타입 |

새로운 빈 리스트를 생성하여 반환합니다. 반환된 핸들은 더 이상 사용하지 않을 때 반드시 [`destroy`](#destroy)를 통해 해제되어야 합니다.

```c
LIST(int) lst = new_list(int);
// ... 사용 ...
destroy(lst);
```

---

## 멤버 타입

`LIST`는 C 매크로 기반 구현이므로 C++의 typedef 멤버 타입과 1:1 대응되는 형태는 아니지만, 동일한 개념이 다음과 같이 매핑됩니다.

| 개념 (C++ 표준) | OpenCSTL 대응 |
|-----------------|--------------|
| `value_type`              | `TYPE` (매크로 인자) |
| `reference` / `pointer`   | 노드 데이터에 대한 `TYPE*` |
| `iterator`                | `TYPE*` — 노드의 데이터 부분을 가리키는 포인터 |
| `reverse_iterator`        | `TYPE*` — `prev()`로 역방향 이동 |
| `size_type`               | `size_t` |
| `difference_type`         | `ptrdiff_t` *(단, `it1 - it2`는 정의되지 않음)* |
| `allocator_type`          | 내부 `calloc`/`free` 사용 (고정) |

> **반복자의 본질**: 반복자는 노드의 **데이터 시작점**을 가리키는 포인터입니다. 노드 헤더(`prev`/`next`)는 데이터 앞쪽 음수 오프셋에 저장되어 있으며, `next()`/`prev()` 함수가 이를 따라 이동합니다. 따라서 **`it++` / `it--`는 절대로 사용해서는 안 됩니다.**

---

## 반복자 무효화

`vector`와 달리 `list`에서 삽입과 삭제는 기존 반복자를 무효화하지 **않습니다**. 노드들이 독립적인 힙 객체이기 때문입니다.

| 연산 | 무효화되는 반복자 |
|------|------------------|
| 모든 읽기 전용 연산 | 없음 |
| `assign`, `clear` | 모든 반복자 |
| `push_back`, `push_front` | 없음 |
| `pop_back`, `pop_front` | 제거된 원소를 가리키는 반복자만 |
| `insert` | 없음 (다른 반복자 모두 유효) |
| `erase` | 제거된 원소(들)를 가리키는 반복자만 |
| `resize` | 축소 시 제거되는 원소들만 |
| `sort` (정렬) | 없음 (노드는 그대로, 링크만 재배치) |
| `reverse` | 없음 (노드는 그대로, 링크 방향만 반전) |
| `destroy` | 모든 반복자 |

> **활용**: 큰 자료구조에서 원소를 가리키는 포인터를 외부에 보관해야 하는 경우, `vector` 대신 `list`를 선택하면 삽입/삭제 후에도 그 포인터를 계속 사용할 수 있습니다.

---

## 멤버 함수

### 생성자 / 소멸자

#### `new_list`

```c
LIST(TYPE) new_list(TYPE);
```

빈 이중 연결 리스트를 생성합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | 새로 생성된 리스트의 핸들 |
| **복잡도** | 상수 **O(1)** |
| **예외** | 메모리 할당 실패 시 `verify()` 실패로 프로그램 중단 |

```c
LIST(int) lst = new_list(int);
printf("%zu\n", size(lst));   // 0
destroy(lst);
```

---

#### `destroy`

```c
void destroy(LIST(TYPE) lst);
```

리스트를 파괴하여 모든 노드와 내부 관리 메모리를 해제합니다. `new_list`로 생성된 모든 리스트는 반드시 한 번 `destroy`되어야 합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 원소 개수에 선형 **O(n)** |
| **반복자 무효화** | 모든 반복자 |

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
destroy(lst);
```

---

### 원소 접근

> **참고**: `list`는 `vector`와 달리 `operator[]`를 지원하지 않습니다. 임의 인덱스 접근이 필요하면 `vector` 또는 `deque`를 사용하세요.

#### `front`

```c
TYPE front(LIST(TYPE) lst);
```

첫 번째 원소의 값을 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | head 노드의 데이터 |
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(lst) == false` — 빈 리스트에 호출 시 **UB** |

```c
LIST(int) lst = new_list(int);
push_back(lst, 10);
push_back(lst, 20);
printf("%d\n", front(lst));   // 10
destroy(lst);
```

---

#### `back`

```c
TYPE back(LIST(TYPE) lst);
```

마지막 원소의 값을 반환합니다.

| 항목 | 내용 |
|------|------|
| **반환값** | tail 노드의 데이터 |
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(lst) == false` — 빈 리스트에 호출 시 **UB** |

```c
LIST(int) lst = new_list(int);
push_back(lst, 10);
push_back(lst, 20);
printf("%d\n", back(lst));   // 20
destroy(lst);
```

---

### 반복자

`LIST`의 반복자 타입은 `TYPE*`이지만, `vector`와는 본질적으로 다릅니다. 원소가 연속되어 있지 않으므로 **포인터 산술(`it++`, `it--`, `it + n`)은 동작하지 않습니다.** 반드시 [`next()`](#next), [`prev()`](#prev)를 사용해 반복자를 이동시켜야 합니다.

#### `begin`

```c
TYPE* begin(LIST(TYPE) lst);
```

첫 번째 원소(head 노드)를 가리키는 반복자를 반환합니다. 리스트가 비어 있으면 `end(lst)`와 같습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
LIST(int) lst = new_list(int);
for (int i = 0; i < 5; i++)
    push_back(lst, i * 10);

for (int *it = begin(lst); it != end(lst); it = next(it))
    printf("[%3d]", *it);   // [  0][ 10][ 20][ 30][ 40]
puts("");
destroy(lst);
```

---

#### `end`

```c
TYPE* end(LIST(TYPE) lst);
```

마지막 원소 **다음** 위치를 나타내는 센티넬(sentinel) 반복자를 반환합니다. OpenCSTL의 `list`에서는 `NULL`로 정의됩니다. **역참조해서는 안 됩니다.**

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

---

#### `rbegin`

```c
TYPE* rbegin(LIST(TYPE) lst);
```

마지막 원소(tail 노드)를 가리키는 역방향 반복자를 반환합니다. 역방향 순회 시 [`prev()`](#prev)로 이동합니다.

```c
LIST(int) lst = new_list(int);
for (int i = 0; i < 5; i++)
    push_back(lst, i);

for (int *it = rbegin(lst); it != rend(lst); it = prev(it))
    printf("[%3d]", *it);   // [  4][  3][  2][  1][  0]
puts("");
destroy(lst);
```

---

#### `rend`

```c
TYPE* rend(LIST(TYPE) lst);
```

첫 번째 원소 **바로 앞** 위치를 나타내는 역방향 센티넬 반복자를 반환합니다. `NULL`로 정의됩니다. **역참조해서는 안 됩니다.**

---

#### `next`

```c
TYPE* next(TYPE* iter);
```

반복자를 **다음** 원소로 진행시킨 후 반환합니다. `list`를 순방향으로 순회하는 유일한 올바른 방법입니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `iter != end(lst)` |

```c
for (int *it = begin(lst); it != end(lst); it = next(it))
    printf("%d\n", *it);
```

> **내부 동작**: 노드의 데이터 시작 주소에서 `-1` 오프셋(즉 `next` 포인터 슬롯)에 저장된 주소를 반환합니다.

---

#### `prev`

```c
TYPE* prev(TYPE* iter);
```

반복자를 **이전** 원소로 이동시킨 후 반환합니다. 역방향 순회에 사용됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
for (int *it = rbegin(lst); it != rend(lst); it = prev(it))
    printf("%d\n", *it);
```

---

### 용량(Capacity)

#### `empty`

```c
bool empty(LIST(TYPE) lst);
```

리스트에 원소가 하나도 없으면 `true`를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
LIST(int) lst = new_list(int);
printf("%d\n", empty(lst));   // 1
push_back(lst, 1);
printf("%d\n", empty(lst));   // 0
destroy(lst);
```

---

#### `size`

```c
size_t size(LIST(TYPE) lst);
```

현재 리스트에 저장된 원소의 개수를 반환합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(내부적으로 길이 카운터를 유지)* |

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 2);
printf("%zu\n", size(lst));   // 2
destroy(lst);
```

---

#### `max_size`

```c
size_t max_size(LIST(TYPE) lst);
```

리스트가 시스템 제약상 보유할 수 있는 원소의 **이론적 최대 개수**를 반환합니다. 실제로는 사용 가능한 RAM 크기에 의해 제한됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |

```c
LIST(int) lst = new_list(int);
printf("max: %zu\n", max_size(lst));
destroy(lst);
```

> **참고**: `list`는 `capacity()`와 `reserve()`를 **지원하지 않습니다.** 각 노드는 필요할 때마다 개별 할당되므로 사전 할당이라는 개념이 존재하지 않습니다. 메모리 단편화에 민감하다면 `vector` 사용을 검토하세요.

---

### 수정자(Modifiers)

#### `assign`

```c
void assign(LIST(TYPE) lst, size_t n);
void assign(LIST(TYPE) lst, size_t n, TYPE value);
```

기존 내용을 모두 폐기하고 `n`개의 원소로 채웁니다. `value`가 주어지면 모든 원소가 그 값으로 초기화되며, 그렇지 않으면 0으로 초기화됩니다.

| 오버로드 | 설명 |
|----------|-----|
| `assign(lst, n)`        | `n`개의 0 초기화 원소 |
| `assign(lst, n, value)` | `n`개의 원소를 모두 `value`로 채움 |

| 항목 | 내용 |
|------|------|
| **복잡도** | `n`에 선형 **O(n)** |
| **반복자 무효화** | 모든 반복자 |

```c
LIST(int) lst = new_list(int);
assign(lst, 4, 7);   // [7][7][7][7]
destroy(lst);
```

---

#### `push_back`

```c
void push_back(LIST(TYPE) lst, TYPE value);
```

리스트의 끝(tail)에 `value`를 추가합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(`vector`와 달리 분할 상환이 아닌 최악-상수)* |
| **반복자 무효화** | 없음 |

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 2);
push_back(lst, 3);   // [1][2][3]
destroy(lst);
```

---

#### `pop_back`

```c
void pop_back(LIST(TYPE) lst);
```

마지막 원소를 제거합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(lst) == false` |
| **반복자 무효화** | 제거된 원소를 가리키는 반복자만 |

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 2);
pop_back(lst);
printf("%d\n", back(lst));   // 1
destroy(lst);
```

---

#### `push_front`

```c
void push_front(LIST(TYPE) lst, TYPE value);
```

리스트의 맨 앞(head)에 `value`를 삽입합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** *(`vector`라면 O(n)이지만 `list`는 O(1))* |
| **반복자 무효화** | 없음 |

```c
LIST(int) lst = new_list(int);
push_back(lst, 2);
push_front(lst, 1);
push_front(lst, 0);   // [0][1][2]
destroy(lst);
```

---

#### `pop_front`

```c
void pop_front(LIST(TYPE) lst);
```

첫 번째 원소를 제거합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 상수 **O(1)** |
| **사전 조건** | `empty(lst) == false` |
| **반복자 무효화** | 제거된 원소를 가리키는 반복자만 |

```c
LIST(int) lst = new_list(int);
push_back(lst, 10);
push_back(lst, 20);
pop_front(lst);
printf("%d\n", front(lst));   // 20
destroy(lst);
```

---

#### `insert`

```c
void insert(LIST(TYPE) lst, TYPE* pos, TYPE value);
void insert(LIST(TYPE) lst, TYPE* pos, size_t n, TYPE value);
```

`pos`가 가리키는 노드 **앞**에 원소(들)를 삽입합니다. 기존 반복자는 무효화되지 않습니다.

| 오버로드 | 설명 |
|----------|-----|
| `insert(lst, pos, value)`    | 단일 원소를 `pos` 앞에 삽입 |
| `insert(lst, pos, n, value)` | `value`를 `n`개 복제하여 `pos` 앞에 삽입 |

| 항목 | 내용 |
|------|------|
| **복잡도** | 삽입 개수에 선형 **O(N)** *(`pos`에서 끝까지 거리에는 무관)* |
| **반복자 무효화** | 없음 |

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 3);

int *it = begin(lst);
it = next(it);               // 3을 가리키게 됨
insert(lst, it, 2);          // [1][2][3]

for (int *p = begin(lst); p != end(lst); p = next(p))
    printf("[%d]", *p);
puts("");
destroy(lst);
```

---

#### `erase`

```c
void erase(LIST(TYPE) lst, TYPE* pos);
void erase(LIST(TYPE) lst, TYPE* first, TYPE* last);
```

리스트에서 원소(들)를 제거합니다. 제거된 반복자만 무효화됩니다.

| 오버로드 | 설명 |
|----------|-----|
| `erase(lst, pos)`         | `pos` 위치의 원소 제거 |
| `erase(lst, first, last)` | 반열림 구간 `[first, last)`의 모든 원소 제거 |

| 항목 | 내용 |
|------|------|
| **복잡도** | 제거하는 원소 개수에 선형 **O(N)** |
| **반복자 무효화** | 제거된 원소(들)만 |

```c
LIST(int) lst = new_list(int);
for (int i = 0; i < 5; i++)
    push_back(lst, i);   // [0][1][2][3][4]

int *it = begin(lst);
it = next(it);
it = next(it);
erase(lst, it);          // [0][1][3][4]

for (int *p = begin(lst); p != end(lst); p = next(p))
    printf("[%d]", *p);
puts("");
destroy(lst);
```

---

#### `resize`

```c
void resize(LIST(TYPE) lst, size_t n);
void resize(LIST(TYPE) lst, size_t n, TYPE value);
```

리스트의 크기가 정확히 `n`이 되도록 조정합니다.

- `n > size(lst)`인 경우: 새 원소가 끝에 추가됩니다 (0 또는 `value`).
- `n < size(lst)`인 경우: 초과 원소가 끝에서부터 제거됩니다.
- `n == size(lst)`인 경우: 아무 일도 일어나지 않습니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | `|n - size(lst)|`에 선형 **O(N)** |
| **반복자 무효화** | 축소 시 제거되는 원소들만 |

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 2);
push_back(lst, 3);

resize(lst, 5, 99);   // [1][2][3][99][99]
resize(lst, 2);        // [1][2]
printf("size: %zu\n", size(lst));   // 2
destroy(lst);
```

---

#### `clear`

```c
void clear(LIST(TYPE) lst);
```

모든 원소를 제거합니다. `size(lst)`는 `0`이 되며 모든 노드가 해제됩니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 원소 개수에 선형 **O(n)** *(각 노드를 `free`하기 때문)* |
| **반복자 무효화** | 모든 반복자 |

> `vector::clear`와 달리 `list::clear`는 각 노드를 해제해야 하므로 **O(n)** 입니다. 메모리는 시스템에 반환됩니다.

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 2);
clear(lst);
printf("size: %zu\n", size(lst));   // 0
destroy(lst);
```

---

### 탐색(Lookup)

#### `find`

```c
TYPE* find(LIST(TYPE) lst, TYPE value);
TYPE* find(LIST(TYPE) lst, TYPE* start, TYPE value);
```

`value`를 선형 탐색하여 일치하는 첫 원소의 반복자를 반환합니다. 찾지 못하면 `end(lst)`(즉 `NULL`)를 반환합니다.

| 오버로드 | 설명 |
|----------|-----|
| `find(lst, value)`        | 처음부터 리스트 전체 탐색 |
| `find(lst, start, value)` | 반복자 `start`부터 탐색 |

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |

> **주의**: `list`는 정렬되어 있어도 이진 탐색을 사용할 수 없습니다(임의 접근 불가). 빠른 키 기반 탐색이 필요하면 `set`/`map`/`unordered_set`/`unordered_map`을 고려하세요.

```c
LIST(int) lst = new_list(int);
for (int i = 0; i < 5; i++)
    push_back(lst, i * 10);   // [0][10][20][30][40]

int *it = find(lst, 20);
if (it != end(lst))
    printf("found: %d\n", *it);   // found: 20

destroy(lst);
```

---

### 연산(Operations)

#### `sort` (`msort` / `qsort`)

```c
void msort(LIST(TYPE) lst, int (*cmp)(const void*, const void*));
void qsort(LIST(TYPE) lst, int (*cmp)(const void*, const void*));
```

리스트의 원소를 비교 함수 `cmp` 기준으로 정렬합니다.

| 함수 | 알고리즘 | 특성 |
|------|---------|------|
| `stable_sort` | 상향식(bottom-up) 머지 정렬 | **안정 정렬(stable)**, 추가 메모리 없음, **O(n log n)** 보장 |
| `sort` | median-of-three 퀵 정렬 (반복형, 스택 사용) | 평균 **O(n log n)**, 최악 **O(n²)**, 평균적으로 더 빠름 |

`cmp(a, b)`는 다음과 같이 반환해야 합니다.

- `a < b` → 음수
- `a == b` → 0
- `a > b` → 양수

| 항목 | 내용 |
|------|------|
| **복잡도** | `msort`: **O(n log n)** 최악 / `qsort`: 평균 **O(n log n)**, 최악 **O(n²)** |
| **반복자 무효화** | **없음** *(노드는 그대로 두고 next/prev 링크만 재배치하므로)* |
| **선택 기준** | 안정성이 필요하거나 최악-경우 보장이 중요하면 `msort`, 평균 성능이 중요하면 `qsort` |

```c
int int_cmp(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

LIST(int) lst = new_list(int);
int xs[] = {3, 1, 4, 1, 5, 9, 2, 6};
for (int i = 0; i < 8; i++) push_back(lst, xs[i]);

stable_sort(lst, int_cmp);

for (int *p = begin(lst); p != end(lst); p = next(p))
    printf("[%d]", *p);   // [1][1][2][3][4][5][6][9]
puts("");
destroy(lst);
```

> **구현 노트**: `msort`는 width를 1, 2, 4, 8 … 로 늘려가며 인접한 두 정렬된 run을 머지하는 방식으로, **모든 작업이 노드의 next/prev 포인터만 재배치하여** 데이터를 이동시키지 않습니다. 따라서 정렬 후에도 기존 노드 주소를 가리키는 외부 포인터가 그대로 유효합니다.

---

#### `reverse`

```c
void reverse(LIST(TYPE) lst);
```

리스트 내 원소들의 순서를 **제자리(in-place)** 에서 뒤집습니다. 각 노드의 `prev` ↔ `next` 포인터를 교환하고 컨테이너의 head ↔ tail을 교환하는 방식으로 동작합니다.

| 항목 | 내용 |
|------|------|
| **복잡도** | 선형 **O(n)** |
| **반복자 무효화** | 없음 *(노드 주소는 변하지 않음)* |

```c
LIST(int) lst = new_list(int);
for (int i = 0; i < 5; i++) push_back(lst, i);   // [0][1][2][3][4]

reverse(lst);                                      // [4][3][2][1][0]
for (int *p = begin(lst); p != end(lst); p = next(p))
    printf("[%3d]", *p);
puts("");
destroy(lst);
```

**출력:**
```
[  4][  3][  2][  1][  0]
```

---

## vector와의 비교

`list`와 `vector`는 모두 시퀀스 컨테이너이지만 성능 특성과 메모리 구조가 크게 다릅니다.

| 항목 | `VECTOR` | `LIST` |
|------|---------|--------|
| **메모리 배치** | 연속(contiguous) | 분산된 노드들이 링크로 연결 |
| **핸들 타입** | `TYPE*` | `TYPE**` |
| **임의 접근 `v[i]`** | O(1) | 미지원 |
| **임의 위치 삽입/삭제** | O(n) | O(1) *(반복자가 주어졌을 때)* |
| **끝에서 push/pop** | 분할 상환 O(1) | O(1) |
| **앞에서 push/pop** | O(n) | O(1) |
| **반복자 무효화** | 재할당 시 모든 반복자 | 거의 없음 |
| **포인터 산술 `it++`** | 가능 | **불가** — `next(it)` 사용 |
| **stdlib 호환 (`qsort`, `bsearch`)** | 가능 | 불가 |
| **캐시 친화성** | 매우 좋음 | 나쁨 (포인터 추적) |
| **메모리 오버헤드** | 작음 (capacity 여유분) | 크다 (노드당 prev/next/타입 헤더) |
| **`reserve`/`capacity`** | 지원 | 미지원 |

**언제 `list`를 선택하나?**
- 중간에서 빈번한 삽입/삭제가 발생하는 경우
- 노드를 가리키는 포인터를 장기간 안정적으로 보관해야 하는 경우
- 정렬 후에도 원소 주소가 변하지 않아야 하는 경우

**언제 `vector`를 선택하나?**
- 임의 접근이 필요한 경우
- 순회 성능이 중요한 경우 (캐시 친화성)
- 끝에서만 주로 추가/삭제가 일어나는 경우

---

## 참고 사항

- `LIST`의 핸들 타입은 `TYPE**`(노드 기반 컨테이너)입니다.
  반면 연속 시퀀스 컨테이너(`VECTOR`, `DEQUE`)는 `TYPE*`을 사용합니다.
- 리스트 반복자에 `it++`나 `it--`를 **사용해서는 안 됩니다.** 반드시 `next(it)`, `prev(it)`를 사용하세요.
- `LIST`는 `[]` 첨자 접근과 `capacity()`/`reserve()`를 지원하지 않습니다.
- `LIST`는 원소가 연속되어 있지 않으므로 `qsort`/`bsearch` 같은 stdlib 함수와 **호환되지 않습니다.** 대신 OpenCSTL이 제공하는 `msort`/`qsort` (list 전용 오버로드)를 사용하세요.
- `float`는 직접 지원되지만, `typedef`로 별칭된 부동소수 타입은 인식되지 않습니다. `float` 키워드를 직접 사용하세요.

### 노드 메모리 레이아웃

각 노드는 다음과 같이 할당됩니다.

```
저주소 ─→ 고주소
┌──────────┬──────┬──────┬──────────┐
│ node_type│ prev │ next │   data   │
└──────────┴──────┴──────┴──────────┘
                          ↑
                          반복자가 가리키는 위치 (TYPE*)
```

- `node_type`: 노드가 어떤 컨테이너 종류에 속하는지를 식별 (예: `OPENCSTL_LIST`)
- `prev`, `next`: 인접 노드의 데이터 시작점에 대한 포인터
- `data`: 실제 사용자 데이터 (`TYPE` 크기)

반복자가 데이터 시작점을 가리키기 때문에 `*it`로 자연스럽게 값에 접근할 수 있으며, 헤더는 음수 오프셋으로 접근됩니다.

### 메모리 추적

`LIST`는 내부적으로 **van Emde Boas tree** 기반의 인터벌 트리(`iveb`)에 자신의 주소 범위를 등록합니다. 이를 통해 OpenCSTL의 일부 알고리즘이 임의의 포인터가 어느 컨테이너에 속하는지를 빠르게 결정할 수 있습니다. `destroy` 호출 시 등록이 해제됩니다.

---

## 예제

```c
#include "opencstl.h"

int main() {
    LIST(int) lst = new_list(int);

    for (int i = 0; i < 10; i++) {
        int val = 10 - i;
        push_back(lst, val);
    }

    // 순방향 순회
    for (int *it = begin(lst); it != end(lst); it = next(it))
        printf("[%3d]", *it);
    puts("");

    // 역방향 순회
    for (int *it = rbegin(lst); it != rend(lst); it = prev(it))
        printf("[%3d]", *it);
    puts("");

    printf("size:  %zu\n", size(lst));
    printf("front: %d\n",  front(lst));
    printf("back:  %d\n",  back(lst));

    destroy(lst);
    return 0;
}
```

**출력:**
```
[ 10][  9][  8][  7][  6][  5][  4][  3][  2][  1]
[  1][  2][  3][  4][  5][  6][  7][  8][  9][ 10]
size:  10
front: 10
back:  1
```

---

## 함수 요약표

### 생성자 / 소멸자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `new_list` | `LIST(T) new_list(T)` | O(1) | 빈 리스트 생성 |
| `destroy`  | `void destroy(lst)`   | O(n) | 리스트 파괴 및 메모리 해제 |

### 원소 접근

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `front` | `T front(lst)` | O(1) | 첫 번째 원소 |
| `back`  | `T back(lst)`  | O(1) | 마지막 원소 |

### 반복자

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `begin`  | `T* begin(lst)`   | O(1) | 첫 원소 반복자 |
| `end`    | `T* end(lst)`     | O(1) | 끝 센티넬 (NULL) |
| `rbegin` | `T* rbegin(lst)`  | O(1) | 역방향: 마지막 원소 |
| `rend`   | `T* rend(lst)`    | O(1) | 역방향 끝 센티넬 (NULL) |
| `next`   | `T* next(it)`     | O(1) | 다음 원소로 이동 |
| `prev`   | `T* prev(it)`     | O(1) | 이전 원소로 이동 |

### 용량(Capacity)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `empty`    | `bool empty(lst)`    | O(1) | 비었는지 검사 |
| `size`     | `size_t size(lst)`   | O(1) | 원소 개수 |
| `max_size` | `size_t max_size(lst)` | O(1) | 보유 가능한 이론적 최대 원소 수 |

### 수정자(Modifiers)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `assign`      | `assign(lst, n)` / `assign(lst, n, val)`             | O(n) | 내용을 `n`개 원소로 치환 |
| `push_back`   | `void push_back(lst, val)`                           | O(1) | 끝에 추가 |
| `pop_back`    | `void pop_back(lst)`                                 | O(1) | 마지막 원소 제거 |
| `push_front`  | `void push_front(lst, val)`                          | O(1) | 앞에 추가 |
| `pop_front`   | `void pop_front(lst)`                                | O(1) | 첫 원소 제거 |
| `insert`      | `insert(lst, pos, val)` / `insert(lst, pos, n, val)` | O(N) | `pos` 앞에 삽입 |
| `erase`       | `erase(lst, pos)` / `erase(lst, first, last)`        | O(N) | 원소(들) 제거 |
| `resize`      | `resize(lst, n)` / `resize(lst, n, val)`             | O(N) | 크기 변경 |
| `clear`       | `void clear(lst)`                                    | O(n) | 모든 원소 제거 |

### 탐색(Lookup)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `find` | `T* find(lst, val)` / `find(lst, start, val)` | O(n) | 선형 탐색 |

### 연산(Operations)

| 함수 | 시그니처 | 복잡도 | 설명 |
|------|---------|-------|------|
| `msort`   | `void msort(lst, cmp)`   | O(n log n) | 안정 머지 정렬 |
| `qsort`   | `void qsort(lst, cmp)`   | 평균 O(n log n) | 퀵 정렬 (불안정) |
| `reverse` | `void reverse(lst)`      | O(n) | 원소 순서 반전 |

---

## 관련 항목

| 컨테이너 | 설명 |
|----------|-----|
| [`VECTOR`](./vector.md)                     | 연속 동적 배열. `[]`로 O(1) 임의 접근 |
| [`DEQUE`](./__deque.md)                     | 양방향 큐. `[]` 접근 지원, 양 끝에서 O(1) 삽입/삭제 |
| [`SET`](./__set.md)                         | 정렬된 유일 키 집합. O(log n) |
| [`MAP`](./__map.md)                         | 정렬된 키-값 저장소. O(log n) |
| [`STACK`](./__stack.md)                     | LIFO 어댑터 |
| [`QUEUE`](./__queue.md)                     | FIFO 어댑터 |
| [`UNORDERED_SET`](./__unordered_set.md)     | 해시 기반 유일 키 집합. 평균 O(1) |
| [`UNORDERED_MAP`](./__unordered_map.md)     | 해시 기반 키-값 저장소. 평균 O(1) |
