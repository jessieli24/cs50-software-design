# !/bin/bash
#
# testing.sh - test querier.c
#
# Jessie Li
# CS 50 2022

tse="/thayerfs/courses/22fall/cosc050/workspace/tse/tse-output"

########## small tests ##########

########## letters, depth 0 ##########
command="./querier $tse/letters-depth-0 $tse/letters-index-0"

# one good word
echo home | $command
echo pLayGRound | $command

# one bad word
echo play | $command

# bad character
echo bad-character | $command

# bad grammar
echo and home | $command
echo home and | $command
echo home and or playground  | $command

# two good words
echo home playground | $command
echo home and playground | $command
echo home or playground | $command

# lots of spaces
echo "  home  or        playground" | $command

# 3+ words
echo home playground the | $command
echo home and playground and the | $command
echo home or playground and the | $command
echo home or playground or the | $command

########## toscrape, depth 1 (with inputs from Querier Example Output) ##########
command="./querier $tse/toscrape-depth-1 $tse/toscrape-index-1"

# ranking results
echo Europe travel | $command
echo Europe and travel | $command
echo Europe or travel | $command
echo Tiny search engine | $command
echo TSE | $command
echo git-flow | $command
echo exit | $command

########## toscrape, depth 1 (with valgrind) ##########
myvalgrind="valgrind --leak-check=full --show-leak-kinds=all"
echo Europe or travel | $myvalgrind $command

./fuzzquery $tse/toscrape-index-2 3 0 | $myvalgrind $command

########## fuzz testing (valid inputs) ##########
./fuzzquery $tse/toscrape-letters-6 10 999 | ./querier $tse/toscrape-depth-2 $tse/letters-index-6
./fuzzquery $tse/toscrape-index-2 10 0 | ./querier $tse/toscrape-depth-2 $tse/toscrape-index-2
./fuzzquery $tse/wikipedia-index-1 10 777 | ./querier $tse/toscrape-depth-2 $tse/toscrape-index-2
