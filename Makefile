# Trying to compile with a makefile for the first time.

all:	# Build all files
	mpicc src/parallel.c src/convolve.c src/sobel.c src/emboss.c src/kernels.c -o out -lm
