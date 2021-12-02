/* front.c - a lexical analyzer system for simple
 arithmetic expressions */
#include <stdio.h>
#include <ctype.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define LEFT_BRACE 27
#define RIGHT_BRACE 28
#define FOR_CODE 29
#define IF_CODE 30
#define DO_CODE 31
#define WHILE_CODE 32
#define RETURN_CODE 33
#define SEMICOLON 34
#define COLON 35
#define VOID_CODE 36
#define MAIN_CODE 37
#define EQUALS 38
#define SWITCH_CODE 39
#define CASE_CODE 40
#define FOREACH_CODE 41


/******************************************************/
/* main driver */
int main(void) {
	/* Open the input data file and process its contents */
	 if ((in_fp = fopen("front.in", "r")) == NULL)
	 	printf("ERROR - cannot open front.in \n");
	 else {
	 	getChar();
	 do {
	 	lex();
         
	 } while (nextToken != EOF);
	 }
	 printf("<Exit main>\n");
	 return 0;
}
/******************************************************/
/* lookup - a function to look up operators and
 parentheses and return the token */
int lookup(char ch) {
	 switch (ch) {
         case '=':
            addChar();
            nextToken = EQUALS;
            break;
         case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
        case ':':
            addChar();
            nextToken = COLON;
            break;
         case '{':
            addChar();
            nextToken = LEFT_BRACE;
            break;
         case '}':
            addChar();
            nextToken = RIGHT_BRACE;
            break;
		 case '(':
			 addChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 nextToken = ADD_OP;
			 break;
		 case '-':
			 addChar();
			 nextToken = SUB_OP;
			 break;
		 case '*':
			 addChar();
			 nextToken = MULT_OP;
			 break;
		 case '/':
			 addChar();
			 nextToken = DIV_OP;
			 break;
		 default:
			 addChar();
			 nextToken = EOF;
			 break;
	 }
     
	 return nextToken;
}


/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
	 if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
		 	charClass = LETTER;
		 else if (isdigit(nextChar))
		 	charClass = DIGIT;
		 else
		 	charClass = UNKNOWN;
	 } else
	 	charClass = EOF;
}

/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}

/******************************************************/
/* lex - a simple lexical analyzer for arithmetic
 expressions */
