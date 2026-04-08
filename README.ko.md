<p align="center">
  <img width="160" height="160" src="assets/OpenCSTL.png">
</p>

<h1 align="center">OpenCSTL</h1>
<p align="center">C++ STL처럼 사용하는 오픈소스 C 컨테이너 라이브러리</p>
<p align="center">C99 &nbsp;·&nbsp; C11 &nbsp;·&nbsp; C17 &nbsp;·&nbsp; C23</p>

<p align="center">
  <a href="README.md">English</a> &nbsp;|&nbsp;
  <a href="README.ko.md">한국어</a> &nbsp;|&nbsp;
  <a href="README.zh.md">中文</a>
</p>

---

## 왜 OpenCSTL인가?

- **단일 헤더** — `#include "opencstl.h"` 한 줄로 끝. 빌드 설정도, 링킹도 필요 없습니다.
- **C++ STL과 동일한 인터페이스** — 함수 이름과 의미가 완전히 동일합니다. 새로운 API를 배울 필요가 없습니다. 하나의 함수 이름이 컨테이너 타입에 맞는 구현으로 자동 분기됩니다. 가변 인자 API로 매개변수 수에 따라 C++ 오버로딩처럼 다른 동작을 수행합니다.
- **대괄호 접근** — `VECTOR`와 `DEQUE`는 C 배열처럼 `v[i]` 첨자 접근을 지원합니다.
- **완전한 이터레이터 지원** — 모든 컨테이너(비순서 컨테이너 포함)가 `begin`, `end`, `rbegin`, `rend` 이터레이터를 제공합니다.
- **stdlib 호환** — `VECTOR`와 `DEQUE` 데이터에 `qsort`, `bsearch` 등 표준 C 함수를 직접 사용할 수 있습니다.
- **보일러플레이트 없음** — 전방 선언도, 타입 등록 매크로도, 초기화 의식도 필요 없습니다.
- **크로스 플랫폼** — Windows, macOS, Linux에서 동작합니다. MSVC, MinGW64, GCC, Clang, icx-cc를 지원합니다.
- **다른 라이브러리보다 훨씬 간단** — 관용적인 C 스타일을 해치지 않으면서 다른 C 컨테이너 라이브러리보다 훨씬 쉽게 사용할 수 있습니다.

## 설치

```bash
curl -LO "https://raw.githubusercontent.com/springkim/OpenCSTL/refs/heads/master/opencstl.h"
```

## 지원 플랫폼

| 컴파일러 | Windows | macOS | Linux |
|----------|:-------:|:-----:|:-----:|
| MSVC (cl) | ✅ | | |
| GNU (gcc) | ✅ | ✅ | ✅ |
| LLVM (clang) | ✅ | ✅ | ✅ |
| MinGW64 | ✅ | | |
| Intel (icx-cc) | ✅ | ✅ | ✅ |

## 문서

[문서](./docs/README.md)

## 예제

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

**출력:**
```
[ 38][ 26][ 13][ 83][ 19][777][ 95][ 78][999][999][999][ 87][ 80][ 86]
[ 13][ 19][ 26][ 38][ 78][ 80][ 83][ 86][ 87][ 95][777][999][999][999]
```

## 레퍼런스

> 선택적 매개변수는 대괄호 `[...]`로 표시됩니다.
> 비교자(comparator)와 해시 함수 인수는 항상 생략 가능하며, 생략 시 해당 타입의 내장 기본값이 사용됩니다.

### `VECTOR`
```
VECTOR(<타입>) v = new_vector(<타입>);

<타입*> begin(v);          <타입*> end(v);
<타입*> rbegin(v);         <타입*> rend(v);
size_t  size(v);           bool    empty(v);        size_t capacity(v);
<타입>  front(v);          <타입>  back(v);

void    push_back(v, 값);
void    pop_back(v);
void    insert(v, iter, 값);
void    insert(v, iter, N, 값);
void    erase(v, iter);
void    erase(v, iter_begin, iter_end);
<타입*> find(v, 값);
<타입*> find(v, iter, 값);
void    resize(v, N [, 값]);
void    assign(v, N [, 값]);
void    clear(v);
void    destroy(v);
```

### `LIST`
```
LIST(<타입>) l = new_list(<타입>);

<타입*> begin(l);          <타입*> end(l);
<타입*> rbegin(l);         <타입*> rend(l);
<타입*> next(iter);        <타입*> prev(iter);    // it++/it-- 대신 사용
size_t  size(l);           bool    empty(l);
<타입>  front(l);          <타입>  back(l);

void    push_back(l, 값);         void    pop_back(l);
void    push_front(l, 값);        void    pop_front(l);
void    insert(l, iter, 값);
void    insert(l, iter, N, 값);
void    erase(l, iter);
void    erase(l, iter_begin, iter_end);
<타입*> find(l, 값);
<타입*> find(l, iter, 값);
void    resize(l, N [, 값]);
void    assign(l, N [, 값]);
void    clear(l);
void    destroy(l);
```

