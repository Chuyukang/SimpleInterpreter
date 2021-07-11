#include "Dfa.h"

using namespace SimpleInterpreter;
//初始化为本词法分析器的dfa数据
//0-13为状态，大类（ID包含多个关键字类）
//1-13均为终态
Dfa::Dfa()
{
	states = new vector<int>
	{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };
	finalstates = new vector<int>
	{ 1,2,3,4,5,6,7,8,9,10,11,12,13 };
	//matrix[0]
	matrix = new map<char, int>[14];
	//0状态 空白字符 转移为0状态
	matrix[0] = {{' ',0},{'\t',0},{'\n',0},
		{'l',1},{'d',2},{'*',4},{'/',6},{'-',7},
		{'+',8},{',',9},{';',10},{'(',11},{')',12} };
	matrix[1] = { {'l',1},{'d',1} };
	matrix[2] = { {'d',2}, {'.',3}};
	matrix[3] = { {'d',3}};
	matrix[4] = {{'*',5}};
	matrix[6] = {{'/',13}};
	matrix[7] = {{'-',13}};
	//如何处理标记为letter的边？
	//将letter映射为'l'，digit映射为'd'
}

//Dfa::Dfa(vector<int>* states_in, vector<int>* finalstates_in, map<char, int>* matrix_in)
//{
//	states = states_in;
//	finalstates = finalstates_in;
//	matrix = matrix_in;
//}
Dfa::~Dfa()
{
    // deleting null has no side-effect
    delete[] matrix;
}

int Dfa::move(int state, char ch)
{
	map<char, int>* m = &(matrix[state]);
	if (ch >= 'a' && ch <= 'z' || ch>='A'&&ch<='Z')
		ch = 'l';
	else if(ch >= '0'&&ch <= '9')
		ch = 'd';
	if (m->find(ch) != m->end())
		return m->at(ch);
	else
		return -1;
}
int Dfa::getInitial()
{
	return 0;
}