<p align="center">
  <img width="160" height="160" src="assets/OpenCSTL.png">
</p>

<h1 align="center">OpenCSTL</h1>
<p align="center">Open Source C Container Library like STL in C++</p>
<p align="center">C99 &nbsp;·&nbsp; C11 &nbsp;·&nbsp; C17 &nbsp;·&nbsp; C23</p>

<p align="center">
  <a href="README.md">English</a> &nbsp;|&nbsp;
  <a href="README.ko.md">한국어</a> &nbsp;|&nbsp;
  <a href="README.zh.md">中文</a>
</p>

---

## Why OpenCSTL?

- **Single-header** — Just `#include "opencstl.h"` and you're done. No build steps, no linking.
- **Identical C++ STL interface** — Same function names, same semantics, zero new API to learn. Auto-dispatch routes one function name to the correct implementation based on the container type. Variadic API: different parameter counts invoke different behaviors, just like C++ overloading.
- **Bracket access** — `VECTOR` and `DEQUE` support `v[i]` subscript access, just like C++ and plain C arrays.
- **Full iterator support** — Every container exposes `begin`, `end`, `rbegin`, and `rend` iterators — including unordered containers.
- **stdlib compatible** — Use standard C functions like `qsort` and `bsearch` directly on `VECTOR` and `DEQUE` data.
- **Zero boilerplate** — No forward declarations, no type registration macros, no initialization ceremony.
- **Cross-platform** — Works on Windows, macOS, and Linux. Compiler support: MSVC, MinGW64, GCC, Clang, and icx-cc.
- **Simpler than any alternative** — Far easier to use than other C container libraries, without breaking idiomatic C style.

## Installation

```bash
curl -LO "https://raw.githubusercontent.com/springkim/OpenCSTL/refs/heads/master/opencstl.h"
```

## Supported Platforms

| Compiler | Windows | macOS | Linux |
|----------|:-------:|:-----:|:-----:|
| MSVC (cl) | ✅ | | |
| GNU (gcc) | ✅ | ✅ | ✅ |
| LLVM (clang) | ✅ | ✅ | ✅ |
| MinGW64 | ✅ | | |
| Intel (icx-cc) | ✅ | ✅ | ✅ |

## Example

```c
#include "opencstl.h"
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
    return *((int *) a) - *((int *) b);
}

void example_deque() {
    DEQUE(int) q = new_deque(int);

    for (int i = 0; i < 10; i++) {
        push_back(q, rand32() % 100);
    }

    insert(q, q + 5, 777);    // Insert 777 at position 5
    insert(q, q + 8, 3, 999); // Insert three 999s at position 8

    for (int i = 0; i < size(q); i++) {
        printf("[%3d]", q[i]); // [] access, just like a plain C array
    }
    puts("");

    qsort(q, size(q), sizeof(int), cmp_int); // Works with standard C functions

    for (int i = 0; i < size(q); i++) {
        printf("[%3d]", q[i]);
    }
    puts("");

    destroy(q);
}

int main() {
    example_deque();
    return 0;
}
```

**Output:**
```
[ 38][ 26][ 13][ 83][ 19][777][ 95][ 78][999][999][999][ 87][ 80][ 86]
[ 13][ 19][ 26][ 38][ 78][ 80][ 83][ 86][ 87][ 95][777][999][999][999]
```

## Reference

> Optional parameters are shown in brackets `[...]`.
> Comparator and hash function arguments are always optional — omitting them uses the built-in default for the type.

### `VECTOR`
```
VECTOR(<type>) v = new_vector(<type>);

<type*> begin(v);          <type*> end(v);
<type*> rbegin(v);         <type*> rend(v);
size_t  size(v);           bool    empty(v);        size_t capacity(v);
<type>  front(v);          <type>  back(v);

void    push_back(v, value);
void    pop_back(v);
void    insert(v, iter, value);
void    insert(v, iter, N, value);
void    erase(v, iter);
void    erase(v, iter_begin, iter_end);
<type*> find(v, value);
<type*> find(v, iter, value);
void    resize(v, N [, value]);
void    assign(v, N [, value]);
void    clear(v);
void    destroy(v);
```

### `LIST`
```
LIST(<type>) l = new_list(<type>);

<type*> begin(l);          <type*> end(l);
<type*> rbegin(l);         <type*> rend(l);
<type*> next(iter);        <type*> prev(iter);    // use instead of it++/it--
size_t  size(l);           bool    empty(l);
<type>  front(l);          <type>  back(l);

void    push_back(l, value);      void    pop_back(l);
void    push_front(l, value);     void    pop_front(l);
void    insert(l, iter, value);
void    insert(l, iter, N, value);
void    erase(l, iter);
void    erase(l, iter_begin, iter_end);
<type*> find(l, value);
<type*> find(l, iter, value);
void    resize(l, N [, value]);
void    assign(l, N [, value]);
void    clear(l);
void    destroy(l);
```

