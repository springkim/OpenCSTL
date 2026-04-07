# VECTOR

Defined in header [`vector.h`](../opencstl/vector.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define VECTOR(TYPE)      TYPE*
#define new_vector(TYPE)  cstl_vector(TYPE)
```

`VECTOR` is a sequence container that encapsulates dynamic size arrays.

The elements are stored **contiguously**, which means that elements can be accessed not only through iterators, but also using offsets on regular pointers to elements, or directly via the `[]` subscript operator — just like a plain C array. This also means that a pointer to an element of a `VECTOR` may be passed to any standard C function that expects a pointer to an array (e.g. `qsort`, `bsearch`).

The storage of the vector is handled automatically, being expanded as needed. Vectors usually occupy more space than static arrays, because more memory is allocated to handle future growth. This way a vector does not need to reallocate each time an element is inserted, but only when the additional memory is exhausted. The total amount of allocated memory can be queried using [`capacity()`](#capacity).

The complexity (efficiency) of common operations on vectors is as follows:

- Random access — constant **O(1)**.
- Insertion or removal of elements at the end — amortized constant **O(1)**.
- Insertion or removal of elements at an arbitrary position — linear in the distance to the end of the vector **O(n)**.

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

### `VECTOR(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the dynamic array. |

Expands to `TYPE*`. Used for variable declaration only; initialization must be done separately with `new_vector`.

```c
VECTOR(int)   v;   // expands to: int*  v;
VECTOR(char*) sv;  // expands to: char** sv;
```

### `new_vector(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the dynamic array. |

Creates and returns a new empty vector. The returned pointer must be released with [`destroy`](#destroy) when no longer needed.

```c
VECTOR(int) v = new_vector(int);
// ... use v ...
destroy(v);
```

---

## Iterator Invalidation

| Operation | Iterators Invalidated |
|-----------|-----------------------|
| All read-only operations | Never |
| `assign`, `clear` | Always |
| `push_back` | If reallocation occurs: all. Otherwise: only `end(v)` |
| `pop_back` | The erased element and `end(v)` |
| `insert` | If reallocation occurs: all. Otherwise: from insertion point to `end(v)` |
| `erase` | The erased element(s) and all elements after them (including `end(v)`) |
| `resize` | If reallocation occurs: all. Otherwise: only `end(v)` and erased elements |
| `destroy` | All |

---

## Functions

### Constructor / Destructor

#### `new_vector`

```c
VECTOR(TYPE) new_vector(TYPE);
```

Constructs an empty vector with no elements. Allocates initial internal storage.

**Return value:** A pointer to the newly created vector.

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

Destroys the vector, releasing all allocated memory. Must be called for every vector created with `new_vector`. After calling `destroy`, the pointer `v` is invalid.

```c
VECTOR(int) v = new_vector(int);
push_back(v, 1);
push_back(v, 2);
destroy(v);
// v is no longer valid
```

---

### Element Access

#### `operator[]`

```c
TYPE v[i];
```

Accesses the element at position `i` (0-based). Returns a reference that can be read or written. Behavior is undefined if `i >= size(v)`.

Available for `VECTOR` and `DEQUE` only. Not available for `LIST`.

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

Returns the value of the first element. Calling `front` on an empty vector is undefined behavior.

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

Returns the value of the last element. Calling `back` on an empty vector is undefined behavior.

```c
VECTOR(int) v = new_vector(int);
push_back(v, 10);
push_back(v, 20);
push_back(v, 30);
printf("%d\n", back(v));   // 30
destroy(v);
```

---

### Iterators

For `VECTOR`, the iterator type is `TYPE*`. Iterator arithmetic (`it++`, `it--`, `it + n`) works identically to plain C pointer arithmetic because elements are stored contiguously.

#### `begin`

```c
TYPE* begin(VECTOR(TYPE) v);
```

Returns an iterator (pointer) to the first element. Equivalent to `v` itself. Increment with `it++` to advance to the next element.

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

Returns an iterator to the element past the last element. Must not be dereferenced. Used as the termination condition in forward iteration.

---

#### `rbegin`

```c
TYPE* rbegin(VECTOR(TYPE) v);
```

Returns a reverse iterator pointing to the last element. Decrement with `it--` to advance in reverse order.

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

Returns a reverse iterator to the position before the first element. Must not be dereferenced. Used as the termination condition in reverse iteration.

---

### Capacity

#### `empty`

```c
bool empty(VECTOR(TYPE) v);
```

Returns `true` if the vector contains no elements, `false` otherwise.

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

Returns the number of elements currently stored in the vector.

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i);
printf("%zu\n", size(v));   // 5
destroy(v);
```

---

#### `capacity`

```c
size_t capacity(VECTOR(TYPE) v);
```

Returns the number of elements the vector can hold without reallocation. Always `>= size(v)`.

```c
VECTOR(int) v = new_vector(int);
for (int i = 0; i < 5; i++)
    push_back(v, i);
printf("size:     %zu\n", size(v));      // 5
printf("capacity: %zu\n", capacity(v));  // >= 5
destroy(v);
```

---

### Modifiers

#### `assign`

```c
void assign(VECTOR(TYPE) v, size_t n);
void assign(VECTOR(TYPE) v, size_t n, TYPE value);
```

Replaces the contents with `n` elements. If `value` is provided, all elements are initialized to `value`. Otherwise, elements are zero-initialized. Any existing content is discarded.

| Overload | Description |
|----------|-------------|
| `assign(v, n)` | Allocate `n` zero-initialized elements |
| `assign(v, n, value)` | Allocate `n` elements, each set to `value` |

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

Appends `value` to the end of the vector. If the internal buffer is full, it is automatically reallocated (all existing iterators/pointers may be invalidated).

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

Removes the last element of the vector. Calling `pop_back` on an empty vector is undefined behavior.

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

Inserts elements before the element pointed to by `pos`. `pos` is specified as `v + index`.

| Overload | Description |
|----------|-------------|
| `insert(v, pos, value)` | Insert a single `value` before `pos` |
| `insert(v, pos, n, value)` | Insert `n` copies of `value` before `pos` |

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

**Output:**
```
[  0][ 10][ 99][ 20][ 77][ 77][ 77][ 30][ 40]
```

---

#### `erase`

```c
void erase(VECTOR(TYPE) v, TYPE* pos);
void erase(VECTOR(TYPE) v, TYPE* first, TYPE* last);
```

Removes element(s) from the vector. All iterators at or after the erased position (including `end(v)`) are invalidated.

| Overload | Description |
|----------|-------------|
| `erase(v, pos)` | Remove the single element at `pos` |
| `erase(v, first, last)` | Remove all elements in the range `[first, last)` |

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

**Output:**
```
[  0][ 40]
```

---

#### `resize`

```c
void resize(VECTOR(TYPE) v, size_t n);
void resize(VECTOR(TYPE) v, size_t n, TYPE value);
```

Resizes the vector to contain exactly `n` elements.

- If `n > size(v)`: new elements are appended; set to `value` if provided, otherwise zero-initialized.
- If `n < size(v)`: elements beyond position `n` are removed.
- If `n == size(v)`: no change.

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

**Output:**
```
[  1][  2][  3][ 99][ 99][ 99]
size: 2
```

---

#### `clear`

```c
void clear(VECTOR(TYPE) v);
```

Erases all elements from the vector. After this call, `size(v)` returns `0`. The internal buffer is retained (capacity is unchanged), making `clear` efficient for reusing the same vector.

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

### Lookup

#### `find`

```c
TYPE* find(VECTOR(TYPE) v, TYPE value);
TYPE* find(VECTOR(TYPE) v, TYPE* start, TYPE value);
```

Performs a linear search for `value` in the vector.

| Overload | Description |
|----------|-------------|
| `find(v, value)` | Search the entire vector from the beginning |
| `find(v, start, value)` | Search starting from iterator `start` |

Returns a pointer (iterator) to the first matching element, or `end(v)` if not found.

**Complexity:** Linear — **O(n)**. For sorted vectors, prefer `bsearch` from the C standard library.

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

## stdlib Compatibility

Because `VECTOR` stores elements in contiguous memory, it is fully compatible with standard C library functions such as `qsort` and `bsearch`.

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

**Output:**
```
apple
banana
cherry
spring
bsearch: spring (index 3)
```

---

## Notes

- The handle type for `VECTOR` is `TYPE*` (contiguous sequence container).  
  Node-based containers (`LIST`, `SET`, `MAP`) use `TYPE**` as their handle type.
- `float` is supported, but `typedef`-aliased float types are not recognized. Use the `float` keyword directly.
- For `VECTOR(int*)` (a vector of pointers to inner vectors), each inner vector must be individually `destroy`ed before the outer vector is destroyed. See the 2D array pattern below.

**2D array (nested vectors):**

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

// Destroy inner vectors first, then the outer vector
for (int i = 0; i < (int)size(matrix); i++)
    destroy(matrix[i]);
destroy(matrix);
```

**Output:**
```
[  0][  0][  0][  0]
[  0][  1][  2][  3]
[  0][  2][  4][  6]
[  0][  3][  6][  9]
```

---

## Example

```c
#include "opencstl.h"

int main() {
    VECTOR(int) v = new_vector(int);

    // Append elements
    for (int i = 0; i < 5; i++)
        push_back(v, i);           // [0][1][2][3][4]

    // Subscript write
    v[2] = -1;                     // [0][1][-1][3][4]

    // Insert at position 1
    insert(v, v + 1, 99);          // [0][99][1][-1][3][4]

    // Erase at index 3
    erase(v, v + 3);               // [0][99][1][3][4]

    // Forward iteration
    for (int *it = begin(v); it != end(v); it++)
        printf("[%3d]", *it);
    puts("");

    // Reverse iteration
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

**Output:**
```
[  0][ 99][  1][  3][  4]
[  4][  3][  1][ 99][  0]
size:     5
capacity: 8
front:    0
back:     4
```

---

## Function Summary

### Constructor / Destructor

| Function | Signature | Description |
|----------|-----------|-------------|
| `new_vector` | `VECTOR(T) new_vector(T)` | Constructs an empty vector |
| `destroy` | `void destroy(v)` | Destroys the vector and frees all memory |

### Element Access

| Function | Signature | Description |
|----------|-----------|-------------|
| `operator[]` | `v[i]` | Access element at index `i` |
| `front` | `T front(v)` | Access the first element |
| `back` | `T back(v)` | Access the last element |

### Iterators

| Function | Signature | Description |
|----------|-----------|-------------|
| `begin` | `T* begin(v)` | Iterator to the first element |
| `end` | `T* end(v)` | Iterator past the last element |
| `rbegin` | `T* rbegin(v)` | Reverse iterator to the last element |
| `rend` | `T* rend(v)` | Reverse iterator before the first element |

### Capacity

| Function | Signature | Description |
|----------|-----------|-------------|
| `empty` | `bool empty(v)` | Checks whether the container is empty |
| `size` | `size_t size(v)` | Returns the number of elements |
| `capacity` | `size_t capacity(v)` | Returns the currently allocated storage capacity |

### Modifiers

| Function | Signature | Description |
|----------|-----------|-------------|
| `assign` | `void assign(v, n)` / `assign(v, n, val)` | Replace contents with `n` elements |
| `push_back` | `void push_back(v, val)` | Append an element to the end |
| `pop_back` | `void pop_back(v)` | Remove the last element |
| `insert` | `void insert(v, pos, val)` / `insert(v, pos, n, val)` | Insert element(s) before `pos` |
| `erase` | `void erase(v, pos)` / `erase(v, first, last)` | Erase element(s) |
| `resize` | `void resize(v, n)` / `resize(v, n, val)` | Change the number of stored elements |
| `clear` | `void clear(v)` | Erase all elements (capacity retained) |

### Lookup

| Function | Signature | Description |
|----------|-----------|-------------|
| `find` | `T* find(v, val)` / `find(v, start, val)` | Linear search; returns iterator or `end(v)` |

---

## See Also

| Container | Description |
|-----------|-------------|
| [`LIST`](../list/README.md) | Doubly-linked list; O(1) insert/erase anywhere via iterator, no `[]` access |
| [`DEQUE`](../deque/README.md) | Double-ended queue; `[]` access, O(1) insert/erase at both ends |
| [`SET`](../set/README.md) | Sorted set of unique keys; O(log n) insert, erase, find |
| [`MAP`](../map/README.md) | Sorted key-value store; O(log n) operations |
| [`STACK`](../stack/README.md) | LIFO adapter |
| [`QUEUE`](../queue/README.md) | FIFO adapter |
| [`PRIORITY_QUEUE`](../priority_queue/README.md) | Max-heap adapter |