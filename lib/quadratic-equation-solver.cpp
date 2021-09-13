#include <stdio.h>
#include <cmath>

#include "quadratic-equation-solver.h"

// Epsilon for floating-point number comparison
static inline bool is_zero(const double value) {
    return fabs(value) <= 1e-9 /* EPSILON */;
}

equation_solution solve_linear_equation(const double b, const double c) {
    if (!std::isfinite(b) || !std::isfinite(c))
        return  { ILLEGAL_INPUT };

    if (is_zero(b)) { // 0x == -c
        // 0x == 0
        if (is_zero(c))
            return  { INF_ROOTS };

        // 0x != 0
        return  { FINITE_ROOTS, /* num roots */ 0 };
    }

    // bx == -c
    return  { FINITE_ROOTS, /* num roots */ 1, { - c / b } };
}

equation_solution
    solve_quadratic_equation(const double a, const double b, const double c) {

    if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c))
        return  { ILLEGAL_INPUT };

    double discriminant = b * b - 4 * a * c;

    if (is_zero(a))
        return solve_linear_equation(b, c);

    // Check for discriminant == 0
    if (is_zero(discriminant))
        return  { FINITE_ROOTS, /* num roots */ 1, { - b / (2 * a) } };

    if (discriminant < 0.0)
        return  { FINITE_ROOTS, /* num roots */ 0 };

    // From here discriminant is guaranteed to be bigger than zero
    double sqrt_from_discriminant = sqrt(discriminant);

    double root1 = (-b + sqrt_from_discriminant) / (2 * a),
           root2 = (-b - sqrt_from_discriminant) / (2 * a);

    return  { FINITE_ROOTS, /* num roots */ 2, { root1, root2 } };
}

int describe_equation_solution(const equation_solution* solution,
                               int buffer_size, char *buffer) {
    // This value changes:         ^~~~~~~~~~~

    // I could define buffer_size as size_t and avoid this check
    // but mixing unsigned and signed types could lead to unexpected
    // errors, so I stick with this:
    if (buffer_size < 0)
        return -1;

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
            return -2;

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

    case ILLEGAL_INPUT:
        return snprintf(buffer, buffer_size, "Solving failed due to illegal input");

    default:
        return -3;
    }
}
