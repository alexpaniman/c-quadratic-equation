#include "test-framework.h"
#include "quadratic-equation-solver.h"
#include <cmath>

TEST(quadratic_equation_with_two_irrational_roots) {
    equation_solution solution
        = solve_quadratic_equation(-5.0, 5.0, 5.0);
    
    ASSERT_EQUAL(solution.status, FINITE_ROOTS);
    ASSERT_EQUAL(solution.number_of_roots, 2);

    ASSERT_EPSILON_EQUAL(solution.root[0], (1 - sqrt(5)) / 2.0);
    ASSERT_EPSILON_EQUAL(solution.root[1], (1 + sqrt(5)) / 2.0);
}

TEST(quadratic_equation_with_two_rational_roots) {
    equation_solution solution
        = solve_quadratic_equation(-7.0, 10.0, -3.0);
    
    ASSERT_EQUAL(solution.status, FINITE_ROOTS);
    ASSERT_EQUAL(solution.number_of_roots, 2);

    ASSERT_EPSILON_EQUAL(solution.root[0], 3.0 / 7.0);
    ASSERT_EPSILON_EQUAL(solution.root[1], 1.0);
}

TEST(quadratic_equation_with_one_rational_root) {
    equation_solution solution
        = solve_quadratic_equation(4.0, 4.0, 1.0);
    
    ASSERT_EQUAL(solution.status, FINITE_ROOTS);
    ASSERT_EQUAL(solution.number_of_roots, 1);

    ASSERT_EPSILON_EQUAL(solution.root[0], - 0.5);
}

TEST(quadratic_equation_with_zero_roots) {
    equation_solution solution
        = solve_quadratic_equation(9.0, -7.0, 16);
    
    ASSERT_EQUAL(solution.status, FINITE_ROOTS);
    ASSERT_EQUAL(solution.number_of_roots, 0);
}

TEST(linear_equation_with_one_root) {
    equation_solution solution
        = solve_quadratic_equation(0.0, 10.0, -3.0);
    
    ASSERT_EQUAL(solution.status, FINITE_ROOTS);
    ASSERT_EQUAL(solution.number_of_roots, 1);

    ASSERT_EPSILON_EQUAL(solution.root[0], 3.0 / 10.0);
}

TEST(linear_equation_with_inf_roots) {
    equation_solution solution
        = solve_quadratic_equation(0.0, 0.0, 0.0);
    
    ASSERT_EQUAL(solution.status, INF_ROOTS);
}

TEST(linear_equation_with_zero_roots) {
    equation_solution solution
        = solve_quadratic_equation(0.0, 0.0, 3.0);
    
    ASSERT_EQUAL(solution.status, FINITE_ROOTS);
    ASSERT_EQUAL(solution.number_of_roots, 0);
}

TEST(nan_illegal_input) {
    equation_solution solution
        = solve_quadratic_equation(1.0, NAN, 3.0);
    
    ASSERT_EQUAL(solution.status, ILLEGAL_INPUT);
}

TEST(infinity_illegal_input) {
    equation_solution solution
        = solve_quadratic_equation(8.0, 2.0, INFINITY);
    
    ASSERT_EQUAL(solution.status, ILLEGAL_INPUT);
}

TEST_MAIN()
