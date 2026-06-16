/*
 * tcc compatibility shims.
 *
 * glibc's <math.h> defines the isnan/isinf macros in terms of the compiler
 * intrinsics __builtin_isnan/__builtin_isinf. GCC and Clang implement these
 * intrinsics directly, but tcc does not: it emits ordinary external calls,
 * which fail to link with "unresolved reference to '__builtin_isnan'".
 *
 * Unity uses isnan/isinf for its floating-point assertions, so we provide the
 * intrinsics as real functions here. This file is only compiled into the test
 * executables when the C compiler is tcc (see CMakeLists.txt).
 */

int __builtin_isnan(double x)
{
    return x != x;
}

int __builtin_isinf(double x)
{
    /* Inf - Inf is NaN, while (finite - finite) and NaN cases are not Inf. */
    double diff = x - x;
    return (diff != diff) && (x == x);
}