### `DEQUE`
```
DEQUE(<type>) q = new_deque(<type>);

<type*> begin(q);          <type*> end(q);
<type*> rbegin(q);         <type*> rend(q);
size_t  size(q);           bool    empty(q);        size_t capacity(q);
<type>  front(q);          <type>  back(q);

void    push_back(q, value);      void    pop_back(q);
void    push_front(q, value);     void    pop_front(q);
void    insert(q, iter, value);
void    insert(q, iter, N, value);
void    erase(q, iter);
void    erase(q, iter_begin, iter_end);
<type*> find(q, value);
<type*> find(q, iter, value);
void    resize(q, N [, value]);
void    assign(q, N [, value]);
void    clear(q);
void    destroy(q);
```

### `SET`
```
SET(<type>) s = new_set(<type> [, comparator]);

<type*> begin(s);          <type*> end(s);
<type*> rbegin(s);         <type*> rend(s);
<type*> next(iter);        <type*> prev(iter);
size_t  size(s);           bool    empty(s);

void    insert(s, value);
void    erase(s, iter);
<type*> find(s, value);
void    clear(s);
void    destroy(s);
```

### `MAP`
```
MAP(<key_type>) m = new_map(<key_type>, <value_type> [, comparator]);

<key_type*>  begin(m);         <key_type*>  end(m);
<key_type*>  rbegin(m);        <key_type*>  rend(m);
<key_type*>  next(iter);       <key_type*>  prev(iter);
size_t       size(m);          bool         empty(m);

<key_type>   first(iter);                          // get key from iterator
<value_type> second(iter, <value_type>);           // get value from iterator

void         insert(m, key, value);
void         erase(m, iter);
<key_type*>  find(m, key);
void         clear(m);
void         destroy(m);
```

### `UNORDERED_SET`
```
UNORDERED_SET(<type>) h = new_unordered_set(<type> [, hash_fn]);

<type*> begin(h);          <type*> end(h);
<type*> rbegin(h);         <type*> rend(h);        // reverse bucket order
<type*> next(iter);        <type*> prev(iter);
size_t  size(h);           bool    empty(h);        size_t capacity(h);
h[i]                                               // direct bucket slot access (0 = empty)

void    insert(h, value);
void    erase(h, iter);
<type*> find(h, value);
void    clear(h);
void    destroy(h);
```

### `UNORDERED_MAP`
```
UNORDERED_MAP(<key_type>) m = new_unordered_map(<key_type>, <value_type> [, hash_fn]);

<key_type*>  begin(m);         <key_type*>  end(m);
<key_type*>  rbegin(m);        <key_type*>  rend(m);
<key_type*>  next(iter);       <key_type*>  prev(iter);
size_t       size(m);          bool         empty(m);        size_t capacity(m);

<key_type>   first(iter);
<value_type> second(iter, <value_type>);

void         insert(m, key, value);
void         erase(m, iter);
<key_type*>  find(m, key);
void         clear(m);
void         destroy(m);
```

### `STACK`
```
STACK(<type>) s = new_stack(<type>);

size_t size(s);    bool   empty(s);
<type> top(s);
void   push(s, value);
void   pop(s);
void   clear(s);
void   destroy(s);
```

### `QUEUE`
```
QUEUE(<type>) q = new_queue(<type>);

size_t size(q);    bool   empty(q);
<type> front(q);   <type> back(q);
void   push(q, value);
void   pop(q);
void   clear(q);
void   destroy(q);
```

### `PRIORITY_QUEUE`
```
PRIORITY_QUEUE(<type>) pq = new_priority_queue(<type> [, comparator]);

size_t size(pq);   bool   empty(pq);
<type> top(pq);
void   push(pq, value);   // O(log n)
void   pop(pq);           // O(log n)
void   clear(pq);
void   destroy(pq);
```

## Notes

- `float` is supported, but `typedef`-aliased float types are not recognized. Use the `float` keyword directly.
- Sequence containers (`VECTOR`, `DEQUE`) use `<type*>` as the handle type. Node-based containers (`LIST`, `SET`, `MAP`, `UNORDERED_SET`, `UNORDERED_MAP`) use `<type**>`.
- For node-based containers, always use `next(it)` and `prev(it)` to advance iterators — never `it++` or `it--`.
- `UNORDERED_SET` and `UNORDERED_MAP` support `rbegin`, `rend`, and `prev`, visiting elements in reverse bucket traversal order (not reverse sorted order).
- Comparator and hash function arguments are **optional**. Omitting them uses the built-in default for the type: `new_set(int)`, `new_map(int, double)`, `new_priority_queue(int)`, `new_unordered_set(int)`.

## Links

[![Hypercommit](https://img.shields.io/badge/Hypercommit-DB2475)](https://hypercommit.com/opencstl)

