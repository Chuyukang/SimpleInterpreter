# 简单函数绘图语言解释器
------
本仓库为西电软件工程专业编译原理课程大作业
------
## 作业要求
实现一个指定简单函数绘图语言的解释器，将以该语言编写的文本解释为简单连线图形

## 文本示例
origin is (0,0);
scale is (10,10);
for T from 0 to 2*PI draw (cos(T), sin(T));

结果：将在以坐标(0,0)（左下角为原点，向右为x方向，向上为y方向）为圆心，半径为1做圆，并将结果x轴方向，y轴方向均放大10倍

## 词法记号

几个约定：

1. 文本中的空格字符默认被解释器跳过
2. 文本的大小写不敏感
3. T约定为语法的一个默认变量（语法默认没有提供定义变量的方式

关键字：ORIGIN, FOR, ROT,  IS, SCALE, TO, T, STEP, DRAW

| 词法记号名称 | 对应字符 |
| ------------ | -------- |
| COMMA        | ,        |
| SEMICO       | ;        |
| L_BRACKET    | (        |
| R_BRACKET    | )        |

## 语言文法
------
### 原始文法

Program -> $\varepsilon$ | Program Statement SEMICO

Statement -> OriginStatement | ScaleStatement | RotStatement | ForStatement

OriginStatement -> ORIGIN IS L_BRACKET Expression COMMA Expression R_BRACKET

ScaleStatement -> SCALE IS L_BRACKET Expression COMMA Expression R_BRACKET

RotStatement -> ROT IS Expression

ForStatement -> FOR T FROM Expression TO Expression STEP Expression 
							DRAW L_BRACKET Expression COMMA Expression R_BRACKET

Expression -> Expression PLUS Expression | Expression MINUS Expression
            | Expression MUL Expression | Expression DIV Expression
            | PLUS Expression | MINUS Expression
            | Expression POWER Expression
            | CONST_ID
            | T
            | FUNC L_BRACKET
            | L_BRACKET Expression R_BRACKET


### 转换过的文法
通过消除二义性，左递归，左因子，改写产生式为EBNF形式，便于指导后续递归下降分析器实现

Expression产生式消除二义性（通过引入优先级的方式

Expression -> Expression PLUS Term | Expression MINUS Term | Term

Term -> Term MUL Factor | Term DIV Factor | Factor

Factor -> PLUS Factor | MINUS Factor | Component

Component -> Atom POWER Component | Atom

Atom -> CONST_ID | T | FINC L_BRACKET Expression R_BRACKET 
        | L_BRACKET Expression R_BRACKET

### EBNF格式文法
消除左递归，左因子，并改写为EBNF范式

Program -> {Statement SEMICO}//右递归，可推出空字符，等价于循环

Expression -> Term {(PLUS | MINUS) Term}

Term -> Factor {(MUL | DIV) Factor}

Factor -> {(PLUS | MINUS)} Factor | Component //Factor可改为Component?

Component -> Atom [POWER Component] 

Atom -> CONST_ID | T | FINC L_BRACKET Expression R_BRACKET 
        | L_BRACKET Expression R_BRACKET

## 程序设计

包含dfa, Lexer, Parser, Painter四个类，分别完成确定有限状态，词法分析器，语法分析和语义分析器，win32绘图辅助类。同时，还有Token和Tree两个类，分别为词法符号和语法树两个数据结构。模块对外暴露接口如下：

```c++
class dfa
{
	vector<int> *states;//状态向量
	vector<int> *finalstates;//终态向量
	map<char, int> *matrix;//转换矩阵 数组 的指针
public:
	dfa();
	~dfa();
	int move(int state, char ch);//为驱动器提供转移查询
	bool isFinal(int state);//判断当前状态是否为终态
	int getInitial();//为驱动器提供初态查询
};

class Lexer
{
	fstream file;//待解析文件流
	dfa		*dfap;//加载的dfa对象的指针
public:
	Lexer(const wstring& fileName);
	Lexer(const string& fileName);
	~Lexer();
	Token* getToken();//提供获取下一个记号的接口
};

class Parser
{
	Lexer* lexerp;//调用的词法分析器对象指针
	Token* curToken;//当前未匹配的记号对象指针
	Painter* painter; //绘图所需对象的指针

	void Match(tokenType expect);//匹配单个指定记号
	//递归下降子程序，在结束语法分析时，调用Painter对象绘图
	void Statement();
	void OriginStatement();
	void RotStatement();
	void ScaleStatement();
	void ForStatement();
	void ColorStatement();
	
	Tree* Expression();
	Tree* Term();
	Tree* Factor();
	Tree* Component();
	Tree* Atom();
public:
	void Program();//外部调用的接口
	//构造函数，参数为待分析文件名和Painter对象指针
	Parser(const wstring& fileName,Painter *p);
	Parser(const string& fileName, Painter* p);
	~Parser();
	//Token* fetchToken();
	Token* updateToken();//更新当前记号的接口
};

class Painter
{
	HDC hdc;//win32设备上下文句柄
	double origin_x, origin_y;//坐标原点
	double scale_x, scale_y;//缩放比例
	double rot;//旋转角度
	unsigned red, green, blue;//绘图颜色
public:
	Painter(HDC h);
	//设置绘图环境上下文接口
	void setOrigin(double x, double y);//设置坐标原点，参数为绝对坐标系下坐标
	void setScale(double x, double y);
	void setRot(double angle);
	void setColor(unsigned red, unsigned green, unsigned blue);
	void setColor(double red, double green, double blue);
	//画出逻辑坐标下的点
	void draw(double x, double y);
};

class Token
{
	string	word;//记号文本
	int row;//记号所处位置
	funp funcp;//函数调用记号的指针
	double value;//字面量记号的值
public:
	//便于测试，将type设为public
	tokenType type;
	Token(tokenType t,string w,int r);//已知token类型时调用
	Token(string w, int r);//识别为标识符时调用
	double getValue();//获取记号的字面值
	int getRow();//获取记号所处行号
	funp getfuncp();//获取记号对应的函数指针
	friend ostream& operator<<(ostream& o,Token t); //重载输出操作符
};

class Tree
{
	tokenType opType;//语法树算符类型
	Tree* left, * right;//左右孩子节点指针
	double rvalue;//存储节点的右值
	double* lvalue;//存储节点的左值
	funp funcp;//函数调用节点的函数指针
public:
	Tree(Token *t);
	~Tree();
	Tree* addLeft(Tree *t);//为当前节点添加左孩子
	Tree* addRight(Tree* t);//为当前节点添加右孩子
	double getValue();//获得以该节点为根的表达式树的值
	//test
	void printTree(int level);//打印语法树
};

```

## 后续改进
1. 可采用Java Antlr工具辅助生成程序，使用Java Swing来实现跨平台绘图。
2. 可拓展变量使用方式，允许自定义变量。
3. 可通过正规式定义拓展词法分析器。
