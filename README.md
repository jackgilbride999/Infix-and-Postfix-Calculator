# Infix-and-Postfix-Calculator
C Program which takes infix and postfix sums as lines of a file. Converts any infix sums to postfix and computes all sums in postfix. Assumes that all lines of the input file are well formed.

Sample well-formed file:

infix 3 + 5 X ( 7 - 22 )
infix 36 X ( 7 + 9 + 8 -3 ) / 6
postfix 7 8 3 X 28 + +

If given a file like above, the program will print the lines of the file, follwing each line with the result of the computation. The calculations must be well formed for the program to work. For example, the current implementation will either run indefinitely or cause a segmentation fault if an infix calculation has a starting bracket without a corresponding closing bracket.
