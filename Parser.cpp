#include "Parser.h"

namespace SimpleInterpreter{
    //参数T的存储位置，全局变量
    double parameter;
    // note:
    //curToken指向当前Token
    //每次匹配后更新curToken
    //确保每次匹配结束后curToken均指向未匹配的Token
    //最后执行的语句为Match()调用或update即可
    //或者在每次匹配时，传递参数决定是否更新字符？

    void errorHandler(TokenType& expect,int row) {
        cout << "ERROR in line " << row << ":Expect Token " << expect << '\n';
        exit(1);
    }
    void errorHandler(const string& expect,int row) {
        cout << "ERROR in line " << row << ":Expect Token " << expect << '\n';
        exit(1);
    }
    void errorHandler(int row) {
        cout << "ERROR in line " << row << ":Invalided character" << '\n';
        exit(2);
    }

    //test
    void enterIn(const string& cur) {
        cout << "Enter in " << cur << '\n';
    }
    void leaveOut(const string& cur) {
        cout << "leave out " << cur << '\n';
    }
}

// Simplify code writing for the code below
using namespace SimpleInterpreter;

void Parser::Match(TokenType expect)
{
//  cout << "Match:" << expect << '\n';
	if (curToken->type != expect) {
		errorHandler(expect,curToken->getRow());
	}
	updateToken();
}

void Parser::Program()
{
//  enterIn("Program");
	while (curToken->type != TokenType::NONTOKEN) {
		Statement();
		Match(TokenType::SEMICO);
	}
//  leaveOut("Program");
}

void Parser::Statement()
{
//  enterIn("Statement");
	switch (curToken->type)
	{
		case TokenType::ORIGIN:
			OriginStatement();
			break;
		case TokenType::ROT:
			RotStatement();
			break;
		case TokenType::SCALE:
			ScaleStatement();
			break;
		case TokenType::FOR:
			ForStatement();
			break;
		case TokenType::COLOR:
			ColorStatement();
			break;
		default:
			errorHandler("Statement",curToken->getRow());
			break;
	}
//  leaveOut("Statement");
}
//在语义被计算后，释放表达式的语法树所占空间？
//对于小程序非必要
//树在内部生成，在外部被delete
//也可传递一Tree对象（根节点对象）
//离开作用域时，自动调用析构函数，释放整棵树的存储空间？
void Parser::OriginStatement()
{
//  enterIn("OriginStatement");
	Match(TokenType::ORIGIN);
	Match(TokenType::IS);
	Match(TokenType::L_BRACKET);
	auto origin_x = Expression();
	Match(TokenType::COMMA);
	auto origin_y = Expression();
	Match(TokenType::R_BRACKET);

	painter->setOrigin(origin_x->getValue(), origin_y->getValue());
	
//  leaveOut("OriginStatement");
}

void Parser::RotStatement()
{
//  enterIn("RotStatement");
	Match(TokenType::ROT);
	Match(TokenType::IS);
	auto rot = Expression();

	double tmp = rot->getValue();
	painter->setRot(tmp);

//  leaveOut("RotStatement");
}

void Parser::ScaleStatement()
{
//  enterIn("ScaleStatement");
	Match(TokenType::SCALE);
	Match(TokenType::IS);
	Match(TokenType::L_BRACKET);
	auto scale_x = Expression();
	Match(TokenType::COMMA);
	auto scale_y = Expression();
	Match(TokenType::R_BRACKET);

	painter->setScale(scale_x->getValue(), scale_y->getValue());
	
//  leaveOut("ScaleStatement");
}

void Parser::ForStatement()
{
//  enterIn("ForStatement");
	Match(TokenType::FOR);
	Match(TokenType::T);
	Match(TokenType::FROM);
	auto start = Expression();
	Match(TokenType::TO);
	auto end = Expression();
	Match(TokenType::STEP);
	auto step = Expression();
	Match(TokenType::DRAW);
	Match(TokenType::L_BRACKET);
	auto x = Expression();
	Match(TokenType::COMMA);
	auto y = Expression();
	Match(TokenType::R_BRACKET);

	double end_v = end->getValue();
	double step_v = step->getValue();
	// TODO: change to use decimal?
	for(double i=start->getValue();i<end_v;i+=step_v)
	{
		parameter = i;
		double x_v = x->getValue();
		double y_v = y->getValue();
		painter->draw(x_v, y_v);
	}
	
//  leaveOut("ForStatement");
}
void Parser::ColorStatement()
{
//  enterIn("ColorStatement");
	double r, g, b;
	Match(TokenType::COLOR);
	Match(TokenType::IS);
	Match(TokenType::L_BRACKET);
	auto r_tree = Expression();
	r = r_tree->getValue();
	Match(TokenType::COMMA);
	auto g_tree = Expression();
	g = g_tree->getValue();
	Match(TokenType::COMMA);
	auto b_tree = Expression();
	b = b_tree->getValue();
	Match(TokenType::R_BRACKET);

	painter->setColor(r,g,b);
//  leaveOut("ColorStatement");
}
unique_ptr<Tree> Parser::Expression()
{
//  enterIn("Expression");
	unique_ptr<Tree> root;
	root = Term();
	//若不为PLUS，或者MINUS，则curToken已指向下一个符号
	while (curToken->type == TokenType::PLUS || curToken->type == TokenType::MINUS)
	{
		TokenType tmp = curToken->type;
		//新建根节点
		auto tempNode = make_unique<Tree>(curToken.get());
		tempNode->addLeft(std::move(root));
		//取走当前记号
		Match(tmp);
		//匹配右操作数
		auto right = Term();
		tempNode->addRight(std::move(right));
        //移动root指针
        root = std::move(tempNode);
	}
	//test
	root->printTree(0);
	cout << "The value is:" << root->getValue() << '\n';
//  leaveOut("Expression");
	return root;
}

