



<p align="center">
  <img width="1024" src="assets/blogo.png">
</p>





---


1. [Overview](#overview)
2. [Preview](#preview)
3. [Comparison](#comparison)
4. [Components](#components)
5. [Installation](#installation)
6. [How to use](#how-to-use)
7. [Performance](#performance)



## Overview


* OpenCSTL은 다른 C-STL라이브러리와 비교할 수 없을 정도로 다릅니다.
  * 단일 헤더 파일.
    * 번거로운 빌드 과정 없이 즉시 사용이 가능합니다.
  * C언어 코딩 스타일 그대로 사용이 가능합니다.
    * vector와 deque는 `[]`를 통해 원소 접근이 가능합니다.
    * `stdlib.h`의 `qsort`를 그대로 사용할 수 있습니다.
  * C++의 STL과 동일한 인터페이스를 제공합니다.
    * STL에 익숙하다면 쉽게 사용이 가능합니다.
    * 오버로딩을 지원합니다. STL의 오버로딩과 동일합니다.
    * `vector`, `list`, `queue`, `stack`, `deque`, `priority_queue`, `set`, `map`, `unordered_set`, `unordered_map` 이 구현되어 있습니다.
    * Iterator를 지원합니다.
  * 매크로 / `void*` 방식이 아닌 컨테이너 자료형 타입을 사용합니다.
    * 매크로 기반 라이브러리와 같은 전방 선언이 필요하지 않습니다.
      * 자료형마다 매번 새롭게 컨테이너 전체를 만들 필요가 없습니다.
      * 새로운 타입의 자료형이나 구조체도 바로 사용할 수 있습니다.
    * `void*`기반 라이브러리와 다르게 값을 직접 전달합니다.
      * 캐시 효율이 더 높고 빠른 속도와 적은 메모리 사용량이 OpenCSTL의 장점입니다.
      * 코드 가독성이 높으며 타입 안전성이 더 높습니다.
    * `_Generic` 기반 라이브러리는 새로운 타입을 사용하기 어렵습니다.
      * 전방 선언이 매번 필요하며 컴파일 속도가 느린 단점이 있습니다.
      * C11 이상에서만 동작합니다.
    * 컨테이너 Header를 사용하고 가변 인자 함수로 제네릭 컨테이너를 구현하였으며 매크로를 통한 Wrapping 방식을 사용합니다.
      * 매크로 기반 라이브러리와 같이 타입을 지정해서 사용하지만 전방 선언이나 init 작업이 필요하지 않습니다.
      * 내부적으로 `void*`를 사용하지만 OpenCSTL은 값 자체를 저장합니다. 따라서 캐시 히트율이 높고 빠른 속도를 보장합니다. 
  * 크로스플랫폼
    * C99, C11, C14, C17, C23을 모두 지원합니다.
    * `Windows`: `MSVC(2015,2017,2019,2022,2026)`, `MinGW64(gcc)`, `clang`, `tcc`를 지원합니다.
    * `Linux`: `gcc`, `clang`, `tcc`를 지원합니다.
    * `MacOS`: `gcc`, `clang`, `tcc`를 지원합니다.
  * 직관적이고 깔끔한 인터페이스.
    * `SET`, `MAP`,`UNORDERED_SET`,`UNORDERED_MAP`,`PRIORITY_QUEUE` 사용시 오버로딩으로 비교 함수를 지정하지 않아도 기본 비교함수인 `memcmp`로 동작합니다.
    * 완전히 새로운 방식의 구현체로 타 라이브러리에 비해 압도적으로 쉽고 깔끔한 인테피이스를 보장합니다.

* * *

## Preview

OpenCSTL과 대응하는 C++ STL 코드입니다.

| OpenCSTL을 사용한 C 코드                                | C++ STL 코드  |
| ----------------------------------------- | ---- |
| ![](./assets/codes/sample01_vector.c.webp) |  ![](./assets/codes/sample01_vector.cpp.webp)     |
| ![](./assets/codes/sample02_vector.c.webp) |  ![](./assets/codes/sample02_vector.cpp.webp)     |
| ![](./assets/codes/sample03_list.c.webp) |  ![](./assets/codes/sample03_list.cpp.webp)     |
| ![](./assets/codes/sample04_stack_queue.c.webp) |  ![](./assets/codes/sample04_stack_queue.cpp.webp)     |
| ![](./assets/codes/sample05_deque.c.webp) |  ![](./assets/codes/sample05_deque.cpp.webp)     |
| ![](./assets/codes/sample06_map.c.webp) |  ![](./assets/codes/sample06_map.cpp.webp)     |
| ![](./assets/codes/sample07_priority_queue.c.webp) |  ![](./assets/codes/sample07_priority_queue.cpp.webp)     |





## Comparison

|-|[OpenCSTL](https://github.com/springkim/OpenCSTL)|[STC](https://github.com/stclib/STC)|[mlib](https://github.com/P-p-H-d/mlib)|[CC](https://github.com/JacksonAllan/CC)|
|-|-|-|-|-|
|`[i]` 인덱스 접근|✅|❌|❌|❌|
|전방 선언 없이 사용|✅|❌|❌|✅|
|C99지원|✅|✅|✅|❌|
|Header Only|✅|✅|✅|✅|
|Single Header File|✅|❌|❌|✅|
|컨테이너별 동일한 인터페이스|✅|❌|❌|✅|



## Components



### Containers library

컨테이너 라이브러리는 C++ STL의 컨테이너와 동일합니다.





```markdown
## Containers
- `VECTOR(T)` — dynamic array
- `LIST(T)` — doubly linked list
- `DEQUE(T)` — double-ended queue
- `SET(T)` — sorted set (red-black tree)
- `MAP(K)` — sorted map (red-black tree)
- `UNORDERED_SET(T)` — hash set
- `STACK(T)` — stack
- `QUEUE(T)` — queue
- `QUEUE(T)` / `new_priority_queue` — priority queue (min/max heap)

## Algorithms
- `sort` — qsort
- `stable_sort` — stable sort
- `find` — linear search
- `LESS(T)` / `GREATER(T)` — built-in comparators

## Iterator
- `begin` / `end` — range iterator
- `rbegin` / `rend` — reverse iterator
- `next` / `prev` — iterator traversal

## Container Operations
- `push_back` / `pop_back` — back insert/remove
- `push_front` / `pop_front` — front insert/remove
- `push` / `pop` / `top` — stack/queue/priority_queue ops
- `insert` — single or range insert (overloaded)
- `erase` — single or range erase
- `assign` — fill assign (overloaded)
- `resize` — resize with optional fill value (overloaded)
- `size` — size queries
- `capacity` — capacity queries
- `empty` — empty check
- `front` / `back` — element access
- `first` / `second` — map key/value access
- `destroy` — memory free

## Map Operations
- `first(it)` — key access
- `second(it, T)` — value access

## Utilities
- `rand32` — random number generator (32bit unsigned)
- `rand64` - random number generator (64bit unsigned)
- `watch` / `now` / `duration` — timer
- `logging.debug` / `.info` / `.warning` / `.error` / `.critical` / `.fatal` / `.message` — 
## logging
- `opencstl_version` — version string
- `opencstl_env` — environment info
```



```mermaid

  flowchart LR
      T(types.h) --> E(error.h)
      D(defines.h) --> E(error.h)
      C(crossplatform.h) --> L(logging.h)
      E(error.h) --> V(vector.h)
      E(error.h) --> LST(list.h)
      E(error.h) --> S(stack.h)
      SALLOC(salloc.h) --> Q(queue.h)
      E(error.h) --> Q(queue.h)
      V(vector.h) --> Q(queue.h)
      E(error.h) --> TREE(tree.h)
      E(error.h) --> DQ(deque.h)
      E(error.h) --> H(hashtable.h)

      subgraph CONTAINERS[Containers]
          V(vector.h)
          LST(list.h)
          S(stack.h)
          Q(queue.h)
          TREE(tree.h)
          DQ(deque.h)
          H(hashtable.h)
      end

      C(crossplatform.h) --> OCSTL(opencstl.h)
      V(vector.h) --> OCSTL(opencstl.h)
      LST(list.h) --> OCSTL(opencstl.h)
      TREE(tree.h) --> OCSTL(opencstl.h)
      S(stack.h) --> OCSTL(opencstl.h)
      Q(queue.h) --> OCSTL(opencstl.h)
      DQ(deque.h) --> OCSTL(opencstl.h)
      H(hashtable.h) --> OCSTL(opencstl.h)
      ISORT(isort.h) --> MSORT(msort.h)
      TSORT(tsort.h) --> SORT(sort.h)
      MSORT(msort.h) --> SORT(sort.h)
      PDQSORT(pdqsort.h) --> SORT(sort.h)
      C(crossplatform.h) --> SORT(sort.h)
      SORT(sort.h) --> OCSTL(opencstl.h)
      VER(version.h) --> OCSTL(opencstl.h)
      FS(fstream.h) --> OCSTL(opencstl.h)
      ALGO(algorithm.h) --> OCSTL(opencstl.h)
      WATCH(watch.h) --> OCSTL(opencstl.h)
```





## Installation

```bash
curl -LO "https://raw.githubusercontent.com/springkim/OpenCSTL/refs/heads/master/opencstl.h"
```



## Supported Platforms

<p align="center">
    <img width="128" height="128" src="assets/windows11.png" >
    <img width="128" height="128" src="assets/macos.png" >
    <img width="128" height="128" src="assets/ubuntu_logo.png" >
</p>

<p align="center">
    <img width="96" height="96" src="assets/MSVC2026.png" >
    <img width="96" height="96" src="assets/MinGW64.png" >
    <img width="96" height="96" src="assets/LLVM.png" >
    <img width="96" height="96" src="assets/GNU.png" >
    <img width="96" height="96" src="assets/tcc.png" >
</p>

| Compiler | Windows | macOS | Linux |
|----------|:-------:|:-----:|:-----:|
| MSVC (cl) | ✅ | ❌ | ❌ |
| GNU / MinGW64 (gcc) | ✅ | ✅ | ✅ |
| LLVM (clang) | ✅ | ✅ | ✅ |
| Intel (icx-cc) | ✅ | ❌ | ✅ |
|  Tiny C Compiler (tcc) | ✅ | ✅ | ✅ |

## Document

[Document](./docs/README.md)



## Example



```c
#include "opencstl.h"

void example_vector() {
    VECTOR(int) arr = new_vector(int);

    for (int i = 0; i < 10; i++) {
        push_back(arr, i);
    }
    for (int *it = begin(arr); it != end(arr); it++) {
        printf("[%3d]", *it);
    }
    puts("");
    destroy(arr);
}

void example_list() {
    LIST(int) arr = new_list(int);
    for (int i = 10; i >= 0; i--) {
        push_back(arr, i);
    }
    for (int *it = begin(arr); it != end(arr); it = next(it)) {
        printf("[%3d]", *it);
    }
    puts("");
    destroy(arr);
}

void example_set() {
    SET(int) set = new_set(int);
    for (int i = 0; i < 10; i++) {
        insert(set, rand()%100);
    }
    for (int *it = begin(set); it != end(set); it = next(it)) {
        printf("[%3d]", *it);
    }
    puts("");
    destroy(set);
}

void example_map() {
    char *containers[] = {
        "vector", "list", "set", "map", "stack", "queue", "priority_queue", "unordered_map", "unordered_set"
    };
    size_t len = sizeof(containers) / sizeof(containers[0]);
    printf("Number of elements: %llu\n", len);
    MAP(int) d = new_map(int, char*);
    for (int i = 0; i < len; i++) {
        insert(d, i, containers[i]);
    }
    for (int *it = rbegin(d); it != rend(d); it = prev(it)) {
        printf("[%3d : %s]\n", first(it), second(it, char*));
    }
    puts("");
    destroy(d);
}

int main() {
    example_vector();
    example_list();
    example_set();
    example_map();
    return 0;
}

```



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

