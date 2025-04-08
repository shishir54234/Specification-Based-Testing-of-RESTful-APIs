#!/bin/bash

# Define log file
LOG_FILE="expoSE_log.txt"

# Compile the C++ files and redirect stdout & stderr to the log file
g++ test_libapplication.cpp jsCodeGenerator/jsCodeGen.cpp ast.hpp algo.hpp library_apispec_examples/Entry.hpp -o expoSE 2>&1 | tee -a "$LOG_FILE"

# Run the compiled program, redirect stdout to expoSECode.txt and stderr to the log file
./expoSE > expoSECode.txt 2>> "$LOG_FILE"

echo "Logs saved in $LOG_FILE"