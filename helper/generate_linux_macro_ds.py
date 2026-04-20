arr = [
    "cstl_push_back", "cstl_push_front", "cstl_insert",
    "cstl_erase", "cstl_resize", "cstl_find", "cstl_push", "cstl_max_size"]
check = [0, 0, 1, 1, 1, 1, 0, 0]
ret = [0, 0, 0, 0, 0, 1, 0, 1]

for t in range(len(arr)):
    name = arr[t]

print(f"#define {name}(C,...) ", end="")

if ret[t] == 1:
    print("({void* _() {", end="")
print(f"_linux_{name}(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)", end="")
print("(C", end="")
if check[t] == 1:
    print(",ARGN(__VA_ARGS__)", end="")
print(",__VA_ARGS__)", end="")
if ret[t] == 1:
    print("}_;});", end="")
print()

print(f"#define _linux_{name}(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _{name} ## _ ## N")

for i in range(11):
    print(f"#define _{name}_{i}", end="")
print("(C", end="")
if check[t] == 1:
    print(",argc", end="")
for j in range(1, i + 1):
    print(f",_{j}", end="")
print(")    {", end="")
print()

print("_CSTL_TYPEOF(&C) __0=&C;", end="")
for j in range(1, i + 1):
    print(f"_CSTL_TYPEOF(_{j}) __{j}=_{j};", end="")
    print()
if ret[t] == 1:
    print("return ", end="")
print(f"_{name}( __0", end="")
if check[t] == 1:
    print(",argc", end="")
for j in range(1, i + 1):
    print(f",&__{j}", end="")
print(");}", end="")
print()
