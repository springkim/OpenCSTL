#!/usr/bin/env zsh

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

test_targets=("${(@f)$(ctest --test-dir "${build_dir}" -N | awk -F': ' '/  Test #[0-9]+: / { print $2 }')}")

if (( ${#test_targets[@]} == 0 )); then
    print -u2 "No CTest tests found in ${build_dir}"
    exit 1
fi

cmake --build "${build_dir}" --target "${test_targets[@]}"
ctest --test-dir "${build_dir}" --output-on-failure

rm -rf "${build_dir}"