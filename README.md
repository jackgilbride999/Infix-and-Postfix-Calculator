# Infix-and-Postfix-Calculator
C Program which takes infix and postfix sums as lines of a file. Converts any infix sums to postfix and computes all sums in postfix.

Sample well-formed file:
infix 3 + 5 X ( 7 - 22 )
infix 36 X ( 7 + 9 + 8 -3 ) / 6
postfix 7 8 3 X 28 + +

USAGE:
- Compile the file in a linux terminal, linking the math library; "cc -o calc calc.c -lm"
- Run the program in the terminal with the input filename; "./calc sample.txt"
- Each line of the file will be outputted in the terminal followed by the answer of its respective calculation.

KNOWN LIMITATIONS:
- The program assumes that the input file is well formed, i.e. every line of the file is the word infix followed by a valid infix expression or the word postix followed by a valid postfix expression. An example of poor usage that will cause errors in the program is an infix expression with one more right bracket than left bracket. This will make the program look for the corresponding left bracket on the stack, causing it to run indefinitely or access memory addresses that it shouldn't.
