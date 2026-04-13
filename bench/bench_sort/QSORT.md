
## MinGW64
```c++
void qsort(
    void *_Base, 
    size_t _NumOfElements, 
    size_t _SizeOfElements, 
    int(*_PtFuncCompare)(const void *, const void *)) 
```

## clang

```c++
void __cdecl qsort(
    void *_Base, 
    size_t _NumOfElements, 
    size_t _SizeOfElements, 
    _CoreCrtNonSecureSearchSortCompareFunction _CompareFunction) 
```

## MSVC

```c++
void __cdecl qsort(
    void *_Base, 
    size_t _NumOfElements, 
    size_t _SizeOfElements, 
    _CoreCrtNonSecureSearchSortCompareFunction _CompareFunction) 
```