#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

using namespace std;
enum type_of_lex
{
LEX_NULL,		//0
LEX_WHILE,		//1
LEX_END,		//2
LEX_ENDTURN,	//3
LEX_IF, 		//4
LEX_THEN, 		//5
LEX_DO,			//6
LEX_ELSE,		//7


LEX_SEMICOLON,  //8
LEX_COMMA,		//9
LEX_MORE,		//10
LEX_LESS,		//11
LEX_SLASH, 		//12
LEX_PERCENT,	//13
LEX_EQ, 		//14
LEX_EMP,		//15
LEX_STRAIGHT, 		//16
LEX_EXCL,		//17
LEX_PLUS,		//18
LEX_MINUS,		//19
LEX_MUL,		//20
LEX_DOT,		//21
LEX_OP_BRACKET,	//22
LEX_CL_BRACKET,	//23
LEX_SQ_OP_BRACKET,	//24
LEX_SQ_CL_BRACKET,	//25
  
LEX_NUM,		//26
LEX_VAR, 		//27
LEX_STR, 		//28
LEX_FUNC,		//29
LEX_FIN,		//30

LEX_PRINT, 		//31
LEX_BUY, 		//32
LEX_SELL, 		//33
LEX_PROD, 		//34
LEX_BUILD, 		//35
//?functions
LEX_MY_ID,		//36
LEX_TURN,		//37
LEX_PLAYERS, 		//38
LEX_ACTIVE_PLAYERS,	//39
LEX_SUPPLY,			//40
LEX_RAW_PRICE,		//41
LEX_DEMAND,			//42
LEX_PRODUCTION_PRICE,//43
LEX_MONEY,			//44
LEX_RAW,			//45
LEX_PRODUCTION,		//46
LEX_FACTORIES,				//47
LEX_AUTO_FACTORIES,			//48
LEX_MANUFACTURED,			//49
LEX_RESULTS_RAW_SOLID,		//50
LEX_RESULTS_RAW_PRICE, 		//51
LEX_RESULT_PROD_BOUGHT,		//52
LEX_RESULT_PROD_PRICE, 		//53		
LEX_AND,
LEX_OR

};

class Lex
{
	type_of_lex t_lex;
	float v_lex;
	public:
	Lex ( type_of_lex t = LEX_NULL, float v = 0){
		t_lex = t; v_lex = v;
	}
	type_of_lex get_type () { return t_lex; }
	int get_value () { return v_lex; }
	friend ostream& operator << ( ostream &s, Lex l );
	Lex &operator=(Lex l){
		v_lex = l.v_lex;
		t_lex = l.t_lex;
		return *this;
	}
	//{
		//s << "type of lexem is: " << l.t_lex << "value of lexem is " << l.v_lex << ");";
		//return s;
//	}
};


/*POLIZ_LABEL,
POLIZ_ADDRESS,
POLIZ_GO,
POLIZ_FGO*/
class ident{
	char *name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
	
	public:
	
	ident(){
		declare= false;
		assign = false;
		name = new char [1];
		strcpy(name,"\0");
	}
	char *get_name (){
		return name;
	}
	void put_name(const char *n){
		name = new char [strlen(n) +1];
		strcpy(name,n);
	}
	bool get_declare(){ return declare;}
	void put_declare(){ declare = true; }
	type_of_lex get_type(){ return type;}
	void put_type(type_of_lex t){
		type = t;
	}
	bool get_assign(){ return assign; }
	void put_assign(){ assign = true; }
	int get_value(){ return value; }
	void put_value(int v){ value = v; }
};

class tabl_ident{
	ident *p;
	int size;
	int top;
	public:
	tabl_ident(int max_size){
		p = new ident[max_size];
		size = max_size;
		top = 1;
	}
	~tabl_ident(){
		delete []p;
	}
	ident& operator[](int k){
		return p[k];
	}
	int put(const char *buf){
		for (int j=1; j<top; ++j)
			if (!strcmp(buf, p[j].get_name()))
				return j;
		p[top].put_name(buf);
		++top;
		return top-1;
	}
};
class Scanner {
	enum state { H, NUMB, VAR, IDENT, INDEX, F, S, DELIM, ER, REAL};
	static char *TW[];
	static type_of_lex words[];
	static char *TD[];
	static type_of_lex funcnames[];
	static type_of_lex dlms[];
	static char *TF[];//table of functions
	state CS;
	FILE *fp;
	int c;
	char buf[80];
	int buf_top;
	public:
	void clear(){
		buf_top = 0;
		for (int j=0; j<80; ++j)
			buf[j] = '\0';
	}
	void add(){
		buf[ buf_top ++] =c;
	}
	int look(const char *buf, char **list){
		int i = 0;
		while (list[i]){
			if (!strcmp(buf, list[i]))
				return i;
			++i;
		}
		return 0;
	}
	
