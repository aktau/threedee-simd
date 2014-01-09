#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <threedee/threedee.h>

#include "timer.h"
#include "bench.h"


static const char *printv(vec4 vec) {
    static char buffer[256];

    const float *x = (const float*)&vec;
    snprintf(buffer, 256, "%2.3f\t%2.3f\t%2.3f\t%2.3f", x[0], x[1], x[2], x[3]);

    return buffer;
}

struct bench_result {
    double time;
    double stddev;
    vec4 val;
    const char *name;
};

struct stat_result {
    double avg;
    double stddev;
};

static int compare(const void *pa, const void *pb) {
    double a = (*(struct bench_result *) pa).time;
    double b = (*(struct bench_result *) pb).time;
    return (a > b) ? 1 : (a < b) ? -1 : 0;
}

#define STAT_ROUNDS 10

/* execute and get some "statistical" values, pull requests welcome (clean
 * cache...) */
static struct stat_result stat_exec(benchfn fn) {
    double results[STAT_ROUNDS] = {0};

    fn();

    for (int i = 0; i < STAT_ROUNDS; ++i) {
        double t1 = nsticks();
        fn();
        results[i] = nsticks() - t1;
    }

    double tot = 0;
    for (int i = 0; i < STAT_ROUNDS; ++i) {
        tot += results[i];
    }
    double avg = tot / STAT_ROUNDS;

    double stddev = 0;
    for (int i = 0; i < STAT_ROUNDS; ++i) {
        double diff = results[i] - avg;
        stddev += diff * diff;
    }
    stddev = sqrt(stddev / STAT_ROUNDS);

    return (struct stat_result){
        .avg = avg,
        .stddev = stddev,
    };
}

int benchmark(benchfn *fns, const char *names[], size_t count) {
    struct bench_result *results = malloc(sizeof(struct bench_result) * count);

    timeinit();

    for (size_t i = 0; i < count; ++i) {
        results[i].val = fns[i]();
        results[i].name = names[i];
        struct stat_result res = stat_exec(fns[i]);
        results[i].time = res.avg;
        results[i].stddev = res.stddev;
    }

    qsort(results, count, sizeof(results[0]), compare);

    double best = results[0].time;

    for (size_t i = 0; i < count; ++i) {
        double t = results[i].time;
        double s = results[i].stddev;
        fprintf(stderr, "%15s: %3.1f%% - time: %10.3f (s: %10.2f, %3.1f%%)\n",
                results[i].name, (t * 100.0) / best, t, s, (s * 100.0) / t);
    }

    free(results);

    return 1;
}
