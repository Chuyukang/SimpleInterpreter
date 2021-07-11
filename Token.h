#ifndef SIMPLEINTERPRETER_TOKEN_H
#define SIMPLEINTERPRETER_TOKEN_H

#include<iostream>
#include<string>
using namespace std;

namespace SimpleInterpreter{
    typedef double (*funp)(double);

    enum class TokenType
    {ID,COMMENT,
        ORIGIN,SCALE,ROT,COLOR,IS,TO,STEP,DRAW,FOR,FROM,
        T,
        SEMICO,L_BRACKET,R_BRACKET,COMMA,
        PLUS,MINUS,MUL,DIV,POWER,
        FUNC,
        CONST_ID,
        NONTOKEN,
        ERRTOKEN
    };
    extern ostream& operator<<(ostream& o, TokenType t);
    class Token
    {
        string	word;//记号文本
        int row;//记号所处位置
        funp funcp;//函数调用记号的指针
        double value;//字面量的值
    public:
        //便于测试，将type设为public
        TokenType type;
        Token(TokenType t, string w, int r);//已知token类型时调用
        Token(string w, int r);//识别为标识符时调用
        double getValue();//获取记号的字面值
        int getRow();//获取记号所处行号
        funp getfuncp();//获取记号对应的函数指针
        friend ostream& operator<<(ostream& o,Token t);
    };

}

#endif //SIMPLEINTERPRETER_TOKEN_H