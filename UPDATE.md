## 1.4.3

* 🟩: 인자 개수에 올바른 함수로 호출.
* 🟦: `_Generic`을 사용하여 가변인자 사용시 자료형 승격을 하지 않음.
* 🟧: 가변인자 사용 및 자료형 승격.

| C Compiler                   | C89 | C99 | C11 | C17 | C23 |
|------------------------------|-----|-----|-----|-----|-----|
| MinGW64 gcc (Windows)        | 🟩  | 🟩  | 🟩  | 🟩  | 🟩  |
| Clang (Windows)              | 🟩  | 🟩  | 🟩  | 🟩  | 🟩  |
| MSVC (Windows)               | 🟧  | 🟧  | 🟦  | 🟦  | 🟦  |
| pocc (Windows)               |     |     | 🟦  |     |     |
| bcc64x (Windows)             |     |     |     |     |     |
| gcc (Linux)                  | 🟩  | 🟩  | 🟩  | 🟩  | 🟩  |
| clang (Linux)                | 🟩  | 🟩  | 🟩  | 🟩  | 🟩  |
| gcc (Mac)                    | 🟩  | 🟩  | 🟩  | 🟩  | 🟩  |
| clang (Mac)                  | 🟩  | 🟩  | 🟩  | 🟩  | 🟩  |
| tcc (Windows, Mac, Linux)                |     | 🟩  |     |     |     |
| zig cc (Windows, Mac, Linux) |     |     |     | 🟩  |     |

## 1.4.2

array의 iterator 버그 수정

memmove로 대체 가능한 부분 변경

mt19937 랜덤 API를 Thread-Safe하게 변경

* TCC의 정렬함수를 clang/gcc로 빌드한 shared lib를 로드해서 테스트

| ENV/SORTING | qsort  | msort  | tsort  | pdqsort | rsort  | pmsort | ipnsort |
|-------------|--------|--------|--------|---------|--------|--------|---------|
| MacOS/GCC   | 632.14 | 538.12 | 539.99 | 409.60  | 144.88 | 133.02 | 411.62  |

| ENV/SORTING | qsort  | msort  | tsort  | pdqsort | rsort  | pmsort | ipnsort |
|-------------|--------|--------|--------|---------|--------|--------|---------|
| MacOS/Clang | 639.42 | 543.58 | 531.82 | 406.43  | 123.48 | 131.62 | 415.10  |

| ENV/SORTING | qsort  | msort   | tsort   | pdqsort | rsort  | pmsort | ipnsort |
|-------------|--------|---------|---------|---------|--------|--------|---------|
| MacOS/TCC   | 847.36 | 1376.38 | 1392.64 | 1229.84 | 476.08 | 196.98 | 1199.94 |

| ENV/SORTING | qsort  | msort_v2 | tsort   | pdqsort | rsort  | pmsort | ipnsort |
|-------------|--------|----------|---------|---------|--------|--------|---------|
| MacOS/TCC   | 856.60 | 1084.99  | 1416.40 | 1238.80 | 482.94 | 194.65 | 1215.42 |

```bash
gcc-15 -O3 -DNDEBUG -mcpu=apple-m1 -mtune=native -flto -dynamiclib -o msort.dylib msort.c
```

## 1.4.1

XCode에서 logging 출력이 일반적인 텍스트로 나오게 수정 완료.

MacOS의 [TCC](https://github.com/TinyCC/tinycc)가 빌드되도록 CMakeLists.txt에 옵션 추가.

* `__VERSION__` 매크로 사용
    * TCC에는 `__VERSION__` 매크로가 없어 새롭게 정의
    * MSVC에도 `__VERSION__` 매크로가 없어 새롭게 정의

MacOS의 `MsgBoxGUI` 구현 완료.

| ENV/SORTING | qsort  | msort  | tsort  | pdqsort | rsort  | pmsort | ipnsort |
|-------------|--------|--------|--------|---------|--------|--------|---------|
| MacOS/GCC   | 633.13 | 532.87 | 543.20 | 410.84  | 143.95 | 120.59 | 409.29  |

| ENV/SORTING | qsort  | msort  | tsort  | pdqsort | rsort  | pmsort | ipnsort |
|-------------|--------|--------|--------|---------|--------|--------|---------|
| MacOS/Clang | 634.10 | 537.96 | 534.02 | 403.15  | 125.06 | 124.50 | 412.48  |

| ENV/SORTING | qsort  | msort   | tsort   | pdqsort | rsort  | pmsort | ipnsort |
|-------------|--------|---------|---------|---------|--------|--------|---------|
| MacOS/TCC   | 927.07 | 1470.92 | 1486.61 | 1306.20 | 492.72 | 196.81 | 1296.02 |

```
gcc: 15.2.0
clang: Homebrew Clang 21.1.8
tcc: TCC Compiler
zig cc: Apple LLVM 21.0.0 (clang-2100.1.1.101)
```
