#pragma warning(disable : 4996)

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*	Define list as a structure. Contains a pointer to the first listnode. */
struct list {
	struct listnode * head;
};

/* Define listnode as a structure. Contains one double and a pointer to the
	next listnode. Double may also represent a char and be casted before a push 
	and after a pop */
struct listnode {
	double data;
	struct listnode * next;
};

/*	newEmptyList function. Creates a list whose head points to NULL. Returns the pointer
	to this list. */
struct list * newEmptyList() {
	struct list * result;				// create a pointer to a list called result
	result = malloc(sizeof(struct list));		// allocate a space in memory the size of a list
	result->head = NULL;				// set the head of the list to null
	return result;
}

/*	addToList function. Given the list and the double to put on it, puts the double on the list (on top
	of the stack). Placed between the head and the next element in the list (if there is one). */
void addToList(struct list * theList, double element) {
	struct listnode * newNode;			// create a pointer to a new node
	newNode = malloc(sizeof(struct listnode));	// allocate memory for the pointer
	newNode->data = element;			// set the double in the node to the passed value
	if (theList->head == NULL) {
		theList->head = newNode;		// if the list is empty, the head points to the new node
		newNode->next = NULL;			// if the list is empty, the newNode will point to NULL
	}
	else {
		newNode->next = theList->head;		// if there are elements in the list, our new node will
							// point to the element that head was pointing to
		theList->head = newNode;		// the head points to the new node at the top of the stack									
	}
}

/*	popFromList function. Given the list, returns the double from the top of the list, popping it off. Assumes that the
	list is non-empty. */ // UPDATED 26/09/18
double popFromList(struct list * theList) {
	double elemToPop = 0;
	struct listnode * nodeToPop;
	nodeToPop = theList->head;			// the node to pop is the node at the top of the stack
	if (nodeToPop != NULL) {
		elemToPop = nodeToPop->data;		// the element to pop off is the one in the node at the top
		theList->head = nodeToPop->next;	// the head points at where the node at the top points, popping it off
		free(nodeToPop);			// free the popped node to avoid memory leak
	}
	return elemToPop;				// return the element as a double
}

/* 	readFile function. Reads in a file of a passed name and returns it as an
	array of characters. Prints the array before it is returned. */
char * readFile(char filename[]) {
	FILE * file;
	file = fopen(filename, "r");
	int stringSize;
	char * string = NULL;
	if (file == NULL) {
		printf("Error. File is NULL.");
		exit(1);
	}
	else {
		fseek(file, 0, SEEK_END);
		stringSize = ftell(file) + 1;
		rewind(file);
		string = (char*)malloc(sizeof(char)*(stringSize));
		char * substring = (char*)malloc(sizeof(char)*(stringSize));
		while(fgets(substring, stringSize, file)!=NULL){
			strcat(string, substring);
		}
		free(substring);
	}
	fclose(file);
	return string;
}

/* isValid function. Checks if a char is valid for the program, i.e. would 
	 appear in a postfix expression */
int isValid(char c){
	if(c>='0' && c<='9'){
		return 1;
	}
	else{
		switch(c){
			case '.':
			case '+':
			case '-':
			case 'X':
			case '/':
			case ' ':
				return 1;
			default:
				return 0;
		}
	}
}
/*compute function. Carries out the relevant mathematical calculations for
	a reverse polish notation calculator. Takes the relevant operator as a char
	('+', '-', 'X', '/' or '^') and the head of a stack, pops the last
	two elements off the stack, carries out the operation and puts the result
	back on the stack. */
double compute(char operator, struct list * theList) {
	double result;
	double secondTerm = popFromList(theList);		// the first term to pop will be the second term of the computation
	double firstTerm = popFromList(theList);
	switch (operator)
	{
	case '+': result = firstTerm + secondTerm; break;
	case '-': result = firstTerm - secondTerm; break;
	case 'X': result = firstTerm * secondTerm; break;
	case '/': result = firstTerm / secondTerm; break;
	case '^': result = pow(firstTerm, secondTerm); break;
	}
	return result;
}

/*readNumber function. Reads a character representation of a number as a substring
  of a larger substring of a larger string and pushes this number as a double onto
  the stack. Is passed the larger string as a character array and the index of the
	first character of the substring. Returns the index following the substring. */
