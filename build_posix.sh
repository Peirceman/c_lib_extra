#!/bin/sh

set -e

CC=cc
CFLAGS="-Wall -Wno-format -pedantic -O2 -I./src/include"

mkdir out

$CC $CFLAGS -o out/vector_example     examples/vector_example.c
$CC $CFLAGS -o out/hash_table_example examples/hash_table_example.c src/hash_table.c
