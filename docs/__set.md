# SET

Defined in header [`set.h`](../opencstl/set.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define SET(TYPE)                  TYPE**
#define new_set(TYPE, comparator)  cstl_set(TYPE, comparator)
```

`SET` is an associative container that stores **unique** elements in **sorted order**. Sorting is determined by the comparator supplied at construction time. No two elements in a set may compare equal.

Internally, `SET` is implemented as a self-balancing binary search tree (BST), so insertion, removal, and lookup all run in **O(log n)**. Iteration over the set visits elements in ascending sorted order.

`SET` does **not** support `[]` subscript access. Traversal requires iterators and the [`next()`](#next) / [`prev()`](#prev) functions.

The complexity of common operations:

- Search, insertion, removal — **O(log n)**.
- In-order traversal — **O(n)**.
- Access to minimum / maximum — **O(1)** via `begin` / `rbegin`.

---

## Contents

1. [Macro Parameters](#macro-parameters)
2. [Iterator Invalidation](#iterator-invalidation)
3. [Functions](#functions)
   - [Constructor / Destructor](#constructor--destructor)
   - [Iterators](#iterators)
   - [Capacity](#capacity)
   - [Modifiers](#modifiers)
   - [Lookup](#lookup)
4. [Notes](#notes)
5. [Example](#example)
6. [See Also](#see-also)

---

## Macro Parameters

### `SET(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the set. |

Expands to `TYPE**`. This is the node-based handle type, shared by `LIST`, `SET`, and `MAP`.

```c
SET(int)   s;   // expands to: int**  s;
SET(char*) ss;  // expands to: char*** ss;
```

### `new_set(TYPE, comparator)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements. |
| `comparator` | A comparison function `int (*)(const void*, const void*)`. Same convention as `qsort`. May be `NULL` to use the default comparator for the type. |

```c
SET(int) s = new_set(int, NULL);         // default comparator
SET(int) s = new_set(int, COMPARE(int)); // explicit ascending comparator
```

---

## Iterator Invalidation

`SET` iterators are node-based. Insertion and erasure of **other** nodes do not invalidate existing iterators. Only the erased node's iterator becomes invalid.

| Operation | Iterators Invalidated |
|-----------|-----------------------|
| All read-only operations | Never |
| `insert` | None (existing iterators remain valid) |
| `erase` | Only the erased element |
| `clear` | All |
| `destroy` | All |

---

## Functions

### Constructor / Destructor

#### `new_set`

```c
SET(TYPE) new_set(TYPE, comparator);
```

Constructs an empty set with the given comparator.

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

Destroys the set, freeing all nodes and internal structure.

---

### Iterators

For `SET`, the iterator type is `TYPE*`. Because the set is node-based, you **must** use [`next()`](#next) and [`prev()`](#prev) to advance — pointer arithmetic (`it++`) does not work.

#### `begin`

```c
TYPE* begin(SET(TYPE) s);
```

Returns an iterator to the smallest element (in-order first). Returns `end(s)` if empty.

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

Returns a sentinel iterator past the largest element. Must not be dereferenced.

---

#### `rbegin`

```c
TYPE* rbegin(SET(TYPE) s);
```

Returns an iterator to the largest element (in-order last).

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

Returns a sentinel reverse iterator before the smallest element. Must not be dereferenced.

---

#### `next`

```c
TYPE* next(TYPE* iter);
```

Returns an iterator to the next element in sorted order.

```c
for (int *it = begin(s); it != end(s); it = next(it))
    printf("%d\n", *it);
```

---

#### `prev`

```c
TYPE* prev(TYPE* iter);
```

Returns an iterator to the previous element in sorted order.

```c
for (int *it = rbegin(s); it != rend(s); it = prev(it))
    printf("%d\n", *it);
```

---

### Capacity

#### `empty`

```c
bool empty(SET(TYPE) s);
```

Returns `true` if the set contains no elements.

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

Returns the number of unique elements in the set.

```c
SET(int) s = new_set(int, NULL);
insert(s, 1);
insert(s, 2);
insert(s, 2);   // duplicate — ignored
printf("%zu\n", size(s));   // 2
destroy(s);
```

---

### Modifiers

#### `insert`

```c
void insert(SET(TYPE) s, TYPE value);
```

Inserts `value` into the set if it is not already present. If an equivalent element exists, the insertion is silently ignored. **O(log n)**.

```c
SET(int) s = new_set(int, NULL);
insert(s, 3);
insert(s, 1);
insert(s, 4);
insert(s, 1);   // duplicate — ignored
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

Removes the element pointed to by `iter`. Only the erased iterator is invalidated. **O(log n)**.

```c
SET(int) s = new_set(int, NULL);
insert(s, 1);
insert(s, 2);
insert(s, 3);

int *it = find(s, 2);
if (it != end(s))
    erase(s, it);   // remove 2

for (int *p = begin(s); p != end(s); p = next(p))
    printf("[%3d]", *p);   // [  1][  3]
puts("");
destroy(s);
```

---

#### `clear`

```c
void clear(SET(TYPE) s);
```

Removes all elements. `size(s)` becomes `0`.

---

### Lookup

#### `find`

```c
TYPE* find(SET(TYPE) s, TYPE value);
```

Searches for `value` in the set using the tree structure. Returns an iterator to the matching element, or `end(s)` if not found.

**Complexity:** **O(log n)**.

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

## Notes

- The handle type for `SET` is `TYPE**` (node-based). Do not use `it++` — use `next(it)`.
- Duplicate values are silently ignored on `insert`. Use `find` before inserting to detect whether a value is already present.
- Elements are always traversed in **ascending** sorted order via `begin` → `end`. Use `rbegin` → `rend` for descending order.
- `SET` does not have `front` / `back` — use `*begin(s)` and `*rbegin(s)` to access min/max.

---

## Example

```c
#include "opencstl.h"

int main() {
    SET(int) s = new_set(int, NULL);

    // Insert with duplicates — duplicates are ignored
    for (int i = 0; i < 10; i++)
        insert(s, rand() % 100);

    printf("size: %zu\n", size(s));

    // Ascending order
    for (int *it = begin(s); it != end(s); it = next(it))
        printf("[%3d]", *it);
    puts("");

    // Descending order
    for (int *it = rbegin(s); it != rend(s); it = prev(it))
        printf("[%3d]", *it);
    puts("");

    // Find and erase
    int *it = find(s, *begin(s));   // find the minimum
    if (it != end(s)) {
        printf("erasing min: %d\n", *it);
        erase(s, it);
    }

    destroy(s);
    return 0;
}
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`MAP`](../map/README.md) | Sorted key-value store; same tree structure as `SET` |
| [`UNORDERED_SET`](../unordered_set/README.md) | Hash-based set; O(1) average lookup, no ordering |
| [`VECTOR`](../vector/README.md) | Unsorted sequence; use with `qsort` + `bsearch` for sorted access |
| [`PRIORITY_QUEUE`](../priority_queue/README.md) | Heap; O(1) max access but no full iteration |