int readNumber(int index, char* file, struct list * theList) {
	int tmpIndex = index;
	while (file[tmpIndex] != 0 && ((file[tmpIndex] >= '0' && file[tmpIndex] <= '9') || (file[tmpIndex] == '.'))) {
		tmpIndex++;
	}
	int numberSize = (tmpIndex - index) + 1;		// the size of number should be space for all the
								// number chars and 0, which terminates a String
	char * number;
	number = malloc(sizeof(char)*numberSize);		// allocate enough memory for the number chars and 0
	tmpIndex = index;
	for (int i = 0; i < numberSize - 1; i++) {
		number[i] = file[tmpIndex];			// copy the number from file to number
		tmpIndex++;
	}
	number[numberSize - 1] = 0;				// null-terminate the string
	double numberAsDouble = atof(number);			// get the string as a double
	addToList(theList, numberAsDouble);			// add the result to the list
	free(number);
	return tmpIndex + 1;					// return the index of the space after the number in file
}

/* readNumForInfix function. Reads a decimal number as a subtring and adds it 
	the passed postfix string. Returns the pointer to the next available index of
	the postfix expression. */
int readNumForInfix(int index, char* file, char* postfixString, int* postfixIndexPointer) {
	int tmpIndex = index;
	while (file[tmpIndex] != 0 && file[tmpIndex] != ' ') {
		tmpIndex++;
	}
	int numberSize = (tmpIndex - index);
	for (int i = 0; i < numberSize; i++) {
		if(file[index]!='\n'){
			postfixString[*postfixIndexPointer] = file[index];	// copy the number from file to number
			(*postfixIndexPointer)++;
		}
		index++;

	}
	postfixString[*postfixIndexPointer] = ' ';
	(*postfixIndexPointer)++;
	return index;
}

/*findPrecedence function. 
  Returns the precedence of an operator presuming '^' = 3 > 'X' = '/' = 2 > '+' = '-' = 1 */
int findPrecedence(char operator) {
	switch (operator) {
	case '^': return 3;
	case 'X':
	case '/': return 2;
	case '+':
	case '-': return 1;
	default: return 0;			// if the function is given a non-operator it returns the lowest precedence 
	}
}

void addToString(char c, char* string) {
	if(isValid(c)==1){
		int i = 0;
		while (string[i] != 0) {
			i++;
		}
		string[i] = c;
		string[i + 1]    = ' ';
	}
}

void instantiateString(char* string) {
	int k = 0;
	while (string[k]!=0) {
		string[k] = 0;
		k++;
	}
}

void printString(char* string) {
	int i = 0;
	while (string[i] != 0) {
		printf("%c", string[i]);
		i++;
	}
	printf("\n");
}

char* infixToPostfix(char* infixString, char postfixString[]) {
	struct list * theList = newEmptyList();
	int index = 0;
	int postfixIndex = 0;
	do {
		char currentChar = infixString[index];
		switch (currentChar) {
		case ' ':
			index++;
			break;
		case '+': case '-': case 'X': case '/': case '^': {
			char operator = (char)popFromList(theList);
			if(isValid(operator)){
				while (findPrecedence(operator) > findPrecedence(currentChar)) { // while(there is an operator on top of the stack wih higher precedence)
					postfixString[postfixIndex]=operator;			 //  pop stack and append popped operator to output string
					postfixIndex++;					
					postfixString[postfixIndex]=' ';
					postfixIndex++;			
					operator = (char)popFromList(theList);
				}																				//	}
				
			}
			if(operator!=0) {addToList(theList, (double)operator);}
			addToList(theList, (double)currentChar);				// push token operator to stack
			index++;
			break; }
		case '(':
			addToList(theList, (double)'('); // push bracket to stack
			index++;
			break;
		case ')': {
			char operator = 0;
			do {																					// while (top of stack != '(' ) {
				operator = (char)popFromList(theList);				// pop operator from stack and append to output string
				if (operator!='(') {
					postfixString[postfixIndex]=operator;
					postfixIndex++;	
					postfixString[postfixIndex]=' ';
					postfixIndex++;
				}
			} while (operator!='(');
			index++;
			break; }
		default: if ((currentChar >= '0' && currentChar <= '9') || currentChar == '.') {
			index = readNumForInfix(index, infixString, postfixString, &postfixIndex); // append token to output string
		}
		}
	} while (infixString[index] != 0 && infixString[index] != '\n');
	char poppedChar = (char)popFromList(theList);
	while (poppedChar != 0) {
		postfixString[postfixIndex]=poppedChar;
		postfixIndex++;	
		postfixString[postfixIndex]=' ';
		postfixIndex++;	
		poppedChar = (char)popFromList(theList);
	}
	postfixString[postfixIndex]=0;
	free(theList);
	return postfixString;
}

