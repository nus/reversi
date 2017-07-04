#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#define RUN(f) do { printf("%s\n", #f); f(); } while(0)

#endif // TEST_H
