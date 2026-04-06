
<p align="center">
  <img width="256" height="256" src="assets/OpenCSTL.png">
</p>


# Open source C container library like C++ STL


## Why OpenCSTL?

- **Single-header** 
	 - Just `#include "opencstl.h"` and you're done. No build steps, no linking.
	
- **Bracket access**
   -  `vector` and `deque` support `v[i]` subscript access, exactly like C++ and plain C arrays.

- **Identical C++ STL interface**
  — same function names, same semantics, zero new API to learn.
  - Auto-dispatch: one function name routes to the correct implementation based on the container type.
  - Variadic API: different parameter counts invoke different behaviors, just like C++ overloading.
- **Cross-platform** 
  - Works on Windows, macOS, and Linux.
  - Compiler support: MSVC, MinGW64, GCC, Clang and icx-cc.

- **Zero boilerplate** 
  - No forward declarations, no type registration macros, no initialization ceremony.

- **stdlib compatible** 
  - Use standard C functions like `qsort` and `bsearch` directly on container data.

- **Full iterator support** 
   - Every container exposes `begin`, `end`, `rbegin`, and `rend` iterators.

- **Simpler than any alternative** 
   - Far easier to use than other C container libraries, without breaking idiomatic C style.

## Usage

```bash
curl -LO "https://raw.githubusercontent.com/springkim/OpenCSTL/refs/heads/master/opencstl.h"
```


## Supported OS / C compilers


<p>
    <img width="128" height="128" src="assets/windows11.png" >
    <img width="128" height="128" src="assets/macos.png" >
    <img width="128" height="128" src="assets/ubuntu_logo.png" >
</p>

<p>
    <img width="96" height="96" src="assets/MSVC2026.png" >
    <img width="96" height="96" src="assets/MinGW64.png" >
    <img width="96" height="96" src="assets/LLVM.png" >
    <img width="96" height="96" src="assets/GNU.png" >
    <img width="96" height="96" src="assets/tcc.png" >
     <img width="96" height="96" src="assets/intel_c_compiler.png" >
</p>


|C Compiler|Windows|MacOS|Linux|
|-|-|-|-|
|MSVC(cl)|✅||||
|GNU(gcc)|✅|✅|✅|
|LLVM(clang)|✅|✅|✅|
|TCC|✅|❌|❌|
|Intel oneAPI DPC++/C++ Compiler||❌|✅|

## Examples



```c
#define USE_CSTL_FUNC
// Defining USE_CSTL_FUNC ensures that short, concise names are used.
// USE_CSTL_FUNC을 정의하면 짧고 간결한 이름들을 사용합니다.
// USE_CSTL_FUNCを定義すると、短く簡潔な名前を使用します。
// 定义 USE_CSTL_FUNC 可确保使用简短、精炼的名称。
#include "opencstl/opencstl.h"

#include <stdlib.h>

// Comparison function for sorting integers in ascending order.
// 정수를 오름차순으로 정렬하기 위한 비교 함수입니다.
// 整数を昇順にソートするための比較関数です。
// 用于将整数按升序排序的比较函数。
int cmp_int(const void *a, const void *b) {
    return *((int *) a) - *((int *) b);
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

`````
[ 38][ 26][ 13][ 83][ 19][777][ 95][ 78][999][999][999][ 87][ 80][ 86]
[ 13][ 19][ 26][ 38][ 78][ 80][ 83][ 86][ 87][ 95][777][999][999][999]
`````


### Sort

```
void qsort(
    void *__base, 
    size_t __nel, 
    size_t __width, 
    int(*__compar)(const void *, const void *)) 