double postfixComputation(char* postfixString) {
	int index = 0;
	struct list* theList = newEmptyList();
	do {
		char currentChar = postfixString[index];
		if (currentChar == ' ') {
			index++;
		}
		else if (currentChar == '+' || currentChar == '-' || currentChar == 'X'
			|| currentChar == '/' || currentChar == '^') {
			double number = compute(currentChar, theList);
			addToList(theList, number);
			index++;
		}
		else if ((currentChar >= '0' && currentChar <= '9') || currentChar == '.') {
			index = readNumber(index, postfixString, theList);
		}
	} while (postfixString[index] != 0 && postfixString[index] != '\n');
	double numToPop = popFromList(theList);		// return the result
	free(theList);
	return numToPop;
}

void appendInfixSum(char* expression, char* outputString){
	strcat(outputString, expression);
	int length = strlen(expression+6);
	char postfixString[length];
	infixToPostfix(expression+6, postfixString);
	double result = postfixComputation(postfixString);
	int integerResult = (int)result;
	char resultString[1024];
	memset(resultString, 0, 1024);
	sprintf(resultString, "%i", integerResult);
	strcat(outputString, resultString);
	int len = strlen(outputString);
	outputString[len]='\n';
	outputString[len+1]=0;
}

void appendPostfixSum(char* expression, char* outputString){
					strcat(outputString, expression);
					double result = postfixComputation(expression+8);
					int integerResult = (int)result;
					char resultString[1024];
					memset(resultString, 0, 1024);
					sprintf(resultString, "%i", integerResult);
					strcat(outputString, resultString);
					int len = strlen(outputString);
					outputString[len]='\n';
					outputString[len+1]=0;
}

/*	main function */
int main(int argc, char ** argv)
{
  char * filename;
  if ( argc == 1 ) {
    printf("Error: No input filename provided\n");
    printf("Usage: %s <input filename>\n", argv[0]);
    exit(1);
  }
  else if ( argc > 2 ) {
    printf("Error: Too many command line parameters\n");
    printf("Usage: %s <input filename>\n", argv[0]);
    exit(1);
  }
  else {
    filename = argv[1];
		char * file = readFile(filename);
		char outputString[2048];
		memset(outputString, 0, strlen(outputString));
		int fileIndex = 0, endOfFile = 0, count = 0;
		while(!endOfFile && count<5){ 
				int stringSize=0;
				for(int tmpIndex=fileIndex; file[tmpIndex]!='\n' && file[tmpIndex]!=0; tmpIndex++){
					stringSize++;
				}
				char expression[stringSize+1];
				memset(expression, 0, stringSize+2);
				int i;
				for(i=0; i<stringSize; i++){
					expression[i]=file[fileIndex];
					fileIndex++;
				}
				expression[i]='\n';
				fileIndex++;
				if(strstr(expression, "postfix") != NULL) {
					appendPostfixSum(expression, outputString);
				}
				else if(strstr(expression, "infix") != NULL) {
					appendInfixSum(expression, outputString);
				}
				else{
					endOfFile=1;
					int len = strlen(outputString);
					outputString[len-1]=0;
				}
		}
		char outputFileName[strlen(filename)+5];
		memset(outputFileName, 0, strlen(outputFileName));
		strcat(outputFileName, filename);
		strcat(outputFileName, ".results\0");
		FILE *outputFile = fopen(outputFileName, "wb");
		fwrite(outputString, sizeof(char), strlen(outputString), outputFile);
		fclose(outputFile);
	}	
	return 0;
}

