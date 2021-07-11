#pragma once
#include "Painter.h"
#include <Windows.h>

namespace SimpleInterpreter
{

    class Win32Painter : public Painter
    {
        HDC hdc;                   //win32设备上下文句柄
        double origin_x, origin_y; //坐标原点
        double scale_x, scale_y;   //缩放比例
        double rot;                //旋转角度
        unsigned red, green, blue; //绘图颜色
    public:
        Win32Painter(HDC h);
        //画出逻辑坐标下的点
        void draw(double x, double y) override;
        //设置绘图环境上下文接
        void setOrigin(double x, double y) override;
        void setColor(double red, double green, double blue) override;
        void setColor(unsigned int red, unsigned int green, unsigned int blue) override;
        void setRot(double angle) override;
        void setScale(double x, double y) override;
    };
}