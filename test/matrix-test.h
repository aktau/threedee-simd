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

NO_INLINE static vec4 bench_mvmul() {
    mat4 m = midentity();
    vec4 v = vec(4, 3, 1, 5);
    for (int i = 0; i < BENCH_ROUNDS; ++i) {
        v = mvmul(m, v);
    }

    return v;
}

NO_INLINE static vec4 bench_mvmul_simple() {
    mat4 m = midentity();
    vec4 v = vec(4, 3, 1, 5);
    for (int i = 0; i < BENCH_ROUNDS; ++i) {
        v = mvmul_simple(m, v);
    }

    return v;
}

NO_INLINE static vec4 bench_mvmul_add() {
    mat4 m = midentity();
    vec4 v = vec(4, 3, 1, 5);
    for (int i = 0; i < BENCH_ROUNDS; ++i) {
        v = mvmul_add_cols(m.cols[0], m.cols[1], m.cols[2], m.cols[3], v);
    }

    return v;
}

NO_INLINE static vec4 bench_mvmul_madd() {
    mat4 m = midentity();
    vec4 v = vec(4, 3, 1, 5);
    for (int i = 0; i < BENCH_ROUNDS; ++i) {
        v = mvmul_madd_cols(m.cols[0], m.cols[1], m.cols[2], m.cols[3], v);
    }

    return v;
}

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

#define BENCH_LINE "==================="

static void bnch_mvmul() {
    printf(BENCH_LINE "\n");

    {
        benchfn fns[] = { bench_mvmul_add, bench_mvmul_simple, bench_mvmul, bench_mvmul_madd };
        const char *names[] = { "mvmul_add", "mvmul_simple", "mvmul", "mvmul_madd" };
        benchmark(fns, names, NELEMS(fns));
    }
}
