#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

for dir in */     # list directories in the form "/tmp/dirname/"
do
    printf "Testing ${dir}main.hd... "
    hdc "${dir}main.hd" < "${dir}/input.txt" > out.txt

    if ! diff -q "${dir}/expected.txt" out.txt &>/dev/null; then
        printf "${RED}Failed${NC}\n"
    else
        printf "${GREEN}Ok!${NC}\n"
    fi
done

rm out.txt