	void gc(){
		c = fgetc (fp);
	}
	Lex get_lex();
	Scanner (const char *program){
			fp = fopen (program,"r");
			CS = H;
			clear();
			gc();
	}
	
};

char *Scanner::TW[] = { 
	"" 	,		//0
	
	"while",	//1
	"end",		//2
	"if",		//3
	"then",		//4
	"print",	//5
	"buy",		//6
	"sell",		//7
	"prod",		//8
	"build",	//9
	"do",		//10
	"else",		//11
	"and",		//12
	"or",		//13
	NULL
};

char *Scanner:: TD[] = {
	"",				//0
	"+",			//1
	"-",			//2
	"*",			//3
	"/",			//4
	"%",			//5
	"<",			//6
	">",			//7
	"=",			//8

	"!",			//10
	";",			//1
	".",			//13
	"[",			//14
	"]",			//15
	"(",
	")",
	NULL
};

char *Scanner::TF[] = {
	"",						//0
	"my_id",				//1
	"turn",					//2
	"players", 				//3
	"active_players",		//4
	"supply",				//5
	"raw_price",				//6
	"demand",				//7
	"production_price",		//8
	"money",				//9
	"raw",					//10
	"production",			//11
	"factories",			//12
	"auto_factories",		//13
	"manufactured",			//14
	"result_raw_sold",		//15
	"result_raw_price",	 	//16
	"result_prod_bought",	//17
	"result_prod_price", 	//18
};
tabl_ident TID(100);
type_of_lex Scanner::words[] = {
	LEX_NULL,		//0
	LEX_WHILE,		//1
	LEX_END,		//2
	LEX_IF,			//3
	LEX_THEN,		//4
	LEX_PRINT,		//5
	LEX_BUY,		//6
	LEX_SELL,		//7
	LEX_PROD,		//8
	LEX_BUILD,		//9
	LEX_DO,			//10
	LEX_ELSE,		//11
	LEX_AND,
	LEX_OR,
	LEX_NULL		//12
	
};

type_of_lex Scanner::dlms[] = {
	LEX_NULL,		//0
	LEX_PLUS,		//1
	LEX_MINUS,		//2
	LEX_MUL,		//3
	LEX_SLASH,		//4
	LEX_PERCENT,	//5
	LEX_LESS,		//6
	LEX_MORE,		//7
	LEX_EQ,			//8
	
	LEX_EXCL,		//11
	
	LEX_SEMICOLON,	//12
	LEX_DOT,		//13
	LEX_SQ_OP_BRACKET,
	LEX_SQ_CL_BRACKET,
	LEX_OP_BRACKET,
	LEX_CL_BRACKET,
	LEX_NULL,		//14
};

type_of_lex Scanner::funcnames[] = {
	LEX_NULL,				//0
	LEX_MY_ID,				//1
	LEX_TURN,				//2
	LEX_PLAYERS, 			//3
	LEX_ACTIVE_PLAYERS,		//4
	LEX_SUPPLY,				//5
	LEX_RAW_PRICE,			//6
	LEX_DEMAND,				//7
	LEX_PRODUCTION_PRICE,	//8
	LEX_MONEY,				//9
	LEX_RAW,				//10
	LEX_PRODUCTION,			//11
	LEX_FACTORIES,			//12
	LEX_AUTO_FACTORIES,		//13
	LEX_MANUFACTURED,		//14
	LEX_RESULTS_RAW_SOLID,	//15
	LEX_RESULTS_RAW_PRICE, 	//16
	LEX_RESULT_PROD_BOUGHT,	//17
	LEX_RESULT_PROD_PRICE, 	//18
};

