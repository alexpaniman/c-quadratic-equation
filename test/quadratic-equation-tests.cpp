#include "test-framework.h"
#include "quadratic-equation-solver.h"

#include <cmath>

#define QUADRATIC_EQUATION_ASSERT(a, b, c, expected_error_code, expected_status,           \
                                  expected_roots_count, expected_root0, expected_root1)    \
    do {                                                                                   \
        equation_solution solution;                                                        \
        int error_code = solve_quadratic_equation((a), (b), (c), &solution);               \
        ASSERT_EQUAL(error_code, (expected_error_code));                                   \
                                                                                           \
        ASSERT_EQUAL(solution.status, (expected_status));                                  \
                                                                                           \
        int expected_num_roots = (expected_roots_count);                                   \
        ASSERT_EQUAL(solution.number_of_roots, expected_num_roots);                        \
                                                                                           \
        if (expected_num_roots >= 1)                                                       \
            ASSERT_EPSILON_EQUAL(solution.root[0], (expected_root0));                      \
                                                                                           \
        if (expected_num_roots >= 2)                                                       \
            ASSERT_EPSILON_EQUAL(solution.root[1], (expected_root1));                      \
    } while(false)

#define QUADRATIC_EQUATION_ASSERT_TWO_ROOTS(a, b, c, expected_root0, expected_root1)       \
    QUADRATIC_EQUATION_ASSERT(a, b, c, 0, FINITE_ROOTS, 2, expected_root0, expected_root1) \

#define QUADRATIC_EQUATION_ASSERT_ONE_ROOT(a, b, c, expected_root)                         \
    QUADRATIC_EQUATION_ASSERT(a, b, c, 0, FINITE_ROOTS, 1, expected_root, 0.0)             \

#define QUADRATIC_EQUATION_ASSERT_ZERO_ROOTS(a, b, c)                                      \
    QUADRATIC_EQUATION_ASSERT(a, b, c, 0, FINITE_ROOTS, 0, 0.0, 0.0)                       \

#define QUADRATIC_EQUATION_ASSERT_INF_ROOTS(a, b, c)                                       \
    do {                                                                                   \
        equation_solution solution;                                                        \
        int error_code = solve_quadratic_equation((a), (b), (c), &solution);               \
        ASSERT_EQUAL(error_code, 0);                                                       \
                                                                                           \
        ASSERT_EQUAL(solution.status, INF_ROOTS);                                          \
    } while(false)

#define QUADRATIC_EQUATION_ASSERT_ERROR_CODE(a, b, c, expected_error_code)                 \
    do {                                                                                   \
        equation_solution solution;                                                        \
        int error_code = solve_quadratic_equation((a), (b), (c), &solution);               \
        ASSERT_EQUAL(error_code, (expected_error_code));                                   \
    } while(false)


TEST(quadratic_equation_with_two_irrational_roots) {
    QUADRATIC_EQUATION_ASSERT_TWO_ROOTS(/* a */ -5.0, /* b */ 5.0, /* c */ 5.0,
                                        /* First  root */ (1 - sqrt(5)) / 2.0,
                                        /* Second root */ (1 + sqrt(5)) / 2.0);
}                                               

TEST(quadratic_equation_with_two_rational_roots) {
    QUADRATIC_EQUATION_ASSERT_TWO_ROOTS(/* a */ -7.0, /* b */ 10.0, /* c */ -3.0,
                                        /* First  root */ 3.0 / 7.0,
                                        /* Second root */ 1.0);
}

TEST(quadratic_equation_with_one_rational_root) {
    QUADRATIC_EQUATION_ASSERT_ONE_ROOT(/* a */ 4.0, /* b */ 4.0, /* c */ 1.0,
                                       /* Root */ - 0.5);
}

TEST(quadratic_equation_with_zero_roots) {
    QUADRATIC_EQUATION_ASSERT_ZERO_ROOTS(/* a */ 9.0, /* b */ -7.0, /* c */ 16);
}

TEST(linear_equation_with_one_root) {
    QUADRATIC_EQUATION_ASSERT_ONE_ROOT(/* a */ 0.0, /* b */ 10.0, /* c */ -3.0,
                                       /* Root */ 3.0 / 10.0);
}

TEST(linear_equation_with_inf_roots) {
    QUADRATIC_EQUATION_ASSERT_INF_ROOTS(/* a */ 0.0, /* b */ 0.0, /* c */ 0.0);
}

TEST(linear_equation_with_zero_roots) {
    QUADRATIC_EQUATION_ASSERT_ZERO_ROOTS(/* a */ 0.0, /* b */ 0.0, /* c */ 3.0);
}

TEST(nan_illegal_input) {
    QUADRATIC_EQUATION_ASSERT_ERROR_CODE(/* a */ 1.0, /* b */ NAN, /* c */ 3.0,
                                         /* Error code */ 2);
}

TEST(infinity_illegal_input) {
    QUADRATIC_EQUATION_ASSERT_ERROR_CODE(/* a */ 8.0, /* b */ 2.0, /* c */ INFINITY,
                                         /* Error code */ 3);
}

TEST_MAIN()
