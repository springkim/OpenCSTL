// #define OPENCSTL_TRACER
// #include "opencstl/opencstl.h"
//
// int main() {
//     FILE *fp = file.open("../test.txt", "r");
//     char *buf = file.read(fp);
//
//     puts(buf);
//     char *buf2 = string.reverse(buf);
//     free(buf);
//
//     puts(buf2);
//
//     FILE *fp2 = file.write(fp, buf2);
//     file.close(fp2);
//
//     free(buf2);
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opencstl.h"

static int str_cmp(const void *a, const void *b)
{
  const char * const *pa = a;
  const char * const *pb = b;
  return strcmp(*pa, *pb);
}

int main(void)
{
  DEQUE(char *) array = new_deque(char *);
  if (!array) {
    abort();
  }

  push_front(array, "World");
  push_front(array, "Hello");
  push_front(array, "!");

  // WORKAROUND: Use qsort directly on the underlying contiguous storage.
  qsort(array, size(array), sizeof(char *), str_cmp);

  for (int i = 0; i < size(array); i++) {
    printf("%s\n", array[i]);
  }

  destroy(array);
  return 0;
}
