#include "test.h"

static mat4 rnd = {{
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
}};

static mat4 cns = {{
    { 1, 2, 3, 4 },
    { 1, 2, 3, 4 },
    { 1, 2, 3, 4 },
    { 1, 2, 3, 4 },
}};

static void test_mmmul() {
    printf("test mmmul:\n");

    mat4 a = midentity();
    mat4 b = smmul(15, a);

    mat4 scal = mmmul(a, b);

    compare_mat_equal(scal, b, "identity preserves");
}

__attribute__((always_inline)) static inline vec4 mvmul_simple(const mat4 m, const vec4 v) {
    return
        m.cols[0] * vsplat(v, 0) +
        m.cols[1] * vsplat(v, 1) +
        m.cols[2] * vsplat(v, 2) +
        m.cols[3] * vsplat(v, 3);
}

#define BENCH_ROUNDS 10000000

#define BENCH_MVMUL(_fn) \
    NO_INLINE static vec4 loop_ ## _fn() { \
        mat4 m = midentity(); \
        vec4 v = vec(4, 3, 1, 5); \
        for (int i = 0; i < BENCH_ROUNDS; ++i) { \
            v = _fn(m, v); \
        } \
        return v; \
    }

#define BENCH_MVMUL_COLS(_fn) \
NO_INLINE static vec4 loop_ ## _fn() { \
    mat4 m = midentity(); \
    vec4 v = vec(4, 3, 1, 5); \
    for (int i = 0; i < BENCH_ROUNDS; ++i) { \
        v = _fn(m.cols[0], m.cols[1], m.cols[2], m.cols[3], v); \
    } \
    return v; \
}

BENCH_MVMUL(mvmul)
BENCH_MVMUL(mvmul_simple)
BENCH_MVMUL_COLS(mvmul_add_cols)
BENCH_MVMUL_COLS(mvmul_madd_cols)

static void test_mvmul() {
    printf("test mvmul:\n");

    vec4 v = vec(1.0f, 2.0f, 3.0f, 4.0f);
    mat4 b = smmul(15, midentity());

    compare_vec_equal(v, mvmul(midentity(), v), "identity preserves");
    compare_vec_equal(v * vscalar(15.0f), mvmul(b, v), "scaled");

    vec4 one = vscalar(1.0f);

    compare_vec_equal(vec(4,8,12,16), mvmul(cns, one), "constant column");
    compare_vec_equal(vec(4,8,12,16), mvmul_add_cols(cns.cols[0], cns.cols[1], cns.cols[2], cns.cols[3], one), "constant column (column add)");
    compare_vec_equal(vec(4,8,12,16), mvmul_madd_cols(cns.cols[0], cns.cols[1], cns.cols[2], cns.cols[3], one), "constant column (column madd)");
}

static void bench_mvmul() {
    printf(BENCH_LINE "\n");

    {
        benchfn fns[] = { loop_mvmul_add_cols, loop_mvmul_simple, loop_mvmul, loop_mvmul_madd_cols };
        const char *names[] = { "mvmul_add", "mvmul_simple", "mvmul", "mvmul_madd" };
        benchmark(fns, names, NELEMS(fns));
    }
}
