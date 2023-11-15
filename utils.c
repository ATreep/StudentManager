#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

struct students_chin *chin_head = NULL;
struct students_chin *chin_foot = NULL;
int chin_size = 0;

void set_lang_type(int is_eng)
{
     config -> is_english = is_eng;
}

char *getl(enum STRINGS string)
{
    return strings_arr[string][config -> is_english];
}

struct students_chin *query_student_by_id(char id[11])
{
    //There is not any data.
    if(chin_size == 0 || chin_head == NULL) return NULL;

    struct students_chin *pointer = chin_head;

    while(pointer != NULL)
    {
        if(strcmp(pointer -> now -> id,id) == 0)
            return pointer;

        pointer = pointer -> next;
    }

    //If the target Student Data cannot be queried, return NULL.
    return NULL;
}

/*
 * Name is not a unique parameter, so it must return a pointer array.
 * Using a struct to return an array and array size number at once.
 */
struct query_return_model query_student_by_name(char name[6])
{
    struct query_return_model return_model = {NULL,0};

    //There is not any data. Return an empty struct.
    if(chin_size == 0 || chin_head == NULL) return return_model;

    struct students_chin **results_array = (struct students_chin **)malloc(sizeof(struct students_chin *) * chin_size);

    struct students_chin *pointer = chin_head;

    while(pointer != NULL)
    {
        if(strcmp(pointer -> now -> name,name) == 0)
        {
            results_array[return_model.return_size++] = pointer;
        }
        pointer = pointer -> next;
    }

    if(return_model.return_size > 0)
        return_model.data_array = results_array;

    //If the target Student Data cannot be queried, return an empty struct.
    return return_model;
}
void save_data()
{
    FILE *file = fopen("StudentManager_Resources/Students.dat","wb+");

    if(chin_size == 0)
    {
        remove("StudentManager_Resources/Students.dat");
        return;
    }

    fputc(chin_size,file);

    struct student_model data_array[chin_size];
    struct students_chin *pointer = chin_head;
    for(int i = 0;i < chin_size;i++)
    {
        if(pointer == NULL)
        {
            break;
        }
        data_array[i] = *(pointer -> now);
        pointer = pointer -> next;
    }

    fwrite(data_array, sizeof(struct student_model),chin_size,file);
    fclose(file);
}


int get_file_size(char* filename)
{
    FILE *fp=fopen(filename,"r");
    if(!fp) return 0;
    fseek(fp,0L,SEEK_END);
    int size=ftell(fp);
    fclose(fp);

    return size;
}

void read_data()
{
    FILE *file = fopen("StudentManager_Resources/Students.dat","rb+");
    if(get_file_size("StudentManager_Resources/Students.dat") == 0) return;

    chin_size = fgetc(file);

    struct student_model *data_array = (struct student_model *)malloc(sizeof(struct student_model) * chin_size);
    struct students_chin *pointer;

    fread(data_array, sizeof(struct student_model),chin_size,file);

    fclose(file);

    chin_head = (struct students_chin *) malloc(sizeof(struct students_chin));
    chin_head -> now = &data_array[0];
    chin_head -> prev = NULL;
    chin_head -> next = NULL;
    pointer = chin_head;

    for(int i = 1;i < chin_size;i++)
    {
        struct students_chin *next = (struct students_chin *) malloc(sizeof(struct students_chin));
        next -> now = &data_array[i];
        next -> prev = pointer;
        next -> next = NULL;
        pointer -> next = next;
        pointer = next;
    }

    chin_foot = pointer;
}