// scary and dumb af function, just accept it for what it is
ostream& operator<< (ostream &s,Lex l){
	switch (l.t_lex){
		
	case LEX_NULL:
			s << "NULL";
		break;
	case LEX_WHILE:
			s << "while";
			break;
	case LEX_END:
			s << "end";
			break;
	case LEX_ENDTURN:
			s << "endturn";
			break;
	case LEX_IF:
			s << "if";
			break;
	case LEX_THEN:
			s << "then";	
			break;
	case LEX_SEMICOLON:
			s << ";";
			break;
	case LEX_COMMA:
			s << ",";
			break;
	case LEX_MORE:
			s << ">";
			break;
	case LEX_LESS:
			s << "<";
			break;
	case LEX_SLASH:
			s << "/";
			break;
	case LEX_PERCENT:	
			s << "%";
			break;
	case LEX_EQ:
			s << "=";
			break;
	case LEX_AND:
			s << "and";
			break;
	case LEX_OR:
			s << "or";
			break;
	case LEX_EXCL:
			s << "!";
			break;
	case LEX_PLUS:
			s << "+";
			break;
	case LEX_MINUS:
			s << "-";
			break;
	case LEX_MUL:
			s << "*";
			break;

	  
	case LEX_NUM:
			s << l.v_lex;
			break;
	case LEX_VAR:
			s<<"variable " ;//<< TID[l.v_lex].get_name();
			break;
	case LEX_STR: 		
			s << "string " ;//<<TID[l.v_lex].get_name();
			break;
	case LEX_FUNC:		
			s << "function";
			break;
	case LEX_PRINT:
			s << "print";
			break;
	case LEX_BUY:
			s << "buy";
			break;
	case LEX_SELL:
			s << "sell";
			break;
	case LEX_PROD:
			s << "prod";
			break;
	case LEX_BUILD:
			s << "build";
			break;
	case LEX_MY_ID:
			s << "my_id";
			break;
	case LEX_TURN:
			s << "turn";
			break;
	case LEX_PLAYERS:
			s << "players";
			break;
	case LEX_ACTIVE_PLAYERS:
			s << "active players";
			break;
	case LEX_SUPPLY:
			s << "supply";
			break;
	case LEX_RAW_PRICE:
			s << "raw_PRICE";
			break;
	case LEX_DEMAND:
			s << "demand";
			break;
	case LEX_PRODUCTION_PRICE:
			s << "production price";
			break;
	case LEX_MONEY:
			s <<"money";
			break;
	case LEX_RAW:
			s <<"raw";
			break;
	case LEX_PRODUCTION:
			s <<"production";
			break;
	case LEX_FACTORIES:	
			s << "factories";
			break;
	case LEX_AUTO_FACTORIES:
			s << "auto factories";
			break;
	case LEX_MANUFACTURED:
			s <<"manufactured";
			break;
	case LEX_RESULTS_RAW_SOLID:
			s << "results raw solid";
			break;
	case LEX_RESULTS_RAW_PRICE:
			s << "results raw price";
			break;
	case LEX_RESULT_PROD_BOUGHT:
			s << "result prod bought";
			break;
	case LEX_RESULT_PROD_PRICE:
			s << "result prod price";
			break;
	case LEX_FIN:
			s<<"eof";
	break;
	case LEX_OP_BRACKET:
			s<< "(";
			break;
	case LEX_CL_BRACKET:
			s<< ")";
			break;
	case LEX_SQ_OP_BRACKET:
			s<< "[";
			break;
	case LEX_SQ_CL_BRACKET:
			s<< "]";
			break;
	
	case LEX_ELSE:
			s << "else";
			break;//47
	case LEX_DOT:
			s << ".";
			break;
			
	case LEX_DO:
			s<<"do";
			break;
		}
}




