#include <stdio.h>
#include <cmath>

#include "quadratic-equation-solver.h"

static void print_introductory_message(void) {
    printf("This is quadratic equation solver!\n"
           "Copyright (c) Alex Paniman 2021 \n\n"

           "This program is designed to solve\n"
           "quadratic equations of form:\n"  
           "      -> ax^2 + bx + c == 0\n\n");
}

static double read_coefficient(const char* coefficient_name) {
    printf("Enter coefficient %s: ", coefficient_name);

    double value = NAN;
    scanf("%lf", &value);

    return value;
}

static void print_solution_description(equation_solution* solution) {
    int buffer_size =
        describe_equation_solution(solution, 0, NULL);

    if (buffer_size < 0) {
        printf("Error due non C99-compatible standart or "
               "equation_solution corruption.");
        return;
    }

    // For null-terminator '\0'
    buffer_size += 1;

    // VLA's are not advised but in this case i know
    // that description string will not be very long.

    // And I also want to show this cool behavior of
    // of describe_equation_solution that allows you
    // to get size of buffer before creating it.
    char description[buffer_size];

    describe_equation_solution(solution, buffer_size, description);
    
    printf("%s", description);
}

int main(void) {
    print_introductory_message();

    double a = read_coefficient("a");
    double b = read_coefficient("b");
    double c = read_coefficient("c");

    equation_solution solution
        = solve_quadratic_equation(a, b, c);

    print_solution_description(&solution);
}
