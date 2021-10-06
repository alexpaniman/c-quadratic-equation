#ifndef QUADRATIC_EQUATION_SOLVER_SOLVER_H
#define QUADRATIC_EQUATION_SOLVER_SOLVER_H

#include <cstddef>

/**
   @brief Enum of possible statuses of an equation solution
 */
enum solution_status {
    FINITE_ROOTS, /**< @brief The equation has a finite number of roots */
    INF_ROOTS,    /**< @brief The equation has an infinite number of roots */
};


/**
   @brief Struct that represents the solution of some equation

   @note Equation of N's power might have up to N roots.
   For now, this struct only allocates space for two roots which is
   enough only for linear and quadratic equations. If in the future
   equations of bigger powers would needed to be represented, this
   number should be manually adjusted.
 */
struct equation_solution {
    solution_status status; /**< @brief Status of the current solution */

    int number_of_roots;    /**< @brief Number of real roots of the equation
                                 @note Guaranteed to be >= zero. */

    double root[2];         /**< @brief Real roots of the equation
                                 @note First #number_of_roots are real roots
                                 and the rest is zero-initialized. */
};


/**
   @brief Solve linear equation of form bx + c == 0

   @param [in]  b        Coefficient b of the equation
   @param [in]  c        Coefficient c of the equation
   @param [out] solution Pointer to output equation solution

   @return 0 on success, illegal coefficient's number (1 for a, 2 for b) otherwise.

   @note If multiple coefficients have illegal value, number of
   the first one is returned.

   @note Coefficients are called @p b and @p c to avoid confusion
   with coefficients of the #solve_quadratic_equation function.
 */
equation_solution solve_linear_equation(const double b, const double c);


/**
   @brief Solve quadratic equation of form ax^2 + bx + c == 0

   @param [in]  a        Coefficient a of the equation
   @param [in]  b        Coefficient b of the equation
   @param [in]  c        Coefficient c of the equation
   @param [out] solution Pointer to output equation solution

   @return 0 on success, illegal coefficient's number (1 for a, 2 for b, etc...)
   otherwise.

   @note If multiple coefficients have illegal value, number of
   the first one is returned.
 */
int solve_quadratic_equation(const double a, const double b, const double c,
                             equation_solution* const solution);

 
/**
   @brief Possible error codes of the #describe_equation_soution function
*/
enum describe_equation_error_code {
    CORRUPTED_SOLUTION = -1,           /**< @brief Solutions' status value doesn't correspond
                                            to #solution_status constant. */
    NULL_BUFFER_IS_NOT_SUPPORTED = -2  /**< @brief Buffer is #NULL and the current C version
                                            forbids passing NULL buffer to snprintf.
                                            @note This should never be the case with C99 or
                                            later because it would contradict the standard.*/
};

/**
   @brief Describe the solution of an equation as a string in English

   @param [in]  solution    Solution to describe
   @param [in]  buffer_size Max number of bytes that allowed to write in buffer
   @param [out] buffer      Output buffer

   @return Number of characters written (excluding null-terminator) on success,
   #describe_equation_error_code on failure.

   @note If the buffer is too small, then output is truncated, and return
   value is the number of characters (excluding null-terminator) that would
   have been written to the final string if enough space had been available.
   This works even if you pass #NULL buffer. This is useful to allocate a
   buffer of the needed size.
*/
int describe_equation_solution(const equation_solution* const solution,
                               size_t buffer_size, char* const buffer);

#endif // QUADRATIC_EQUATION_SOLVER_SOLVER_H
