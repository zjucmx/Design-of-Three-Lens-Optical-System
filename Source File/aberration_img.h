# ifndef ABERRATION_IMG_H
# define ABERRATION_IMG_H

# include "opt_main.h"
# include "svpng.h"
# include "file_read.h"
# include "part1.h"
# include "part2.h"
# include "part3.h"
# include "fonts_table.h"
# include "string.h"
# include "stdlib.h"

# define IMG_WIDTH 450
# define IMG_HEIGHT 270

// 主函数调用PlotAberration即可在当前文件目录下创建图片（可能文件路径还得在源文件修改）
// PlotAberration输入参数：实际物体object（用到物高和物距）；折射面链表face（不含出瞳，即不含index=0!如果使用文件读取函数返回head,应该输入head->next）；孔径光阑puple；
void PlotAberration( struct GAUSS object, struct FACE* face, struct PUPLE puple);
unsigned char* PlotCurve( int dimension, double** curve, unsigned int** color);
// focal_shift计算不同波长下的焦点位移，order3_det计算3阶行列式，dispersion计算不同波长的折射率；
double order3_det( double** mat );
double dispersion( double* n, double wave_len );
double focal_shift( double lf_, double wave_len, struct FACE* head, struct PUPLE puple);
void Write_char( double x0, double y0, char ch , unsigned char* img );
void Write_string( double x0, double y0, char* str, unsigned char* img );
void Plot_legend( unsigned char* img, char* title, char* x_axis, char* y_axis, char** mark, unsigned int** color, int dimension);
void Plot_scale( unsigned char* img, double min_x, double max_x, double min_y, double max_y);

# endif