Lex Scanner::get_lex(){
	int  d=0,j=0;
	double v=0,r=0;
	CS = H;
	do{
		switch(CS){
			case H:
				if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
					gc();
				else if (isalpha(c)){
					clear();
					add();
					gc();
				//	cout<<"ident"<<endl;
					CS= IDENT;
				}
				else if (isdigit(c)){
					d = c-'0';
					gc();
					CS = NUMB;
				}
				else if ( c == '$'){
					clear();
					gc();
					CS  = VAR;
				}
				else if (c == '?'){
					clear();
					gc();
					CS = F;
				}
				else if (c == '"'){
					clear();
					gc();
					CS = S;
				}
				else if  (c == EOF)
					return Lex(LEX_FIN);
				else  CS = DELIM;
				break;
			case IDENT:
				if (isalpha(c) || isdigit(c)){
					add();
					gc();
				}
				else
					if (j= look(buf,TW))
						return Lex(words[j],j);
					else {
						//j = TID.put(buf);
						//return Lex(LEX_ID,j);
						throw "error";
					}
					break;
			case NUMB:
				//cout<<"num"<<endl;
				while (isdigit(c)){
					d = d*10 + (c- '0');
					gc();
				}
				if (c == '.'){
					gc();
					CS = REAL;
				//	cout<<"real"<<endl;
				}
				else
					return Lex(LEX_NUM,d);
				break;
			case REAL:
					if (isdigit(c)){
						r = r*10  + c- '0';
						gc();
					}
					else{
						while (r>=1) r = r *0.1;
						v = d +  r;
						cout<<v<<endl;
						return Lex(LEX_NUM,v);
					}
					break;
						
			case S:
				if (c != '"' && c!= EOF){
					add();
					gc();
				}
				else if (c == EOF) throw "end of string is undefined";
				else {// -- ?? adding string to TID? 
					gc();
					j = TID.put(buf);
					return Lex(LEX_STR,j);
				}
				break;
			case F:
				if (isdigit(c) || isalpha(c) || c == '_'){
					add();
					gc();
				}
				else if (j = look(buf,TF))
					return Lex(funcnames[j],j);
				else throw "undefined reference to function";
				
			break;
			case VAR:
				
				if (isalpha(c) || isdigit(c)){
					add();
					gc();
				}
				else if (c == '[' ){
					//ungetc(c,fp);
					j = TID.put(buf);
					return Lex(LEX_VAR, j);
					//add();
					//gc();
					//CS = INDEX;
				}
				
				else if (c == ']'){
						//ungetc(c,fp);
					
					//cout<<"put"<<endl;
					j = TID.put(buf);
					return Lex(LEX_VAR, j);
				}
				else {
					j = TID.put(buf);
					return Lex(LEX_VAR, j);
				}
				break;
			case INDEX:
				if (c == '$'){
					add();
					gc();
					//CS = VINDEX;
				}
				else if (isdigit(c)){
					add();
					gc();
				}
				else if (c == ']'){
					add();
					gc();
					j = TID.put(buf);
					return Lex(LEX_VAR, j);
				}
				break;
			case DELIM:
				clear();
				add();
				if (j = look(buf, TD)){
					gc();
					return Lex(dlms[j], j);
				}
				else throw c;
				break;
			}
		}
		while(true);
	}
	
struct lexlist{
	Lex data;
	lexlist *next;
};



//-------------------       PARSING       ------------------------------

	
class Parser{
	Lex curr_lex;//current lexem
	type_of_lex c_type;
	int c_val;
	Scanner scan;
	
//	Stack < int, 100 > st_int;
//	Stack < type_of_lex, 100 > st_lex;
	public:
	void gl(){
	//	cout<<"gl"<<endl;
		curr_lex = scan.get_lex();
		cout<<curr_lex<<endl;
		c_type = curr_lex.get_type();
		c_val = curr_lex.get_value();
	}
	Parser ( const char *program) : scan (program) {}
	void S();
	void A();
	void L();
	void F();
	void I();
	void O();
	void W();
	void E();
	void R();
	
	void R1();
//	void Q();
	void D();
	void V();
	void E1();
	int analyze();//analyzator w/actions
};
int Parser:: analyze(){
	gl();
	S();
	cout<<"Correct"<<endl;
	return 1;
}
void Parser::S(){
	cout<<"S"<<endl;
	while (c_type != LEX_FIN){
		A();
	//	gl();
		cout<<c_type<<endl;
	//	if (c_type != LEX_SEMICOLON && c_type != LEX_FIN)
		//	throw curr_lex;
		
	}
}

void Parser::A(){
	cout<<"A"<<endl;
	if (c_type == LEX_PRINT){
		L();
		gl();
	}
	else if (c_type == LEX_WHILE || c_type == LEX_IF)
		W();
	else if (c_type == LEX_VAR){
		D();
		gl();
	}
	else if (c_type == LEX_SEMICOLON) gl();
	else {O(); gl(); }
	
}
void Parser::L(){
	cout<<"L"<<endl;
	while (c_type != LEX_SEMICOLON){
		gl();
		if (c_type == LEX_STR );
		else if (c_type == LEX_VAR) V();
		else if (c_type >=LEX_MY_ID) F();
		else if (c_type == LEX_SEMICOLON) return;
		else throw curr_lex;
		
	}
	
}

void Parser::F(){
	cout<<"F"<<endl;
	cout<<c_type<<endl;
	if (c_type <= 44)
	
		return;
	
	gl();
	cout<<curr_lex<<endl;
	if (c_type == LEX_OP_BRACKET){
		I();
		gl();
		if (c_type != LEX_CL_BRACKET)
			throw curr_lex;
	}	
	else throw curr_lex;
}
void Parser:: I(){
	gl();
	if (c_type == LEX_VAR)
		V();
	else if (c_type >= LEX_MY_ID) F();
	else if (c_type == LEX_NUM) return;
	else {
		cout<<"invalid argument"<<endl;
		throw curr_lex;
	}
}

