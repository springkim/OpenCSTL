# STACK

Defined in header [`stack.h`](../opencstl/stack.h)  
*(included via `#include "opencstl.h"`)*

---

```c
#define STACK(TYPE)      TYPE*
#define new_stack(TYPE)  cstl_stack(TYPE)
```

`STACK` is a container adaptor that provides a **last-in, first-out (LIFO)** data structure. Elements are pushed onto and popped from the top of the stack. Only the top element is directly accessible.

`STACK` does not support iteration, random access, or `[]` subscript.

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

### `STACK(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements held by the stack. |

```c
STACK(int)   s;   // expands to: int*  s;
STACK(char*) ss;  // expands to: char** ss;
```

### `new_stack(TYPE)`

| Parameter | Description |
|-----------|-------------|
| `TYPE` | The type of the elements held by the stack. |

Creates and returns a new empty stack. Must be released with [`destroy`](#destroy).

```c
STACK(int) s = new_stack(int);
// ... use s ...
destroy(s);
```

---

## Functions

### Constructor / Destructor

#### `new_stack`

```c
STACK(TYPE) new_stack(TYPE);
```

Constructs an empty stack.

```c
STACK(int) s = new_stack(int);
printf("%zu\n", size(s));   // 0
destroy(s);
```

---

#### `destroy`

```c
void destroy(STACK(TYPE) s);
```

Destroys the stack, freeing all allocated memory.

---

### Element Access

#### `top`

```c
TYPE top(STACK(TYPE) s);
```

Returns the value of the top element (the most recently pushed). Calling `top` on an empty stack is undefined behavior.

```c
STACK(int) s = new_stack(int);
push(s, 10);
push(s, 20);
push(s, 30);
printf("%d\n", top(s));   // 30
destroy(s);
```

---

### Capacity

#### `empty`

```c
bool empty(STACK(TYPE) s);
```

Returns `true` if the stack contains no elements.

```c
STACK(int) s = new_stack(int);
printf("%d\n", empty(s));   // 1
push(s, 1);
printf("%d\n", empty(s));   // 0
destroy(s);
```

---

#### `size`

```c
size_t size(STACK(TYPE) s);
```

Returns the number of elements in the stack.

```c
STACK(int) s = new_stack(int);
push(s, 1);
push(s, 2);
push(s, 3);
printf("%zu\n", size(s));   // 3
destroy(s);
```

---

### Modifiers

#### `push`

```c
void push(STACK(TYPE) s, TYPE value);
```

Pushes `value` onto the top of the stack. Amortized **O(1)**.

```c
STACK(int) s = new_stack(int);
for (int i = 0; i < 5; i++)
    push(s, i);
printf("%d\n", top(s));   // 4
destroy(s);
```

---

#### `pop`

```c
void pop(STACK(TYPE) s);
```

Removes the top element. **O(1)**. Calling `pop` on an empty stack is undefined behavior.

```c
STACK(int) s = new_stack(int);
push(s, 10);
push(s, 20);
push(s, 30);

while (!empty(s)) {
    printf("[%3d]", top(s));
    pop(s);
}
puts("");
// [30][20][10]
destroy(s);
```

---

#### `clear`

```c
void clear(STACK(TYPE) s);
```

Removes all elements. `size(s)` becomes `0`. Internal storage is retained.

```c
STACK(int) s = new_stack(int);
push(s, 1);
push(s, 2);
clear(s);
printf("size: %zu\n", size(s));   // 0
destroy(s);
```

---

## Notes

- `STACK` does not support `begin`, `end`, `[]`, or any form of iteration.
- `STACK` and `QUEUE` both use `push` and `pop`, but differ in which end is accessed: `STACK` uses `top`, `QUEUE` uses `front`.
- The underlying storage grows automatically; no manual capacity management is required.

---

## Example

```c
#include "opencstl.h"

int main() {
    STACK(int) s = new_stack(int);

    for (int i = 0; i < 10; i++)
        push(s, i);

    printf("size: %zu\n", size(s));   // 10
    printf("top:  %d\n",  top(s));    // 9

    // Pop all elements (LIFO order: 9, 8, 7 ... 0)
    while (!empty(s)) {
        printf("[%3d]", top(s));
        pop(s);
    }
    puts("");

    destroy(s);
    return 0;
}
```

**Output:**
```
size: 10
top:  9
[  9][  8][  7][  6][  5][  4][  3][  2][  1][  0]
```

---

## See Also

| Container | Description |
|-----------|-------------|
| [`QUEUE`](../queue/README.md) | FIFO adapter; uses `front` and `back` |
| [`PRIORITY_QUEUE`](../priority_queue/README.md) | Max-heap adapter |
| [`VECTOR`](../vector/README.md) | Underlying sequence with full access |
| [`DEQUE`](../deque/README.md) | Double-ended sequence; supports both `push_front` and `push_back` |
