<p align="center">
  <img width="160" height="160" src="assets/OpenCSTL.png">
</p>

<h1 align="center">OpenCSTL</h1>
<p align="center">像 C++ STL 一样使用的开源 C 容器库</p>
<p align="center">C99 &nbsp;·&nbsp; C11 &nbsp;·&nbsp; C17 &nbsp;·&nbsp; C23</p>

<p align="center">
  <a href="README.md">English</a> &nbsp;|&nbsp;
  <a href="README.ko.md">한국어</a> &nbsp;|&nbsp;
  <a href="README.zh.md">中文</a>
</p>

---

## 为什么选择 OpenCSTL？

- **单头文件** — 只需 `#include "opencstl.h"`，无需构建步骤，无需链接。
- **与 C++ STL 接口完全一致** — 函数名称和语义完全相同，无需学习新 API。自动分发机制将同一函数名路由到正确的容器实现。可变参数 API 根据参数数量触发不同行为，与 C++ 重载机制一致。
- **下标访问** — `VECTOR` 和 `DEQUE` 支持 `v[i]` 下标访问，与 C 数组完全一致。
- **完整的迭代器支持** — 所有容器（包括无序容器）均提供 `begin`、`end`、`rbegin`、`rend` 迭代器。
- **兼容标准库** — 可直接对 `VECTOR` 和 `DEQUE` 数据使用 `qsort`、`bsearch` 等标准 C 函数。
- **零样板代码** — 无需前向声明，无需类型注册宏，无需初始化仪式。
- **跨平台** — 支持 Windows、macOS 和 Linux，兼容 MSVC、MinGW64、GCC、Clang、icx-cc。
- **比同类库更简单** — 在不破坏惯用 C 风格的前提下，远比其他 C 容器库易用。

## 安装

```bash
curl -LO "https://raw.githubusercontent.com/springkim/OpenCSTL/refs/heads/master/opencstl.h"
```

## 支持的平台

| 编译器 | Windows | macOS | Linux |
|--------|:-------:|:-----:|:-----:|
| MSVC (cl) | ✅ | | |
| GNU (gcc) | ✅ | ✅ | ✅ |
| LLVM (clang) | ✅ | ✅ | ✅ |
| MinGW64 | ✅ | | |
| Intel (icx-cc) | ✅ | ✅ | ✅ |

## 示例

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

    insert(q, q + 5, 777);    // 在位置 5 插入 777
    insert(q, q + 8, 3, 999); // 在位置 8 插入三个 999

    for (int i = 0; i < size(q); i++) {
        printf("[%3d]", q[i]); // [] 访问，与普通 C 数组完全一致
    }
    puts("");

    qsort(q, size(q), sizeof(int), cmp_int); // 直接使用标准 C 函数

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

**输出：**
```
[ 38][ 26][ 13][ 83][ 19][777][ 95][ 78][999][999][999][ 87][ 80][ 86]
[ 13][ 19][ 26][ 38][ 78][ 80][ 83][ 86][ 87][ 95][777][999][999][999]
```

## 参考文档

> 可选参数用方括号 `[...]` 标注。
> 比较器和哈希函数参数始终可省略，省略时将使用该类型的内置默认值。

### `VECTOR`
```
VECTOR(<类型>) v = new_vector(<类型>);

<类型*> begin(v);          <类型*> end(v);
<类型*> rbegin(v);         <类型*> rend(v);
size_t  size(v);           bool    empty(v);        size_t capacity(v);
<类型>  front(v);          <类型>  back(v);

void    push_back(v, 值);
void    pop_back(v);
void    insert(v, iter, 值);
void    insert(v, iter, N, 值);
void    erase(v, iter);
void    erase(v, iter_begin, iter_end);
<类型*> find(v, 值);
<类型*> find(v, iter, 值);
void    resize(v, N [, 值]);
void    assign(v, N [, 值]);
void    clear(v);
void    destroy(v);
```

### `LIST`
```
LIST(<类型>) l = new_list(<类型>);

<类型*> begin(l);          <类型*> end(l);
<类型*> rbegin(l);         <类型*> rend(l);
<类型*> next(iter);        <类型*> prev(iter);    // 替代 it++/it--
size_t  size(l);           bool    empty(l);
<类型>  front(l);          <类型>  back(l);

void    push_back(l, 值);         void    pop_back(l);
void    push_front(l, 值);        void    pop_front(l);
void    insert(l, iter, 值);
void    insert(l, iter, N, 值);
void    erase(l, iter);
void    erase(l, iter_begin, iter_end);
<类型*> find(l, 值);
<类型*> find(l, iter, 值);
void    resize(l, N [, 值]);
void    assign(l, N [, 值]);
void    clear(l);
void    destroy(l);
```

