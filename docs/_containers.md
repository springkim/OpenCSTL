# Containers Library

Defined in header [`opencstl.h`](../opencstl.h)

```c
#include "opencstl.h"
```

---

The OpenCSTL Containers library is a collection of generic macros and functions that allow C programmers to use common data structures — dynamic arrays, linked lists, trees, hash tables, stacks, queues — with an interface **identical to C++ STL**. No build steps, no linking, no type registration. Just `#include "opencstl.h"`.

There are three classes of containers:

- **Sequence containers** — data structures accessible sequentially.
- **Associative containers** — sorted data structures searchable in O(log n).
- **Unordered associative containers** — hash-based data structures searchable in O(1) average.

In addition, **container adaptors** provide a restricted interface over the sequence containers.

The container manages its own storage and provides functions to access elements directly or through **iterators** (typed pointers with properties similar to C++ iterators). Most containers share a common set of function names, following the same semantics as their C++ STL equivalents.

---

## Contents

1. [Installation](#installation)
2. [Sequence Containers](#sequence-containers)
3. [Associative Containers](#associative-containers)
4. [Unordered Associative Containers](#unordered-associative-containers)
5. [Container Adaptors](#container-adaptors)
6. [Iterator Model](#iterator-model)
7. [Iterator Invalidation](#iterator-invalidation)
8. [Built-in Comparators](#built-in-comparators)
9. [stdlib Compatibility](#stdlib-compatibility)
10. [Function Table](#function-table)
11. [Choosing the Right Container](#choosing-the-right-container)

---

## Installation

```bash
curl -LO "https://raw.githubusercontent.com/springkim/OpenCSTL/refs/heads/master/opencstl.h"
```

Then in your source file:

```c
#include "opencstl.h"
```

No additional build configuration required. Supports C99, C11, C17, and C23.

---

## Sequence Containers

Sequence containers implement data structures that can be accessed sequentially. Elements are stored in a specific order determined by the programmer, not by any key or comparator.

| Container | Macro / Constructor | Description |
|-----------|---------------------|-------------|
| [`VECTOR`](__vector.md) | `VECTOR(T)` / `new_vector(T)` | Resizable contiguous array. Supports `[]`, `begin`/`end`, `push_back`, `pop_back`. Compatible with `qsort` and `bsearch`. |
| [`DEQUE`](__deque.md) | `DEQUE(T)` / `new_deque(T)` | Double-ended resizable array. Supports `[]` and O(1) `push_front` / `pop_front` in addition to all `VECTOR` operations. |
| [`LIST`](__list.md) | `LIST(T)` / `new_list(T)` | Doubly-linked list. O(1) insert and erase anywhere given an iterator. No `[]` access; iterate with `next()` / `prev()`. |

### Handle Types

| Container | Handle Type | Iterator Advance |
|-----------|-------------|-----------------|
| `VECTOR` | `T*` | `it++` |
| `DEQUE` | `T*` | `it++` |
| `LIST` | `T**` | `it = next(it)` |

---

## Associative Containers

Associative containers implement sorted data structures that can be searched in **O(log n)**. Internally backed by self-balancing binary search trees. Iteration always visits elements in ascending sorted order.

| Container | Macro / Constructor | Description |
|-----------|---------------------|-------------|
| [`SET`](__set.md) | `SET(T)` / `new_set(T [, cmp])` | Collection of unique keys, sorted by a comparator. Duplicate inserts are silently ignored. |
| [`MAP`](__map.md) | `MAP(K)` / `new_map(K, V [, cmp])` | Collection of unique key-value pairs, sorted by key. Access values via `first(iter)` and `second(iter, V)`. |

The `comparator` argument is **optional**. When omitted, a built-in default comparator for the type is used.

### Handle Types

| Container | Handle Type | Iterator Advance |
|-----------|-------------|-----------------|
| `SET` | `T**` | `it = next(it)` |
| `MAP` | `K**` | `it = next(it)` |

---

## Unordered Associative Containers

Unordered associative containers implement hash-based data structures. Average O(1) lookup, insertion, and removal. **Reverse iteration is supported** via `rbegin` / `rend` / `prev`, visiting elements in reverse bucket traversal order.

| Container | Macro / Constructor | Description |
|-----------|---------------------|-------------|
| [`UNORDERED_SET`](__unordered_set.md) | `UNORDERED_SET(T)` / `new_unordered_set(T [, hash_fn])` | Collection of unique keys, hashed. Supports `[]` bucket access, `capacity`, `rbegin`/`rend`/`prev`. |
| [`UNORDERED_MAP`](__unordered_map.md) | `UNORDERED_MAP(K)` / `new_unordered_map(K, V [, hash_fn])` | Collection of unique key-value pairs, hashed. Supports `rbegin`/`rend`/`prev`. |

The `hash_fn` argument is **optional**. When omitted, a built-in default hash for the type is used.

### Handle Types

| Container | Handle Type | Iterator Advance |
|-----------|-------------|-----------------|
| `UNORDERED_SET` | `T**` | `it = next(it)` / `it = prev(it)` |
| `UNORDERED_MAP` | `K**` | `it = next(it)` / `it = prev(it)` |

---

## Container Adaptors

Container adaptors provide a specialized, restricted interface over an underlying sequence container. They do not expose iterators or direct element access beyond what their interface specifies.

| Container | Macro / Constructor | Description |
|-----------|---------------------|-------------|
| [`STACK`](__stack.md) | `STACK(T)` / `new_stack(T)` | LIFO (last-in, first-out). Access via `top`. Supports `push` and `pop`. |
| [`QUEUE`](__queue.md) | `QUEUE(T)` / `new_queue(T)` | FIFO (first-in, first-out). Access via `front` and `back`. Supports `push` and `pop`. |
| [`PRIORITY_QUEUE`](__priority_queue.md) | `PRIORITY_QUEUE(T)` / `new_priority_queue(T [, cmp])` | Max-heap by default. The element with highest priority is always at `top`. O(log n) push and pop. |

The `comparator` argument for `PRIORITY_QUEUE` is **optional**.

---

## Iterator Model

OpenCSTL uses **typed pointers** as iterators, mirroring C++ iterator semantics in plain C.

### Contiguous containers (`VECTOR`, `DEQUE`)

The iterator is a `TYPE*`. Standard pointer arithmetic applies:

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++) push_back(v, i);

for (int *it = begin(v); it != end(v); it++)       // forward
    printf("%d ", *it);

for (int *it = rbegin(v); it != rend(v); it--)     // reverse
    printf("%d ", *it);
```

### Node-based containers (`LIST`, `SET`, `MAP`)

The iterator is `TYPE*` but **pointer arithmetic does not work**. Use `next()` and `prev()`:

```c
LIST(int) lst = new_list(int);
for (int i = 0; i < 5; i++) push_back(lst, i);

for (int *it = begin(lst); it != end(lst); it = next(it))    // forward
    printf("%d ", *it);

for (int *it = rbegin(lst); it != rend(lst); it = prev(it))  // reverse
    printf("%d ", *it);
```

### Hash-based containers (`UNORDERED_SET`, `UNORDERED_MAP`)

Same rules as node-based: use `next()` and `prev()`. Both forward **and** reverse iteration are supported:

```c
UNORDERED_SET(int) h = new_unordered_set(int);
for (int i = 1; i <= 5; i++) insert(h, i);

for (int *it = begin(h); it != end(h); it = next(it))   // forward (bucket order)
    printf("%d ", *it);

for (int *it = rbegin(h); it != rend(h); it = prev(it)) // reverse (reverse bucket order)
    printf("%d ", *it);
```

### Summary

| Function | `VECTOR`, `DEQUE` | `LIST`, `SET`, `MAP` | `UNORDERED_SET`, `UNORDERED_MAP` |
|----------|-------------------|----------------------|----------------------------------|
| `begin` | ✅ | ✅ | ✅ |
| `end` | ✅ | ✅ | ✅ |
| `rbegin` | ✅ | ✅ | ✅ |
| `rend` | ✅ | ✅ | ✅ |
| `next` | (use `it++`) | ✅ required | ✅ required |
| `prev` | (use `it--`) | ✅ required | ✅ required |

---

## Iterator Invalidation

Read-only operations never invalidate iterators. The table below summarizes invalidation behavior for mutating operations.

| Container | After insertion | After erasure |
|-----------|----------------|---------------|
| `VECTOR` | If reallocation: **all**. Otherwise: at and after insertion point | Erased element and all after it |
| `DEQUE` | If reallocation: **all**. Otherwise: at and after insertion point | Erased element and all after it |
| `LIST` | **None** | Only the erased node |
| `SET` | **None** | Only the erased node |
| `MAP` | **None** | Only the erased node |
| `UNORDERED_SET` | If rehash: **all**. Otherwise: none | Only the erased node |
| `UNORDERED_MAP` | If rehash: **all**. Otherwise: none | Only the erased node |
| `STACK` | N/A | N/A |
| `QUEUE` | N/A | N/A |
| `PRIORITY_QUEUE` | N/A | N/A |

---

## Built-in Comparators

Comparators are required by `SET`, `MAP`, and `PRIORITY_QUEUE` but are **optional** — omitting the argument uses the built-in default for the type.

| Comparator | Type | Order |
|------------|------|-------|
| `COMPARE(TYPE)` | any primitive | Ascending (default) |
| `IntCmp` | `int` | Descending (max at top for `PRIORITY_QUEUE`) |
| `FloatCmp` | `float` | Descending |
| `DoubleCmp` | `double` | Descending |
| `StringCmp` | `char*` | Descending lexicographic |

```c
SET(int) s1 = new_set(int);              // default comparator (optional)
SET(int) s2 = new_set(int, NULL);        // same
SET(int) s3 = new_set(int, COMPARE(int)); // explicit ascending

MAP(int) m = new_map(int, double);       // default comparator (optional)

PRIORITY_QUEUE(int) pq = new_priority_queue(int);        // default (max-heap)
PRIORITY_QUEUE(int) pq2 = new_priority_queue(int, IntCmp); // explicit max-heap
```

---

## stdlib Compatibility

Contiguous containers (`VECTOR`, `DEQUE`) store elements in a single flat memory block, making them directly compatible with standard C library functions:

```c
#include "opencstl.h"

int IntAscCmp(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main() {
    VECTOR(int) v = new_vector(int);
    push_back(v, 5);
    push_back(v, 1);
    push_back(v, 3);

    qsort(v, size(v), sizeof(int), IntAscCmp);

    int key = 3;
    int *result = bsearch(&key, v, size(v), sizeof(int), IntAscCmp);
    if (result)
        printf("found %d at index %td\n", *result, result - v);

    destroy(v);
    return 0;
}
```

Node-based containers (`LIST`, `SET`, `MAP`, `UNORDERED_SET`, `UNORDERED_MAP`) are **not** compatible with `qsort` or `bsearch`.

---

## Function Table

✅ = supported, ❌ = not supported

### Constructor / Destructor

| Function | `VECTOR` | `DEQUE` | `LIST` | `SET` | `MAP` | `UNORDERED_SET` | `UNORDERED_MAP` | `STACK` | `QUEUE` | `PRIORITY_QUEUE` |
|----------|:--------:|:-------:|:------:|:-----:|:-----:|:---------------:|:---------------:|:-------:|:-------:|:----------------:|
| `new_*` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| `destroy` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |

### Element Access

| Function | `VECTOR` | `DEQUE` | `LIST` | `SET` | `MAP` | `UNORDERED_SET` | `UNORDERED_MAP` | `STACK` | `QUEUE` | `PRIORITY_QUEUE` |
|----------|:--------:|:-------:|:------:|:-----:|:-----:|:---------------:|:---------------:|:-------:|:-------:|:----------------:|
| `v[i]` (operator[]) | ✅ | ✅ | ❌ | ❌ | ❌ | ✅ (bucket) | ❌ | ❌ | ❌ | ❌ |
| `front` | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ❌ |
| `back` | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ❌ |
| `top` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ❌ | ✅ |
| `first` | ❌ | ❌ | ❌ | ❌ | ✅ | ❌ | ✅ | ❌ | ❌ | ❌ |
| `second` | ❌ | ❌ | ❌ | ❌ | ✅ | ❌ | ✅ | ❌ | ❌ | ❌ |

### Iterators

| Function | `VECTOR` | `DEQUE` | `LIST` | `SET` | `MAP` | `UNORDERED_SET` | `UNORDERED_MAP` | `STACK` | `QUEUE` | `PRIORITY_QUEUE` |
|----------|:--------:|:-------:|:------:|:-----:|:-----:|:---------------:|:---------------:|:-------:|:-------:|:----------------:|
| `begin` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| `end` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| `rbegin` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| `rend` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| `next` | (use `it++`) | (use `it++`) | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| `prev` | (use `it--`) | (use `it--`) | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |

### Capacity

| Function | `VECTOR` | `DEQUE` | `LIST` | `SET` | `MAP` | `UNORDERED_SET` | `UNORDERED_MAP` | `STACK` | `QUEUE` | `PRIORITY_QUEUE` |
|----------|:--------:|:-------:|:------:|:-----:|:-----:|:---------------:|:---------------:|:-------:|:-------:|:----------------:|
| `empty` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| `size` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| `capacity` | ✅ | ✅ | ❌ | ❌ | ❌ | ✅ | ✅ | ❌ | ❌ | ❌ |

### Modifiers

| Function | `VECTOR` | `DEQUE` | `LIST` | `SET` | `MAP` | `UNORDERED_SET` | `UNORDERED_MAP` | `STACK` | `QUEUE` | `PRIORITY_QUEUE` |
|----------|:--------:|:-------:|:------:|:-----:|:-----:|:---------------:|:---------------:|:-------:|:-------:|:----------------:|
| `assign` | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| `push_back` | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| `pop_back` | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| `push_front` | ❌ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| `pop_front` | ❌ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| `push` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| `pop` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| `insert` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| `erase` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| `resize` | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| `clear` | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |

### Lookup

| Function | `VECTOR` | `DEQUE` | `LIST` | `SET` | `MAP` | `UNORDERED_SET` | `UNORDERED_MAP` | `STACK` | `QUEUE` | `PRIORITY_QUEUE` |
|----------|:--------:|:-------:|:------:|:-----:|:-----:|:---------------:|:---------------:|:-------:|:-------:|:----------------:|
| `find` | ✅ O(n) | ✅ O(n) | ✅ O(n) | ✅ O(log n) | ✅ O(log n) | ✅ O(1) avg | ✅ O(1) avg | ❌ | ❌ | ❌ |

---

## Choosing the Right Container

```
Do you need key-value pairs?
├── YES → Do you need sorted order?
│         ├── YES  → MAP              (O(log n), sorted iteration)
│         └── NO   → UNORDERED_MAP   (O(1) avg, rbegin/rend supported)
└── NO  → Do you need only unique keys (set-like)?
          ├── YES → Do you need sorted order?
          │         ├── YES  → SET              (O(log n), sorted iteration)
          │         └── NO   → UNORDERED_SET    (O(1) avg, rbegin/rend/[] supported)
          └── NO  → What kind of sequence?
                    ├── LIFO (stack behavior)            → STACK
                    ├── FIFO (queue behavior)            → QUEUE
                    ├── Priority-ordered access           → PRIORITY_QUEUE
                    ├── Fast front AND back operations    → DEQUE
                    ├── O(1) insert/erase anywhere        → LIST
                    └── Random access + stdlib compat     → VECTOR (default choice)
```

### Quick Comparison

| Requirement | Best choice |
|-------------|-------------|
| Fast random access (`v[i]`) | `VECTOR`, `DEQUE` |
| `qsort` / `bsearch` compatible | `VECTOR`, `DEQUE` |
| O(1) insert/erase at front | `DEQUE`, `LIST` |
| O(1) insert/erase anywhere (with iterator) | `LIST` |
| Sorted unique keys | `SET` |
| Sorted key-value pairs | `MAP` |
| Fast membership test + bucket inspection | `UNORDERED_SET` |
| Fast key-value lookup | `UNORDERED_MAP` |
| LIFO semantics | `STACK` |
| FIFO semantics | `QUEUE` |
| Always access largest element | `PRIORITY_QUEUE` |

---

## Example

```c
#include "opencstl.h"

int main() {
    // VECTOR: resizable array with [] access
    VECTOR(int) v = new_vector(int);
    for (int i = 0; i < 5; i++) push_back(v, i);
    v[2] = 99;
    for (int *it = begin(v); it != end(v); it++) printf("[%d]", *it);
    puts("");
    destroy(v);

    // LIST: doubly-linked, O(1) insert/erase anywhere
    LIST(int) lst = new_list(int);
    push_front(lst, 1);
    push_back(lst, 2);
    push_front(lst, 0);
    for (int *it = begin(lst); it != end(lst); it = next(it)) printf("[%d]", *it);
    puts("");
    destroy(lst);

    // SET: sorted unique keys (comparator optional)
    SET(int) s = new_set(int);
    int vals[] = {5, 3, 8, 3, 1};
    for (int i = 0; i < 5; i++) insert(s, vals[i]);
    for (int *it = begin(s); it != end(s); it = next(it)) printf("[%d]", *it);
    puts("");
    destroy(s);

    // MAP: sorted key-value pairs (comparator optional)
    MAP(int) m = new_map(int, char*);
    insert(m, 2, "two");
    insert(m, 1, "one");
    insert(m, 3, "three");
    for (int *it = begin(m); it != end(m); it = next(it))
        printf("[%d:%s]", first(it), second(it, char*));
    puts("");
    destroy(m);

    // UNORDERED_SET: hash set with rbegin/rend/prev support
    UNORDERED_SET(int) h = new_unordered_set(int);
    for (int i = 1; i <= 5; i++) insert(h, i);
    // Reverse bucket order traversal
    for (int *it = rbegin(h); it != rend(h); it = prev(it)) printf("[%d]", *it);
    puts("");
    destroy(h);

    // STACK + QUEUE: adaptors
    STACK(int) stk = new_stack(int);
    QUEUE(int) que = new_queue(int);
    for (int i = 0; i < 4; i++) { push(stk, i); push(que, i); }
    while (!empty(stk)) { printf("[%d]", top(stk));   pop(stk); } puts("");
    while (!empty(que)) { printf("[%d]", front(que));  pop(que); } puts("");
    destroy(stk);
    destroy(que);

    // PRIORITY_QUEUE: max-heap (comparator optional)
    PRIORITY_QUEUE(int) pq = new_priority_queue(int);
    for (int i = 0; i < 5; i++) push(pq, i);
    while (!empty(pq)) { printf("[%d]", top(pq)); pop(pq); } puts("");
    destroy(pq);

    return 0;
}
```

**Output:**
```
[0][1][99][3][4]
[0][1][2]
[1][3][5][8]
[1:one][2:two][3:three]
[reverse bucket order]
[3][2][1][0]
[0][1][2][3]
[4][3][2][1][0]
```

---

## See Also

| Document | Description |
|----------|-------------|
| [`VECTOR`](__vector.md) | Resizable contiguous array |
| [`DEQUE`](__deque.md) | Double-ended resizable array |
| [`LIST`](__list.md) | Doubly-linked list |
| [`SET`](__set.md) | Sorted unique keys |
| [`MAP`](__map.md) | Sorted key-value pairs |
| [`UNORDERED_SET`](__unordered_set.md) | Hash-based unique keys; `rbegin`/`rend`/`[]`/`capacity` supported |
| [`UNORDERED_MAP`](__unordered_map.md) | Hash-based key-value pairs; `rbegin`/`rend`/`prev` supported |
| [`STACK`](__stack.md) | LIFO adaptor |
| [`QUEUE`](__queue.md) | FIFO adaptor |
| [`PRIORITY_QUEUE`](__priority_queue.md) | Max-heap adaptor |
| [GitHub](https://github.com/springkim/OpenCSTL) | Source code and issue tracker |
