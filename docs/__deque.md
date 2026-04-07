# DEQUE

Defined in header [`deque.h`](../opencstl/deque.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define DEQUE(TYPE)      TYPE*
#define new_deque(TYPE)  cstl_deque(TYPE)
```

`DEQUE` (double-ended queue) is a sequence container that allows fast insertion and deletion at **both** the front and the back.

Like `VECTOR`, `DEQUE` supports `[]` subscript access and is compatible with standard C library functions (`qsort`, `bsearch`). Unlike `VECTOR`, inserting or removing at the **front** is also **O(1)** amortized, not O(n).

The complexity (efficiency) of common operations on deques is as follows:

- Random access — constant **O(1)**.
- Insertion or removal at the front or back — amortized constant **O(1)**.
- Insertion or removal in the middle — linear **O(n)**.

---

## Contents

1. [Macro Parameters](#macro-parameters)
2. [Iterator Invalidation](#iterator-invalidation)
3. [Functions](#functions)
   - [Constructor / Destructor](#constructor--destructor)
   - [Element Access](#element-access)
   - [Iterators](#iterators)
   - [Capacity](#capacity)
   - [Modifiers](#modifiers)
   - [Lookup](#lookup)
4. [stdlib Compatibility](#stdlib-compatibility)
5. [Notes](#notes)
6. [Example](#example)
7. [See Also](#see-also)

---

## Macro Parameters

### `DEQUE(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the deque. |

Expands to `TYPE*`. Shares the same handle type as `VECTOR`.

```c
DEQUE(int)   q;   // expands to: int*  q;
DEQUE(char*) sq;  // expands to: char** sq;
```

### `new_deque(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the deque. |

Creates and returns a new empty deque. Must be released with [`destroy`](#destroy).

```c
DEQUE(int) q = new_deque(int);
// ... use q ...
destroy(q);
```

---

## Iterator Invalidation

| Operation | Iterators Invalidated |
|-----------|-----------------------|
| All read-only operations | Never |
| `assign`, `clear` | Always |
| `push_back`, `push_front` | If reallocation occurs: all. Otherwise: only `end(q)` |
| `pop_back` | The erased element and `end(q)` |
| `pop_front` | The erased element and `begin(q)` |
| `insert` | If reallocation occurs: all. Otherwise: from insertion point to `end(q)` |
| `erase` | The erased element(s) and all elements after them |
| `resize` | If reallocation occurs: all. Otherwise: only erased/added elements |
| `destroy` | All |

---

## Functions

### Constructor / Destructor

#### `new_deque`

```c
DEQUE(TYPE) new_deque(TYPE);
```

Constructs an empty deque.

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

Destroys the deque and releases all allocated memory.

---

### Element Access

#### `operator[]`

```c
TYPE q[i];
```

Accesses the element at position `i` (0-based). Supports both read and write. Behavior is undefined if `i >= size(q)`.

```c
DEQUE(int) q = new_deque(int);
assign(q, 5, 0);
q[0] = 100;
q[4] = 999;
printf("%d %d\n", q[0], q[4]);   // 100 999
destroy(q);
```

---

#### `front`

```c
TYPE front(DEQUE(TYPE) q);
```

Returns the value of the first element.

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

Returns the value of the last element.

```c
DEQUE(int) q = new_deque(int);
push_back(q, 10);
push_front(q, 5);
printf("%d\n", back(q));   // 10
destroy(q);
```

---

### Iterators

For `DEQUE`, the iterator type is `TYPE*` and standard pointer arithmetic works the same as for `VECTOR`.

#### `begin`

```c
TYPE* begin(DEQUE(TYPE) q);
```

Returns an iterator to the first element.

---

#### `end`

```c
TYPE* end(DEQUE(TYPE) q);
```

Returns an iterator past the last element.

---

#### `rbegin`

```c
TYPE* rbegin(DEQUE(TYPE) q);
```

Returns a reverse iterator to the last element.

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

Returns a reverse iterator before the first element.

---

### Capacity

#### `empty`

```c
bool empty(DEQUE(TYPE) q);
```

Returns `true` if the deque contains no elements.

---

#### `size`

```c
size_t size(DEQUE(TYPE) q);
```

Returns the number of elements currently stored in the deque.

---

#### `capacity`

```c
size_t capacity(DEQUE(TYPE) q);
```

Returns the number of elements the deque can hold without reallocation.

---

### Modifiers

#### `assign`

```c
void assign(DEQUE(TYPE) q, size_t n);
void assign(DEQUE(TYPE) q, size_t n, TYPE value);
```

Replaces the contents with `n` elements, optionally initialized to `value`.

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

Appends `value` to the end. Amortized **O(1)**.

---

#### `pop_back`

```c
void pop_back(DEQUE(TYPE) q);
```

Removes the last element. **O(1)**. Undefined behavior on empty deque.

---

#### `push_front`

```c
void push_front(DEQUE(TYPE) q, TYPE value);
```

Inserts `value` at the front. Amortized **O(1)**.

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

Removes the first element. **O(1)**. Undefined behavior on empty deque.

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

Inserts element(s) before `pos`. `pos` is specified as `q + index`.

```c
DEQUE(int) q = new_deque(int);
for (int i = 0; i < 5; i++)
    push_back(q, i * 10);       // [0][10][20][30][40]

insert(q, q + 5, 777);          // [0][10][20][30][40][777]
insert(q, q + 8, 3, 999);       // out-of-bounds guard: appended

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

Removes element(s). Iterators at or after the erased position are invalidated.

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

**Output:**
```
[  0][ 40]
```

---

#### `resize`

```c
void resize(DEQUE(TYPE) q, size_t n);
void resize(DEQUE(TYPE) q, size_t n, TYPE value);
```

Resizes the deque to `n` elements. Appends or removes from the end as needed.

---

#### `clear`

```c
void clear(DEQUE(TYPE) q);
```

Removes all elements. `size(q)` becomes `0`. Capacity is retained.

---

### Lookup

#### `find`

```c
TYPE* find(DEQUE(TYPE) q, TYPE value);
TYPE* find(DEQUE(TYPE) q, TYPE* start, TYPE value);
```

Linear search. Returns an iterator to the first matching element, or `end(q)` if not found.

**Complexity:** **O(n)**.

---

## stdlib Compatibility

Because `DEQUE` stores elements contiguously, it supports `qsort` and `bsearch` directly.

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

    insert(q, q + 2, 777);    // insert 777 at position 2
    insert(q, q + 4, 3, 999); // insert three 999s at position 4

    for (int i = 0; i < (int)size(q); i++)
        printf("[%3d]", q[i]);
    puts("");

    qsort(q, size(q), sizeof(int), cmp_int);

    for (int i = 0; i < (int)size(q); i++)
        printf("[%3d]", q[i]);
    puts("");

    destroy(q);
    return 0;
}
```

---

## Notes

- `DEQUE` and `VECTOR` share the same handle type (`TYPE*`) and the same iterator semantics.
- `push_front` and `pop_front` are available on `DEQUE` but **not** on `VECTOR`.
- `float` is supported; `typedef`-aliased float types are not.

---

## Example

```c
#include "opencstl.h"

int main() {
    DEQUE(int) q = new_deque(int);

    for (int i = 1; i <= 5; i++)
        push_back(q, i);      // [1][2][3][4][5]

    push_front(q, 0);         // [0][1][2][3][4][5]
    pop_back(q);              // [0][1][2][3][4]

    insert(q, q + 2, 99);    // [0][1][99][2][3][4]

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

**Output:**
```
[  0][  1][ 99][  2][  3][  4]
front: 0
back:  4
size:  6
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`VECTOR`](../vector/README.md) | Dynamic array; O(1) at back only |
| [`LIST`](../list/README.md) | Doubly-linked list; O(1) insert/erase anywhere, no `[]` access |
| [`QUEUE`](../queue/README.md) | FIFO adapter (typically backed by deque) |
| [`STACK`](../stack/README.md) | LIFO adapter |
