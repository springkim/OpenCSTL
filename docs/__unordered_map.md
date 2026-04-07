# UNORDERED_MAP

Defined in header [`unordered_map.h`](../opencstl/unordered_map.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define UNORDERED_MAP(KEY_TYPE)                            KEY_TYPE**
#define new_unordered_map(KEY_TYPE, VALUE_TYPE)            cstl_unordered_map(KEY_TYPE, VALUE_TYPE)
#define new_unordered_map(KEY_TYPE, VALUE_TYPE, hash_fn)   cstl_unordered_map(KEY_TYPE, VALUE_TYPE, hash_fn)
```

`UNORDERED_MAP` is an associative container that stores **key-value pairs** organized into **hash buckets**, allowing average **O(1)** insertion, removal, and lookup. Unlike `MAP`, `UNORDERED_MAP` does not maintain sorted order.

Reverse iteration is supported via `rbegin` / `rend` / `prev`, visiting entries in reverse bucket traversal order. Subscript access and `capacity` are also available for low-level bucket inspection.

The complexity of common operations:

- Lookup, insertion, removal — average **O(1)**, worst case **O(n)**.
- Full traversal — **O(capacity)**.

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
4. [Comparison: MAP vs UNORDERED\_MAP](#comparison-map-vs-unordered_map)
5. [Notes](#notes)
6. [Example](#example)
7. [See Also](#see-also)

---

## Macro Parameters

### `UNORDERED_MAP(KEY_TYPE)`

| Parameter | Description |
|-----------|-------------|
| `KEY_TYPE` | The type of the keys stored in the map. |

Expands to `KEY_TYPE**`. The value type is captured internally at construction time.

```c
UNORDERED_MAP(int)   m;   // expands to: int**  m;
UNORDERED_MAP(char*) sm;  // expands to: char*** sm;
```

### `new_unordered_map(KEY_TYPE, VALUE_TYPE [, hash_fn])`

| Parameter | Description |
|-----------|-------------|
| `KEY_TYPE` | The type of the keys. |
| `VALUE_TYPE` | The type of the values associated with each key. |
| `hash_fn` *(optional)* | A hash function `size_t (*)(const void*)`. Omit or pass `NULL` to use the built-in default hash. |

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);         // default hash
UNORDERED_MAP(int) m = new_unordered_map(int, char*, NULL);   // same
// ... use m ...
destroy(m);
```

---

## Iterator Invalidation

`UNORDERED_MAP` iterators may be invalidated by rehashing. Rehashing occurs automatically when the load factor exceeds an internal threshold.

| Operation | Iterators Invalidated |
|-----------|-----------------------|
| All read-only operations | Never |
| `insert` (no rehash) | None |
| `insert` (rehash occurs) | All |
| `erase` | Only the erased element |
| `clear` | All |
| `destroy` | All |

---

## Functions

### Constructor / Destructor

#### `new_unordered_map`

```c
UNORDERED_MAP(KEY_TYPE) new_unordered_map(KEY_TYPE, VALUE_TYPE);
UNORDERED_MAP(KEY_TYPE) new_unordered_map(KEY_TYPE, VALUE_TYPE, hash_fn);
```

Constructs an empty unordered map.

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

Destroys the unordered map, freeing all internal storage.

---

### Element Access

#### `first`

```c
KEY_TYPE first(KEY_TYPE* iter);
```

Returns the **key** of the pair pointed to by `iter`.

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

Returns the **value** of the pair pointed to by `iter`. The `VALUE_TYPE` argument is required for type deduction.

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
insert(m, 1, "apple");
insert(m, 2, "banana");

for (int *it = begin(m); it != end(m); it = next(it))
    printf("%d -> %s\n", first(it), second(it, char*));

destroy(m);
```

---

### Iterators

For `UNORDERED_MAP`, the iterator type is `KEY_TYPE*`. Because elements are node-based, you **must** use [`next()`](#next) and [`prev()`](#prev) to advance — pointer arithmetic (`it++`, `it--`) does not work.

#### `begin`

```c
KEY_TYPE* begin(UNORDERED_MAP(KEY_TYPE) m);
```

Returns an iterator to the first entry in bucket traversal order.

---

#### `end`

```c
KEY_TYPE* end(UNORDERED_MAP(KEY_TYPE) m);
```

Returns a sentinel iterator marking the end of forward traversal.

---

#### `rbegin`

```c
KEY_TYPE* rbegin(UNORDERED_MAP(KEY_TYPE) m);
```

Returns a reverse iterator to the last entry in bucket traversal order. Use [`prev()`](#prev) to advance.

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

Returns a sentinel reverse iterator before the first element. Must not be dereferenced.

---

#### `next`

```c
KEY_TYPE* next(KEY_TYPE* iter);
```

Advances to the next entry in bucket traversal order.

---

#### `prev`

```c
KEY_TYPE* prev(KEY_TYPE* iter);
```

Moves to the previous entry in bucket traversal order. Used for reverse iteration.

```c
for (int *it = rbegin(m); it != rend(m); it = prev(it))
    printf("%d -> %s\n", first(it), second(it, char*));
```

---

### Capacity

#### `empty`

```c
bool empty(UNORDERED_MAP(KEY_TYPE) m);
```

Returns `true` if the map contains no entries.

---

#### `size`

```c
size_t size(UNORDERED_MAP(KEY_TYPE) m);
```

Returns the number of key-value pairs in the map.

---

#### `capacity`

```c
size_t capacity(UNORDERED_MAP(KEY_TYPE) m);
```

Returns the total number of hash bucket slots currently allocated. Always `>= size(m)`.

---

### Modifiers

#### `insert`

```c
void insert(UNORDERED_MAP(KEY_TYPE) m, KEY_TYPE key, VALUE_TYPE value);
```

Inserts a key-value pair. If the key already exists, the existing entry is **not** modified. Average **O(1)**.

```c
UNORDERED_MAP(int) m = new_unordered_map(int, char*);
insert(m, 0, "zero");
insert(m, 1, "one");
insert(m, 0, "ZERO");   // duplicate key — ignored

int *it = find(m, 0);
printf("%s\n", second(it, char*));   // zero (unchanged)
destroy(m);
```

---

#### `erase`

```c
void erase(UNORDERED_MAP(KEY_TYPE) m, KEY_TYPE* iter);
```

Removes the entry pointed to by `iter`. Only the erased iterator becomes invalid. Average **O(1)**.

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

Removes all entries. `size(m)` becomes `0`.

---

### Lookup

#### `find`

```c
KEY_TYPE* find(UNORDERED_MAP(KEY_TYPE) m, KEY_TYPE key);
```

Searches for `key` using the hash function. Returns an iterator to the matching entry, or `end(m)` if not found.

**Complexity:** Average **O(1)**, worst case **O(n)**.

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

## Comparison: MAP vs UNORDERED\_MAP

| Feature | `MAP` | `UNORDERED_MAP` |
|---------|-------|-----------------|
| Ordering | Sorted by key (ascending) | Bucket traversal order |
| Lookup | O(log n) | O(1) average |
| Insert | O(log n) | O(1) average |
| Erase | O(log n) | O(1) average |
| Iterator stability on insert | Always stable | May invalidate on rehash |
| `rbegin` / `rend` / `prev` | ✅ Supported | ✅ Supported |
| `capacity` | ❌ Not supported | ✅ Supported |
| Min/Max key in O(1) | ✅ `begin` / `rbegin` | ❌ Not O(1) |
| Best for | Ordered traversal, sorted output | Fast key-based lookup |

---

## Notes

- `UNORDERED_MAP` supports `rbegin`, `rend`, and `prev`. Reverse iteration visits entries in reverse bucket traversal order (not reverse sorted order).
- The `hash_fn` argument is **optional** — `new_unordered_map(int, char*)` works without it.
- Inserting a duplicate key is silently ignored. There is no `insert_or_assign`. To update a value, use `erase` then `insert`.
- The `VALUE_TYPE` must remain consistent across all operations on the same map instance.

---

## Example

```c
#include "opencstl.h"

int main() {
    UNORDERED_MAP(int) m = new_unordered_map(int, char*);

    char *names[] = {"Alice", "Bob", "Carol", "Dave", "Eve"};
    for (int i = 0; i < 5; i++)
        insert(m, i, names[i]);

    printf("size: %zu\n", size(m));   // 5

    // Forward traversal (bucket order)
    printf("Forward:\n");
    for (int *it = begin(m); it != end(m); it = next(it))
        printf("  %d -> %s\n", first(it), second(it, char*));

    // Reverse traversal (reverse bucket order)
    printf("Reverse:\n");
    for (int *it = rbegin(m); it != rend(m); it = prev(it))
        printf("  %d -> %s\n", first(it), second(it, char*));

    // Lookup
    int *it = find(m, 2);
    if (it != end(m))
        printf("key 2 -> %s\n", second(it, char*));   // key 2 -> Carol

    // Erase
    it = find(m, 1);
    if (it != end(m)) erase(m, it);
    printf("size after erase: %zu\n", size(m));   // 4

    destroy(m);
    return 0;
}
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`MAP`](__map.md) | Sorted key-value store; O(log n) operations, ascending ordered iteration |
| [`UNORDERED_SET`](__unordered_set.md) | Hash-based set (keys only, no values) |
| [`SET`](__set.md) | Sorted unique set; O(log n) operations |
