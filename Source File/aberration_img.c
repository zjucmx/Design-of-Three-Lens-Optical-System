# include "aberration_img.h"
void PlotAberration( struct GAUSS object, struct FACE* head, struct PUPLE puple)
{
    unsigned char* img;
    unsigned plot_x, plot_y;
    unsigned int **color;
    char** mark;
    int i, j;
    double Kr, Kw, L, U, l_[3], y_[3];
    double **dL;
    double **dY;
    double **dx;
    double **df;
    double lf_[3], f_[3], lH_[3];
    double max_dL = 0;
    double max_dY = 0;
    double max_dx = 0;
    double max_df = 0;
    double wave_len;
    struct GAUSS ideal_image;
    struct GAUSS temp_image;

    FILE *fp1 = fopen(".\\photo\\sperical.png", "wb");
    FILE *fp2 = fopen(".\\photo\\distortion.png", "wb");
    FILE *fp3 = fopen(".\\photo\\field_curve.png", "wb");
    FILE *fp4 = fopen(".\\photo\\focla_shift.png", "wb");
    if(fp1==NULL || fp2==NULL || fp3==NULL || fp4==NULL)
    {
       printf("file open failed ! \n");
       printf("open fail errno = %d \n", errno);
       system("pause");
       exit(1);
    }

    for(i=0;i<3;i++)
        {
        // optical system parameter: lf_, f_, lH_
        Initialize( INFINITY, 0.0, 1, 1, puple, &L, &U);
        temp_image = GaussianImage( head->n[0], L, U, 0, head->next, i);
        lf_[i] = temp_image.l;
        f_[i] = head->r / temp_image.u;
        lH_[i] = lf_[i] - f_[i];

        // ideal image: l_, y_
        Kr = 1; Kw = 1;
        Initialize( object.l, 0, Kr, Kw, puple, &L, &U);
        ideal_image = GaussianImage(head->n[0], L, U, object.y, head->next, i);
        l_[i] = ideal_image.l;
        if( fabs(object.l) == INFINITY )
            // 无穷远物，不能用拉赫不变量求解y_
            y_[i] = tan( object.y ) * (lH_[i] - l_[i]);
        else
            y_[i] = ideal_image.y;
        }

        // calculate aberrations:
        dL = (double**)malloc( sizeof(double*) * IMG_HEIGHT );
        for(i=0; i<IMG_HEIGHT; i++)
            dL[i] = (double*)malloc( sizeof(double) * 3 );
        dY = (double**)malloc( sizeof(double*) * IMG_HEIGHT );
        for(i=0; i<IMG_HEIGHT; i++)
            dY[i] = (double*)malloc( sizeof(double) * 4 );
        dx = (double**)malloc( sizeof(double*) * IMG_HEIGHT );
        for(i=0; i<IMG_HEIGHT; i++)
            dx[i] = (double*)malloc( sizeof(double) * 3 );
        df = (double**)malloc( sizeof(double*) * IMG_HEIGHT );
        for(i=0; i<IMG_HEIGHT; i++)
            df[i] = (double*)malloc( sizeof(double) * 1 );


        for( j = 0; j < 3; j++) {
            Initialize( object.l, 0, 1, 1, puple, &L, &U);
            // 近轴条件下计算色球差
            temp_image = GaussianImage(head->n[0], L, U, object.y, head->next, j);
            dL[0][j] = temp_image.l - l_[0];
            // 视场角为0时无畸变
            dY[0][j] = 0;
            // i = 1开始循环，遍历所有height
            for( i = 1; i < IMG_HEIGHT; i++ )
            {
                // 步长 1/IMG_HEIGHT，不同孔径角的轴上点边缘光线，求解实际物距L
                Kr = 1.0*i / IMG_HEIGHT; Kw = 1;
                Initialize( object.l, 0, Kr, Kw, puple, &L, &U);
                Iteration(head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, j);
                // 与理想像距之差为球差
                dL[i][j] = temp_image.l - l_[0];
                // printf("dL[%d][%d] = %lf\t", i,j,dL[i][j]);

                // 步长 1/IMG_HEIGHT，不同视场角的轴外点中心光线，求解实际物高Y
                Kr = 0; Kw = 1.0*i / IMG_HEIGHT;
                Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
                Iteration(head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, j);
                // 像方视场角不为0，计算结果有效；有限远物且像方视场角为0，则像高必为0
                if(temp_image.u != 0)
                    temp_image.y = (temp_image.l - l_[0]) * tan(temp_image.u);
                else
                    temp_image.y = 0;

                // 有限远处和无限远处不同视场角对理想像高的影响不同，分开计算
                if( fabs(object.l)==INFINITY )
                    dY[i][j] = fabs(temp_image.y) - fabs( tan( object.y * Kw ) * (lH_[0] - l_[0]) );
                else
                    dY[i][j] = fabs(temp_image.y) - fabs(y_[0])*Kw;
                // printf("Y_ = %lf, y_ = %lf\n", fabs(temp_image.y), fabs());
                // printf("Kw = %lf, dY = %lf\n", Kw,dY[i][j]);

                if( fabs(dL[i][j]) > max_dL ) max_dL = fabs(dL[i][j]);
                if( fabs(dY[i][j]) > max_dY ) max_dY = fabs(dY[i][j]);

                if(j==2) dY[i][3] = dY[i][2]-dY[i][1];
                if( fabs(dY[i][3]) > max_dY ) max_dY = fabs(dY[i][3]);
            }
        }

        for( i=0; i<IMG_HEIGHT; i++)
        {
            Kr = 0; Kw = 1.0*i/IMG_HEIGHT;
            Initialize(object.l, object.y, Kr, Kw, puple, &L, &U);
            cal_part3(U, object.l, L, l_[0], &(dx[i][1]), &(dx[i][2]), &(dx[i][0]), head->next, 0);
            for( j=0; j<3; j++)
                if( fabs(dx[i][j]) > max_dx ) max_dx = fabs(dx[i][j]);
            // printf("%lf - %lf = %lf\n", dx[i][1], dx[i][2], dx[i][0]);
        }

        for( i=0; i<IMG_HEIGHT; i++ )
        {
            wave_len = (0.656-0.486) *i / IMG_HEIGHT + 0.486;
            df[i][0] = focal_shift( lf_[0], wave_len, head, puple);
            if( fabs(df[i][0]) > max_df ) max_df = fabs(df[i][0]);
            // printf("wave_len = %lf, focal_shift = %lf\n", wave_len, df[i][0]);
        }

        color = (unsigned int**)malloc(sizeof(unsigned int*)*4);
        for(i=0;i<4;i++) color[i] = (unsigned int)malloc(sizeof(unsigned int)*3);
        color[0][0] = 50;  // color 1
        color[0][1] = 50;
        color[0][2] = 50;
        color[1][0] = 255;  // color 2
        color[1][1] = 0;
        color[1][2] = 0;
        color[2][0] = 0;  // color 3
        color[2][1] = 0;
        color[2][2] = 255;
        color[3][0] = 150;  // color 4
        color[3][1] = 0;
        color[3][2] = 150;

        mark = (char**)malloc(sizeof(char*)*4);
        for(i=0;i<4;i++) mark[i] = (char*)malloc(sizeof(char)*30);


        img = PlotCurve( 3, dL, color);
        strcpy( mark[0], " 0.588 um");
        strcpy( mark[1], " 0.656 um");
        strcpy( mark[2], " 0.486 um");
        Plot_legend(img, "Spherical Aberration", "delta l' (mm)", "Kn", mark, color, 3);
        Plot_scale(img, -1.0*max_dL, 1.0*max_dL, 0.0, 1.0);
        svpng(fp1, IMG_WIDTH, IMG_HEIGHT, img, 0);
        fclose(fp1);

        img = PlotCurve( 4, dY, color);
        strcpy( mark[0], " 0.588 um real height");
        strcpy( mark[1], " 0.656 um real height");
        strcpy( mark[2], " 0.486 um real height");
        strcpy( mark[3], " lateral chromatic aberration");
        Plot_legend(img, "Distortion & Lateral Color", "y' (mm)", "Kn", mark, color, 4);
        Plot_scale(img, -1.0*max_dY, 1.0*max_dY, 0.0, 1.0);
        svpng(fp2, IMG_WIDTH, IMG_HEIGHT, img, 0);
        fclose(fp2);

        img = PlotCurve( 3, dx, color);
        strcpy( mark[0], " astigmatism");
        strcpy( mark[1], " sagittal");
        strcpy( mark[2], " tangential");
        Plot_legend(img, "Field Curvature & Astigmatism", "delta x' (mm)", "Kw", mark, color, 3);
        Plot_scale(img, -1.0*max_dx, 1.0*max_dx, 0.0, 1.0);
        svpng(fp3, IMG_WIDTH, IMG_HEIGHT, img, 0);
        fclose(fp3);

        img = PlotCurve( 1, df, &color[2]);
        strcpy( mark[0], " chromatic focal shift");
        Plot_legend(img, "Focal Shift", "delta l' (mm)", "wave length (um)", mark, &color[2], 1);
        Plot_scale(img, -1.0*max_df, 1.0*max_df, 0.486, 0.656);
        svpng(fp4, IMG_WIDTH, IMG_HEIGHT, img, 0);
        fclose(fp4);
        // printf("len = %d\n", strlen("hello!"));
        // printf("%d th char = %c\n", 2, "hello!"[2]);
}

