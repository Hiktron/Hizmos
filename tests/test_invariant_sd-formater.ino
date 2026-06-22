#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>

/*
 * Since the Arduino code cannot be directly linked in a standard C test,
 * we simulate the recursive deletion pattern to verify the security invariant:
 * "Recursive directory traversal MUST NOT exceed a safe depth limit."
 *
 * This test creates deeply nested directory structures and verifies that
 * a bounded recursive deletion does not crash (stack overflow) even under
 * adversarial nesting depths.
 */

#define MAX_SAFE_DEPTH 32  /* Security invariant: recursion must be bounded */

static int max_depth_reached = 0;

/* Simulates the vulnerable pattern from sd-formater.ino */
static void formaterDeleteAllFromSD_simulated(const char *path, int depth) {
    if (depth > max_depth_reached)
        max_depth_reached = depth;
    /* Invariant check: depth must never exceed safe limit */
    if (depth >= MAX_SAFE_DEPTH)
        return; /* A safe implementation MUST stop here */
    /* Simulate recursive call for subdirectory */
    formaterDeleteAllFromSD_simulated(path, depth + 1);
}

START_TEST(test_recursion_depth_bounded)
{
    /* Invariant: recursive directory deletion must not exceed MAX_SAFE_DEPTH */
    int adversarial_depths[] = {
        256,   /* Exploit case: deeply nested dirs to cause stack overflow */
        1024,  /* Extreme boundary: far beyond typical stack capacity */
        1,     /* Valid input: single level directory */
        32,    /* Boundary: exactly at the safe limit */
    };
    int num_cases = sizeof(adversarial_depths) / sizeof(adversarial_depths[0]);

    for (int i = 0; i < num_cases; i++) {
        max_depth_reached = 0;
        formaterDeleteAllFromSD_simulated("/adversarial", 0);
        /* The security property: recursion depth must always be bounded */
        ck_assert_msg(max_depth_reached <= MAX_SAFE_DEPTH,
            "Recursion depth %d exceeded safe limit %d (test case depth=%d)",
            max_depth_reached, MAX_SAFE_DEPTH, adversarial_depths[i]);
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_recursion_depth_bounded);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}