#include "Token.h"
#include<map>
#include<vector>
#include<cmath>
#include<algorithm>

using namespace std;
using namespace SimpleInterpreter;

//已知具体类型时调用
Token::Token(TokenType t, string w, int r)
{
	type = t; word = w; row = r; value = 0.0;
	if(type == TokenType::CONST_ID)
		value = stod(w);
}
//识别为标识符，未知具体类型时调用
Token::Token(string w, int r)
{
	static map<string,TokenType> hash = {{"pi",   TokenType::CONST_ID}, {"e", TokenType::CONST_ID},
                                         {"sin",  TokenType::FUNC}, {"cos", TokenType::FUNC}, {"tan", TokenType::FUNC}, {"ln", TokenType::FUNC}, {"exp", TokenType::FUNC},
                                         {"sqrt", TokenType::FUNC}, {"origin", TokenType::ORIGIN}, {"scale", TokenType::SCALE}, {"rot", TokenType::ROT}, {"color", TokenType::COLOR},
                                         {"is",   TokenType::IS}, {"for", TokenType::FOR}, {"from", TokenType::FROM}, {"to", TokenType::TO}, {"step", TokenType::STEP},
                                         {"draw", TokenType::DRAW}, {"t", TokenType::T}};
	static map<string, funp> funcpTable = { {"sin",sin} ,{"cos",cos},{"tan",tan},{"ln",log},{"exp",exp},{"sqrt",sqrt}};

	string tmp=w;
	transform(w.begin(),w.end(),tmp.begin(),::tolower);

	if(hash.find(tmp)==hash.end()){
		type = TokenType::ERRTOKEN;
	}
	else{
		type = hash[tmp];
	}

	word = w; row = r; value = 0.0;

	if(type == TokenType::CONST_ID){
		if(tmp=="pi")
			value = 3.141593;
		else if(tmp=="e")
			value = 2.71828;
		// else   会不会出问题？？
	}
	else if(type == TokenType::FUNC){
		funcp = funcpTable[w];
	}
}

double Token::getValue()
{
	return value;
}

int Token::getRow()
{
	return row;
}

funp Token::getfuncp()
{
	return funcp;
}

namespace SimpleInterpreter{

ostream& operator<<(ostream& o, TokenType t) {
    static vector<string> ttos={
            "ID","COMMENT",
            "ORIGIN","SCALE","ROT","COLOR","IS","TO","STEP","DRAW","FOR","FROM",
            "T",
            "SEMICO","L_BRACKET","R_BRACKET","COMMA",
            "PLUS","MINUS","MUL","DIV","POWER",
            "FUNC",
            "CONST_ID",
            "NONTOKEN",
            "ERRTOKEN"
    };
    o << ttos[static_cast<int>(t)];
    return o;
}
ostream& operator<<(ostream& o,Token t){
    o << t.type;
    return o;
}

bool isDigital(char c){
    return c >= '0'&&c <= '9';
}

}
