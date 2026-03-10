//  412, Carlos Martin Gallardo, Alejandro Quirante Sanz
//  100522258@alumnos.uc3m.es,  100522183@alumnos.uc3m.es

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define T_NUMBER 	1001
#define T_OPERATOR	1002
#define T_VARIABLE  1003
#define T_TERNARY   1004 

void ParseE () ;
void ParseP () ;
void ParseO () ;

struct s_tokens {
	int token ;					// Here we store the current token/literal
	int old_token ; 			// Sometimes we need to check the previous token
	int number ;				// The value of the number
	int old_number ;			// old number value
	char variable_name [8] ;		/// variable name
	char old_var_name [8] ;			/// old variable name
	int token_val ;				// the arithmetic operator
	int old_token_val ;			// old arithmetic operator
} ;

struct s_tokens tokens = {0, 0, 0, -1, "", "", 0, -1};


int line_counter = 1 ;


void update_old_token ()
{
	tokens.old_token = tokens.token ;
	tokens.old_number = tokens.number ;
	strcpy (tokens.old_var_name, tokens.variable_name) ;
	tokens.old_token_val = tokens.token_val ;
}


void init_tokens ()
{
    tokens.token = 0;
    tokens.old_token = 0 ;
    tokens.number = 0 ;
    tokens.old_number = -1 ;
    strcpy (tokens.old_var_name, "") ;
    strcpy (tokens.variable_name, "") ;
    tokens.token_val = 0;
    tokens.old_token_val = -1;
}


int rd_lex ()
{
    int c ;
    int cc ;

    do {
        c = getchar () ;
        if (c == '\n')
            line_counter++ ;
    } while (c == '\t' || c == ' ' || c == '\r') ;

    if (isdigit (c)) {
        ungetc (c, stdin) ;
        update_old_token () ;
        scanf ("%d", &tokens.number) ;
        tokens.token = T_NUMBER ;
        return (tokens.token) ;
    }

    if (isalpha(c)) {
        update_old_token () ;
        cc = getchar () ;
        if (isdigit (cc) || isalpha (cc)) {
            sprintf (tokens.variable_name, "%c%c", c, cc) ;
        } else {
            ungetc (cc, stdin) ;
            sprintf (tokens.variable_name, "%c", c) ;
        }
        tokens.token = T_VARIABLE ;
        return (tokens.token) ;
    } 
    
    // Operadores binarios (+, -, *, /, =)
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
        update_old_token () ;
        tokens.token_val = c ;
        tokens.token = T_OPERATOR ;
        return (tokens.token) ;
    }
    
    // Operador ternario (?)
    if (c == '?') {
        update_old_token () ;
        tokens.token_val = c ;
        tokens.token = T_TERNARY ;
        return (tokens.token) ;
    }

    if (c == EOF) {
        exit (0) ;
    }  
    
    update_old_token () ;
    tokens.token = c ;
    return (tokens.token) ;
}


void rd_syntax_error (int expected, int token, char *output)
{
	fprintf (stderr, "ERROR in line %d: ", line_counter) ;
	fprintf (stderr, output, expected, token) ;
	fprintf (stderr, "\n") ;
	exit (0) ;
}


void MatchSymbol (int expected_token)
{
	if (tokens.token != expected_token) {
		rd_syntax_error (expected_token, tokens.token, "token %d expected, but %d was read") ;
	} else {
	 	rd_lex () ;
	}
}


void ParseO() {
    // O -> + | - | * | /
    MatchSymbol(T_OPERATOR);
}


void ParseP() {
    // P -> n | v | ( ... ) | = V P | ? P P P
    if (tokens.token == T_NUMBER) {
        printf("%d", tokens.number);
        MatchSymbol(T_NUMBER);
    } 
    else if (tokens.token == T_VARIABLE) {
        printf("%s", tokens.variable_name);
        MatchSymbol(T_VARIABLE);
    } 
    else if (tokens.token == '(') {
        MatchSymbol('(');
        printf("(");
        
        // Dentro de los paréntesis puede haber:
        if (tokens.token == T_TERNARY) {
            // Caso 1: Operador ternario ? P P P
            MatchSymbol(T_TERNARY);
            ParseP();  // Condición
            printf(" ? ");
            ParseP();  // Si verdadero
            printf(" : ");
            ParseP();  // Si falso
        }
        else if (tokens.token == T_OPERATOR) {
            // Caso 2: Operador binario O P P
            int op = tokens.token_val;
            ParseO();
            ParseP();
            printf(" %c ", op);
            ParseP();
        }
        else {
            rd_syntax_error(-1, tokens.token, "Expected operator or '?' after '(' in ParseP");
        }
        
        printf(")");
        MatchSymbol(')');
    } 
    else if (tokens.token == '=') {
        // = V P  (asignación)
        MatchSymbol('=');
        printf("(");
        if (tokens.token != T_VARIABLE) {
            rd_syntax_error(T_VARIABLE, tokens.token, "Variable expected after '=' in ParseP");
        }
        printf("%s = ", tokens.variable_name);
        MatchSymbol(T_VARIABLE);
        ParseP();
        printf(")");
    } 
    else {
        rd_syntax_error(-1, tokens.token, "Unexpected token in ParseP");
    }
}


void ParseE() {
    // E -> n | v | ( ... ) | = V P | ? P P P
    if (tokens.token == T_NUMBER) {
        printf("%d", tokens.number);
        MatchSymbol(T_NUMBER);
    } 
    else if (tokens.token == T_VARIABLE) {
        printf("%s", tokens.variable_name);
        MatchSymbol(T_VARIABLE);
    } 
    else if (tokens.token == '(') {
        MatchSymbol('(');
        printf("(");
        
        // Dentro de los paréntesis puede haber:
        if (tokens.token == T_TERNARY) {
            // Caso 1: Operador ternario ? P P P
            MatchSymbol(T_TERNARY);
            ParseP();  // Condición
            printf(" ? ");
            ParseP();  // Si verdadero
            printf(" : ");
            ParseP();  // Si falso
        }
        else if (tokens.token == T_OPERATOR) {
            // Caso 2: Operador binario O P P
            int op = tokens.token_val;
            ParseO();
            ParseP();
            printf(" %c ", op);
            ParseP();
        }
        else {
            rd_syntax_error(-1, tokens.token, "Expected operator or '?' after '(' in ParseE");
        }
        
        printf(")");
        MatchSymbol(')');
    } 
    else if (tokens.token == '=') {
        // = V P  (asignación)
        MatchSymbol('=');
        printf("(");
        if (tokens.token != T_VARIABLE) {
            rd_syntax_error(T_VARIABLE, tokens.token, "Variable expected after '=' in ParseE");
        }
        printf("%s = ", tokens.variable_name);
        MatchSymbol(T_VARIABLE);
        ParseP();
        printf(")");
    } 
    else {
        rd_syntax_error(-1, tokens.token, "Unexpected token in ParseE");
    }
}


void ParseYourGrammar()
{
    ParseE();
}


void ParseAxiom ()
{
	ParseYourGrammar() ;
	if (tokens.token == '\n') {
	    printf("\n");
		MatchSymbol ('\n') ;
	} else {
		rd_syntax_error (-1, tokens.token, "Unexpected Token at end of Parsing (expected NEWLINE)");
	}
}


int main (int argc, char **argv)
{
	int flagMultiple = 1 ;
	
	if (argc >= 2) {
		if (strcmp ("-s", argv [1]) == 0) {
			flagMultiple = 0 ;
		}
	}
	
	rd_lex () ;
	
	do {
		ParseAxiom () ;
	} while (flagMultiple) ;
	
	exit (0) ;
}