```





### Usage

```
git clone https://github.com/springkim/OpenCSTL
```

Copy `OpenCSTL/opencstl/*.h` into your project directory.

Write `#include<opencstl/opencstl.h>` above your c source file.

### Example

#### `vector example`

```cpp
void vector_example(){
    VECTOR(int) arr = cstl_vector(int);        //Create vector.
    cstl_push_back(arr,88);                //Append(push_back) (literal) into vector.
    for (int i = 0; i < 10; i++) {
        cstl_push_back(arr, i);            //Append(push_back) (variable) into vector.
    }
    cstl_pop_back(arr);                    //pop_back
    cstl_insert(arr, arr + 0, 777);        //Insert 777 into the 0th.
    cstl_insert(arr, arr + 5, 5,999);    //Insert 999 5 times into the 5th.
    cstl_erase(arr, arr + 3, arr+5);    //Erase elements from 3rd to 4th

    cstl_resize(arr, 15, -1);           //Resize vector size to 15.

    //Print each element using iterator.
    for (int* it = cstl_begin(arr); it != cstl_end(arr); it++) {
        printf("[%3d] ", *it);
    }
    puts("");

    int* pos = cstl_find(arr, 7);       //Find 7 in vector
    if(pos!=NULL)
        printf("find : [%3d]\n", *pos);
    cstl_free(arr);                        //Memory release
}
```

#### `list example`

```cpp
void list_example(){
    LIST(int) list = cstl_list(int);        //Create list.
    for (int i = 0; i < 10; i++) {
        cstl_push_back(list, i);        //Append(push_back) into list.
    }
    cstl_pop_back(list);
    cstl_insert(list, cstl_begin(list), 777);        //Insert 777 into the 0th.
    cstl_insert(list, cstl_find(list,4), 5, 999);    //Insert 999 5 times before 4(element).
    cstl_erase(list, cstl_find(list, 2), cstl_find(list, 999));        //Erase elements from 2 to 999(first).
    //Print each element using iterator.
    for (int* it = cstl_begin(list); it != cstl_end(list); it = cstl_next(it)) {
        printf("[%3d] ", *it);
    }
    puts("");
    cstl_free(list);        //Memory release
}
```

#### `set example`

```cpp

int IntCmp(const void* a, const void* b) {
    return *(int*)a < *(int*)b ? -1 : *(int*)a > *(int*)b;
}
void set_example(){
    SET(int) tree = cstl_set(int, IntCmp);        //Create set
    for (int i = 0; i < 100; i++) {
        cstl_insert(tree,i);                //Insert value into set.
    }
    for (int i = 50; i < 70; i++) {
        int* it = cstl_find(tree, i);        //Find iterator
        cstl_erase(tree, it);                //Erase (50,70] from set.
    }
    //Print each element using iterator.
    for (int* it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
        printf("[%3d]", *it);
    }
    puts("");
    cstl_free(tree);    //Memory release
}
```

#### `map example`

```cpp
typedef const void *CVP;
int IntCmp(CVP a, CVP b) {
    return (*(int *) a > *(int *) b) - (*(int *) a < *(int *) b);
}
void map_example(){
	MAP(int) tree = cstl_map(int, float, IntCmp);
    for (int i = 0; i < 10; i++) {
        float d = (float) i;
        cstl_insert(tree, i, d * d);
    }
    ///[0]{0} [1]{1} [2]{4} [3]{9} [4]{16} [5]{25} [6]{36} [7]{49} [8]{64} [9]{81}
    for (int i = 0; i < 10; i += 2) {
        int *it = cstl_find(tree, i);
        cstl_erase(tree, it);
    }
    ///[1]{1} [3]{9} [5]{25} [7]{49} [9]{81}
    for (int *it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
        printf("[%3d]{%.3f} ", *it, cstl_value(it, float));
    }
    puts("");
    printf("size : %d\n", cstl_size(tree));
    cstl_free(tree);
}
```

#### `deque example`

```cpp
void deque_example(){
    DEQUE(int) = cstl_deque(int);
    for (int i = 0; i < 10; i++) {
        cstl_push_back(deque, i);
    }
    for (int i = 0; i < 10; i++) {
        cstl_push_front(deque, i);
    }
    for(int i=0;i<cstl_size(deque);i++){
        printf("[%3d] ", deque[i]);
    }
    puts("");
    cstl_free(deque);
}
```

#### `stack example`

```cpp
void stack_example(){
    STACK(int) stack = cstl_stack(int);
    for (int i = 0; i < 10; i++) {
        cstl_push(stack, i);
    }
    for (int i = 0; i < 10; i++) {
        printf("[%3d] ", cstl_top(stack);
        cstl_pop(stack, i);
    }
    puts("");
    cstl_free(stack);
}
```

#### `queue example`

```cpp
void queue_example(){
    QUEUE(int) queue = cstl_queue(int);
    for (int i = 0; i < 10; i++) {
        cstl_push(queue, i);
    }
    for (int i = 0; i < 10; i++) {
        printf("[%3d] ", cstl_front(queue);
        cstl_pop(queue, i);
    }
    puts("");
    cstl_free(queue);
}
```

#### `priority_queue example`

```cpp
void queue_example(){
    QUEUE(int) queue = cstl_priority_queue(int,IntCmp);
    for (int i = 0; i < 10; i++) {
        cstl_push(queue, i);
    }
    while(!cstl_empty(queue)){
        printf("[%3d]", cstl_top(queue));
        cstl_pop(queue);
    }
    puts("");
    cstl_free(queue);
}
```

### Reference

#### `vector references`

```
<type*> cstl_vector(<type>);
<type*> cstl_begin(<vector>);
<type*> cstl_end(<vector>);
void cstl_clear(<vector>);
bool cstl_empty(<vector>);
<type*> cstl_find(<vector>,<iter>,<value>);
<type*> cstl_find(<vector>,<value>);
<type> cstl_front(<vector>)
<type> cstl_back(<vector>)
void cstl_erase(<vector>,<iter>);
void cstl_erase(<vector>,<iter_begin>,<iter_end>);
void cstl_insert(<vector>,<iter>,<value>);
void cstl_insert(<vector>,<iter>,<N>,<value>);
void cstl_pop_back(<vector>);
void cstl_push_back(<vector>,<value>);
<type*> cstl_rbegin(<vector>);
<type*> cstl_rend(<vector>);
void cstl_size(<vector>);
void cstl_resize(<vector>,<N>);
void cstl_resize(<vector>,<N>,<value>);
void cstl_free(<vector>);
void cstl_assign(<vector>,<n>);
void cstl_assign(<vector>,<n>,<value>)
```

#### `list references`

```
<type**> cstl_list(<type>);
<type*> cstl_begin(<list>);
<type*> cstl_end(<list>);
void cstl_clear(<list>);
bool cstl_empty(<list>);
<type*> cstl_find(<list>,<iter>,<value>);
<type*> cstl_find(<list>,<value>);
<type> cstl_front(<list>)
<type> cstl_back(<list>)
void cstl_erase(<list>,<iter>);
void cstl_erase(<list>,<iter_begin>,<iter_end>);
void cstl_insert(<list>,<iter>,<value>);
void cstl_insert(<list>,<iter>,<N>,<value>);
void cstl_pop_back(<list>);
void cstl_push_back(<list>,<value>);
void cstl_pop_front(<list>);
void cstl_push_front(<list>,<value>);
<type*> cstl_rbegin(<list>);
<type*> cstl_rend(<list>);
void cstl_size(<list>);
void cstl_resize(<list>,<N>);
void cstl_resize(<list>,<N>,<value>);
void cstl_free(<list>);
void cstl_assign(<list>,<n>);
void cstl_assign(<list>,<n>,<value>)
```

### `set references`

```
<type**> cstl_set(<type>);
<type*> cstl_begin(<set>);
<type*> cstl_end(<set>);
void cstl_clear(<set>);
bool cstl_empty(<set>);
<type*> cstl_find(<set>,<value>);
void cstl_erase(<set>,<iter>);
void cstl_insert(<set>,<iter>,<value>);
<type*> cstl_rbegin(<set>);
<type*> cstl_rend(<set>);
void cstl_size(<set>);
void cstl_free(<set>);
<type*> cstl_next(<iter>);
<type*> cstl_prev(<iter>);
```

### `map references`

```
<type**> cstl_map(<type>);
<type*> cstl_begin(<map>);
<type*> cstl_end(<map>);
void cstl_clear(<map>);
bool cstl_empty(<map>);
<type*> cstl_find(<map>,<value>);
void cstl_erase(<map>,<iter>);
void cstl_insert(<map>,<iter>,<value>);
<type*> cstl_rbegin(<map>);
<type*> cstl_rend(<map>);
void cstl_size(<map>);
void cstl_free(<map>);
<type*> cstl_next(<iter>);
<type*> cstl_prev(<iter>);
<type> cstl_value(<iter>,<type>);
```

### `deque references`

```
<type*> cstl_deque(<type>);
<type*> cstl_begin(<deque>);
<type*> cstl_end(<deque>);
void cstl_clear(<deque>);
bool cstl_empty(<deque>);
<type*> cstl_find(<deque>,<iter>,<value>);
<type*> cstl_find(<deque>,<value>);
<type> cstl_front(<deque>)
<type> cstl_back(<deque>)
void cstl_erase(<deque>,<iter>);
void cstl_erase(<deque>,<iter_begin>,<iter_end>);
void cstl_insert(<deque>,<iter>,<value>);
void cstl_insert(<deque>,<iter>,<N>,<value>);
void cstl_pop_back(<deque>);
void cstl_push_back(<deque>,<value>);
void cstl_pop_front(<deque>);
void cstl_push_front(<deque>,<value>);
<type*> cstl_rbegin(<deque>);
<type*> cstl_rend(<deque>);
void cstl_size(<deque>);
void cstl_resize(<deque>,<N>);
void cstl_resize(<deque>,<N>,<value>);
void cstl_free(<deque>);
void cstl_assign(<list>,<n>);
void cstl_assign(<list>,<n>,<value>)
```

### `stack references`

```
<type*> cstl_stack(<type>);
void cstl_clear(<stack>);
bool cstl_empty(<stack>);
<type> cstl_top(<stack>);
void cstl_pop(<stack>);
void cstl_push(<stack>,<value>);
void cstl_free(<stack>);
```

### `queue references`

```
<type*> cstl_queue(<type>);
void cstl_clear(<queue>);
bool cstl_empty(<queue>);
<type> cstl_front(<queue>)
<type> cstl_back(<queue>)
void cstl_pop(<queue>);
void cstl_push(<queue>,<value>);
void cstl_free(<queue>);
```

### Warnings

* Unable to recognize typedef float
  * OpenCSTL is based on variadic function.
  * Basically it can handle char,short,int,int64,double and struct.
  * But we made this can handle float too.
  * But other float name can't be handle.
* Array based structure use <type*>, Node based structure use <type**>.

### TODO

- [x] unorded set/map(hash)

- [x] algorithm

## Links

[![Hypercommit](https://img.shields.io/badge/Hypercommit-DB2475)](https://hypercommit.com/opencstl)
