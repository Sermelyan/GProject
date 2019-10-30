#!/usr/bin/env bash

set -e
set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

print_header "RUN cppcheck"

CPPCHECK="cppcheck"

${CPPCHECK} src --enable=all -I include # --check-config

print_header "RUN cpplint.py"
python2.7 ./linters/cpplint/cpplint.py include/* src/*

print_header "SUCCESS"
