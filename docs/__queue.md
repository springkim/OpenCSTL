# QUEUE

Defined in header [`queue.h`](../opencstl/queue.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define QUEUE(TYPE)      TYPE*
#define new_queue(TYPE)  cstl_queue(TYPE)
```

`QUEUE` is a container adaptor that provides a **first-in, first-out (FIFO)** data structure. Elements are pushed to the back and popped from the front. Only the front and back elements are directly accessible.

`QUEUE` does not support iteration, random access, or `[]` subscript.

---

## Contents

1. [Macro Parameters](#macro-parameters)
2. [Functions](#functions)
   - [Constructor / Destructor](#constructor--destructor)
   - [Element Access](#element-access)
   - [Capacity](#capacity)
   - [Modifiers](#modifiers)
3. [Notes](#notes)
4. [Example](#example)
5. [See Also](#see-also)

---

## Macro Parameters

### `QUEUE(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements held by the queue. |

```c
QUEUE(int)   q;   // expands to: int*  q;
QUEUE(char*) sq;  // expands to: char** sq;
```

### `new_queue(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements held by the queue. |

Creates and returns a new empty queue. Must be released with [`destroy`](#destroy).

```c
QUEUE(int) q = new_queue(int);
// ... use q ...
destroy(q);
```

---

## Functions

### Constructor / Destructor

#### `new_queue`

```c
QUEUE(TYPE) new_queue(TYPE);
```

Constructs an empty queue.

```c
QUEUE(int) q = new_queue(int);
printf("%zu\n", size(q));   // 0
destroy(q);
```

---

#### `destroy`

```c
void destroy(QUEUE(TYPE) q);
```

Destroys the queue, freeing all allocated memory.

---

### Element Access

#### `front`

```c
TYPE front(QUEUE(TYPE) q);
```

Returns the value of the first element (the next element to be popped). Calling `front` on an empty queue is undefined behavior.

```c
QUEUE(int) q = new_queue(int);
push(q, 10);
push(q, 20);
push(q, 30);
printf("%d\n", front(q));   // 10
destroy(q);
```

---

#### `back`

```c
TYPE back(QUEUE(TYPE) q);
```

Returns the value of the last element (the most recently pushed). Calling `back` on an empty queue is undefined behavior.

```c
QUEUE(int) q = new_queue(int);
push(q, 10);
push(q, 20);
push(q, 30);
printf("%d\n", back(q));   // 30
destroy(q);
```

---

### Capacity

#### `empty`

```c
bool empty(QUEUE(TYPE) q);
```

Returns `true` if the queue contains no elements.

```c
QUEUE(int) q = new_queue(int);
printf("%d\n", empty(q));   // 1
push(q, 1);
printf("%d\n", empty(q));   // 0
destroy(q);
```

---

#### `size`

```c
size_t size(QUEUE(TYPE) q);
```

Returns the number of elements in the queue.

```c
QUEUE(int) q = new_queue(int);
push(q, 1);
push(q, 2);
push(q, 3);
printf("%zu\n", size(q));   // 3
destroy(q);
```

---

### Modifiers

#### `push`

```c
void push(QUEUE(TYPE) q, TYPE value);
```

Inserts `value` at the back of the queue. Amortized **O(1)**.

```c
QUEUE(int) q = new_queue(int);
for (int i = 0; i < 5; i++)
    push(q, i);
printf("%d\n", front(q));   // 0
printf("%d\n", back(q));    // 4
destroy(q);
```

---

#### `pop`

```c
void pop(QUEUE(TYPE) q);
```

Removes the front element. **O(1)**. Calling `pop` on an empty queue is undefined behavior.

```c
QUEUE(int) q = new_queue(int);
push(q, 10);
push(q, 20);
push(q, 30);

while (!empty(q)) {
    printf("[%3d]", front(q));
    pop(q);
}
puts("");
// [10][20][30]
destroy(q);
```

---

#### `clear`

```c
void clear(QUEUE(TYPE) q);
```

Removes all elements. `size(q)` becomes `0`. Internal storage is retained.

```c
QUEUE(int) q = new_queue(int);
push(q, 1);
push(q, 2);
clear(q);
printf("size: %zu\n", size(q));   // 0
destroy(q);
```

---

## Notes

- `QUEUE` does not support `begin`, `end`, `[]`, `top`, or any form of iteration.
- `QUEUE` and `STACK` both use `push` and `pop`, but have opposite access patterns:
  - `STACK`: last-in, first-out — access via `top`.
  - `QUEUE`: first-in, first-out — access via `front` and `back`.
- `PRIORITY_QUEUE` uses the same `push`/`pop`/`top` interface but orders elements by a comparator rather than insertion order.

---

## Example

```c
#include "opencstl.h"

int main() {
    STACK(int) s = new_stack(int);
    QUEUE(int) q = new_queue(int);

    for (int i = 0; i < 10; i++) {
        push(s, i);
        push(q, i);
    }

    // Stack: LIFO — prints 9, 8, 7 ... 0
    printf("Stack (LIFO):\n");
    for (int i = 0; i < 10; i++) {
        printf("[%3d]", top(s));
        pop(s);
    }
    puts("");

    // Queue: FIFO — prints 0, 1, 2 ... 9
    printf("Queue (FIFO):\n");
    for (int i = 0; i < 10; i++) {
        printf("[%3d]", front(q));
        pop(q);
    }
    puts("");

    destroy(s);
    destroy(q);
    return 0;
}
```

**Output:**
```
Stack (LIFO):
[  9][  8][  7][  6][  5][  4][  3][  2][  1][  0]
Queue (FIFO):
[  0][  1][  2][  3][  4][  5][  6][  7][  8][  9]
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`STACK`](../stack/README.md) | LIFO adapter; uses `top` |
| [`PRIORITY_QUEUE`](../priority_queue/README.md) | Heap-ordered adapter; greatest element at `top` |
| [`DEQUE`](../deque/README.md) | Double-ended sequence; full access at both ends |
| [`LIST`](../list/README.md) | Doubly-linked list; O(1) insert/erase at both ends |
