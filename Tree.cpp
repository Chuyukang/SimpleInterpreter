#include "Tree.h"
#include <cmath>

using namespace SimpleInterpreter;

Tree::Tree(Token* t)
{
    // member value default initiate, not necessary
	left = nullptr;
	right = nullptr;
	lvalue = nullptr;
	funcp = nullptr;
	rvalue = 0.0;
	opType = t->type;
	if (t->type == TokenType::CONST_ID) {
		rvalue = t->getValue();
	}
	if (t->type == TokenType::T) {
		lvalue = &parameter;
	}
	if (t->type == TokenType::FUNC) {
		funcp = t->getfuncp();
	}
}

Tree::~Tree()
{
	//调用子节点的析构函数
//	delete left;
//	delete right;
	//lvalue为parser对象的成员变量,不在此回收
}

void Tree::addLeft(unique_ptr<Tree> t)
{
    // change the value of t to nullptr
    // transfer the ownership to left
    //
    // explicitly transfer the variable t
    // equal to code below?
    // left.swap(t);

    // named rvalue reference is treated as lvalue

    // std::move only change the function choice
    // and complete move action in the appropriate operator= function
	left = std::move(t);
}

void Tree::addRight(unique_ptr<Tree> t)
{
	right = std::move(t);
}

void Tree::printTree(int level)
{
	for (int i = 0; i < level; i++)
		cout << "  ";
	switch (opType)
	{
	case TokenType::PLUS:
		cout << '+';
		break;
	case TokenType::MINUS:
		cout << '-';
		break;
	case TokenType::MUL:
		cout << '*';
		break;
	case TokenType::DIV:
		cout << '/';
		break;
	case TokenType::POWER:
		cout << "POW";
		break;
	case TokenType::CONST_ID:
		cout << "CONST_ID" << ' ' << rvalue;
		break;
	case TokenType::T:
		cout << "T" << ' ' << lvalue;
		break;
	case TokenType::FUNC:
		cout << "FUNC" << ' ' << funcp;
		break;
	default:
		break;
	}
	cout << '\n';
	if(this->left)
		this->left->printTree(level+1);
	if(this->right)
		this->right->printTree(level+1);
}

double Tree::getValue()
{
	double value = 0.0;
	switch (opType)
	{
	case TokenType::T:
		value = *lvalue;
		break;
	case TokenType::PLUS:
		if (left)
			value = left->getValue() + right->getValue();
		else
			value = right->getValue();
		break;
	case TokenType::MINUS:
		if (left)
			value = left->getValue() - right->getValue();
		else
			value = -(right->getValue());
		break;
	case TokenType::MUL:
		value = left->getValue() * right->getValue();
		break;
	case TokenType::DIV:
		value = left->getValue() / right->getValue();
		break;
	case TokenType::POWER:
		value = pow(left->getValue(),right->getValue());
		break;
	case TokenType::FUNC:
		value = funcp(right->getValue());
		break;
	case TokenType::CONST_ID:
		value = rvalue;
		break;
	default:
		//??
		break;
	}
	return value;
}
