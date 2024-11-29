# !/bin/bash
# test script for indexer.c
#
# Jessie Li
# CS 50, Fall 2022

tse="/thayerfs/courses/22fall/cosc050/workspace/tse/tse-output"
indexcmp="/thayerfs/courses/22fall/cosc050/workspace/tse/indexcmp"
output="../data"

####################################################
# invalid arguments

# no arguments
./indexer

# one argument
./indexer $tse/letters-depth-0

# three or more arguments
./indexer $tse/letters-depth-0 $output/letters-index-0 $output/extra
./indexer $tse/letters-depth-0 $output/letters-index-0 $output/extra1 $output/extra2

# invalid pageDirectory (non-existent path)
./indexer $tse/dne $output/letters-index-0

# invalid pageDirectory (not a crawler directory)
./indexer ../libcs50 $output/letters-index-0

# invalid indexFile (non-existent path)
./indexer $tse/letters-depth-0 ../dne/letters-index-0

# invalid indexFile (read-only directory)
./indexer $tse/letters-depth-0 $tse/letters-index-invalid

# invalid indexFile (existing, read-only file)
./indexer $tse/letters-depth-0 $tse/letters-index-0

####################################################
# Valgrind on both indexer and indextest to ensure no memory leaks or errors
myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'

$myvalgrind ./indexer $tse/toscrape-depth-1 "${output}/toscrape-index-1"

$myvalgrind ./indextest "${output}/toscrape-index-1" "${output}/toscrape-test-1"

# compare indexer and indextest output
$indexcmp "${output}/toscrape-index-1" "${output}/toscrape-test-1"

# compare my indexer output to CS 50 indexer output
$indexcmp "${output}/toscrape-index-1" "$tse/toscrape-index-1"

####################################################
# Run indexer on a variety of pageDirectories
# Use indextest as one means of validating the resulting index

function indexTest() {
    # slice array: https://unix.stackexchange.com/questions/82060/bash-slice-of-positional-parameters
    for i in "${@:2}"; do
        echo "indexing: $tse/${1}-${i}\n"
        
        ./indexer "$tse/${1}-depth-${i}" "$output/${1}-index-${i}"
        ./indextest "$output/${1}-index-${i}" "$output/${1}-test-${i}"
        
        # compare with indextest
        $indexcmp "$output/${1}-index-${i}" "$output/${1}-test-${i}"
        # compare with output in tse folder
        $indexcmp "$output/${1}-index-${i}" "$tse/${1}-index-${i}"
    done
}

# letters
indexTest letters 0 6

# toscrape
indexTest toscrape 0 2

# wikipedia
indexTest wikipedia 0 1