unsigned char* PlotCurve( int dimension, double** curve, unsigned int** color)
{
        unsigned char* img = (unsigned char*)malloc(sizeof(unsigned char)* IMG_HEIGHT * IMG_WIDTH * 3);
        unsigned int plot_x, plot_y;
        double max_range=0;
        int i,j;
        char string[30];

        // find max
        max_range = 0;
        for(i=0; i<IMG_HEIGHT; i++){
            for(j=0; j<dimension; j++)
                if(fabs(curve[i][j])>max_range) max_range = fabs(curve[i][j]);
        }

        // plot background
        for (plot_y = 0; plot_y < IMG_HEIGHT; plot_y++){
            for (plot_x = 0; plot_x < IMG_WIDTH; plot_x++){
                img[plot_y*IMG_WIDTH*3 + plot_x*3 + 0] = (unsigned char)255;
                img[plot_y*IMG_WIDTH*3 + plot_x*3 + 1] = (unsigned char)255;
                img[plot_y*IMG_WIDTH*3 + plot_x*3 + 2] = (unsigned char)255;
            }
        }

        // plot curve
        for( j=0; j<dimension; j++ ){
            for (plot_y = 0; plot_y < IMG_HEIGHT; plot_y++){
            plot_x = round( (curve[ IMG_HEIGHT-1 - plot_y][j] / max_range + 1)/2 * IMG_WIDTH );
            img[ plot_y*IMG_WIDTH*3 + plot_x*3 + 0 ] = color[j][0];
            img[ plot_y*IMG_WIDTH*3 + plot_x*3 + 1 ] = color[j][1];
            img[ plot_y*IMG_WIDTH*3 + plot_x*3 + 2 ] = color[j][2];
            }
        }

        // plot cordinate
        for( i=0; i<IMG_WIDTH; i++){
            // x axis
            img[ (IMG_HEIGHT-1)*IMG_WIDTH*3 + i*3 + 0 ] = 0;
            img[ (IMG_HEIGHT-1)*IMG_WIDTH*3 + i*3 + 1 ] = 0;
            img[ (IMG_HEIGHT-1)*IMG_WIDTH*3 + i*3 + 2 ] = 0;
            // 刻度线
            if( i%(IMG_WIDTH/10)==0 )
            {
                for( j=0; j<IMG_HEIGHT/60; j++){
                img[ (IMG_HEIGHT-1-j)*IMG_WIDTH*3 + i*3 + 0 ] = 0;
                img[ (IMG_HEIGHT-1-j)*IMG_WIDTH*3 + i*3 + 1 ] = 0;
                img[ (IMG_HEIGHT-1-j)*IMG_WIDTH*3 + i*3 + 2 ] = 0;
                }
                // gcvt(1.0*(-IMG_WIDTH/2+i)/IMG_WIDTH, 4, string);
                // Write_string( 1.0*i/IMG_WIDTH, 1.0 - 1.0*(IMG_HEIGHT/60+FONTS_HEIGHT)/IMG_HEIGHT, string, img);
            }
        }
        for( i=0; i<IMG_HEIGHT; i++){
            // y axis
            img[ i*IMG_WIDTH*3 + IMG_WIDTH/2*3 + 0 ] = 0;
            img[ i*IMG_WIDTH*3 + IMG_WIDTH/2*3 + 1 ] = 0;
            img[ i*IMG_WIDTH*3 + IMG_WIDTH/2*3 + 2 ] = 0;
            // 刻度线
            if( i%(IMG_HEIGHT/10)==0 )
            {
                for( j=0; j<IMG_HEIGHT/30; j++){
                img[ i*IMG_WIDTH*3 + (IMG_WIDTH/2 + j - IMG_HEIGHT/60)*3 + 0 ] = 0;
                img[ i*IMG_WIDTH*3 + (IMG_WIDTH/2 + j - IMG_HEIGHT/60)*3 + 1 ] = 0;
                img[ i*IMG_WIDTH*3 + (IMG_WIDTH/2 + j - IMG_HEIGHT/60)*3 + 2 ] = 0;
                }
                // gcvt(1.0*(IMG_HEIGHT-i)/IMG_HEIGHT, 4, string);
                // Write_string( 1.0*(IMG_WIDTH/2 + IMG_HEIGHT/60) / IMG_WIDTH, 1.0*i/IMG_HEIGHT, string, img);
            }
        }

        return img;
}

