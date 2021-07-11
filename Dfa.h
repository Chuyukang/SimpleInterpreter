#include<vector>
#include<map>

using namespace std;

namespace SimpleInterpreter{
    class Dfa
    {
        vector<int> *states;//状态向量
        vector<int> *finalstates;//终态向量
        map<char, int> *matrix;//转换矩阵 数组 的指针
    public:
        Dfa();
        ~Dfa();
        int move(int state, char ch);//为驱动器提供转移查询
        bool isFinal(int state);//判断当前状态是否为终态
        int getInitial();//为驱动器提供初态查询
    };
}

