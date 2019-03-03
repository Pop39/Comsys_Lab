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
char variable[50];
enum { plus , minus , times , divide , mod , lparen , rparen , number , eof , illegal , var };
static int symbol;

// Declare structure of tree
typedef struct NodeDesc* Node;
typedef struct NodeDesc{
	int kind;	// plus , minus , times , dive , number , var
	int val;	// in case is number, value
	char name[53];
	Node left;  // plus , minus , times , divide
	Node right; // plus , minus , times, divide
} NodeDesc;

// Thid function will use together by using global variable please carefull to use that
static void SInit( char* file_name ); // for inint file open by save file variable to FILE line7

static void CopyString( char* target , char* origin );

static void Number(); // for get value of realnumber into val (global variable)

static void String();

static int SGet( ); // for get type of that char by using enum in line 10 to identify

static void Factor( Node* node , int sign ); 

static void Term( Node* node , int sign );

static void Expression( Node* node );

static void Print( Node root , int level );

static void Prefix( Node root , unsigned int start );

static void Postfix( Node root , unsigned int start );

static void Infix( Node root , unsigned int start );

int main( int argc , char** argv ){

	Node node = ( Node ) malloc( sizeof( NodeDesc ) );

	if( argc == 2 ){
		SInit( argv[1] );
		symbol = SGet();
		Expression( &node );
		assert( symbol == eof );
		Print( node , 0);
		printf( "Prefix is ");
		Prefix( node , 1 );		
		printf( "Postfix is ");
		Postfix( node , 1 );		
		printf( "Infix is ");
		Infix( node , 1 );		
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

static void String(){ // We ensure before call this ch will in case of char
	variable[0] = ch;
	int run = 0;
	ch = getc( file );
	while( ( ch >= 'A' && ch <= 'Z') || ( ch >= 'a' && ch <= 'z' ) || ch == '_' ){
		assert( run < 48 );
		run++;
		variable[run] = ch;
		ch = getc( file );
	}
	run++;
	variable[ run ] = '\0';
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
	if( symbol == illegal ){
		if( (ch >= 'A' && ch <= 'Z') || ( ch >= 'a' && ch <= 'z' ) ){
			symbol = var;
			String();
		}
	}

	if( symbol == number ) Number(); 
	// this pattern we give you have symbol is number and value in val

	return symbol;
}

static void Factor( Node* node , int sign ){

	assert( ( symbol == number ) || ( symbol == lparen ) || ( symbol == var ) );
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
		if( symbol == var ){
			Node new_node = ( Node ) malloc( sizeof( NodeDesc) );
			Node var_node = ( Node ) malloc( sizeof( NodeDesc) );
			var_node->kind = var;
			CopyString(var_node->name , variable);
			new_node->kind = val;
			new_node->left = *(node);
			new_node->right = var_node;
			*node = new_node;
			symbol = SGet();
		}
	}
	else if( symbol == var ){
		symbol = SGet();
		(*node)->kind = var;
		CopyString( (*node)->name , variable );
		if( symbol == number ){
			Node new_node = ( Node ) malloc( sizeof( NodeDesc) );
			Node val_node = ( Node ) malloc( sizeof( NodeDesc) );
			val_node->kind = number;
			val_node->val = val;
			new_node->kind = val;
			new_node->left = val_node;
			new_node->right = *(node);
			*node = new_node;
			symbol = SGet();			
		}
	}
	else{
		symbol = SGet();
		Expression( node );
		assert( symbol == rparen ); // Check that have close tuple or not
		symbol = SGet();
	}

}

static void Term( Node* node , int sign ){

	Factor( node , sign );
	Node new_node;
	while( 1 ){
		switch( symbol ){
			case times	:	symbol = SGet();
							new_node = ( Node ) malloc( sizeof(NodeDesc) );
							new_node->kind = times;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							Factor( &((*node)->right) , 1 );
							continue;
			case divide	:	symbol = SGet();
							new_node = ( Node ) malloc( sizeof(NodeDesc) );
							new_node->kind = divide;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							Factor( &((*node)->right) , 1 );
							continue;
			case mod	:	symbol = SGet();
							new_node = ( Node ) malloc( sizeof(NodeDesc) );
							new_node->kind = mod;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							Factor( &((*node)->right) , 1);
							continue;
		}
		break;
	}
}

static void Expression( Node* node ){

	int sign_of_number = 1;
	switch( symbol ){
		case minus	: sign_of_number = -1;
		case plus	: symbol = SGet();
	}
	#ifdef _DEBUG_EXPRESSION_NODE_
		printf( "Expression sign_of_number before loop %d\n" , sign_of_number );
	#endif
	Term( node , sign_of_number ) ;

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
							Term( &(*node)->right , 1 );
							continue;
			case plus	:	new_node = ( Node ) malloc( sizeof( NodeDesc ) );
							new_node->kind = symbol;
							new_node->left = *node;
							*node = new_node;
							(*node)->right = ( Node ) malloc( sizeof( NodeDesc ) );
							symbol = SGet();
							Term( &(*node)->right , 1 );
							continue;
		}
		break;
	}
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
			case var	:	printf("%s\n" , root->name ); break;
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
			case var	:	printf("%s " , root->name ); break;
		}
		Prefix( root->left , 0 );
		Prefix( root->right , 0 );
	}
	if( start ) printf("\n");
}

static void Postfix( Node root , unsigned int start ){
	if( root != NULL ){
		Postfix( root->left , 0 );
		Postfix( root->right , 0 );
		switch( root->kind ){
			case plus	:	printf("+ "); break;
			case minus	:	printf("- "); break;
			case times	:	printf("* "); break;
			case divide :	printf("/ "); break;
			case mod	:	printf("m "); break;
			case number :	printf("%2d " , root->val ); break;
			case var	:	printf("%s " , root->name ); break;
		}
	}	
	if( start ) printf("\n");
}

static void Infix( Node root , unsigned int start ){
	if( root != NULL ){
		switch( root->kind ){
			case plus	:	printf("( "); Infix( root->left , 0 );
							printf("+ "); Infix( root->right , 0 );
							printf(") "); break;
			case minus	:	printf("( "); Infix( root->left , 0 );
							printf("- "); Infix( root->right , 0 );
							printf(") "); break;
			case times	:	printf("( "); Infix( root->left , 0 );
							printf("* "); Infix( root->right , 0 );
							printf(") "); break;
			case divide	:	printf("( "); Infix( root->left , 0 );
							printf("/ "); Infix( root->right , 0 );
							printf(") "); break;
			case mod	:	printf("( "); Infix( root->left , 0 );
							printf("m "); Infix( root->right , 0 );
							printf(") "); break;
			case number	:	printf("%d " , root->val ); break;
			case var	:	printf("%s " , root->name ); break;
		}
	}
	if( start ) printf("\n");
}

static void CopyString( char* target , char* origin ){
	for( int run = 0 ; ; run++ ){
		target[run] = origin[run];
		if( target[run] == '\0') break;
	}
}