double order3_det( double** mat )
{
    double ans;
    ans = mat[0][0] * ( mat[1][1]*mat[2][2] - mat[2][1]*mat[1][2] ) \
        - mat[1][0] * ( mat[0][1]*mat[2][2] - mat[2][1]*mat[0][2] ) \
        + mat[2][0] * ( mat[0][1]*mat[1][2] - mat[1][1]*mat[0][2] ) ;
    return ans;
}

double dispersion( double* n, double wave_len )
{
    int i,j;
    double a,b,c;
    double lambda[3] = { 0.588, 0.656, 0.486 }; // d光，C光，F光
    double **D, **D1, **D2, **D3;

    D = (double**)malloc(sizeof(double*)*3);
    for( i=0;i<3;i++ ) D[i] = (double*)malloc(sizeof(double)*3);
    for( i=0;i<3;i++ )
    {
        D[i][0] = 1; D[i][1] = pow(lambda[i], -2); D[i][2] = pow(lambda[i], -4);
    }

    D1 = (double**)malloc(sizeof(double*)*3);
    for( i=0;i<3;i++ ) D1[i] = (double*)malloc(sizeof(double)*3);
    for( i=0;i<3;i++ )
    {
        D1[i][0] = n[i]; D1[i][1] = pow(lambda[i], -2); D1[i][2] = pow(lambda[i], -4);
    }

    D2 = (double**)malloc(sizeof(double*)*3);
    for( i=0;i<3;i++ ) D2[i] = (double*)malloc(sizeof(double)*3);
    for( i=0;i<3;i++ )
    {
        D2[i][0] = 1; D2[i][1] = n[i]; D2[i][2] = pow(lambda[i], -4);
    }

    D3 = (double**)malloc(sizeof(double*)*3);
    for( i=0;i<3;i++ ) D3[i] = (double*)malloc(sizeof(double)*3);
    for( i=0;i<3;i++ )
    {
        D3[i][0] = 1; D3[i][1] = pow(lambda[i], -2); D3[i][2] = n[i];
    }

    a = order3_det(D1) / order3_det(D);
    b = order3_det(D2) / order3_det(D);
    c = order3_det(D3) / order3_det(D);

    return (double)( a + b*pow(wave_len, -2) + c*pow(wave_len, -4) );
}

