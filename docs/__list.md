# LIST

Defined in header [`list.h`](../opencstl/list.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define LIST(TYPE)     TYPE**
#define new_list(TYPE) cstl_list(TYPE)
```

`LIST` is a sequence container that supports constant time insertion and removal of elements from anywhere in the container. Fast random access is **not** supported.

Elements are not stored contiguously — `LIST` is a doubly-linked list. Each element lives in its own dynamically allocated node, linked to its predecessor and successor. Because of this, `[]` subscript access and standard C functions (`qsort`, `bsearch`) are **not** available. Traversal must be done with iterators using [`next()`](#next) and [`prev()`](#prev).

The complexity (efficiency) of common operations on lists is as follows:

- Accessing the first or last element — constant **O(1)**.
- Insertion or removal of elements (given an iterator) — constant **O(1)**.
- Search (linear scan) — **O(n)**.
- No random access.

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
4. [Notes](#notes)
5. [Example](#example)
6. [See Also](#see-also)

---

## Macro Parameters

### `LIST(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the list. |

Expands to `TYPE**`. The handle is a pointer-to-pointer because each node is heap-allocated and linked. This is different from `VECTOR` (`TYPE*`).

```c
LIST(int)   lst;   // expands to: int**  lst;
LIST(char*) slst;  // expands to: char*** slst;
```

### `new_list(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the list. |

Creates and returns a new empty list. The returned handle must be released with [`destroy`](#destroy) when no longer needed.

```c
LIST(int) lst = new_list(int);
// ... use lst ...
destroy(lst);
```

---

## Iterator Invalidation

Unlike `VECTOR`, insertion and erasure in `LIST` do **not** invalidate any existing iterators, because nodes are independent heap objects.

| Operation | Iterators Invalidated |
|-----------|-----------------------|
| All read-only operations | Never |
| `assign`, `clear` | All |
| `push_back`, `push_front` | None |
| `pop_back`, `pop_front` | Only the erased element |
| `insert` | None (other iterators remain valid) |
| `erase` | Only the erased element(s) |
| `resize` | Only erased elements (if shrinking) |
| `destroy` | All |

---

## Functions

### Constructor / Destructor

#### `new_list`

```c
LIST(TYPE) new_list(TYPE);
```

Constructs an empty doubly-linked list.

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

Destroys the list, freeing all nodes and internal bookkeeping memory. Must be called for every list created with `new_list`.

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
destroy(lst);
```

---

### Element Access

#### `front`

```c
TYPE front(LIST(TYPE) lst);
```

Returns the value of the first element. Calling `front` on an empty list is undefined behavior.

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

Returns the value of the last element. Calling `back` on an empty list is undefined behavior.

```c
LIST(int) lst = new_list(int);
push_back(lst, 10);
push_back(lst, 20);
printf("%d\n", back(lst));   // 20
destroy(lst);
```

---

### Iterators

For `LIST`, the iterator type is `TYPE*`. Unlike `VECTOR`, pointer arithmetic (`it++`, `it--`) does **not** work because elements are not contiguous. You must use [`next()`](#next) and [`prev()`](#prev) to advance the iterator.

#### `begin`

```c
TYPE* begin(LIST(TYPE) lst);
```

Returns an iterator to the first element. Returns `end(lst)` if the list is empty.

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

Returns a sentinel iterator representing the position past the last element. Must not be dereferenced.

---

#### `rbegin`

```c
TYPE* rbegin(LIST(TYPE) lst);
```

Returns a reverse iterator pointing to the last element. Use [`prev()`](#prev) to advance backward.

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

Returns a sentinel reverse iterator before the first element. Must not be dereferenced.

---

#### `next`

```c
TYPE* next(TYPE* iter);
```

Advances the iterator to the next element and returns it. This is the only correct way to move forward through a `LIST`.

```c
for (int *it = begin(lst); it != end(lst); it = next(it))
    printf("%d\n", *it);
```

---

#### `prev`

```c
TYPE* prev(TYPE* iter);
```

Moves the iterator to the previous element and returns it. Used for reverse traversal.

```c
for (int *it = rbegin(lst); it != rend(lst); it = prev(it))
    printf("%d\n", *it);
```

---

### Capacity

#### `empty`

```c
bool empty(LIST(TYPE) lst);
```

Returns `true` if the list contains no elements.

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

Returns the number of elements currently in the list.

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 2);
printf("%zu\n", size(lst));   // 2
destroy(lst);
```

---

### Modifiers

#### `assign`

```c
void assign(LIST(TYPE) lst, size_t n);
void assign(LIST(TYPE) lst, size_t n, TYPE value);
```

Replaces the contents with `n` elements. Elements are set to `value` if provided, otherwise zero-initialized.

| Overload | Description |
|----------|-------------|
| `assign(lst, n)` | `n` zero-initialized elements |
| `assign(lst, n, value)` | `n` elements each set to `value` |

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

Appends `value` to the end of the list. **O(1)**. No reallocation occurs.

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

Removes the last element. **O(1)**. Calling on an empty list is undefined behavior.

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

Inserts `value` at the beginning of the list. **O(1)**.

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

Removes the first element. **O(1)**. Calling on an empty list is undefined behavior.

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

Inserts element(s) before the node pointed to by `pos`. Does not invalidate any existing iterators.

| Overload | Description |
|----------|-------------|
| `insert(lst, pos, value)` | Insert a single element before `pos` |
| `insert(lst, pos, n, value)` | Insert `n` copies of `value` before `pos` |

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 3);

int *it = begin(lst);
it = next(it);               // point to 3
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

Removes element(s) from the list. Only the erased iterator(s) are invalidated.

| Overload | Description |
|----------|-------------|
| `erase(lst, pos)` | Remove the element at `pos` |
| `erase(lst, first, last)` | Remove all elements in `[first, last)` |

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

Resizes the list to contain exactly `n` elements.

- If `n > size(lst)`: new elements are appended (zero or `value`).
- If `n < size(lst)`: excess elements are removed from the end.

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

Removes all elements. `size(lst)` becomes `0`. All nodes are freed.

```c
LIST(int) lst = new_list(int);
push_back(lst, 1);
push_back(lst, 2);
clear(lst);
printf("size: %zu\n", size(lst));   // 0
destroy(lst);
```

---

### Lookup

#### `find`

```c
TYPE* find(LIST(TYPE) lst, TYPE value);
TYPE* find(LIST(TYPE) lst, TYPE* start, TYPE value);
```

Performs a linear search for `value`. Returns an iterator to the first matching element, or `end(lst)` if not found.

**Complexity:** Linear — **O(n)**.

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

## Notes

- The handle type for `LIST` is `TYPE**` (node-based container).  
  Contiguous containers (`VECTOR`, `DEQUE`) use `TYPE*`.
- Do **not** use `it++` or `it--` on a list iterator — use `next(it)` and `prev(it)` instead.
- `LIST` does not support `[]` subscript access or `capacity()`.
- `LIST` is not compatible with `qsort` or `bsearch` since elements are not contiguous.

---

## Example

```c
#include "opencstl.h"

int main() {
    LIST(int) lst = new_list(int);

    for (int i = 0; i < 10; i++) {
        int val = 10 - i;
        push_back(lst, val);
    }

    // Forward traversal
    for (int *it = begin(lst); it != end(lst); it = next(it))
        printf("[%3d]", *it);
    puts("");

    // Reverse traversal
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

**Output:**
```
[ 10][  9][  8][  7][  6][  5][  4][  3][  2][  1]
[  1][  2][  3][  4][  5][  6][  7][  8][  9][ 10]
size:  10
front: 10
back:  1
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`VECTOR`](../vector/README.md) | Contiguous dynamic array; O(1) random access via `[]` |
| [`DEQUE`](../deque/README.md) | Double-ended queue; `[]` access, O(1) insert/erase at both ends |
| [`STACK`](../stack/README.md) | LIFO adapter |
| [`QUEUE`](../queue/README.md) | FIFO adapter |
