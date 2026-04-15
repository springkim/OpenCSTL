//
// Created by spring on 4/15/2026.
//

#ifndef OPENCSTL_VERIFY_H
#define OPENCSTL_VERIFY_H
#include <assert.h>
#include "logging.h"

#define verify(EXPR) do { if(!(EXPR)) __verify(#EXPR,__FILE__,__LINE__); } while(0)

int __verify(char *expression, char *file, int line) {
    logging.error("Verification failed: %s, file %s, line %d",
                  expression, file, line);
    abort();
}
#endif //OPENCSTL_VERIFY_H
