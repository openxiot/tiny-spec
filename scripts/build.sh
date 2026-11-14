#!/usr/bin/env bash
#
# TinySpec build/test driver.
#
# One entry point for the commonly used configuration matrix. Every mode
# configures a CMake build tree, builds the library with the test suite and
# then runs ctest. The build tree for a mode is fixed so that a second run
# reuses the configured directory (incremental).
#
# Usage:
#   ./scripts/build.sh [mode]
#
# Modes:
#   default  - plain Debug build into build/          (default)
#   asan     - Address/UB sanitizer build into build-asan/
#   shared   - shared library build into build-shared/
#
# Sanitizer note: LeakSanitizer is not supported by AddressSanitizer on macOS
# (see README "Tests"), so the asan mode disables detect_leaks there. On Linux
# detect_leaks is left on, which makes the asan mode also verify for leaks.
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${PROJECT_DIR}"

MODE="${1:-default}"
JOBS="$(getconf _NPROCESSORS_ONLN 2>/dev/null || printf '4')"

# --- resolve mode flags -------------------------------------------------------
BUILD_DIR=""
CMAKE_ARGS=(-DTINYSPEC_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug)
IS_ASAN=0
TEST_ENV=()

case "${MODE}" in
    default|debug|release)
        BUILD_DIR="build"
        ;;
    asan|sanitize|sanitizer)
        BUILD_DIR="build-asan"
        CMAKE_ARGS+=(-DTINYSPEC_ENABLE_SANITIZERS=ON)
        IS_ASAN=1
        ;;
    shared)
        BUILD_DIR="build-shared"
        CMAKE_ARGS+=(-DBUILD_SHARED_LIBS=ON)
        ;;
    *)
        printf 'unknown mode: %s\n' "${MODE}" >&2
        printf 'usage: %s [default|asan|shared]\n' "$0" >&2
        exit 2
        ;;
esac

# --- configure ----------------------------------------------------------------
printf '==> configure: %s (%s)\n' "${BUILD_DIR}" "${MODE}"
cmake -S . -B "${BUILD_DIR}" "${CMAKE_ARGS[@]}"

# --- build --------------------------------------------------------------------
printf '==> build (%d jobs)...\n' "${JOBS}"
cmake --build "${BUILD_DIR}" -j "${JOBS}"

# --- test ---------------------------------------------------------------------
if [ "${IS_ASAN}" -eq 1 ]; then
    # UBSan: stop on the first error so a bad shift/overflow is not lost in
    # the noise of a long suite.
    TEST_ENV+=("UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1")

    # LSan: unavailable on macOS, keep on elsewhere.
    ASAN_OPTIONS=""
    case "$(uname -s)" in
        Darwin)
            printf '==> sanitizer: LeakSanitizer unsupported on macOS, detect_leaks off\n'
            ASAN_OPTIONS="detect_leaks=0"
            ;;
        *)
            ASAN_OPTIONS="detect_leaks=1"
            ;;
    esac
    TEST_ENV+=("ASAN_OPTIONS=${ASAN_OPTIONS}")
fi

printf '==> test\n'
if [ "${#TEST_ENV[@]}" -gt 0 ]; then
    env "${TEST_ENV[@]}" ctest --test-dir "${BUILD_DIR}" --output-on-failure
else
    ctest --test-dir "${BUILD_DIR}" --output-on-failure
fi

printf '\n==> done: %s all green\n' "${MODE}"