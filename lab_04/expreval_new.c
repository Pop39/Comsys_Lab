#include	<stdlib.h>

#include	<stdio.h>

#include	<assert.h>

#define _PRINT_COND_

static FILE *file;
static int ch;
static unsigned int val;
enum { plus , minus , times , divide , mod , lparen , rparen , number , eof , illegal };
static int symbol;

// Thid function will use together by using global variable please carefull to use that
static void SInit( char* file_name ); // for inint file open by save file variable to FILE line7

static void Number(); // for get value of realnumber into val (global variable)

static int SGet(); // for get type of that char by using enum in line 10 to identify

static int Factor(); 

static int Term();

static int Expression();

int main( int argc , char** argv ){

	if( argc == 2 ){
		SInit( argv[1] );
		symbol = SGet();
		int result = Expression();
		assert( symbol == eof );
		printf( "Result of expression is %d\n" , result );
	}	
}

static void SInit( char* file_name ){
	ch = EOF;
	file = fopen( file_name , "r+t" ); // open text file for reading and writing
	if( file != NULL ) ch = getc( file );
	// function getc is get character from stream in this situation stream file is file variable
}

static void Number(){
	val = 0;
	while( ( '0' <= ch ) && ( ch <= '9' ) ){ // this use term of ascii code
		val = val * 10 + ch - '0';
		ch = getc( file );
	} 
}

static int SGet(){
	register int sym; // introduce complier to save this variable into register
	while( ( ch != EOF ) && ( ch <= ' ' ) ) ch = getc( file ); // less that is space in special

	switch( ch ){
		case EOF : symbol = eof;	break;
		case '+' : symbol = plus;	ch = getc(file); break;
		case '-' : symbol = minus;	ch = getc(file); break;
		case '*' : symbol = times;	ch = getc(file); break;
		case '/' : symbol = divide; ch = getc(file); break;
		case '%' : symbol = mod;	ch = getc(file); break;
		case '(' : symbol = lparen;	ch = getc(file); break;
		case ')' : symbol = rparen; ch = getc(file); break;

		case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : 
		case '8' : case '9' : symbol = number; break;
		default  : symbol = illegal;
	}

	if( symbol == number ) Number(); 
	// this pattern we give you have symbol is number and value in val

	return symbol;
}

static int Factor(){

	assert( ( symbol == number ) || ( symbol == lparen ) );
	if( symbol == number ){
		symbol = SGet(); // don't worry if you care to lose old or current value
		return val;
	}
	else{
		symbol = SGet();
		int result = Expression();
		assert( symbol == rparen ); // Check that have close tuple or not
		symbol = SGet();
		return result;
	}

}

static int Term(){

	int result = Factor();
	while( 1 ){
		switch( symbol ){
			case times	:	result *= Factor(); 
							#ifdef _PRINT_COND_
								printf( "Multiple Case     : %10d" , result );
							#endif
							continue;
			case divide	:	result /= Factor();
							#ifdef _PRINT_COND_
								printf( "Divider Case      : %10d" , result );
							#endif
							continue;
			case mod	:	result %= Factor();
							#ifdef _PRINT_COND_
								printf( "Modulat Case      : %10d" , result );
							#endif
							continue;
		}
		break;
	}
}

static int Expression(){

	int sign_of_number = 1;
	switch( symbol ){
		case minus	: sign_of_number = -1;
		case plus	: symbol = SGet();
	}

	int result = Term() * sign_of_number ;

	while( 1 ){
		switch( symbol ){
			sign_of_number = 1;
			case minus	:	sign_of_number = -1;
			case plus	:	symbol = SGet();
							result += ( Term() * sign_of_number ) ;
							continue;
		}
		break;
	}
	return result;

}

