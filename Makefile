# Trying to compile with a makefile for the first time.

all:	# Build all files
	gcc src/sequential.c src/convolve.c src/sobel.c src/emboss.c src/kernels.c -o out -lm
