#ifndef SIMPLEINTERPRETER_PAINTER_H
#define SIMPLEINTERPRETER_PAINTER_H

namespace SimpleInterpreter{
    // abstract class
    class Painter
    {
        double origin_x, origin_y;//坐标原点
        double scale_x, scale_y;//缩放比例
        double rot;//旋转角度
        unsigned red, green, blue;//绘图颜色
    public:
        //设置绘图环境上下文接口
        virtual void setOrigin(double x, double y)= 0;//设置坐标原点，参数为绝对坐标系下坐标
        virtual void setScale(double x, double y) = 0;
        virtual void setRot(double angle) = 0;
        virtual void setColor(unsigned red, unsigned green, unsigned blue) = 0;
        virtual void setColor(double red, double green, double blue) = 0;
        //画出逻辑坐标下的点
        virtual void draw(double x, double y) = 0;
    };

}

#endif