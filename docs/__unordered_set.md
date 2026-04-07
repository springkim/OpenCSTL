# UNORDERED_SET

Defined in header [`unordered_set.h`](../opencstl/unordered_set.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define UNORDERED_SET(TYPE)                TYPE**
#define new_unordered_set(TYPE)            cstl_unordered_set(TYPE)
#define new_unordered_set(TYPE, hash_fn)   cstl_unordered_set(TYPE, hash_fn)
```

`UNORDERED_SET` is an associative container that stores **unique** elements organized into **hash buckets**, allowing average **O(1)** insertion, removal, and lookup. Unlike `SET`, there is no guaranteed sort order on iteration — however, `rbegin` / `rend` / `prev` **are** supported, visiting elements in reverse bucket order.

The internal layout is a flat hash table with open addressing. Subscript `h[i]` provides direct access to bucket slot `i` (zero means empty), and `capacity` returns the total number of slots.

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
4. [Comparison: SET vs UNORDERED\_SET](#comparison-set-vs-unordered_set)
5. [Notes](#notes)
6. [Example](#example)
7. [See Also](#see-also)

---

## Macro Parameters

### `UNORDERED_SET(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements stored in the unordered set. |

Expands to `TYPE**`. Shares the node-based handle convention with `SET` and `MAP`.

```c
UNORDERED_SET(int)   h;   // expands to: int**  h;
UNORDERED_SET(char*) sh;  // expands to: char*** sh;
```

### `new_unordered_set(TYPE [, hash_fn])`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements. |
| `hash_fn` *(optional)* | A hash function `size_t (*)(const void*)`. Omit or pass `NULL` to use the built-in default hash for the type. |

```c
UNORDERED_SET(int) h = new_unordered_set(int);          // default hash
UNORDERED_SET(int) h = new_unordered_set(int, NULL);    // same
// ... use h ...
destroy(h);
```

---

## Iterator Invalidation

`UNORDERED_SET` iterators may be invalidated by rehashing. Rehashing occurs automatically when the load factor exceeds an internal threshold.

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

#### `new_unordered_set`

```c
UNORDERED_SET(TYPE) new_unordered_set(TYPE);
UNORDERED_SET(TYPE) new_unordered_set(TYPE, hash_fn);
```

Constructs an empty unordered set.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
printf("%zu\n", size(h));   // 0
destroy(h);
```

---

#### `destroy`

```c
void destroy(UNORDERED_SET(TYPE) h);
```

Destroys the unordered set, freeing all internal storage.

---

### Element Access

#### `operator[]`

```c
TYPE h[i];
```

Direct access to bucket slot `i`. A slot value of `0` (zero) means the slot is empty. Use `capacity` to determine the total number of slots. This is a low-level inspection tool; for normal element access, use iterators or `find`.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
for (int i = 1; i < 10; i++) insert(h, i);

size_t cap = capacity(h);
for (int i = 0; i < (int)cap; i++) {
    int k = h[i];
    if (k == 0)
        printf("[---]");
    else
        printf("[%3d]", k);
}
puts("");
destroy(h);
```

---

### Iterators

For `UNORDERED_SET`, the iterator type is `TYPE*`. Because elements are node-based, you **must** use [`next()`](#next) to advance forward and [`prev()`](#prev) to advance in reverse — pointer arithmetic (`it++`, `it--`) does not work.

#### `begin`

```c
TYPE* begin(UNORDERED_SET(TYPE) h);
```

Returns an iterator to the first element in bucket traversal order.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 3);
insert(h, 1);
insert(h, 4);

for (int *it = begin(h); it != end(h); it = next(it))
    printf("[%d]", *it);   // order: bucket traversal (not sorted)
puts("");
destroy(h);
```

---

#### `end`

```c
TYPE* end(UNORDERED_SET(TYPE) h);
```

Returns a sentinel iterator marking the end of forward traversal.

---

#### `rbegin`

```c
TYPE* rbegin(UNORDERED_SET(TYPE) h);
```

Returns a reverse iterator to the last element in bucket traversal order. Use [`prev()`](#prev) to advance.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
for (int i = 1; i <= 5; i++) insert(h, i);

for (int *it = rbegin(h); it != rend(h); it = prev(it))
    printf("[%d]", *it);
puts("");
destroy(h);
```

---

#### `rend`

```c
TYPE* rend(UNORDERED_SET(TYPE) h);
```

Returns a sentinel reverse iterator before the first element. Must not be dereferenced.

---

#### `next`

```c
TYPE* next(TYPE* iter);
```

Advances to the next element in bucket traversal order.

```c
for (int *it = begin(h); it != end(h); it = next(it))
    printf("[%d]", *it);
```

---

#### `prev`

```c
TYPE* prev(TYPE* iter);
```

Moves to the previous element in bucket traversal order. Used for reverse traversal with `rbegin` / `rend`.

```c
for (int *it = rbegin(h); it != rend(h); it = prev(it))
    printf("[%d]", *it);
```

---

### Capacity

#### `empty`

```c
bool empty(UNORDERED_SET(TYPE) h);
```

Returns `true` if the set contains no elements.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
printf("%d\n", empty(h));   // 1
insert(h, 1);
printf("%d\n", empty(h));   // 0
destroy(h);
```

---

#### `size`

```c
size_t size(UNORDERED_SET(TYPE) h);
```

Returns the number of unique elements currently stored.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 10);
insert(h, 20);
insert(h, 10);   // duplicate — ignored
printf("%zu\n", size(h));   // 2
destroy(h);
```

---

#### `capacity`

```c
size_t capacity(UNORDERED_SET(TYPE) h);
```

Returns the total number of hash bucket slots currently allocated. Always `>= size(h)`. Used for low-level inspection of the internal table layout via `h[i]`.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
for (int i = 1; i < 10; i++) insert(h, i);

printf("size:     %zu\n", size(h));      // 9
printf("capacity: %zu\n", capacity(h));  // >= 9

// Inspect all slots
for (int i = 0; i < (int)capacity(h); i++) {
    int k = h[i];
    printf(k == 0 ? "[---]" : "[%3d]", k);
}
puts("");
destroy(h);
```

---

### Modifiers

#### `insert`

```c
void insert(UNORDERED_SET(TYPE) h, TYPE value);
```

Inserts `value` if it is not already present. Duplicates are silently ignored. Average **O(1)**.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 100);
insert(h, 200);
insert(h, 100);   // duplicate — no effect
printf("%zu\n", size(h));   // 2
destroy(h);
```

---

#### `erase`

```c
void erase(UNORDERED_SET(TYPE) h, TYPE* iter);
```

Removes the element pointed to by `iter`. Only the erased iterator becomes invalid. Average **O(1)**.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 1);
insert(h, 2);
insert(h, 3);

int *it = find(h, 2);
if (it != end(h))
    erase(h, it);

printf("size: %zu\n", size(h));   // 2
destroy(h);
```

---

#### `clear`

```c
void clear(UNORDERED_SET(TYPE) h);
```

Removes all elements. `size(h)` becomes `0`.

---

### Lookup

#### `find`

```c
TYPE* find(UNORDERED_SET(TYPE) h, TYPE value);
```

Searches for `value` using the hash function. Returns an iterator to the matching element, or `end(h)` if not found.

**Complexity:** Average **O(1)**, worst case **O(n)**.

```c
UNORDERED_SET(int) h = new_unordered_set(int);
insert(h, 10);
insert(h, 20);

int *it = find(h, 10);
if (it != end(h))
    printf("found: %d\n", *it);   // found: 10

it = find(h, 99);
if (it == end(h))
    printf("not found\n");

destroy(h);
```

---

## Comparison: SET vs UNORDERED\_SET

| Feature | `SET` | `UNORDERED_SET` |
|---------|-------|-----------------|
| Ordering | Sorted ascending | Bucket traversal order |
| Lookup | O(log n) | O(1) average |
| Insert | O(log n) | O(1) average |
| Erase | O(log n) | O(1) average |
| Iterator stability on insert | Always stable | May invalidate on rehash |
| `rbegin` / `rend` / `prev` | ✅ Supported | ✅ Supported |
| `capacity` / `operator[]` | ❌ Not supported | ✅ Supported (bucket inspection) |
| Min/Max in O(1) | ✅ `*begin` / `*rbegin` | ❌ Not O(1) |
| Best for | Ordered traversal, range queries | Fast membership testing |

---

## Notes

- `UNORDERED_SET` supports `rbegin`, `rend`, and `prev`, unlike typical hash sets. Reverse iteration visits elements in reverse bucket traversal order (not reverse sorted order).
- `h[i]` accesses bucket slot `i` directly. A value of `0` indicates an empty slot. This is useful for debugging the internal table layout.
- `capacity` returns the number of bucket slots, not the number of elements. Use `size` for element count.
- The comparator argument is **optional** — `new_unordered_set(int)` works without it.
- Duplicate insertions are silently ignored.

---

## Example

```c
#include "opencstl.h"

int main() {
    UNORDERED_SET(int) h = new_unordered_set(int);

    for (int i = 1; i < 100; i++)
        insert(h, i);

    // Inspect raw bucket layout
    size_t cap = capacity(h);
    for (int i = 0; i < (int)cap; i++) {
        int k = h[i];
        if (k == 0)
            printf("[---]");
        else
            printf("[%3d]", k);
    }
    puts("");

    printf("size:     %zu\n", size(h));
    printf("capacity: %zu\n", cap);

    // Reverse iteration (bucket reverse order)
    printf("Reverse:\n");
    for (int *it = rbegin(h); it != rend(h); it = prev(it))
        printf("[%3d]", *it);
    puts("");

    // Membership test
    int *it = find(h, 42);
    printf("contains 42: %s\n", (it != end(h)) ? "yes" : "no");

    // Erase one element
    it = find(h, 50);
    if (it != end(h)) erase(h, it);
    printf("size after erase: %zu\n", size(h));

    destroy(h);
    return 0;
}
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`SET`](__set.md) | Sorted unique set; O(log n) operations, sorted iteration |
| [`UNORDERED_MAP`](__unordered_map.md) | Hash-based key-value store |
| [`VECTOR`](__vector.md) | Contiguous sequence; sort manually with `qsort` |