int lex(void) {
    
	 lexLen = 0;
	 getNonBlank();
	 
	 switch (charClass) {
		/* Identifiers */
		 case LETTER:
			 addChar();
			 getChar();
			 while (charClass == LETTER || charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
             if (!strcmp(lexeme, "foreach")){
				     nextToken = FOREACH_CODE;
				     break;
			 }
             if (!strcmp(lexeme, "for")){
				     nextToken = FOR_CODE;
				     break;
			 }
             if (!strcmp(lexeme, "if")){
				     nextToken = IF_CODE;
				     break;
			 }
             if (!strcmp(lexeme, "while")){
				     nextToken = WHILE_CODE;
				     break;
			 }
             if (!strcmp(lexeme, "return")){
				     nextToken = RETURN_CODE;
				     break;
			 }
             if (!strcmp(lexeme, "do")){
				     nextToken = DO_CODE;
				     break;
			 }
             if (!strcmp(lexeme, "void")){
				     nextToken = VOID_CODE;
				     break;
			 }
             if (!strcmp(lexeme, "main")){
				     nextToken = MAIN_CODE;
				     break;
			 }
			 if (!strcmp(lexeme, "switch")){
				     nextToken = SWITCH_CODE;
				     break;
			 }
			 if (!strcmp(lexeme, "case")){
				     nextToken = CASE_CODE;
				     break;
			 }
			 
             
			 nextToken = IDENT;
			 break;
		/* Integer literals */
		 case DIGIT:
			 addChar();
			 getChar();
			 while (charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			 nextToken = INT_LIT;
		 	break;
		/* Parentheses and operators */
		 case UNKNOWN:
			 lookup(nextChar);
			 getChar();
			 break;
			/* EOF */
		case FOR_CODE:
		    addChar();
			getChar();
		case EOF:
			 nextToken = EOF;
			 lexeme[0] = 'E';
			 lexeme[1] = 'O';
			 lexeme[2] = 'F';
			 lexeme[3] = '\0';
		 	break;
	 } /* End of switch */
	 
	 printf("Next token is: %d, Next lexeme is %s\n",
	 nextToken, lexeme);
	 syn();
	 return nextToken;
} /* End of function lex */



void syn(charClass) {
	switch (nextToken) {
		/* Identifiers */
		case FOR_CODE:
            forstmt();
            break;
		/* Integer literals */
		case IF_CODE:
			ifstmt();
		 	break;
        
        case WHILE_CODE:
			whilestmt();
		 	break;

        case RETURN_CODE:
			returns();
		 	break;
        case DO_CODE:
			dostmt();
		 	break;
        case VOID_CODE:
            program();
            break;
        case SWITCH_CODE:
            switches();
            break;
        case FOREACH_CODE:
            foreach();
            break;
        case EQUALS:
            assignment();
            break;    
	 } /* End of switch */
	 return;
}


void forstmt(void) {
    if (nextToken != FOR_CODE){
 	    error();
    }
    else {
        printf("<Enter for>\n");
	    lex();
 	    if (nextToken != LEFT_PAREN){
 		    error();
 	    } else {
 		    while (nextToken != RIGHT_PAREN){
 		    lex();
 		    }
 	    }
    }
    lex();
    block();
    printf("<Exit for>\n");
    return;
}

void foreach(void) {
    if (nextToken != FOREACH_CODE){
 	    error();
    } else {
        printf("<Enter foreach>\n");
	    lex();
 	    if (nextToken != LEFT_PAREN){
 		    error();
 	    } else {
 	        while (nextToken != COLON){
 		        lex();
 		    }
 		    while (nextToken != RIGHT_PAREN){
 		    lex();
 		    }
 	    }
    }
    lex();
    block();
    printf("<Exit foreach>\n");
    return;
}

void assignment(void) {
    if (nextToken != EQUALS){
 	    error();
    } else {
        printf("<Enter assignment>\n");
	    lex();
 		while (nextToken != SEMICOLON){
 		   if(nextToken = RIGHT_PAREN){ 
 		       break;
 		   }
 		   lex();
 		}
    }
    printf("<Exit assignment>\n");
    lex();
    return;
}

void block(void) {
    printf("<Enter block>\n");
    if (nextToken != LEFT_BRACE){
 	    error();
    } else {
	lex();
    while (nextToken != RIGHT_BRACE){
        lex();
        }
    }
    printf("<Exit block>\n");
    return;
}

void program(void) {
    printf("<Enter main>\n");
    if (nextToken != VOID_CODE){
 	    error();
    }else {
	    lex();
 		while (nextToken != MAIN_CODE){
            lex();
        }
    }
    lex();
    if (nextToken != LEFT_PAREN){
 		error();
 	} else {
 		while (nextToken != RIGHT_PAREN){
            lex();
        }
 	}
 	lex();
    block();
    return;
}

void switches(void) {
    printf("<Enter switch>\n");
    if (nextToken != SWITCH_CODE){
 	    error();
    }else {
	    lex();
 	        if (nextToken != LEFT_PAREN){
 		        error();
 	        }else {
 		        while (nextToken != RIGHT_PAREN){
                    lex();
                }
 	        }
 	    lex();
 	    if (nextToken != LEFT_BRACE){
 	        error();
        }else {
            lex();
            if (nextToken != CASE_CODE){
                error();
            }else {
                lex();
                while (nextToken != COLON){
                    lex();
                }
            }
	        lex();
            while (nextToken != RIGHT_BRACE){
                if(nextToken = CASE_CODE){
                    while (nextToken != COLON){
                    lex();
                    }
                }
                lex();
            }
        }   
    }  
    printf("<Exit switch>\n");
    lex();
    return;
}

void ifstmt(void) {
    printf("<Enter if>\n");
    if (nextToken != IF_CODE){
 	    error();
    } else {
        lex();
        if (nextToken != LEFT_PAREN)
            error();
        else {
            while (nextToken != RIGHT_PAREN){
                lex();
            }
        }
    }
    lex();
    block();
    printf("<Exit if>\n");
    return;
}

void whilestmt(void) {
    printf("<Enter while>\n");
    if (nextToken != WHILE_CODE){
 	    error();
    }else {
	    lex();
 	    if (nextToken != LEFT_PAREN){
 		    error();
 	    } else {
 		    while (nextToken != RIGHT_PAREN){
                lex();
            }
 	    }
    }
    lex();
    if(nextToken == LEFT_BRACE){
        block();
    }
    printf("<Exit while>\n");
    return;
}

void returns(void) {
    printf("<Enter return>\n");
    if (nextToken != RETURN_CODE){
 	    error();
    } else {
	    lex();
 		    while (nextToken != SEMICOLON){
                lex();
            }
    }
    printf("<Exit return>\n");
    return;
}

void dostmt(void) {
    printf("<Enter dowhile>'\n");
    if (nextToken != DO_CODE){
 	error();
    } else {
	    lex();
        block();
        if(nextToken == WHILE_CODE){
            whilestmt();
        }
        lex();
        printf("<Exit dowhile>\n");
        return;
    }
}