void Parser::O(){
	cout<<"O"<<endl;
	if (c_type == LEX_PROD || c_type == LEX_BUILD) I();
	else if (c_type == LEX_BUY || c_type == LEX_SELL){
		I();
		I();
		if (c_type != LEX_SEMICOLON) gl();
	}
	else if (c_type != LEX_ENDTURN) 
		throw curr_lex;
}
void Parser::W(){
	cout<<"W"<<endl;
	if (c_type == LEX_WHILE){
		E();
		
		if (c_type != LEX_DO) throw curr_lex;
		gl();
		W();
	}
	else if (c_type == LEX_IF){
		E1();
	//	gl();
		if (c_type != LEX_THEN) throw curr_lex;
		gl();
		W();
	//	if (c_type == LEX_SEMICOLON) return;
		gl();
		if (c_type == LEX_ELSE){
			gl();
			W();
			gl();
		}
		else return;
	}
	else if (c_type == LEX_VAR) D();
	else if (c_type <=35 && c_type >=32 || c_type == LEX_ENDTURN) O();
	else if (c_type == LEX_PRINT) L();
}
void Parser::D(){
	cout<<"D"<<endl;
	V();
	if (c_type != LEX_EQ)
		gl();
	if (c_type != LEX_EQ)
		 throw curr_lex;
	gl();
	R();
	
}
void Parser::V(){// do something about variables declarations =C
	cout<<"v"<<endl;
	gl();
	if (c_type == LEX_SQ_OP_BRACKET){
		gl();
		if (c_type == LEX_VAR) 
			V();
		else if (c_type == LEX_NUM) gl();
		if (c_type != LEX_SQ_CL_BRACKET) {
			cout<<" ] expected"<<endl;
			throw curr_lex;
		}
		
	}
	
} 
void Parser::R(){
	cout<<"R"<<endl;
	if (c_type == LEX_OP_BRACKET){
		gl();
		R();
		if (c_type != LEX_CL_BRACKET) { cout<<"bracket"<<endl;throw curr_lex;}
		else {
			gl();
			R();
		}
	}
	else if (c_type == LEX_VAR) {
		V();
		if (c_type == LEX_SQ_CL_BRACKET) gl();
	}
	else if (c_type == LEX_NUM) {gl(); cout<<curr_lex<<"out R"<<endl;}
	else if (c_type >=LEX_MY_ID && c_type!=LEX_AND &&c_type != LEX_OR) {F(); gl(); } 
	
	 if (c_type == LEX_MINUS || c_type == LEX_PLUS || c_type == LEX_MUL || c_type == LEX_SLASH || c_type == LEX_PERCENT){
		gl();
		R();
	}
}
	
void Parser::E(){
	cout<<"E"<<endl;
	gl();
	R1();
	while (c_type == LEX_OR || c_type == LEX_AND){
			gl();
			R1();
	}
	
	 if (c_type != LEX_DO )
		gl();
	 cout<<"out E"<<endl;
}
void Parser::E1(){
	cout<<"E1"<<endl;
	gl();
	R1();
	while (c_type == LEX_OR || c_type == LEX_AND){
			gl();
			R1();
	}
	
	 if (c_type != LEX_THEN )
		gl();
	 cout<<"out E"<<endl;
}

void Parser::R1(){
	cout<<"R1"<<endl;
	R();
	if (c_type == LEX_CL_BRACKET) gl();
	if (c_type == LEX_LESS || c_type == LEX_MORE){
		gl();
		R();
	}
}
int main(){
	try{
		Parser p("prog.txt");
		p.analyze();
	}
/*
int main(){
	try{
		lexlist *l = new lexlist,*h = l;
		
		cout<<"begin"<<endl;
		Scanner scan("prog.txt");
		cout<<"scan"<<endl;
		Lex lexem;
		while (lexem.get_type() != LEX_FIN){
			lexem = scan.get_lex();
			l->data = lexem;
			l->next = new lexlist;
			l = l->next;
			cout<<lexem<<endl;
			//if (lexem.get_type() == 20 || lexem.get_type() == 21)
		//		cout<<"   " <<TID[lexem.get_value()].get_name();
		//	cout<<"\n";
			//cout<<TID[lexem.get_value()].get_name()<<endl;
		}
	l->next = NULL;
	l = h;
	while (l != NULL){
		cout<<l->data<<endl;
		if (l->data.get_type() == LEX_STR || l->data.get_type() == LEX_VAR)
				cout<<"   " <<TID[l->data.get_value()].get_name();
		l = l->next;
	}

	} */
	catch(const char* s){
		cout<<s<<endl;
	}
	catch (Lex l){
		cout<<"error on "<<l<<endl;
	}
//	catch (...){
		
//		cout<<"unexpected behavior"<<endl;
//	}
}	
				
				
				
					
	
	
	
	
