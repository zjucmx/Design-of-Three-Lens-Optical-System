#include "opt_main.h"
# ifndef PART2_H
# define PART2_H

void Initialize( double l_object, double y, double Kr, double Kw, struct PUPLE, double *L, double *U );
void Iteration( double _n, double L, double U, double* L_, double* U_, struct FACE* face, int mode);
# endif