unique_ptr<Tree> Parser::Term()
{
//  enterIn("Term");
	// root指向当前树根节点
	unique_ptr<Tree> root;
	//Term未匹配，表示语法分析出现错误
	root = Factor();
	while (curToken->type == TokenType::MUL || curToken->type == TokenType::DIV)
	{
		TokenType tmp = curToken->type;
		//新建根节点
		auto tempNode = make_unique<Tree>(curToken.get());
		tempNode->addLeft(std::move(root));
		//取走当前记号
		Match(tmp);
		auto right = Factor();
		tempNode->addRight(std::move(right));
		//更改指针
		// reassign moved variable
		root = std::move(tempNode);
        // tempNode is moved, the destructor will be call on a empty object
	}
//  leaveOut("Term");
    return root;
}
//匹配正负Component
unique_ptr<Tree> Parser::Factor()
{
//  enterIn("Factor");
	unique_ptr<Tree> root;
	TokenType tmp = curToken->type;
	switch (tmp)
	{
	//Component的First集合
	case TokenType::CONST_ID:
	case TokenType::T:
	case TokenType::FUNC:
	case TokenType::L_BRACKET:
		root=Component();
		break;
	case TokenType::PLUS:
	case TokenType::MINUS:
    {
        root = make_unique<Tree>(curToken.get());
        Match(tmp);
        // 右递归
        auto right = Factor();
        //单操作数加减法（默认左操作数为0）
        //添加到右子树
        root->addRight(std::move(right));
        break;
    }
	default:
		// 错误递归分支，递归基应当为Component
		errorHandler("Component",curToken->getRow());
		break;
	}
	//右递归，更新符号可能会执行多次？
	//在Component中更新即可？待测试！

//  leaveOut("Factor");
	return root;
}

unique_ptr<Tree> Parser::Component()
{
//  enterIn("Component");
	unique_ptr<Tree> root;
	root = Atom();
	//power在语法中允许嵌套，右递归形式
	if (curToken->type == TokenType::POWER) {
		auto tempNode = make_unique<Tree>(curToken.get());
		tempNode->addLeft(std::move(root));
		Match(TokenType::POWER);
		auto right = Component();
		tempNode->addRight(std::move(right));
		root = std::move(tempNode);
	}
//  leaveOut("Component");
    return root;
}

unique_ptr<Tree> Parser::Atom()
{
//  enterIn("Atom");
	unique_ptr<Tree> root;
	TokenType tmp = curToken->type;
	switch (tmp)
	{
	case TokenType::CONST_ID:
		root = make_unique<Tree>(curToken.get());
		Match(TokenType::CONST_ID);
		break;
	case TokenType::T:
		root = make_unique<Tree>(curToken.get());
		Match(TokenType::T);
		break;
	case TokenType::L_BRACKET:
		Match(TokenType::L_BRACKET);
		root = Expression();
		Match(TokenType::R_BRACKET);
		break;
	case TokenType::FUNC:
    {
        root = make_unique<Tree>(curToken.get());
        Match(TokenType::FUNC);
        Match(TokenType::L_BRACKET);
        auto right = Expression();
        // move the value
        root->addRight(std::move(right));
        Match(TokenType::R_BRACKET);
        break;
    }
	default:
		errorHandler("Atom", curToken->getRow());
		break;
	}
//  leaveOut("Atom");
	return root;
}

#ifdef WIN32
Parser::Parser(const wstring& fileName,Painter *p)
{
	curToken = nullptr;
	lexerp = new Lexer(fileName);
	painter = p;
	if (!lexerp) {
		cout << "Lexer initiated failed.\n";
		exit(-2);
	}
	updateToken();
}
#endif

//为兼容调试程序保留接口
Parser::Parser(const string& fileName,Painter *p)
{
	curToken = nullptr;
	lexerp = new Lexer(fileName);
	painter = p;
	if (!lexerp) {
		cout << "Lexer initiated failed.\n";
		exit(-2);
	}
	updateToken();
}

Parser::~Parser()
{
	delete lexerp;
}

Token* Parser::updateToken()
{
    /* this step is done automatically by smart pointer */
	//释放前一个记号的存储空间
	//delete curToken;

	//更新当前记号
	curToken= lexerp->getToken();
	//丢弃注释记号
	while (curToken->type == TokenType::COMMENT)
		curToken = lexerp->getToken();
	if (curToken->type == TokenType::ERRTOKEN) {
		errorHandler(curToken->getRow());
	}
	return curToken.get();
}
