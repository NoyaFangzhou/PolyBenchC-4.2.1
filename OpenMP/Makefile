#===- runtime/Makefile -------------------------*- Makefile -*-===##

# Relative path to the top of the source tree.
#
LEVEL=..

BIN_DIR=$(LEVEL)/bin
HEADER_DIR=./header
UTIL_DIR=$(LEVEL)/utilities

PAPI_LIB_DIR=/localdisk/local/include
PAPI_LIB=/localdisk/local/lib/libpapi.a

CC= g++
CCFLAG=-O3 -std=c++11 -g

all_bench = stencil 2mm 3mm adi atax bicg cholesky correlation covariance deriche doitgen durbin fdtd_2d floyd_warshall gemm gemver gesummv gramschmidt heat_3d jacobi_1d jacobi_2d lu ludcmp mvt nussinov seidel_2d symm syr2k syrk trisolv trmm

constant_bound_bench = 2mm 3mm mvt atax bicg gemm gemver gesummv doitgen deriche fdtd-2d floyd-warshall heat-3d jacobi-1d jacobi-2d seidel-2d adi

check:
	$(foreach name, $(constant_bound_bench), $(CC) $(CCFLAG) $(name).cpp -I$(HEADER_DIR) -I$(UTIL_DIR) -o $(BIN_DIR)/$(name) ;)


veryclean:
	rm $(BIN_DIR)/*
