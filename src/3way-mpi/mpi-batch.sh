#!/bin/bash -l

module load OpenMPI

mpirun //usr/bin/time -o /homes/bewewer/proj4/CIS520P4/src/3way-mpi/time/time-10k/time-$RANDOM.txt /homes/bewewerproj4/CIS520P4/src/3way-mpi/output/output-10k/mpi-10k