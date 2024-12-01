#include <z3.h>
#include <stdio.h>

int main() {
    /* Create Z3 context */
    Z3_config cfg = Z3_mk_config();
    Z3_context c = Z3_mk_context(cfg);

    /* Declare a real variable x */
    Z3_sort real_sort = Z3_mk_real_sort(c);
    Z3_symbol x_symbol = Z3_mk_string_symbol(c, "x");
    Z3_ast x = Z3_mk_const(c, x_symbol, real_sort);

    /* Create the equation: (+ (* x 4) 4) = 20 */
    Z3_solver s = Z3_mk_solver(c);
    Z3_ast four = Z3_mk_real(c, 4, 1);
    Z3_ast twenty = Z3_mk_real(c, 20, 1);
    Z3_ast mul_args[2] = {x, four};
    Z3_ast x_times_4 = Z3_mk_mul(c, 2, mul_args);
    Z3_ast sum_args[2] = {x_times_4, four};
    Z3_ast equation = Z3_mk_add(c, 2, sum_args);
    Z3_ast assertion = Z3_mk_eq(c, equation, twenty);
    Z3_solver_assert(c, s, assertion);

    /* Check satisfiability */
    Z3_lbool result = Z3_solver_check(c, s);

    if (result == Z3_L_TRUE) {
        /* Get the model */
        Z3_model m;
        m = Z3_solver_get_model(c, s);

        /* Get the value of x from the model */
        Z3_ast x_value;
        Z3_model_eval(c, m, x, Z3_L_TRUE, &x_value);

        /* Print the result */
        printf("Satisfiable. x = %s\n", Z3_ast_to_string(c, x_value));
    } else if (result == Z3_L_FALSE) {
        printf("Unsatisfiable.\n");
    } else {
        printf("Unknown result.\n");
    }

    /* Clean up */
    Z3_del_context(c);
    Z3_del_config(cfg);

    return 0;
}

