import os

N = 128
for i in range(1, N + 1):
    c_func = f'int _memcmp{i}(const void *a, const void *b) {{ return memcmp(a, b, {i}); }}'
    print(c_func)

print(f'_OpenCSTLCompareFunc _memcmp_funcs[{N+1}]={{')
print(f'\tNULL,')
for i in range(1, N + 1):
    print(f'\t_memcmp{i},')
print(f'}};')
