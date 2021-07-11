#include<fstream>
#include <memory>
#include "Token.h"
#include "Dfa.h"

using namespace std;

namespace SimpleInterpreter{
    class Lexer
    {
        fstream file;//待解析文件流
        Dfa		*dfap;//加载的dfa对象的指针
    public:
#ifdef WIN32
        // windows use wstring parameter
	Lexer(const wstring& fileName);
#endif
        Lexer(const string& fileName);
        ~Lexer();
        unique_ptr<Token> getToken();//提供获取下一个记号的接口
    };

}