### `DEQUE`
```
DEQUE(<타입>) q = new_deque(<타입>);

<타입*> begin(q);          <타입*> end(q);
<타입*> rbegin(q);         <타입*> rend(q);
size_t  size(q);           bool    empty(q);        size_t capacity(q);
<타입>  front(q);          <타입>  back(q);

void    push_back(q, 값);         void    pop_back(q);
void    push_front(q, 값);        void    pop_front(q);
void    insert(q, iter, 값);
void    insert(q, iter, N, 값);
void    erase(q, iter);
void    erase(q, iter_begin, iter_end);
<타입*> find(q, 값);
<타입*> find(q, iter, 값);
void    resize(q, N [, 값]);
void    assign(q, N [, 값]);
void    clear(q);
void    destroy(q);
```

### `SET`
```
SET(<타입>) s = new_set(<타입> [, 비교자]);

<타입*> begin(s);          <타입*> end(s);
<타입*> rbegin(s);         <타입*> rend(s);
<타입*> next(iter);        <타입*> prev(iter);
size_t  size(s);           bool    empty(s);

void    insert(s, 값);
void    erase(s, iter);
<타입*> find(s, 값);
void    clear(s);
void    destroy(s);
```

### `MAP`
```
MAP(<키_타입>) m = new_map(<키_타입>, <값_타입> [, 비교자]);

<키_타입*>  begin(m);          <키_타입*>  end(m);
<키_타입*>  rbegin(m);         <키_타입*>  rend(m);
<키_타입*>  next(iter);        <키_타입*>  prev(iter);
size_t      size(m);           bool        empty(m);

<키_타입>   first(iter);                         // 이터레이터에서 키 반환
<값_타입>   second(iter, <값_타입>);             // 이터레이터에서 값 반환

void        insert(m, 키, 값);
void        erase(m, iter);
<키_타입*>  find(m, 키);
void        clear(m);
void        destroy(m);
```

### `UNORDERED_SET`
```
UNORDERED_SET(<타입>) h = new_unordered_set(<타입> [, 해시_함수]);

<타입*> begin(h);          <타입*> end(h);
<타입*> rbegin(h);         <타입*> rend(h);        // 버킷 역순 순회
<타입*> next(iter);        <타입*> prev(iter);
size_t  size(h);           bool    empty(h);        size_t capacity(h);
h[i]                                               // 버킷 슬롯 직접 접근 (0 = 빈 슬롯)

void    insert(h, 값);
void    erase(h, iter);
<타입*> find(h, 값);
void    clear(h);
void    destroy(h);
```

### `UNORDERED_MAP`
```
UNORDERED_MAP(<키_타입>) m = new_unordered_map(<키_타입>, <값_타입> [, 해시_함수]);

<키_타입*>  begin(m);          <키_타입*>  end(m);
<키_타입*>  rbegin(m);         <키_타입*>  rend(m);
<키_타입*>  next(iter);        <키_타입*>  prev(iter);
size_t      size(m);           bool        empty(m);        size_t capacity(m);

<키_타입>   first(iter);
<값_타입>   second(iter, <값_타입>);

void        insert(m, 키, 값);
void        erase(m, iter);
<키_타입*>  find(m, 키);
void        clear(m);
void        destroy(m);
```

### `STACK`
```
STACK(<타입>) s = new_stack(<타입>);

size_t size(s);    bool   empty(s);
<타입> top(s);
void   push(s, 값);
void   pop(s);
void   clear(s);
void   destroy(s);
```

### `QUEUE`
```
QUEUE(<타입>) q = new_queue(<타입>);

size_t size(q);    bool   empty(q);
<타입> front(q);   <타입> back(q);
void   push(q, 값);
void   pop(q);
void   clear(q);
void   destroy(q);
```

### `PRIORITY_QUEUE`
```
PRIORITY_QUEUE(<타입>) pq = new_priority_queue(<타입> [, 비교자]);

size_t size(pq);   bool   empty(pq);
<타입> top(pq);
void   push(pq, 값);   // O(log n)
void   pop(pq);        // O(log n)
void   clear(pq);
void   destroy(pq);
```

## 참고사항

- `float`는 지원되지만 `typedef`로 별칭된 float 타입은 인식되지 않습니다. `float` 키워드를 직접 사용하세요.
- 시퀀스 컨테이너(`VECTOR`, `DEQUE`)의 핸들 타입은 `<타입*>`입니다. 노드 기반 컨테이너(`LIST`, `SET`, `MAP`, `UNORDERED_SET`, `UNORDERED_MAP`)의 핸들 타입은 `<타입**>`입니다.
- 노드 기반 컨테이너에서는 이터레이터를 이동할 때 반드시 `next(it)`, `prev(it)`를 사용하세요. `it++`나 `it--`는 동작하지 않습니다.
- `UNORDERED_SET`과 `UNORDERED_MAP`은 `rbegin`, `rend`, `prev`를 지원합니다. 버킷 역순으로 순회하며, 이는 정렬 역순이 아닙니다.
- 비교자와 해시 함수 인수는 **선택적**입니다. 생략하면 해당 타입의 내장 기본값이 사용됩니다: `new_set(int)`, `new_map(int, double)`, `new_priority_queue(int)`, `new_unordered_set(int)`.

## 링크

[![Hypercommit](https://img.shields.io/badge/Hypercommit-DB2475)](https://hypercommit.com/opencstl)
