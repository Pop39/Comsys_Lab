#include	<stdlib.h>

#include	<stdio.h>

#include	<assert.h>

#define _PRINT_TERM_
#define _PRINT_FACTOR_
#define _PRINT_NUMBER_
#define _PRINT_EXPRESSION_
//#define _PRINT_DEBUG_PRINT_
//#define _DEBUG_FACTOR_NODE_
#define _DEBUG_EXPRESSION_NODE_

static FILE *file;
static int ch;
static unsigned int val;
enum { plus , minus , times , divide , mod , lparen , rparen , number , eof , illegal };
static int symbol;

// Declare structure of tree
typedef struct NodeDesc* Node;
typedef struct NodeDesc{
	int kind;	// plus , minus , times , dive , number
	int val;	// in case is number, value
	Node left;  // plus , minus , times , divide
	Node right; // plus , minus , times, divide
} NodeDesc;

// Thid function will use together by using global variable please carefull to use that
static void SInit( char* file_name ); // for inint file open by save file variable to FILE line7

static void Number(); // for get value of realnumber into val (global variable)

static int SGet( ); // for get type of that char by using enum in line 10 to identify

static int Factor( Node* node , int sign ); 

static int Term( Node* node , int sign );

static int Expression( Node* node );

static void Print( Node root , int level );

static void Prefix( Node root , unsigned int start );

static void Postfix( Node root , unsigned int start );

static void Infix( Node root , unsigned int start );

int main( int argc , char** argv ){

	Node node = ( Node ) malloc( sizeof( NodeDesc ) );

	if( argc == 2 ){
		SInit( argv[1] );
		symbol = SGet();
		int result = Expression( &node );
		assert( symbol == eof );
		printf( "Result of expression is %d\n" , result );
		Print( node , 0);
		printf( "Prefix is ");
		Prefix( node , 1 );		
	}
	else{
		printf( "Usage : expreval_new <file_name>\n");
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
	#ifdef _PRINT_NUMBER_
		printf("Number val is %d\n" , val );
	#endif
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

static int Factor( Node* node , int sign ){

	assert( ( symbol == number ) || ( symbol == lparen ) );
	if( symbol == number ){
		symbol = SGet(); // don't worry if you care to lose old or current value
		#ifdef _PRINT_FACTOR_
			printf("Factor will return %d\n" , val );
		#endif
		#ifdef _DEBUG_FACTOR_NODE_
			printf("Factor val : sign are %5d : %d\n" , val , sign );
		#endif
		(*node)->kind = number;
		(*node)->val = val * sign;
		return val;
	}
	else{
		symbol = SGet();
		int result = Expression( node );
		assert( symbol == rparen ); // Check that have close tuple or not
		symbol = SGet();
		#ifdef _PRINT_FACTOR_
			printf("Factor will return %d\n" , result );
		#endif
		return result;
	}

}

static int Term( Node* node , int sign ){

	int result = Factor( node , sign );
	#ifdef _PRINT_TERM_
		printf("Term before in Loop : %d\n" , result );
	#endif
	int temp_value;
	Node new_node;
	while( 1 ){
		switch( symbol ){
			case times	:	symbol = SGet();
							new_node = ( Node ) malloc( sizeof(NodeDesc) );
							new_node->kind = times;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							temp_value = Factor( &((*node)->right) , 1 );
							result *= temp_value;
							#ifdef _PRINT_TERM_
								printf( "Multiple Case     : %d\n" , result );
							#endif
							continue;
			case divide	:	symbol = SGet();
							new_node = ( Node ) malloc( sizeof(NodeDesc) );
							new_node->kind = divide;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							temp_value = Factor( &((*node)->right) , 1 );
							result /= temp_value;
							#ifdef _PRINT_TERM_
								printf( "Divider Case      : %d\n" , result );
							#endif
							continue;
			case mod	:	symbol = SGet();
							new_node = ( Node ) malloc( sizeof(NodeDesc) );
							new_node->kind = mod;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							temp_value = Factor( &((*node)->right) , 1);
							result %= temp_value;
							#ifdef _PRINT_TERM_
								printf( "Modulation Case   : %d\n" , result );
							#endif
							continue;
		}
		break;
	}
	#ifdef _PRINT_TERM_
		printf("Term return value : %d\n" , result );
	#endif
	return result;
}

static int Expression( Node* node ){

	int sign_of_number = 1;
	switch( symbol ){
		case minus	: sign_of_number = -1;
		case plus	: symbol = SGet();
	}
	#ifdef _DEBUG_EXPRESSION_NODE_
		printf( "Expression sign_of_number before loop %d\n" , sign_of_number );
	#endif
	int result = Term( node , sign_of_number )*sign_of_number ;
	#ifdef _PRINT_EXPRESSION_
		printf("Expression before in loop %d\n" , result );
	#endif

	int temp_value;
	Node new_node;
	while( 1 ){
		sign_of_number = 1;
		switch( symbol ){
			case minus	:	new_node = ( Node ) malloc( sizeof( NodeDesc ) );
							new_node->kind = symbol;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							symbol = SGet();
							temp_value = Term( &(*node)->right , 1 );
							result -= ( temp_value ) ;
							continue;
			case plus	:	new_node = ( Node ) malloc( sizeof( NodeDesc ) );
							new_node->kind = symbol;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							symbol = SGet();
							temp_value = Term( &(*node)->right , 1 );
							result += ( temp_value ) ;
							continue;
		}
		break;
	}
	#ifdef _PRINT_EXPRESSION_
		printf("Expression after loop %d\n" , result );
	#endif
	return result;

}

static void Print( Node root , int level ){
	register int i;

	if( root != NULL ){
		#ifdef _PRINT_DEBUG_PRINT_
			printf( "Function print kind is  %d\n" , root->kind);
		#endif
		Print( root->right , level+1 );
		for( i = 0 ; i < level ; i++ ) printf("   ");
		
		switch( root->kind ){ // Have space for sign number
			case plus	:	printf(" +\n"); break;
			case minus	:	printf(" -\n"); break;
			case times	:	printf(" *\n"); break;
			case divide	:	printf(" /\n"); break;
			case mod	:	printf(" m\n"); break; // m  is modulation
			case number	:	printf("%2d\n" , root->val ); break;
		}
	
		Print( root->left , level+1 ); 
	}
}

static void Prefix( Node root , unsigned int start ){
	if( root != NULL ){
		switch( root->kind ){
			case plus	:	printf("+ "); break;
			case minus	:	printf("- "); break;
			case times	:	printf("* "); break;
			case divide :	printf("/ "); break;
			case mod	:	printf("m "); break;
			case number :	printf("%2d " , root->val ); break;
		}
		Prefix( root->left , 0 );
		Prefix( root->right , 0 );
	}
	if( start ) printf("\n");
}
