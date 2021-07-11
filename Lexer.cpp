#include "Lexer.h"
#include <algorithm>
#include <memory>

using namespace SimpleInterpreter;

#ifdef WIN32
Lexer::Lexer(const wstring& fileName)
{
	//使用异常，会使得后续读到EOF也抛出异常，
	//执行逻辑不好写，改用返回值判断
	//file.exceptions(std::ifstream::failbit);
		file.open(fileName,ios::in);
		if (!file.is_open()) {
			cout << "File open failed.";
			exit(-1);
		}
		dfap = new Dfa();
}
#endif
//为兼容调试程序保留接口
Lexer::Lexer(const string& fileName)
{
		file.open(fileName,ios::in);
		if (!file.is_open()) {
			cout << "File open failed.";
			exit(-1);
		}
		dfap = new Dfa();
}

Lexer::~Lexer()
{
	try
	{
		if (file.is_open())
			file.close();
	}
	catch (exception& e)
	{
		cout << e.what();
		cout << "A exception is thrown when file close.";
	}
	//delete空指针无影响
	delete dfap;
}

unique_ptr<Token> Lexer::getToken()
{
	static int row = 0;
	int ch=file.get();
	int state = dfap->getInitial();
	string tmp;
	while (ch != EOF|| !tmp.empty())
	{
		if (ch == '\n') {
			row++;
		}
		//无转移作为结束标志
		int ns=dfap->move(state,ch);
		if(ns != -1)
		{
			state = ns;
			//在有转移的集合中引入了空白字符（不是记号文本的一部分）
			if(ch!=' '&&ch!='\t'&&ch!='\n')
				tmp.push_back(ch);//附加记号文本
		}
		else
		{
			//无转移，且不为空字符，退回字符
			//去除了作为记号分割的空白字符
			if(ch!=' '&&ch!='\t'&&ch!='\n')
				file.putback(ch);
			unique_ptr<Token> token= nullptr;
			TokenType type;
			switch (state)
			{
			case 1:
				token = make_unique<Token>(tmp, row);
				break;
			case 2:
			case 3:
				type = TokenType::CONST_ID;
				break;
			case 4:
				type = TokenType::MUL;
				break;
			case 5:
				type = TokenType::POWER;
				break;
			case 6:
				type = TokenType::DIV;
				break;
			case 7:
				type = TokenType::MINUS;
				break;
			case 8:
				type = TokenType::PLUS;
				break;
			case 9:
				type = TokenType::COMMA;
				break;
			case 10:
				type = TokenType::SEMICO;
				break;
			case 11:
				type = TokenType::L_BRACKET;
				break;
			case 12:
				type = TokenType::R_BRACKET;
				break;
			case 13:
				type = TokenType::COMMENT;
				token = make_unique<Token>(type, tmp, row);
				//放回可能会读入的回车符
				file.putback(ch);
				//在此，读出并丢弃注释行的其他字符
				int t;
				while ((t = file.get()) != '\n'&&t!=EOF);
				//在此直接创建对象
				break;
			default://不是终态
				type = TokenType::ERRTOKEN;
				break;
			}

			if (!token)
				token = make_unique<Token>(type, tmp, row);
			tmp.erase(tmp.begin(),tmp.end());
			return token;
		}
		ch = file.get();
	}
	return std::make_unique<Token>(TokenType::NONTOKEN, "", row);
}