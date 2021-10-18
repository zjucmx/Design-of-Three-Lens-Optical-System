# include "part2.h"
# include <stdio.h>
# include <math.h>

void Iteration( double _n, double L, double U, double* L_, double* U_, struct FACE* face, int mode)
{
    struct FACE* p = face;
    double temp_L = L;
    double temp_U = U;
    double sinI, sinI_;

    while( p!= NULL)
    {   
        // printf("%d th face\n", p->index);
        // printf("L = %lf, U = %lf\n",  temp_L, temp_U*180/M_PI);
        if( temp_U == 0)            // 如果孔径角为0，是平行光，约定temp_L储存H
            sinI = temp_L / p->r ;  // 使用H计算sinI
        else
            sinI = (temp_L - p->r) / p->r * sin(temp_U);
        sinI_ = (_n / p->n[mode]) * sinI;
        _n = p->n[mode];
        temp_U = temp_U + asin(sinI) - asin(sinI_);
        if( temp_U == 0)            // 折射后孔径角为0，出射光为平行光，约定temp_L储存H
            temp_L = temp_L * sin( asin(sinI_) - asin(sinI) ); // 用入射光线计算H
        else
            temp_L = p->r + p->r * (sinI_ / sin(temp_U));
        if(p->next!=NULL)
        {
            if( temp_U == 0)
                temp_L = temp_L;    // 平行光，temp_L=H不改变
            else
                temp_L = temp_L - p->d;
            temp_U = temp_U;
        }
        // printf("I = %lf, I_ = %lf\n", 180*asin(sinI)/M_PI, 180*asin(sinI_)/M_PI);
        // printf("L_ = %lf, U_ = %lf\n\n", temp_L, temp_U*180/M_PI);
        p = p->next;
    }

    *L_ = temp_L;
    *U_ = temp_U;
    return ; 
}

void Initialize( double l_object, double y, double Kr, double Kw, struct PUPLE puple, double *L, double *U )
{
    if( fabs(l_object) == INFINITY )    // 物距为无穷远
    {
        *U = Kw * y;                    // y 参数代表视场角 W
        if(*U != 0)                     // 如果不是轴上点
            *L = puple.l + ( Kr * puple.a ) / tan( *U );
        else                            // 孔径角为0，约定L储存光束高度H
            *L = Kr*puple.a;
    }
    else                                // 物距非无穷远
    {
        if( puple.l - l_object != 0)    // 分母不为0，入瞳和物不重合
        {
            *U = atan( ( Kw*y - Kr*puple.a ) / ( puple.l - l_object ));
            if( *U != 0)                // 等效孔径角不为0
                *L = puple.l + (Kr*puple.a) / tan( *U );
            else                        // 有限远物且等效孔径角为0，约定L储存H，且此时为0
                *L = 0;
        }
        else                            // 入瞳和物重合，等效孔径角 pi/2
        {
            *U = 0.5*M_PI;
            *L = puple.l;
        }
            
    }
    return ;
}

