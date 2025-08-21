#!/bin/bash
python3 ../helper_scripts/coverage_dir_visualizer.py -i \
    coverage_log/dl_no_cov_k_0.25 \
    coverage_log/dl_no_cov_k_0.5 \
    coverage_log/dl_no_cov_k_0.75 \
    coverage_log/dl_no_cov_k_1 \
    coverage_log/dl_no_cov_k_2 \
    coverage_log/dl_no_cov_k_3 \
    -l \
    "RANDOM FUZZER (\$k\$=0.25)" \
    "RANDOM FUZZER (\$k\$=0.5)" \
    "RANDOM FUZZER (\$k\$=0.75)" \
    "RANDOM FUZZER (\$k\$=1)" \
    "RANDOM FUZZER (\$k\$=2)" \
    "RANDOM FUZZER (\$k\$=3)" \
    -y \
    "SRSUE COVERAGE" \
    -c \
    "green" \
    "orange" \
    "red" \
    "black" \
    "brown" \
    "aqua"