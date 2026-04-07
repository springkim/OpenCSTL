# OpenCSTL

**Open Source C Container Library like STL in C++**

C99 · C11 · C17 · C23

---

## Why OpenCSTL?

- **Single-header** — Just `#include "opencstl.h"` and you're done. No build steps, no linking.
- **Identical C++ STL interface** — Same function names, same semantics, zero new API to learn. Auto-dispatch routes one function name to the correct implementation based on the container type.
- **Bracket access** — `VECTOR` and `DEQUE` support `v[i]` subscript access, just like C++ and plain C arrays.
- **Full iterator support** — Every container exposes `begin`, `end`, `rbegin`, and `rend` iterators — including unordered containers.
- **stdlib compatible** — Use standard C functions like `qsort` and `bsearch` directly on `VECTOR` and `DEQUE` data.
- **Zero boilerplate** — No forward declarations, no type registration macros, no initialization ceremony.
- **Cross-platform** — Windows, macOS, Linux. MSVC, MinGW64, GCC, Clang, icx-cc.

## Installation

```bash
curl -LO "https://raw.githubusercontent.com/springkim/OpenCSTL/refs/heads/master/opencstl.h"
```

Then include it in your source:

```c
#include "opencstl.h"
```

## Supported Platforms

| Compiler | Windows | macOS | Linux |
|----------|:-------:|:-----:|:-----:|
| MSVC (cl) | ✅ | | |
| GNU (gcc) | ✅ | ✅ | ✅ |
| LLVM (clang) | ✅ | ✅ | ✅ |
| MinGW64 | ✅ | | |
| Intel (icx-cc) | ✅ | ✅ | ✅ |

## Quick Example

```c
#include "opencstl.h"
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
    return *((int *) a) - *((int *) b);
}

int main() {
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
    return 0;
}
```

**Output:**
```
[ 38][ 26][ 13][ 83][ 19][777][ 95][ 78][999][999][999][ 87][ 80][ 86]
[ 13][ 19][ 26][ 38][ 78][ 80][ 83][ 86][ 87][ 95][777][999][999][999]
```

## Containers at a Glance

| Container | Category | Handle | `[]` | Iterator Advance |
|-----------|----------|--------|:----:|-----------------|
| [`VECTOR`](__vector.md) | Sequence | `T*` | ✅ | `it++` |
| [`LIST`](__list.md) | Sequence | `T**` | ❌ | `next(it)` |
| [`DEQUE`](__deque.md) | Sequence | `T*` | ✅ | `it++` |
| [`SET`](__set.md) | Associative | `T**` | ❌ | `next(it)` |
| [`MAP`](__map.md) | Associative | `K**` | ❌ | `next(it)` |
| [`UNORDERED_SET`](__unordered_set.md) | Unordered | `T**` | ✅ (bucket) | `next(it)` |
| [`UNORDERED_MAP`](__unordered_map.md) | Unordered | `K**` | ❌ | `next(it)` |
| [`STACK`](__stack.md) | Adaptor | `T*` | ❌ | — |
| [`QUEUE`](__queue.md) | Adaptor | `T*` | ❌ | — |
| [`PRIORITY_QUEUE`](__priority_queue.md) | Adaptor | `T*` | ❌ | — |

## Notes

- `float` is supported, but `typedef`-aliased float types are not recognized. Use the `float` keyword directly.
- Comparator and hash function arguments are **optional**. Omitting them uses the built-in default for the type.
- For node-based containers, always use `next(it)` / `prev(it)` to advance iterators — never `it++` or `it--`.
- `UNORDERED_SET` and `UNORDERED_MAP` support `rbegin`, `rend`, and `prev`.
