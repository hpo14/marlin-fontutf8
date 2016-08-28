#!/bin/sh


cat > ../test.c <<EOF
#include "lcdprint.h"


extern void test_show_uchar();

int
main(void)
{
  test_show_uchar();
  return 0;
}
EOF

g++ -g -Wall -I. ../test.c lcdprint.c fontutils.cpp  lcdprint_hd44780.cpp && ./a.out
