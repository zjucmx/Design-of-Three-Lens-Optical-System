# include "part1.h"

struct GAUSS GaussianImage(double _n, double l, double u, double y, struct FACE *ptr, int mode) 
{
	struct GAUSS x;
    struct FACE* p = ptr;
    double l_, u_, i, i_, temp;
    double h = l;       
	double init_u = u;
	
	while( p!=NULL )
	{
        // printf("%d th face\n", p->index);
        // printf("l = %lf, u = %lf\n", l, u);
		if (u == 0)
			i = h / p->r;
		else
			i = (l - p->r) / p->r * u;

        i_ = (_n / p->n[mode]) * i;
        _n = p->n[mode];
		u_ = i + u - i_;
		if (u_ == 0)
			l_ = INFINITY;
		else
			l_ = p->r + p->r * i_ / u_;
        // printf("l_ = %lf, u_ = %lf\n\n", l_, u_);
		if(p->next != NULL)
		{
			h = h - p->d * tan(u_);
			l = l_ - p->d;
			u = u_;
		}
		else
		{
			h = h;
			l = l_;
			u = u_;
		}
        p = p->next;
	}
	x.l = l;
	x.u = u;
    if( init_u != 0)
	    x.y = y*init_u/u;
    else 
        x.y = INFINITY;
	return x;
}
