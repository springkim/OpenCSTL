// main.c
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void (*MSORT_FUNC)(
    void *base,
    size_t number,
    size_t width,
    int (*compare)(const void *, const void *)
);

MSORT_FUNC msort_v2;

static int int_cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;

    return (x > y) - (x < y);
}

int main(void) {
    void *handle = dlopen("./msort.dylib", RTLD_NOW);

    if (!handle) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        return 1;
    }

    dlerror();

    msort_v2 = (MSORT_FUNC)dlsym(handle, "msort");

    {
        const char *err = dlerror();
        if (err) {
            fprintf(stderr, "dlsym failed: %s\n", err);
            dlclose(handle);
            return 1;
        }
    }

    int arr[] = { 5, 1, 9, 3, 7, 2, 8, 6, 4, 0 };
    size_t n = sizeof(arr) / sizeof(arr[0]);

    msort_v2(arr, n, sizeof(arr[0]), int_cmp);

    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    dlclose(handle);
    return 0;
}