### `DEQUE`
```
DEQUE(<类型>) q = new_deque(<类型>);

<类型*> begin(q);          <类型*> end(q);
<类型*> rbegin(q);         <类型*> rend(q);
size_t  size(q);           bool    empty(q);        size_t capacity(q);
<类型>  front(q);          <类型>  back(q);

void    push_back(q, 值);         void    pop_back(q);
void    push_front(q, 值);        void    pop_front(q);
void    insert(q, iter, 值);
void    insert(q, iter, N, 值);
void    erase(q, iter);
void    erase(q, iter_begin, iter_end);
<类型*> find(q, 值);
<类型*> find(q, iter, 值);
void    resize(q, N [, 值]);
void    assign(q, N [, 值]);
void    clear(q);
void    destroy(q);
```

### `SET`
```
SET(<类型>) s = new_set(<类型> [, 比较器]);

<类型*> begin(s);          <类型*> end(s);
<类型*> rbegin(s);         <类型*> rend(s);
<类型*> next(iter);        <类型*> prev(iter);
size_t  size(s);           bool    empty(s);

void    insert(s, 值);
void    erase(s, iter);
<类型*> find(s, 值);
void    clear(s);
void    destroy(s);
```

### `MAP`
```
MAP(<键类型>) m = new_map(<键类型>, <值类型> [, 比较器]);

<键类型*>  begin(m);          <键类型*>  end(m);
<键类型*>  rbegin(m);         <键类型*>  rend(m);
<键类型*>  next(iter);        <键类型*>  prev(iter);
size_t     size(m);           bool       empty(m);

<键类型>   first(iter);                        // 从迭代器获取键
<值类型>   second(iter, <值类型>);             // 从迭代器获取值

void       insert(m, 键, 值);
void       erase(m, iter);
<键类型*>  find(m, 键);
void       clear(m);
void       destroy(m);
```

### `UNORDERED_SET`
```
UNORDERED_SET(<类型>) h = new_unordered_set(<类型> [, 哈希函数]);

<类型*> begin(h);          <类型*> end(h);
<类型*> rbegin(h);         <类型*> rend(h);        // 桶的逆序遍历
<类型*> next(iter);        <类型*> prev(iter);
size_t  size(h);           bool    empty(h);        size_t capacity(h);
h[i]                                               // 直接访问桶槽（0 表示空槽）

void    insert(h, 值);
void    erase(h, iter);
<类型*> find(h, 值);
void    clear(h);
void    destroy(h);
```

### `UNORDERED_MAP`
```
UNORDERED_MAP(<键类型>) m = new_unordered_map(<键类型>, <值类型> [, 哈希函数]);

<键类型*>  begin(m);          <键类型*>  end(m);
<键类型*>  rbegin(m);         <键类型*>  rend(m);
<键类型*>  next(iter);        <键类型*>  prev(iter);
size_t     size(m);           bool       empty(m);        size_t capacity(m);

<键类型>   first(iter);
<值类型>   second(iter, <值类型>);

void       insert(m, 键, 值);
void       erase(m, iter);
<键类型*>  find(m, 键);
void       clear(m);
void       destroy(m);
```

### `STACK`
```
STACK(<类型>) s = new_stack(<类型>);

size_t size(s);    bool   empty(s);
<类型> top(s);
void   push(s, 值);
void   pop(s);
void   clear(s);
void   destroy(s);
```

### `QUEUE`
```
QUEUE(<类型>) q = new_queue(<类型>);

size_t size(q);    bool   empty(q);
<类型> front(q);   <类型> back(q);
void   push(q, 值);
void   pop(q);
void   clear(q);
void   destroy(q);
```

### `PRIORITY_QUEUE`
```
PRIORITY_QUEUE(<类型>) pq = new_priority_queue(<类型> [, 比较器]);

size_t size(pq);   bool   empty(pq);
<类型> top(pq);
void   push(pq, 值);   // O(log n)
void   pop(pq);        // O(log n)
void   clear(pq);
void   destroy(pq);
```

## 注意事项

- 支持 `float`，但不支持通过 `typedef` 定义的 float 别名类型。请直接使用 `float` 关键字。
- 序列容器（`VECTOR`、`DEQUE`）的句柄类型为 `<类型*>`。基于节点的容器（`LIST`、`SET`、`MAP`、`UNORDERED_SET`、`UNORDERED_MAP`）的句柄类型为 `<类型**>`。
- 对于基于节点的容器，推进迭代器时必须使用 `next(it)` 和 `prev(it)`，不能使用 `it++` 或 `it--`。
- `UNORDERED_SET` 和 `UNORDERED_MAP` 支持 `rbegin`、`rend` 和 `prev`，以桶的逆序遍历元素（非排序逆序）。
- 比较器和哈希函数参数均为**可选**。省略时将使用该类型的内置默认值：`new_set(int)`、`new_map(int, double)`、`new_priority_queue(int)`、`new_unordered_set(int)`。

## 链接

[![Hypercommit](https://img.shields.io/badge/Hypercommit-DB2475)](https://hypercommit.com/opencstl)
