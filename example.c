#include "opencstl/opencstl.h"

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
