#pragma once
#include<fstream>
#include<string>
#include "Lexer.h"
#include "Painter.h"
#include "Tree.h"

using namespace std;
namespace SimpleInterpreter{
    // 参数T声明，定义声明在Parser.cpp中
    // have been declared in "Tree.h"
    // extern double parameter;

    class Parser
    {
        Lexer* lexerp;//调用的词法分析器对象指针
        // ownership is managed here
        unique_ptr<Token> curToken;//当前未匹配的记号对象指针
        Painter* painter; //绘图所需对象的指针

        void Match(TokenType expect);//匹配单个指定记号

        void Statement();
        void OriginStatement();
        void RotStatement();
        void ScaleStatement();
        void ForStatement();
        void ColorStatement();

        unique_ptr<Tree> Expression();
        unique_ptr<Tree> Term();
        unique_ptr<Tree> Factor();
        unique_ptr<Tree> Component();
        unique_ptr<Tree> Atom();
    public:
        void Program();//外部调用的接口
        //构造函数，参数为待分析文件名和Painter对象指针
#ifdef WIN32
        Parser(const wstring& fileName,Painter *p);
#endif
        Parser(const string& fileName, Painter* p);
        ~Parser();
        // note: token ownership is hold by member variable curToken
        Token* updateToken();//更新当前记号的接口
    };

}

