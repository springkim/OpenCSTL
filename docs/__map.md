# MAP

Defined in header [`map.h`](../opencstl/map.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define MAP(KEY_TYPE)                               KEY_TYPE**
#define new_map(KEY_TYPE, VALUE_TYPE, comparator)   cstl_map(KEY_TYPE, VALUE_TYPE, comparator)
```

`MAP` is an associative container that stores **key-value pairs** in **sorted key order**. Each key must be unique. Sorting and lookup are governed by the comparator supplied at construction time.

Internally, `MAP` is implemented as a self-balancing binary search tree (BST), so all key-based operations run in **O(log n)**. Iterators visit pairs in ascending key order.

`MAP` does **not** support `[]` subscript access. Traversal requires iterators with [`next()`](#next) / [`prev()`](#prev), and values are accessed through [`first()`](#first) / [`second()`](#second).

The complexity of common operations:

- Lookup by key — **O(log n)**.
- Insertion, removal — **O(log n)**.
- In-order traversal — **O(n)**.

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

### `MAP(KEY_TYPE)`

| Parameter | Description |
|-----------|-------------|
| `KEY_TYPE` | The type of the keys stored in the map. |

Expands to `KEY_TYPE**`. The value type is recorded internally at construction time and is not part of the handle type.

```c
MAP(int)   m;   // expands to: int**  m;  (values can be any type)
MAP(char*) sm;  // expands to: char*** sm;
```

### `new_map(KEY_TYPE, VALUE_TYPE, comparator)`

| Parameter | Description |
|-----------|-------------|
| `KEY_TYPE` | The type of the keys. |
| `VALUE_TYPE` | The type of the values associated with each key. |
| `comparator` | A comparison function `int (*)(const void*, const void*)`. Same convention as `qsort`. Pass `NULL` for the default comparator, or use `COMPARE(KEY_TYPE)`. |

```c
MAP(int) m = new_map(int, char*, NULL);
MAP(int) m = new_map(int, char*, COMPARE(int));
```

---

## Iterator Invalidation

`MAP` iterators are node-based. Insertion and erasure of **other** nodes never invalidate existing iterators.

| Operation | Iterators Invalidated |
|-----------|-----------------------|
| All read-only operations | Never |
| `insert` | None |
| `erase` | Only the erased element |
| `clear` | All |
| `destroy` | All |

---

## Functions

### Constructor / Destructor

#### `new_map`

```c
MAP(KEY_TYPE) new_map(KEY_TYPE, VALUE_TYPE, comparator);
```

Constructs an empty map. The value type is captured at construction time and used for memory layout.

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

Destroys the map, freeing all nodes and internal structure.

---

### Element Access

#### `first`

```c
KEY_TYPE first(KEY_TYPE* iter);
```

Returns the **key** of the key-value pair pointed to by `iter`.

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

Returns the **value** of the key-value pair pointed to by `iter`. The `VALUE_TYPE` argument is required for type deduction.

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 1, "apple");
insert(m, 2, "banana");

for (int *it = begin(m); it != end(m); it = next(it))
    printf("%d: %s\n", first(it), second(it, char*));

destroy(m);
```

---

### Iterators

For `MAP`, the iterator type is `KEY_TYPE*`. Because the map is node-based, you **must** use [`next()`](#next) and [`prev()`](#prev) — pointer arithmetic (`it++`) does not work.

#### `begin`

```c
KEY_TYPE* begin(MAP(KEY_TYPE) m);
```

Returns an iterator to the entry with the smallest key.

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

Returns a sentinel iterator past the largest key. Must not be dereferenced.

---

#### `rbegin`

```c
KEY_TYPE* rbegin(MAP(KEY_TYPE) m);
```

Returns an iterator to the entry with the largest key.

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

Returns a sentinel reverse iterator before the smallest key. Must not be dereferenced.

---

#### `next`

```c
KEY_TYPE* next(KEY_TYPE* iter);
```

Advances to the next entry in ascending key order.

---

#### `prev`

```c
KEY_TYPE* prev(KEY_TYPE* iter);
```

Moves to the previous entry in ascending key order (i.e. the next in descending order).

---

### Capacity

#### `empty`

```c
bool empty(MAP(KEY_TYPE) m);
```

Returns `true` if the map contains no key-value pairs.

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

Returns the number of key-value pairs currently in the map.

```c
MAP(int) m = new_map(int, int, NULL);
insert(m, 1, 10);
insert(m, 2, 20);
printf("%zu\n", size(m));   // 2
destroy(m);
```

---

### Modifiers

#### `insert`

```c
void insert(MAP(KEY_TYPE) m, KEY_TYPE key, VALUE_TYPE value);
```

Inserts a key-value pair. If the key already exists, the existing entry is **not** modified (no overwrite). **O(log n)**.

```c
MAP(int) m = new_map(int, char*, NULL);
insert(m, 0, "zero");
insert(m, 1, "one");
insert(m, 0, "ZERO");   // duplicate key — ignored

for (int *it = begin(m); it != end(m); it = next(it))
    printf("%d: %s\n", first(it), second(it, char*));
// 0: zero    (not overwritten)
// 1: one
destroy(m);
```

---

#### `erase`

```c
void erase(MAP(KEY_TYPE) m, KEY_TYPE* iter);
```

Removes the key-value pair pointed to by `iter`. Only the erased iterator is invalidated. **O(log n)**.

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

Removes all key-value pairs. `size(m)` becomes `0`.

---

### Lookup

#### `find`

```c
KEY_TYPE* find(MAP(KEY_TYPE) m, KEY_TYPE key);
```

Searches for `key` using the tree structure. Returns an iterator to the matching entry, or `end(m)` if not found.

**Complexity:** **O(log n)**.

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

## Notes

- The handle type for `MAP` is `KEY_TYPE**` (node-based). Do **not** use `it++` — use `next(it)`.
- Values are not accessible via the handle type. Use `second(iter, VALUE_TYPE)` to retrieve a value.
- Inserting a duplicate key is silently ignored. There is no `insert_or_assign`. Use `erase` + `insert` to update.
- Keys are traversed in **ascending** sorted order via `begin` → `end`. Use `rbegin` → `rend` for descending.
- The `VALUE_TYPE` must be consistent across all operations on the same map instance. Mixing types causes undefined behavior.

---

## Example

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

**Output:**
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

## See Also

| Container | Description |
|-----------|-------------|
| [`SET`](../set/README.md) | Sorted set of unique keys (no values) |
| [`UNORDERED_MAP`](../unordered_map/README.md) | Hash-based key-value store; O(1) average lookup, no ordering |
| [`VECTOR`](../vector/README.md) | Contiguous sequence; pair-like patterns with index as key |
