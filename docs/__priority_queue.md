# PRIORITY_QUEUE

Defined in header [`priority_queue.h`](../opencstl/priority_queue.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define PRIORITY_QUEUE(TYPE)                    TYPE*
#define new_priority_queue(TYPE, comparator)    cstl_priority_queue(TYPE, comparator)
```

`PRIORITY_QUEUE` is a container adaptor that provides a **heap-ordered** data structure. The element with the highest priority (as defined by the comparator) is always accessible at the top.

By default, using a descending comparator (e.g. `IntCmp`) makes the greatest element appear at the top — equivalent to `std::priority_queue` in C++. Supplying the opposite comparator produces a min-heap.

`PRIORITY_QUEUE` does not support iteration, random access, or `[]` subscript. Only the top element is accessible.

The complexity of common operations:

- Access the top element — constant **O(1)**.
- Insert an element — **O(log n)**.
- Remove the top element — **O(log n)**.

---

## Contents

1. [Macro Parameters](#macro-parameters)
2. [Comparators](#comparators)
3. [Functions](#functions)
   - [Constructor / Destructor](#constructor--destructor)
   - [Element Access](#element-access)
   - [Capacity](#capacity)
   - [Modifiers](#modifiers)
4. [Notes](#notes)
5. [Example](#example)
6. [See Also](#see-also)

---

## Macro Parameters

### `PRIORITY_QUEUE(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements held by the priority queue. |

```c
PRIORITY_QUEUE(int)   pq;   // expands to: int*  pq;
```

### `new_priority_queue(TYPE, comparator)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements. |
| `comparator` | A comparison function pointer of type `int (*)(const void*, const void*)`. Follows the same convention as `qsort`: negative if `a < b`, zero if equal, positive if `a > b`. A descending comparator (positive when `a > b`) yields a **max-heap**. |

```c
PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);
// ... use pq ...
destroy(pq);
```

---

## Comparators

OpenCSTL provides built-in comparators for common types:

| Comparator | Type | Order |
|------------|------|-------|
| `IntCmp` | `int` | Descending (max-heap) |
| `FloatCmp` | `float` | Descending (max-heap) |
| `DoubleCmp` | `double` | Descending (max-heap) |
| `StringCmp` | `char*` | Descending lexicographic (max-heap) |
| `COMPARE(TYPE)` | any | Default ascending comparator for the type |

To create a **min-heap**, define a custom comparator that returns the opposite sign:

```c
int IntCmpMin(const void *a, const void *b) {
    return *(int *)b - *(int *)a;   // ascending: smallest element at top
}

PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmpMin);
```

---

## Functions

### Constructor / Destructor

#### `new_priority_queue`

```c
PRIORITY_QUEUE(TYPE) new_priority_queue(TYPE, comparator);
```

Constructs an empty priority queue ordered by the given comparator.

```c
PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);
printf("%zu\n", size(pq));   // 0
destroy(pq);
```

---

#### `destroy`

```c
void destroy(PRIORITY_QUEUE(TYPE) pq);
```

Destroys the priority queue and frees all allocated memory.

---

### Element Access

#### `top`

```c
TYPE top(PRIORITY_QUEUE(TYPE) pq);
```

Returns the value of the highest-priority element without removing it. Calling `top` on an empty priority queue is undefined behavior.

```c
PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);
push(pq, 5);
push(pq, 1);
push(pq, 9);
push(pq, 3);
printf("%d\n", top(pq));   // 9 (max element)
destroy(pq);
```

---

### Capacity

#### `empty`

```c
bool empty(PRIORITY_QUEUE(TYPE) pq);
```

Returns `true` if the priority queue contains no elements.

```c
PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);
printf("%d\n", empty(pq));   // 1
push(pq, 42);
printf("%d\n", empty(pq));   // 0
destroy(pq);
```

---

#### `size`

```c
size_t size(PRIORITY_QUEUE(TYPE) pq);
```

Returns the number of elements in the priority queue.

```c
PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);
push(pq, 1);
push(pq, 2);
push(pq, 3);
printf("%zu\n", size(pq));   // 3
destroy(pq);
```

---

### Modifiers

#### `push`

```c
void push(PRIORITY_QUEUE(TYPE) pq, TYPE value);
```

Inserts `value` into the priority queue and re-heapifies. **O(log n)**.

```c
PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);
push(pq, 3);
push(pq, 1);
push(pq, 4);
push(pq, 1);
push(pq, 5);
printf("%d\n", top(pq));   // 5
destroy(pq);
```

---

#### `pop`

```c
void pop(PRIORITY_QUEUE(TYPE) pq);
```

Removes the top (highest-priority) element and re-heapifies. **O(log n)**. Calling `pop` on an empty priority queue is undefined behavior.

```c
PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);
for (int i = 0; i < 5; i++)
    push(pq, i);

while (!empty(pq)) {
    printf("[%3d]", top(pq));
    pop(pq);
}
puts("");
// [  4][  3][  2][  1][  0]
destroy(pq);
```

---

#### `clear`

```c
void clear(PRIORITY_QUEUE(TYPE) pq);
```

Removes all elements. `size(pq)` becomes `0`.

---

## Notes

- `PRIORITY_QUEUE` does not expose `front` or `back`; only `top` is available.
- The heap property is maintained internally — you cannot observe intermediate ordering through iteration.
- The comparator function signature must match `int (*)(const void*, const void*)`, identical to `qsort`.
- Unlike `QUEUE`, the order in which elements come out is determined by their priority, not their insertion order.

---

## Example

```c
#include "opencstl.h"

int main() {
    // Max-heap (default with IntCmp)
    PRIORITY_QUEUE(int) pq = new_priority_queue(int, IntCmp);

    for (int i = 0; i < 10; i++)
        push(pq, i);

    printf("Max-heap pop order:\n");
    while (!empty(pq)) {
        printf("[%3d]", top(pq));
        pop(pq);
    }
    puts("");
    destroy(pq);

    // Min-heap (custom comparator)
    int IntCmpMin(const void *a, const void *b) {
        return *(int *)b - *(int *)a;
    }
    PRIORITY_QUEUE(int) min_pq = new_priority_queue(int, IntCmpMin);

    for (int i = 9; i >= 0; i--)
        push(min_pq, i);

    printf("Min-heap pop order:\n");
    while (!empty(min_pq)) {
        printf("[%3d]", top(min_pq));
        pop(min_pq);
    }
    puts("");
    destroy(min_pq);

    return 0;
}
```

**Output:**
```
Max-heap pop order:
[  9][  8][  7][  6][  5][  4][  3][  2][  1][  0]
Min-heap pop order:
[  0][  1][  2][  3][  4][  5][  6][  7][  8][  9]
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`QUEUE`](../queue/README.md) | FIFO adapter; insertion-order access |
| [`STACK`](../stack/README.md) | LIFO adapter |
| [`SET`](../set/README.md) | Sorted unique set; supports full iteration |
