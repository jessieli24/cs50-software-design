# !/bin/bash
# test script for crawler.c
#
# Jessie Li
# CS 50, Fall 2022

letters="http://cs50tse.cs.dartmouth.edu/tse/letters/index.html"
toscrape="http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html"
wikipedia="http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html"
output="../data"

function crawlTest() {
    # slice array: https://unix.stackexchange.com/questions/82060/bash-slice-of-positional-parameters
    for i in "${@:3}"; do
        if [ ! -d "${2}-${i}" ]; then mkdir -p "${2}-${i}"; fi
        echo "passing arguments: $1 ${2}-${i} ${i}\n"
        ./crawler "$1" "${2}-${i}" "${i}"
    done
}

####################################################
# Testing erroneous arguments
mkdir "${output}/letters-err"

# external seedURL
./crawler "http://www.cs.dartmouth.edu/~tjp/cs50/schedule.html" "${output}/letters-err" 1

# invalid maxDepth
./crawler $letters "${output}/letters-err" -1
./crawler $letters "${output}/letters-err" 11

# pageDirectory that doesn't exist
./crawler $letters "${output}/dne" 1

####################################################
# Run with valgrind over a moderate-sized test case (such as toscrape at depth 1)
myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'
mkdir "${output}/test-valgrind"
$myvalgrind ./crawler $toscrape "${output}/test-valgrind" 1

####################################################
# Run over all three CS50 websites

# crawlTest $letters "${output}/letters" 0 1 2 10
crawlTest $letters "${output}/letters" 0 10

# crawlTest $toscrape "${output}/toscrape" 0 1 2 3
crawlTest $toscrape "${output}/toscrape" 0 1

# crawlTest $wikipedia "${output}/wikipedia" 0 1 2
crawlTest $wikipedia "${output}/wikipedia" 0 1 

exit 0;