#!usr/bin/python

import os
import subprocess
import numpy as np

DATAMINING_DIR="datamining"
MEDLEY_DIR="medley"
STENCIL_DIR="stencils"
LINEAR_ALGEBRA_BLAS_DIR="linear-algebra/blas"
LINEAR_ALGEBRA_KERNEL_DIR="linear-algebra/kernels/"
LINEAR_ALGEBRA_SOLVER_DIR="linear-algebra/solvers"
LINEAR_ALGEBRA_BLAS=["gemm", "gesummv", "gemver"]
LINEAR_ALGEBRA_KERNEL=["2mm", "3mm", "atax", "bicg", "mvt"]
STENCIL=["adi", "fdtd-2d", "heat-3d", "jacobi-1d", "jacobi-2d"]
MEDLEY=["deriche"]
OPENMP_LIST = LINEAR_ALGEBRA_BLAS + LINEAR_ALGEBRA_KERNEL + STENCIL + MEDLEY

def result_compare(seq, para):
    if len(seq) != len(para):
        print("Length does not match. {} != {}".format(len(seq), len(para)))
        return False
    for i in range(len(seq)):
        if seq[i] != para[i]:
            print("Value [{}] does not match. {} != {}".format(i, seq[i], para[i]))
            return False
    return True


def compile(path):
    subprocess.check_output(["make", "clean", "-C", path])
    subprocess.check_output(["make", "-C", path])


def load_dump_array(cmd):

    p = subprocess.Popen([cmd], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    start_load = False
    dump_content = list()
    for l in list(filter(None, stderr.decode().split("\n"))):
        if l.startswith("begin dump:"):
            start_load = True
            continue
        elif l.startswith("end   dump:"):
            start_load = False
            break
        if start_load:
            dump_content += [float(e) for e in list(filter(None, l.split(" ")))]
    return dump_content



if __name__ == '__main__':
    for prog in OPENMP_LIST:
        print(prog)
        if prog in LINEAR_ALGEBRA_BLAS:
            compile(LINEAR_ALGEBRA_BLAS_DIR + "/" + prog)
            seq_result = load_dump_array("{}/{}/{}".format(LINEAR_ALGEBRA_BLAS_DIR, prog, prog))
        elif prog in LINEAR_ALGEBRA_KERNEL:
            compile(LINEAR_ALGEBRA_KERNEL_DIR + "/" + prog)
            seq_result = load_dump_array("./{}/{}/{}".format(LINEAR_ALGEBRA_KERNEL_DIR, prog, prog))
        elif prog in STENCIL:
            compile(STENCIL_DIR + "/" + prog)
            seq_result = load_dump_array("./{}/{}/{}".format(STENCIL_DIR, prog, prog))
        elif prog in MEDLEY:
            compile(MEDLEY_DIR + "/" + prog)
            seq_result = load_dump_array("./{}/{}/{}".format(MEDLEY_DIR, prog, prog))
        par_result = load_dump_array("./bin/{}".format(prog))
        # print(seq_result)
        # print(par_result)
        if not result_compare(seq_result, par_result):
            print("[{}] Sanity Check Fail".format(prog))
        else:
            print("[{}] Sanity Check Pass".format(prog))


