#include "solution.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

uint32_t solution_init(solution_t* solution_ptr, uint32_t n, uint32_t m, uint32_t is_unbounded) {
    if (!solution_ptr) {
        return 0;
    }

    gsl_vector* x = gsl_vector_calloc(m);
    if (!x) {
        return 0;
    }

    solution_ptr->n = n;
    solution_ptr->m = m;
    solution_ptr->x = x;
    solution_ptr->z = 0.0;
    solution_ptr->is_unbounded = is_unbounded;
    solution_ptr->pI_iter = 0;
    solution_ptr->pII_iter = 0;

    return 1;
}

// Checks if the i-th component of the solution is an integer
uint32_t solution_var_is_integer(const solution_t* solution_ptr, uint32_t i) {
    if (!solution_ptr) {
        fprintf(stderr, "solution_ptr is NULL in solution_var_is_integer\n");
        return 0;
    }

    double xi = gsl_vector_get(solution_ptr->x, i);
    double diff = fabs(xi - round(xi));
    return diff < 1e-8;
}

// Pretty print
void solution_print(const solution_t* solution_ptr, const char* name) {
    if (!solution_ptr) {
        return;
    }

    printf("\n================== %s ==================\n", name);
    if (solution_ptr->is_unbounded) {
        printf("infinite\n");
    } else {
        printf("Optimal found in %u iterations (PhaseI %u + PhaseII %u)\nz*: %lf\nx*: (",
               solution_ptr->pI_iter + solution_ptr->pII_iter, solution_ptr->pI_iter, solution_ptr->pII_iter,
               solution_ptr->z);
    }

    if (!solution_ptr->is_unbounded) {
        for (uint32_t i = 0; i < solution_ptr->m; i++) {
            printf("%.3lf", gsl_vector_get(solution_ptr->x, i));
            if (i < solution_ptr->m - 1) {
                printf(", ");
            }
        }
        puts(")");
    }
}

void solution_free(solution_t* solution_ptr) {
    if (!solution_ptr) {
        return;
    }

    gsl_vector_free(solution_ptr->x);
}

/* GETTERS */
const gsl_vector* solution_x(const solution_t* solution_ptr) {
    return solution_ptr ? solution_ptr->x : NULL;
}

gsl_vector* solution_x_mut(solution_t* solution_ptr) {
    return solution_ptr ? solution_ptr->x : NULL;
}

double solution_z(const solution_t* solution_ptr) {
    return solution_ptr ? solution_ptr->z : 0.0;
}

uint32_t solution_is_unbounded(const solution_t* solution_ptr) {
    return solution_ptr ? solution_ptr->is_unbounded : 0;
}

uint32_t solution_pI_iterations(const solution_t* solution_ptr) {
    return solution_ptr ? solution_ptr->pI_iter : 0;
}

uint32_t solution_pII_iterations(const solution_t* solution_ptr) {
    return solution_ptr ? solution_ptr->pII_iter : 0;
}

/* SETTERS */

uint32_t solution_set_optimal_value(solution_t* solution_ptr, double optimal_value) {
    if (!solution_ptr) {
        return 0;
    }

    solution_ptr->z = optimal_value;

    return 1;
}

uint32_t solution_set_pI_iter(solution_t* solution_ptr, uint32_t pI_iter) {
    if (!solution_ptr) {
        return 0;
    }

    solution_ptr->pI_iter = pI_iter;

    return 1;
}

uint32_t solution_set_pII_iter(solution_t* solution_ptr, uint32_t pII_iter) {
    if (!solution_ptr) {
        return 0;
    }

    solution_ptr->pII_iter = pII_iter;

    return 1;
}