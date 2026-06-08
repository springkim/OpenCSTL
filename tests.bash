#!/usr/bin/env bash

set -euo pipefail

compiler="${1:-clang}"

case "${compiler}" in
    gcc|clang|tcc)
        ;;
    *)
        compiler="clang"
        ;;
esac

build_dir="test_build/tests-${compiler}"

cmake -S . -B "${build_dir}" -DCMAKE_C_COMPILER="${compiler}"

# Enumerate test targets from CTestTestfile.cmake directly. Using `ctest -N`
# before the build is unreliable: ctest 4.x prints "Could not find executable"
# warnings to stdout and omits the "Test #N: name" line for some tests when
# their binaries don't yet exist, so only a subset gets built.
testfile="${build_dir}/CTestTestfile.cmake"
if [[ ! -f "${testfile}" ]]; then
    echo "CTestTestfile.cmake not found in ${build_dir}" >&2
    exit 1
fi
mapfile -t test_targets < <(awk -F'[()]' '/^add_test\(/ { split($2, a, " "); print a[1] }' "${testfile}")

if (( ${#test_targets[@]} == 0 )); then
    echo "No CTest tests found in ${build_dir}" >&2
    exit 1
fi

cmake --build "${build_dir}" --target "${test_targets[@]}"
ctest --test-dir "${build_dir}" --output-on-failure

rm -rf "${build_dir}"
