# ifndef OPT_MAIN_H
# define OPT_MAIN_H

# include <stdio.h>
# include <math.h>
# define INFINITY 10e10

struct FACE
{
    int index;	//��������
    double r;	//���������ʰ뾶
    double n[3];//�����ʣ�nd, nC, nF
    double d;	//����һ��ľ���
    double Vd; 	//��������
    struct FACE * next;	//ָ����һ�ڵ�
};

struct PUPLE
{
    int index;
    double l;
    double a;
};

struct GAUSS
{
    double l;//���ߵ�Ч���
    double u;//���ߵ�Ч�׾���
    double y;//���
};
# endif
