/**
 * Copyright (c) 2013 Nicolas Hillegeer
 * Copyright (c) 2013 Riku Salminen
 *
 * All matrices used by this library are in column-major format
 *
 * The reference values for cos/sin/tan et cetera are calculated
 * by the standard C library, as they are slow but accurate
 */

#include "test.h"
#include "matrix-test.h"

static void test_quat_to_mat() {
    const float cos45 = cosf(T_PI4);
    const float sin45 = sinf(T_PI4);

    /* rotate 45 degrees around the y-axis */
    {
        mat4 rot_y_45_ref = {{
            { cos45,    0,  -sin45,  0 },
            { 0,        1,  0,      0 },
            { sin45,    0,  cos45,  0 },
            { 0,        0,  0,      1 }
        }};

        vec4 axisangle = vec(0.0f, 1.0f, 0.0f, T_PI4);
        vec4 qua       = quat_axisangle(axisangle);

        {
            const char *name  = "quaternion to matrix, rotate 45 deg around y (default)";
            mat4 mat_from_qua = quat_to_mat(qua);

            compare_mat_equal(rot_y_45_ref, mat_from_qua, name);
        }

        {
            const char *name  = "quaternion to matrix, rotate 45 deg around y (mmul)";
            mat4 mat_from_qua = quat_to_mat_mmul(qua);

            compare_mat_equal(rot_y_45_ref, mat_from_qua, name);
        }

        {
            const char *name  = "quaternion to matrix, rotate 45 deg around y (scalar)";
            mat4 mat_from_qua = quat_to_mat_scalar(qua);

            compare_mat_equal(rot_y_45_ref, mat_from_qua, name);
        }
    }

    const float cos30 = cosf(T_PI6);
    const float sin30 = sinf(T_PI6);

    {
        mat4 rot_x_30_ref = {{
            { 1,        0,      0,      0 },
            { 0,        cos30,  sin30,  0 },
            { 0,        -sin30, cos30,  0 },
            { 0,        0,      0,      1 }
        }};

        vec4 axisangle = vec(1.0f, 0.0f, 0.0f, T_PI6);
        vec4 qua       = quat_axisangle(axisangle);

        {
            const char *name  = "quaternion to matrix, rotate 30 deg around x (default)";
            mat4 mat_from_qua = quat_to_mat(qua);

            compare_mat_equal(rot_x_30_ref, mat_from_qua, name);
        }

        {
            const char *name  = "quaternion to matrix, rotate 30 deg around x (mmul)";
            mat4 mat_from_qua = quat_to_mat_mmul(qua);

            compare_mat_equal(rot_x_30_ref, mat_from_qua, name);
        }

        {
            const char *name  = "quaternion to matrix, rotate 30 deg around x (scalar)";
            mat4 mat_from_qua = quat_to_mat_scalar(qua);

            compare_mat_equal(rot_x_30_ref, mat_from_qua, name);
        }
    }
}

#define BENCH_ROUNDS 10000000

#define BENCH_QUAT_FROM_AXISANGLE(_fn) \
    NO_INLINE static vec4 loop_ ## _fn() { \
        vec4 axisangle = vec(0.0f, 1.0f, 0.0f, T_PI4); \
        vec4 qua = _fn(axisangle); \
        for (int i = 0; i < BENCH_ROUNDS; ++i) { \
            qua = _fn(qua); \
        } \
        return qua; \
    }

BENCH_QUAT_FROM_AXISANGLE(quat_axisangle)
BENCH_QUAT_FROM_AXISANGLE(quat_axisangle_clever)
BENCH_QUAT_FROM_AXISANGLE(quat_axisangle_shuf)

static void bench_quat_from_axisangle() {
    printf(BENCH_LINE "\n");

    {
        benchfn fns[] = { loop_quat_axisangle, loop_quat_axisangle_clever, loop_quat_axisangle_shuf };
        const char *names[] = { "default", "clever", "shuf" };
        benchmark(fns, names, NELEMS(fns));
    }
}

int main() {
    test_quat_to_mat();
    test_mmmul();
    test_mvmul();

    printf("\nrunning benchmarks:\n");
    bench_mvmul();
    bench_quat_from_axisangle();

    return 0;
}
