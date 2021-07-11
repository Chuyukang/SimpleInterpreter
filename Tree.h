#include "Token.h"
#include <memory>

using std::unique_ptr;

namespace SimpleInterpreter{
    //表达式的抽象语法树类型
    //T的实际存储地址，定义在Parser类中
    extern double parameter;

    class Tree
    {
        TokenType opType;//语法树算符类型
        unique_ptr<Tree> left, right;//左右孩子节点指针
        double rvalue;//存储节点的右值
        double* lvalue;//存储节点的左值
        funp funcp;//函数调用节点的函数指针
    public:
        // note: t is not preserved in Tree node
        //      the ownership of t is managed outside
        explicit Tree(Token *t);
        ~Tree();
        // note: call with rvalue, std::move
        void addLeft(unique_ptr<Tree> t);//为当前节点添加左孩子
        void addRight(unique_ptr<Tree> t);//为当前节点添加右孩子
        double getValue();//获得以该节点为根的表达式树的值
        //test
        void printTree(int level);//打印语法树
    };

}
