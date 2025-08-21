#!/bin/bash
python3 ../helper_scripts/coverage_dir_visualizer.py -i \
    coverage_log/ul_no_cov_k_0.25 \
    coverage_log/ul_srsenb_cov_beta_2_k_0.25 \
    coverage_log/ul_srsenb_cov_beta_2_k_0.5 \
    coverage_log/ul_srsenb_cov_beta_2_k_1 \
    coverage_log/ul_srsenb_cov_beta_2_k_3 \
    coverage_log/ul_srsenb_cov_beta_2_k_4 \
    -l \
    "RANDOM UL FUZZER (\$k\$=0.25)" \
    "GREY-BOX UL FUZZER (\$\beta\$=2, \$k\$=0.25)" \
    "GREY-BOX UL FUZZER (\$\beta\$=2, \$k\$=0.5)" \
    "GREY-BOX UL FUZZER (\$\beta\$=2, \$k\$=1)" \
    "GREY-BOX UL FUZZER (\$\beta\$=2, \$k\$=3)" \
    "GREY-BOX UL FUZZER (\$\beta\$=2, \$k\$=4)" \
    -y \
    "SRSENB COVERAGE"