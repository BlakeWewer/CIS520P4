#!/bin/bash -l

module load OpenMPI

mpirun //usr/bin/time -o /homes/bewewer/cis520/proj4/CIS520P4-1/src/3way-mpi/time/time-10k/time-$RANDOM.txt /homes/bewewer/cis520/proj4/CIS520P4-1/src/3way-mpi/output/output-10k/mpi-10k