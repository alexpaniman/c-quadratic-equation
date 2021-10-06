#include <cstddef>
#include <stdio.h>
#include <cmath>

#include "quadratic-equation-solver.h"

// Epsilon for floating-point number comparison
static inline bool is_zero(const double value) {
    return fabs(value) <= 1e-9 /* EPSILON */;
}

int solve_linear_equation(const double b, const double c,
                          equation_solution* const solution) {

    if (!std::isfinite(b))
        return 1;

    if (!std::isfinite(c))
        return 2;


    if (is_zero(b)) { // 0x == -c
        // 0x == 0
        if (is_zero(c)) {
            *solution = { INF_ROOTS };
            return 0;
        }

        // 0x != 0
        *solution = { FINITE_ROOTS, /* num roots */ 0 };
        return 0;
    }

    // bx == -c
    *solution = { FINITE_ROOTS, /* num roots */ 1, /* root */ { - c / b } };
    return 0;
}

int solve_quadratic_equation(const double a, const double b, const double c,
                             equation_solution* const solution) {

    if (!std::isfinite(a))
        return 1;

    if (!std::isfinite(b))
        return 2;

    if (!std::isfinite(c))
        return 3;


    double discriminant = b * b - 4 * a * c;

    if (is_zero(a)) { // a == 0 => This is a linear equation
        int return_code = solve_linear_equation(b, c, solution);
        return return_code == 0 ? 0 : /* one less argument */ return_code + 1;
    }

    // Check for discriminant == 0
    if (is_zero(discriminant))
        *solution = { FINITE_ROOTS, /* num roots */ 1,
                      /* root */ { - b / (2 * a) } };

    else if (discriminant < 0.0)
        *solution = { FINITE_ROOTS, /* num roots */ 0 };

    else {
        // From here discriminant is guaranteed to be bigger than zero
        double sqrt_from_discriminant = sqrt(discriminant);

        double root1 = (-b + sqrt_from_discriminant) / (2 * a),
            root2 = (-b - sqrt_from_discriminant) / (2 * a);

        *solution = { FINITE_ROOTS, /* num roots */ 2, { root1, root2 } };
    }

    return 0;
}

int describe_equation_solution(const equation_solution* const solution,
                               size_t buffer_size, char* const buffer) {
    // This value changes:            ^~~~~~~~~~~

    switch (solution->status) {
    case FINITE_ROOTS: {
        const int number_of_roots = solution->number_of_roots;

        if (number_of_roots == 0)
            return snprintf(buffer, buffer_size, "Equation has no solution");

        int written_total =
            snprintf(buffer, buffer_size, "Equation has %d roots: ",
                     number_of_roots);

        if (buffer_size >= written_total)
            buffer_size -= written_total;

        // Some C standarts allow snprintf to return negative number
        // in case buffer is NULL. It's not the case with C99 and later,
        // but let's take precaution anyway:
        if (written_total < 0)
            return NULL_BUFFER_IS_NOT_SUPPORTED;

        for (int i = 0; i < number_of_roots; ++ i) {
            char* shifted_buffer = buffer;
            if (buffer != NULL)
                shifted_buffer += written_total;

            double current_root = solution->root[i];

            int written = 0;
            if (i != solution->number_of_roots - 1) // Not last root
                written = snprintf(shifted_buffer, buffer_size, "%lf, ",
                                   current_root); // Add comma      ^~
            else // Last root
                written = snprintf(shifted_buffer, buffer_size, "%lf",
                                   current_root);

            // Some C standarts allow snprintf to return negative number
            // in case buffer is NULL. It's not the case with C99 and later,
            // but let's take precaution anyway:
            if (written < 0)
                return -2;

            written_total += written;

            if (buffer_size >= written) 
                buffer_size -= written;
        }

        return written_total;
    }

    case INF_ROOTS:
        return snprintf(buffer, buffer_size, "This equation has infinite number of roots");

    default:
        return CORRUPTED_SOLUTION;
    }
}
