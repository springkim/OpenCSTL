#include <stdio.h>

// 리스트 정의
#define ERROR_LIST \
X(ERROR_NONE)        \
X(ERROR_INVALID)     \
X(ERROR_TIMEOUT)     \
X(ERROR_OVERFLOW)

// enum 정의
enum {
#define X(name) name,
    ERROR_LIST
#undef X
    ERROR_COUNT
};

// 문자열 배열 생성
const char *ErrorNames[] = {
#define X(name) #name,
    ERROR_LIST
#undef X
};

int main() {
    for (int i = 0; i < ERROR_COUNT; i++) {
        printf("Error Code: %d, Name: %s\n", i, ErrorNames[i]);
    }
    return 0;
}