double focal_shift( double lf_, double wave_len, struct FACE* head, struct PUPLE puple)
{
    struct GAUSS temp_image;
    struct FACE* pesudo_head = NULL;
    struct FACE* p = head;
    struct FACE* pesudo_p = NULL;
    double L, U;

    pesudo_head = (struct FACE*)malloc(sizeof(struct FACE));
    pesudo_p = pesudo_head;
    pesudo_p->index = p->index;
    pesudo_p->r     = p->r;
    pesudo_p->d     = p->d;
    pesudo_p->next  = NULL;
    pesudo_p->n[0] = p->n[0];
    pesudo_p->n[1] = p->n[1];
    pesudo_p->n[2] = p->n[2];
    p = p->next;

    while( p != NULL )
    {
        pesudo_p->next = (struct FACE*)malloc(sizeof(struct FACE));
        pesudo_p = pesudo_p->next;

        pesudo_p->index = p->index;
        pesudo_p->r     = p->r;
        pesudo_p->d     = p->d;
        pesudo_p->next  = NULL;
        pesudo_p->n[0]  = dispersion(p->n, wave_len);
        pesudo_p->n[1] = pesudo_p->n[0];
        pesudo_p->n[2] = pesudo_p->n[0];
        p = p->next;
    }

    Initialize( INFINITY, 0.0, 1, 1, puple, &L, &U);
    temp_image = GaussianImage( head->n[0], L, U, 0, pesudo_head->next, 0);

    return temp_image.l - lf_;
}

