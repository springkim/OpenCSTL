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
| GNU (gcc) |  | ✅ | ✅ |
| LLVM (clang) | ✅ | ✅ | ✅ |
| MinGW64 | ✅ | | |
| Intel (icx-cc) | ✅ |  | ✅ |

## Quick Example

```c
#include "opencstl.h"

#include <stdlib.h>

// Comparison function for sorting integers in ascending order.
// 정수를 오름차순으로 정렬하기 위한 비교 함수입니다.
// 整数を昇順にソートするための比較関数です。
// 用于将整数按升序排序的比较函数。
int cmp_int(const void *a, const void *b) {
    int x = *((const int *) a);
    int y = *((const int *) b);
    return (x > y) - (x < y); // -1, 0, 1 반환
}

void example_deque() {
    DEQUE(int) q = new_deque(int);

    for (int i = 0; i < 10; i++) {
        int val = rand32() % 100;
        // It uses the same names as the C++ STL.
        // C++의 STL과 동일한 이름을 사용합니다.
        // C++のSTLと同じ名前を使用します。
        // 名称与C++的STL相同。
        push_back(q, val);
    }

    // You can use `insert` in a similar way to overloading in STL.
    // STL의 오버로딩과 비슷하게 insert를 사용할 수 있습니다.
    // STLのオーバーロードと同様に、insertを使用できます。
    // 您可以像使用STL的重载一样使用insert。

    // Inserts 777 at the 5th position.
    // 5번째 자리에 777을 삽입합니다.
    // 5番目の位置に777を挿入します。
    // 在第5个位置插入777。
    insert(q, q + 5, 777);

    // Inserts three 999s at the 8th position.
    // 8번째 자리에 3개의 999를 삽입합니다.
    // 8番目の位置に999を3つ挿入します。
    // 在第8个位置插入3个999。
    insert(q, q + 8, 3, 999);

    for (int i = 0; i < size(q); i++) {
        // You can access elements in a deque or vector using [].
        // deque, vector에서 []로 접근이 가능합니다.
        // dequeやvectorでは[]でアクセスできます。
        // 可以使用[]访问deque或vector中的元素。
        printf("[%3d]", q[i]);
    }
    puts("");

    // You can use existing C functions.
    // 기존 C의 함수를 사용할 수 있습니다.
    // 既存のC関数を使用できます。
    // 可以使用现有的C函数。
    qsort(q, size(q), sizeof(int), cmp_int);

    for (int i = 0; i < size(q); i++) {
        printf("[%3d]", q[i]);
    }
    puts("");
    // Use `destroy` to free up memory.
    // destroy를 이용해서 메모리를 해제합니다.
    // destroyを使用してメモリを解放します。
    // 使用destroy释放内存。
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

- `float` is supported on MSVC, but `typedef`-aliased float types are not recognized. Use the `float` keyword directly.
- Comparator and hash function arguments are **optional**. Omitting them uses the built-in default for the type.
- For node-based containers, always use `next(it)` / `prev(it)` to advance iterators — never `it++` or `it--`.
