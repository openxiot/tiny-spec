/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

/*
 * Minimal self contained test harness.  Each test executable includes this
 * header once and defines main() as:
 *
 *     int main(void)
 *     {
 *         TINY_TEST_BEGIN("module");
 *         test_something();
 *         return TINY_TEST_FINISH();
 *     }
 */

#ifndef TINYSPEC_TEST_H
#define TINYSPEC_TEST_H

#include <stdio.h>

static int tiny_test_failures = 0;
static int tiny_test_checks = 0;
static const char *tiny_test_name = "test";

#define TINY_TEST_BEGIN(name)   \
    do                          \
    {                           \
        tiny_test_name = (name); \
        tiny_test_failures = 0; \
        tiny_test_checks = 0;   \
    } while (0)

#define TINY_TEST_CHECK(condition)                                                      \
    do                                                                                  \
    {                                                                                   \
        ++tiny_test_checks;                                                             \
        if (!(condition))                                                               \
        {                                                                               \
            fprintf(stderr, "FAIL [%s] %s:%d: %s\n",                                    \
                    tiny_test_name, __FILE__, __LINE__, #condition);                    \
            ++tiny_test_failures;                                                       \
        }                                                                               \
    } while (0)

#define TINY_TEST_CHECK_MSG(condition, ...)                                             \
    do                                                                                  \
    {                                                                                   \
        ++tiny_test_checks;                                                             \
        if (!(condition))                                                               \
        {                                                                               \
            fprintf(stderr, "FAIL [%s] %s:%d: %s: ",                                    \
                    tiny_test_name, __FILE__, __LINE__, #condition);                    \
            fprintf(stderr, __VA_ARGS__);                                               \
            fprintf(stderr, "\n");                                                      \
            ++tiny_test_failures;                                                       \
        }                                                                               \
    } while (0)

#define TINY_TEST_FINISH() tiny_test_finish()

static int tiny_test_finish(void)
{
    if (tiny_test_failures == 0)
    {
        printf("PASS %s (%d checks)\n", tiny_test_name, tiny_test_checks);
        return 0;
    }

    fprintf(stderr, "FAIL %s (%d of %d checks failed)\n",
            tiny_test_name, tiny_test_failures, tiny_test_checks);

    return 1;
}

#endif /* TINYSPEC_TEST_H */
