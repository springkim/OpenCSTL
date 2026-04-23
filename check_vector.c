#include "opencstl/opencstl.h"

int main() {
    VECTOR(int) v = new_vector(int);
    const int N = 100000000;
    FILE *fp = fstream.open("vector01.txt", "wt");
    for (int i = 0; i < N; i++) {
        push_back(v, i);
        fprintf(fp, "%d:%d\n",size(v),capacity(v));
    }
    fclose(fp);
    destroy(v);
}