void Write_char( double x0, double y0, char ch , unsigned char* img )
{
    int pose;
    int x,y;        // x0, y0 < 1 是定位在图中的相对位置，x, y 是点阵的相对偏移量
    int plot_x, plot_y;

    pose = ch - ' ';
    if( pose>95 || pose<0 )
    {
        printf("invalid character!\n");
        return ;
    }
    for( x=0; x<FONTS_WIDTH; x++ )
    {
        for( y=0; y<FONTS_HEIGHT; y++ )
        {
            if(Fonts[ pose*5 + x ] & (0x01<<y))
            {
                plot_x = round(x0*IMG_WIDTH) + x; plot_y = round(y0*IMG_HEIGHT) + y;
                img[ plot_y*IMG_WIDTH*3 + plot_x*3 + 0 ] = 0;
                img[ plot_y*IMG_WIDTH*3 + plot_x*3 + 1 ] = 0;
                img[ plot_y*IMG_WIDTH*3 + plot_x*3 + 2 ] = 0;
            }
        }
    }
}

void Write_string( double x0, double y0, char* str, unsigned char* img )
{
    int len = strlen(str);
    int i;
    for( i=0; i<len; i++ )
    {
        Write_char( x0 + 1.0*i*(FONTS_WIDTH+1)/IMG_WIDTH, y0, str[i], img );
    }
}

void Plot_legend( unsigned char* img, char* title, char* x_axis, char* y_axis, char** mark, unsigned int** color, int dimension)
{
    double x0 = 0.05;
    double y0 = 0.7;
    int line_len = 6*FONTS_WIDTH;
    int index;
    char buf[100];
    int i,j;

    Write_string( x0, y0, title, img);
    Write_string( x0, y0+1.5*(FONTS_HEIGHT+1)/IMG_HEIGHT, "x axis:", img);
    Write_string( x0+1.0*(2+strlen("x axis:"))*FONTS_WIDTH/IMG_WIDTH, y0+(1.5*FONTS_HEIGHT+1)/IMG_HEIGHT, \
                  x_axis, img);
    Write_string( x0, y0+3.0*(FONTS_HEIGHT+1)/IMG_HEIGHT, "y axis:", img);
    Write_string( x0+1.0*(2+strlen("y axis:"))*FONTS_WIDTH/IMG_WIDTH, y0+3.0*(FONTS_HEIGHT+1)/IMG_HEIGHT, \
                  y_axis, img);
    for( i=0; i<dimension; i++)
    {
        // 画线作为图例;
        for( j=0; j<line_len; j++)
        {
            index = ( round(y0*IMG_HEIGHT)+(i+5.0)*(FONTS_HEIGHT+1) )*IMG_WIDTH*3 + (round(x0*IMG_WIDTH)+j)*3 ;
            img[ index + 0 ] = color[i][0];
            img[ index + 1 ] = color[i][1];
            img[ index + 2 ] = color[i][2];
        }
        Write_string( x0+1.0*line_len/IMG_WIDTH, y0+(i+4.5)*(FONTS_HEIGHT+1)/IMG_HEIGHT, \
                    mark[i], img);
    }
}

void Plot_scale( unsigned char* img, double min_x, double max_x, double min_y, double max_y)
{
    char string[30];
    int i,j;
    // x axis
    // min x
    gcvt( min_x, 4, string );
    Write_string( 0.0, 1.0 - 1.0*(IMG_HEIGHT/60+FONTS_HEIGHT)/IMG_HEIGHT, string, img);
    // max x
    gcvt( max_x, 4, string );
    Write_string( 1.0 - 1.0*strlen(string)*(FONTS_WIDTH+1)/IMG_WIDTH, 1.0 - 1.0*(IMG_HEIGHT/60+FONTS_HEIGHT)/IMG_HEIGHT, string, img);
    // y axis
    for( i=0; i<IMG_HEIGHT; i++){
        if( i%(IMG_HEIGHT/10)==0 )
        {
            gcvt((max_y - min_y) * 1.0 * (IMG_HEIGHT-i) / IMG_HEIGHT + min_y, 4, string);
            Write_string( 1.0*(IMG_WIDTH/2 + IMG_HEIGHT/60) / IMG_WIDTH, 1.0*i/IMG_HEIGHT, string, img);
        }
    }
}
