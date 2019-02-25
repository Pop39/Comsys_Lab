#include	<stdlib.h>
#include	<stdio.h>
#include	<assert.h>

static FILE *f;
static int ch;
static unsigned int val;
enum { plus, minus, times, divide, mod, lparen, rparen, number, eof, illegal};
static int sym;

static void Expr();
static void Factor();
static void Term();

// This function use for readfile into variable f
static void SInit( char* filename ){ 
	ch = EOF;
	f = fopen( filename , "r+t" ); // open text file for reading and writing
	if( f!= NULL ) ch = getc(f); 
	// function getc is get character from stream in this situation stream file is f variable
}

// This function use for get value of number
static void Number(){
	val = 0;
	while( ('0' <= ch ) && ( ch <= '9' ) ){
		val = val * 10 + ch - '0';
		ch = getc( f );
	}
}

// This function we determine type of data in case that is number will collect in val
static int SGet(){
	register int sym;
	
	while( (ch != EOF ) && ( ch <= ' ' ) ) ch = getc(f);

	switch( ch ){
		case EOF : sym = eof; break;
		case '+' : sym = plus; ch = getc(f); break;
		case '-' : sym = minus; ch = getc(f); break;
		case '*' : sym = times; ch = getc(f); break;
		case '/' : sym = divide; ch = getc(f); break;
		case '&' : sym = mod; ch = getc(f); break;
		case '(' : sym = lparen; ch = getc(f); break;
		case ')' : sym = rparen; ch = getc(f); break;
		case '0' : case '1' : case '2' : case '3' : case '4' :
		case '5' : case '6' : case '7' : case '8' : case '9' :
				sym = number; Number(); break;
		default  : sym = illegal;
	}

	return sym;
}

// This function is expression
static void Expr(){
	if( (sym == minus ) || (sym == plus) ){
		sym = SGet();
	}

	Term();

	while( ( sym == plus ) || ( sym == minus ) ){
		sym = SGet();
		Term();
	}
}


// Function assert use to aborted program when argument is 0
static void Factor(){
	assert( ( sym == number ) || ( sym == lparen ) );
	if( sym == number ){
		sym = SGet();
	}
	else{
		sym = SGet();
		Expr();
		assert( sym == rparen );
		sym = SGet();
	}
}

static void Term(){
	Factor();
	while( ( sym == times ) || ( sym == divide ) || (sym == mod ) ){
		sym = SGet();
		Factor();
	}
}

int main( int argc , char* argv[] ){
	register int result;

	printf("Argument argc is %d\n" , argc );
	printf("Argument argv is %s\n" , argv[1] );
	
	if( argc == 2 ){
		SInit( argv[1] );
		sym = SGet();
		Expr();
		assert( sym == eof );
	}
	else{
		printf("usage : expreval <filenam>\n");
	}
	return 0;
}
