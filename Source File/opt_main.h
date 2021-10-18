# ifndef OPT_MAIN_H
# define OPT_MAIN_H

# include <stdio.h>
# include <math.h>
# define INFINITY 10e10

struct FACE
{
    int index;	//折射面编号
    double r;	//折射面曲率半径
    double n[3];//折射率：nd, nC, nF
    double d;	//到下一面的距离
    double Vd; 	//阿贝常数
    struct FACE * next;	//指向下一节点
};

struct PUPLE
{
    int index;
    double l;
    double a;
};

struct GAUSS
{
    double l;//光线等效物距
    double u;//光线等效孔径角
    double y;//物高
};
# endif
