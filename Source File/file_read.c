# include "file_read.h"

struct FACE* ReadFile( void )
{
    struct FACE* head = NULL;
    struct FACE* temp_face = NULL;
    FILE *fp = NULL;
    char filename[40] = "./optical_cal/opt_system.txt";

    printf("open file: %s\n", filename);
    fp = fopen( filename, "r");
    if(fp==NULL)
    {
        printf("file open failed ! \n");
        printf("open fail errno = %d \n", errno);
        system("pause");
        exit(1);
    }

        // 读取第一面（孔径光阑）作为链表头，动态申请内存
    head = (struct FACE*)malloc(sizeof(struct FACE));
    fscanf( fp, "%d %lf %lf %lf %lf %lf %lf", &(head->index), &(head->r),
                                      &(head->n[0]), &(head->n[1]),
                                      &(head->n[2]), &(head->d),
                                      &(head->Vd));
    fgetc(fp);          // 吞掉一行末尾的回车
    head->next = NULL;
    temp_face = head;

    while( !feof(fp) )
    {   // 循环读取折射面的参数，动态申请内存
        temp_face->next = (struct FACE*)malloc(sizeof(struct FACE));
        fscanf( fp, "%d %lf %lf %lf %lf %lf %lf", &(temp_face->next->index),&(temp_face->next->r),
                      &(temp_face->next->n[0]), &(temp_face->next->n[1]), &(temp_face->next->n[2]),
                      &(temp_face->next->d),&(temp_face->next->Vd));
        fgetc(fp);     // 吞掉换行的回车
        temp_face->next->next = NULL;
        temp_face = temp_face->next;
    }

    fclose(fp);
    return head;
}
