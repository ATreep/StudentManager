#include "lang.h"
#include <string.h>
#ifndef STUDENT_MANAGER_UTILS_H
#define STUDENT_MANAGER_UTILS_H
extern struct config_model{
    int is_english; // "1" means using English for display language.
    char school_name[21];
} *config;

enum SEX_CLASSIFICATION{
    SEX_MALE,
    SEX_FEMALE,
    SEX_OTHER
};

struct date_model{
    int year;
    int month;
    int day;
};

struct student_model{
    char id[11]; //The length of Student ID is 10 characters.
    char name[11]; //Assume the length of the longest name is 10 characters or 5 Chinese words.
    enum SEX_CLASSIFICATION sex;
    int age;
    struct date_model birthday;
    char address[31];
    char telephone[12];
    char email[31];
    float grade;
};

struct students_chin{
    struct students_chin *prev;
    struct student_model *now;
    struct students_chin *next;
};

struct query_return_model{
    struct students_chin **data_array;
    int return_size;
};

extern struct students_chin *chin_head;
extern struct students_chin *chin_foot;
extern int chin_size;

char *getl(enum STRINGS); //Get the string of special language. (Used by UI text showing.)
struct students_chin *query_student_by_id(char[11]); //Query a data by School ID and return.
struct query_return_model query_student_by_name(char[6]); //Query the data through its(their) name and return.
void save_data(); //Save the linked list of student information to a file in disk (Students.dat).
void read_data(); //Read student information and save them as linked list in memory from Students.dat .
int get_file_size(char*); //Get the size of a normal file. If the file do not exist, return 0.

#endif
