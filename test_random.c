#include "opencstl/opencstl.h"

#define NUM_BUCKETS         10
#define SAMPLES_PER_THREAD  100000

typedef struct {
    int id;
    long counts[NUM_BUCKETS];
    long total;
} thread_data_t;

static void *worker(void *arg) {
    thread_data_t *td = (thread_data_t *) arg;
    // Each thread has its own TLS RNG state; seed it independently.
    mt19937.seed(random_device());
    long i;
    for (i = 0; i < SAMPLES_PER_THREAD; i++) {
        int r = (int) mt19937.randint(0, NUM_BUCKETS - 1);
        if (r >= 0 && r < NUM_BUCKETS) {
            td->counts[r]++;
            td->total++;
        }
    }
    return NULL;
}

int main(void) {
    mt19937.seed(random_device());

    int n = cpu_count();
    if (n < 2) { n = 2; }

    printf("threads        : %d\n", n);
    printf("samples/thread : %d\n", SAMPLES_PER_THREAD);
    printf("buckets        : %d (range [0, %d])\n\n", NUM_BUCKETS, NUM_BUCKETS - 1);

    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * n);
    thread_data_t *data = (thread_data_t *) calloc((size_t) n, sizeof(thread_data_t));

    double t0 = ttime();
    {
        int i;
        for (i = 0; i < n; i++) {
            data[i].id = i;
            pthread_create(&threads[i], NULL, worker, &data[i]);
        }
        for (i = 0; i < n; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    double t1 = ttime();

    long agg[NUM_BUCKETS] = {0};
    long total = 0;
    {
        int i, b;
        for (i = 0; i < n; i++) {
            for (b = 0; b < NUM_BUCKETS; b++) {
                agg[b] += data[i].counts[b];
            }
            total += data[i].total;
        }
    }

    double expected = (double) total / (double) NUM_BUCKETS;
    double chi2 = 0.0;

    printf("bucket   count        deviation\n");
    printf("------   ----------   ---------\n");
    {
        int b;
        for (b = 0; b < NUM_BUCKETS; b++) {
            double diff = (double) agg[b] - expected;
            chi2 += (diff * diff) / expected;
            printf("  %2d     %-10ld   %+.3f%%\n",
                   b, agg[b], diff * 100.0 / expected);
        }
    }

    long expected_total = (long) n * SAMPLES_PER_THREAD;
    printf("\ntotal samples  : %ld\n", total);
    printf("expected total : %ld\n", expected_total);
    printf("elapsed        : %.2f ms\n", t1 - t0);
    printf("chi-square     : %.3f  (df=%d, 0.05 critical ~ 16.92)\n",
           chi2, NUM_BUCKETS - 1);

    if (total != expected_total) {
        printf("\n[!] lost %ld samples — likely data race on shared mt19937 state\n",
               expected_total - total);
    }
    if (chi2 > 16.92) {
        printf("[!] chi-square exceeds 0.05 critical value — distribution looks non-uniform\n");
    } else {
        printf("[ok] distribution within 0.05 significance bound\n");
    }

    free(threads);
    free(data);
    return 0;
}
