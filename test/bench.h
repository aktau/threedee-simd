#ifndef BENCH_H
#define BENCH_H

typedef vec4 (*benchfn)(void);

int benchmark(benchfn *fns, const char *names[], size_t count);

#endif
