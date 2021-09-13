#ifndef QUADRATIC_EQUATION_SOLVER_SOLVER_H
#define QUADRATIC_EQUATION_SOLVER_SOLVER_H

/**
   @brief Enum of possible statuses of an equation solution
 */
enum solution_status {
    FINITE_ROOTS, /**< @brief An equation has a finite number of roots */
    INF_ROOTS,    /**< @brief An equation has an infinite number of roots */
    ILLEGAL_INPUT /**< @brief Solution failed due to illegal input */
};

/**
   @brief Struct that represents the solution of some equation

   @note Equation of N's power might have up to N roots.
   For now, this struct only allocates space for two roots which is
   enough only for linear and quadratic equations. If in the future
   equations of bigger power needed to be represented by this,
   this number should be manually adjusted.
 */
struct equation_solution {
    const solution_status status; /** @brief Status of the current solution
                                      @note Can be #ILLEGAL_INPUT that
                                      represents failure due to illegal input */

    const int number_of_roots;    /**< @brief Number of real roots of an equation
                                       @note Guaranteed to be >= zero. */

    const double root[2];         /**< @brief Real roots of the equation
                                       @note First #number_of_roots are real roots
                                       and the rest is zero-initialized. */
};

/**
   @brief Solve linear equation of form bx + c == 0

   @param b Coefficient b of the equation
   @param c Coefficient c of the equation

   @return Equation solution
 */
equation_solution solve_linear_equation(const double b, const double c);


/**
   @brief Solve quadratic equation of form ax^2 + bx + c == 0

   @param [in] a Coefficient a of the equation
   @param [in] b Coefficient b of the equation
   @param [in] c Coefficient c of the equation

   @return Equation solution
 */
equation_solution
    solve_quadratic_equation(const double a, const double b, const double c);

/**
   @brief Describe the solution of an equation as a string in English

   @param [in]  solution    Solution to describe
   @param [in]  buffer_size Max number of bytes that allowed to write in buffer
   @param [out] buffer      Output buffer

   @return Number of characters written (excluding null-terminator)

   @note If the buffer is too small, then output is truncated, and return
   value is the number of characters that would have been written to the
   final string if enough space had been available.

   @note You can pass NULL buffer, and still get the size of output string
   as return value (excluding null-terminator). This is useful to allocate
   a buffer of the needed size.

   @note Return number less than zero indicates an error.
   This can happen in 3 circumstances:

   1. You passed @p buffer_size smaller than zero.
   2. Buffer is NULL and the current C version forbids passing NULL buffer to
   snprintf. Note this should never be the case with C99 or later because
   it would contradict the standard.
   3. Solution's state field has an illegal value.
*/
int describe_equation_solution(const equation_solution* solution,
                               int buffer_size, char *buffer);
#endif // QUADRATIC_EQUATION_SOLVER_SOLVER_H
