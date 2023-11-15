#include <stdio.h>
#include <io.h>
#include "utils.h"
#include <stdlib.h>
#include <windows.h>

#define ITEMS_NUMBER_IN_ONE_PAGE 5

void init_config(); //Load config.dat file to memory.
void show_menu(); //Print main menu.

//Print one data simple block (include School ID, Name, Sex, Age, Birthday and Grade).
void print_student_data_block(struct student_model *,int);

void show_page(int); //Print the data of a special page number (at most 5 data simple block).
void start_inputting(); //Start to input new data.
void edit_data(struct students_chin *); //Edit an existed data.
void query(); //Query the data through its(their) name.
void delete_in_batch(); //Delete data in batch.
void browse_in_sort(); //To view data in School ID sort or grades sort.

struct config_model *config;
int now_page = 1;

void show_app_title()
{
    printf("\033[34;1m- %s -\033[0m\n\n", getl(APP_NAME));
}

void show_app_title_with_school()
{
    printf("\033[34;1m- %s | %s -\033[0m\n\n",config->school_name,getl(APP_NAME));
}

int main()
{
    init_config();

    show_menu();

    return 0;
}

void print_student_data_block(struct student_model *model,int is_show_header)
{
    char sex_text[7];
    switch(model -> sex)
    {
        case SEX_MALE:
            strcpy(sex_text,getl(MALE));
            break;
        case SEX_FEMALE:
            strcpy(sex_text,getl(FEMALE));
            break;
        case SEX_OTHER:
            strcpy(sex_text,getl(OTHER));
            break;
    }
    if(is_show_header)
    {
        printf("%10s / %10s / %6s / %3s / %10s / %6s\n",
               getl(HEADER_SCHOOL_ID),getl(HEADER_NAME),getl(HEADER_SEX),getl(HEADER_AGE),getl(HEADER_BIRTHDAY),getl(HEADER_GRADE));
    }
    printf("%10s / %10s / %6s / %3d / %4d.%02d.%02d / %6.2f\n",
           model -> id,model -> name,sex_text,model -> age,
           model -> birthday.year,model -> birthday.month,model -> birthday.day,
           model -> grade);
}

void show_page(int page)
{
    int page_size = chin_size / ITEMS_NUMBER_IN_ONE_PAGE + (chin_size % ITEMS_NUMBER_IN_ONE_PAGE > 0 ? 1 : 0);
    if(page > page_size)
    {
        page = page_size;
    }
    if(page < 1)
    {
        page = 1;
    }
    struct students_chin *pointer = chin_head;

    //Make the pointer point to the first item of target page.
    for(int i = 0;i < (page - 1) * ITEMS_NUMBER_IN_ONE_PAGE;i++)
    {
        pointer = pointer -> next;
    }

    printf("----------[%s %d / %d | %s %d]----------\n", getl(PAGE_INDEX),page,page_size, getl(COUNT_NUM),chin_size);

    for(int i = 0;i < ITEMS_NUMBER_IN_ONE_PAGE;i++)
    {
        if(pointer == NULL) break;
        print_student_data_block(pointer -> now,!i);
        pointer = pointer -> next;
    }

    now_page = page;
}

void start_inputting()
{
    struct students_chin *head = NULL;
    struct students_chin *foot = NULL;

    if(chin_size > 0) {
        printf("%s\n", getl(PREPARE_FOR_INPUTTING_DATA));
        printf("\n%s\n\n%s\n", getl(WHERE_TO_INSERT_INPUTTING_DATA), getl(INPUTTING_DATA_RULE));

        while (1) {
            char id[11];
            scanf("%s", id);
            getchar();
            if (strcmp("#", id) == 0) {
                head = NULL;
                foot = chin_head;
            } else if (strcmp("-", id) == 0) {
                head = chin_foot;
                foot = NULL;
            } else {
                if ((head = query_student_by_id(id)) == NULL) {
                    printf("\033[31;1m%s\033[0m\n", getl(PLEASE_TYPE_RIGHT_ID));
                    continue;
                } else {
                    foot = head->next;
                }

                printf("\n\033[32;1m%s\n", getl(INSERT_DATA_BELOW_THIS_DATA));
                print_student_data_block(head->now,1);
                printf("\033[0m\n");
                system("pause");
            }

            break;
        }
    }

    system("cls");
    show_app_title_with_school();
    printf("%s\n\n", getl(START_INPUTTING));


    struct students_chin *first = (struct students_chin *)malloc(sizeof(struct students_chin));
    struct students_chin *pointer = first;
    pointer -> prev = head;

    int i = 0;

    for(i = 0;;i++)
    {
        printf("\033[34m#%d\033[0m",i + 1);
        struct student_model *model_pointer = (struct student_model *)malloc(sizeof(struct student_model));

        while(1)
        {
            printf("\n%s ", getl(INPUT_SCHOOL_ID));
            scanf("%s",model_pointer -> id);
            getchar();

            if(query_student_by_id(model_pointer -> id) != NULL)
            {
                printf("\n\033[31;1m%s\033[0m\n", getl(PLEASE_TYPE_RIGHT_ID));
                continue;
            }
            break;
        }

        printf("\n%s ", getl(INPUT_NAME));
        scanf("%s",model_pointer -> name);
        getchar();
        printf("\n%s ", getl(INPUT_SEX));
        scanf("%d",&model_pointer -> sex);
        getchar();
        printf("\n%s ", getl(INPUT_AGE));
        scanf("%d",&model_pointer -> age);
        getchar();
        printf("\n%s ", getl(INPUT_BIRTHDAY));
        scanf("%4d.%2d.%2d",&model_pointer -> birthday.year,
              &model_pointer -> birthday.month,&model_pointer -> birthday.day);
        getchar();
        printf("\n%s ", getl(INPUT_ADDRESS));
        scanf("%s",model_pointer -> address);
        getchar();
        printf("\n%s ", getl(INPUT_TELEPHONE));
        scanf("%s",model_pointer -> telephone);
        getchar();
        printf("\n%s ", getl(INPUT_EMAIL));
        scanf("%s",model_pointer -> email);
        getchar();

        while(1)
        {
            printf("\n%s ", getl(INPUT_GRADE));
            scanf("%f",&model_pointer -> grade);
            getchar();
            if(model_pointer -> grade < 0 || model_pointer -> grade > 100)
            {
                printf("\n\033[31;1m%s\033[0m\n", getl(INVALID_RETRY));
                continue;
            }
            break;
        }

        pointer -> now = model_pointer;

        printf("\033[33m%s\033[0m",getl(DO_YOU_WANT_T0_CONTINUE));
        char choice;
        choice = getchar();
        getchar();
        if(choice == 'Y' || choice == 'y')
        {
            pointer -> next = (struct students_chin *)malloc(sizeof(struct students_chin));
            struct students_chin *prev_temp = pointer;
            pointer = pointer -> next;
            pointer -> prev = prev_temp;
            continue;
        }
        else
        {
            break;
        }
    }

    pointer -> next = foot;

    if(head == NULL)
        chin_head = first;
    else
        head -> next = first;

    if(foot == NULL)
        chin_foot = pointer;
    else
        foot -> prev = pointer;

    chin_size += i + 1;

    show_menu();
}

void edit_data(struct students_chin *data)
{
    system("cls");
    show_app_title_with_school();
    printf("%s\n\n", getl(EDIT_DATA));

    printf("[I] %s %s\n", getl(INPUT_SCHOOL_ID),data -> now -> id);
    printf("[N] %s %s\n", getl(INPUT_NAME),data -> now -> name);
    printf("[S] %s %d\n", getl(INPUT_SEX),data -> now -> sex);
    printf("[A] %s %d\n", getl(INPUT_AGE),data -> now -> age);
    printf("[B] %s %4d.%02d.%02d\n", getl(INPUT_BIRTHDAY),data -> now -> birthday.year,
           data -> now -> birthday.month,data -> now -> birthday.day);
    printf("[L] %s %s\n", getl(INPUT_ADDRESS),data -> now -> address);
    printf("[T] %s %s\n", getl(INPUT_TELEPHONE),data -> now -> telephone);
    printf("[E] %s %s\n", getl(INPUT_EMAIL),data -> now -> email);
    printf("[G] %s %6.2f\n", getl(INPUT_GRADE),data -> now -> grade);
    printf("\033[41m[D] %s\033[0m\n", getl(DELETE_DATA));
    printf("\n%s ", getl(PLEASE_INPUT_COMMAND_OR_EDIT));
    char code = getchar();
    getchar(); //Get Enter character read.

    switch (code) {
        case 'I':
        case 'i':
            while(1)
            {
                char id[11];
                printf("\n%s ", getl(INPUT_SCHOOL_ID));
                scanf("%s",id);
                getchar();

                if(query_student_by_id(id) != NULL)
                {
                    printf("\n\033[31;1m%s\033[0m\n", getl(PLEASE_TYPE_RIGHT_ID));
                    continue;
                }

                memset(data -> now -> id,0,11);
                strcpy(data -> now -> id,id);
                break;
            }
            edit_data(data);
            break;
        case 'N':
        case 'n':
            printf("\n%s ", getl(INPUT_NAME));
            scanf("%s",data -> now -> name);
            getchar();
            edit_data(data);
            break;
        case 'S':
        case 's':
            printf("\n%s ", getl(INPUT_SEX));
            scanf("%d",&data -> now -> sex);
            getchar();
            edit_data(data);
            break;
        case 'A':
        case 'a':
            printf("\n%s ", getl(INPUT_AGE));
            scanf("%d",&data -> now -> age);
            getchar();
            edit_data(data);
            break;
        case 'B':
        case 'b':
            printf("\n%s ", getl(INPUT_BIRTHDAY));
            scanf("%4d.%2d.%2d",&data -> now -> birthday.year,
                  &data -> now -> birthday.month,&data -> now -> birthday.day);
            getchar();
            edit_data(data);
            break;
        case 'L':
        case 'l':
            printf("\n%s ", getl(INPUT_ADDRESS));
            scanf("%s",data -> now -> address);
            getchar();
            edit_data(data);
            break;
        case 'T':
        case 't':
            printf("\n%s ", getl(INPUT_TELEPHONE));
            scanf("%s",data -> now -> telephone);
            getchar();
            edit_data(data);
            break;
        case 'E':
        case 'e':
            printf("\n%s ", getl(INPUT_EMAIL));
            scanf("%s",data -> now -> email);
            getchar();
            edit_data(data);
            break;
        case 'G':
        case 'g':
            while(1)
            {
                printf("\n%s ", getl(INPUT_GRADE));
                scanf("%f",&data -> now -> grade);
                getchar();
                if(data -> now -> grade < 0 || data -> now -> grade > 100)
                {
                    printf("\n\033[31;1m%s\033[0m\n", getl(INVALID_RETRY));
                    continue;
                }
                break;
            }
            edit_data(data);
            break;
        case 'D':
        case 'd':
            printf("\033[31;1m%s\033[0m ", getl(DELETE_DATA_PROMPT));
            char prompt = getchar();
            if(prompt == 'Y' || prompt == 'y')
            {
                if(data -> prev != NULL)
                    data -> prev -> next = data -> next;
                else
                {
                    chin_head = data -> next;
                    if(chin_head != NULL)
                        chin_head -> prev = NULL;
                }

                if(data -> next != NULL)
                    data -> next -> prev = data -> prev;
                else
                {
                    chin_foot = data -> prev;
                    if(chin_foot != NULL)
                        chin_foot -> next = NULL;
                }
                chin_size--;
                show_menu();
            }
            else
            {
                edit_data(data);
            }
            break;
        default:
            show_menu();
            break;
    }
}

void query()
{
    system("cls");
    show_app_title_with_school();
    printf("%s\n\n", getl(QUERY));

    printf("%s", getl(PLEASE_INPUT_A_NAME_TO_QUERY));
    char name[11];
    gets(name);
    struct query_return_model results = query_student_by_name(name);
    if(results.return_size == 0)
    {
        printf("\n\033[31m%s\033[0m\n", getl(NO_QUERYING_RESULT));
    }
    else
    {
        printf("\n");
        print_student_data_block(results.data_array[0] -> now,1);
        for(int i = 1;i < results.return_size;i++)
        {
            print_student_data_block(results.data_array[i] -> now,0);
        }
    }
    printf("\033[33m%s\033[0m",getl(DO_YOU_WANT_T0_CONTINUE));
    char choice;
    choice = getchar();
    getchar();
    if(choice == 'Y' || choice == 'y')
    {
        query();
    }
    else
    {
        show_menu();
    }
}

void delete_in_batch()
{
    system("cls");
    show_app_title_with_school();
    printf("%s\n\n", getl(DELETE_IN_BATCH));

    printf("%s ", getl(PLEASE_INPUT_ID_TO_DELETE));
    char id[11];
    gets(id);

    struct students_chin *target = query_student_by_id(id);
    if(target == NULL)
    {
        printf("\n\033[31;1m%s\033[0m\n", getl(PLEASE_TYPE_RIGHT_ID));
    }
    else
    {
        if(target -> prev != NULL)
            target -> prev -> next = target -> next;
        else
        {
            chin_head = target -> next;
            if(chin_head != NULL)
                chin_head -> prev = NULL;
        }

        if(target -> next != NULL)
            target -> next -> prev = target -> prev;
        else
        {
            chin_foot = target -> prev;
            if(chin_foot != NULL)
                chin_foot -> next = NULL;
        }
        chin_size--;
        printf("\n\033[32;1m%s\033[0m\n", getl(DELETE_DONE));
    }

    printf("\033[33m%s\033[0m",getl(DO_YOU_WANT_T0_CONTINUE));
    char choice;
    choice = getchar();
    getchar();
    if(choice == 'Y' || choice == 'y')
    {
        delete_in_batch();
    }
    else
    {
        show_menu();
    }
}

void browse_in_sort()
{
    system("cls");
    show_app_title_with_school();
    printf("%s\n\n", getl(BROWSE_IN_SORT));

    if(chin_size == 0)
    {
        printf("%s\n",getl(NO_ANY_DATA));
        system("pause");
        show_menu();
        return;
    }

    printf("[1] %s\n[2] %s\n", getl(SORT_BY_SCHOOL_ID), getl(SORT_BY_GRADE));
    printf("%s ", getl(PLEASE_CHOOSE_A_SORT_TYPE));

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

    char choice;
    choice = getchar();
    getchar();

    if(choice == '1')
    {
        for(int s = chin_size - 1;s > 0;s--)
        {
            for(int i = 0;i < s;i++)
            {
                if(strcmp(data_array[i].id,data_array[i + 1].id) > 0)
                {
                    struct student_model temp = data_array[i];
                    data_array[i] = data_array[i + 1];
                    data_array[i + 1] = temp;
                }
            }
        }
    }
    else if(choice == '2')
    {
        for(int s = chin_size - 1;s > 0;s--)
        {
            for(int i = 0;i < s;i++)
            {
                if(data_array[i].grade < data_array[i + 1].grade)
                {
                    struct student_model temp = data_array[i];
                    data_array[i] = data_array[i + 1];
                    data_array[i + 1] = temp;
                }
            }
        }
    }
    else
    {
        show_menu();
        return;
    }

    system("cls");
    show_app_title_with_school();
    printf("%s\n\n", getl(BROWSE_IN_SORT));
    printf("\033[32;1m%s\033[0m\n\n", getl(choice == '1' ? SORT_BY_SCHOOL_ID : SORT_BY_GRADE));

    print_student_data_block(&data_array[0],1);
    for(int i = 1;i < chin_size;i++)
    {
        print_student_data_block(&data_array[i],0);
    }

    system("pause");
    show_menu();
}

void show_menu()
{
    system("cls");
    show_app_title_with_school();

    printf("\033[32;1m|| %s ||\033[0m\n\n", getl(FUNCTION_MENU));
    printf("[I] %-15s | [B] %-15s | [N] %-15s\n", getl(START_INPUTTING),
           getl(BACK_PAGE), getl(NEXT_PAGE));
    printf("[Q] %-15s | [D] %-15s | [S] %-15s\n", getl(QUERY), getl(DELETE_IN_BATCH),
           getl(BROWSE_IN_SORT));
    printf("[R] %-15s | [H] %-15s | [E] %-15s\n\n",getl(RESET_CONFIG), getl(HELP), getl(EXIT));

    printf("\033[32;1m|| %s ||\033[0m\n\n", getl(DATA_VIEW));
    if(chin_size == 0)
    {
        printf("\033[33m%s\033[0m\n", getl(NO_DATA_INF));
    }
    else
    {
        show_page(now_page);
    }

    printf("\n%s ",getl(PLEASE_INPUT_COMMAND));
    char code[11];
    gets(code);

    printf("\n");

    switch (code[0]) {
        case 'I':
        case 'i':
            start_inputting();
            break;
        case 'B':
        case 'b':
            now_page--;
            show_menu();
            break;
        case 'N':
        case 'n':
            now_page++;
            show_menu();
            break;
        case 'Q':
        case 'q':
            query();
            break;
        case 'D':
        case 'd':
            delete_in_batch();
            break;
        case 'S':
        case 's':
            browse_in_sort();
            break;
        case 'R':
        case 'r':
            remove("StudentManager_Resources/Config.dat");
            system("pause");
            system("cls");
            init_config();
            show_menu();
            break;
        case 'E':
        case 'e':
            save_data();
            exit(0);
        default: {
            struct students_chin *target = query_student_by_id(code);
            if(target == NULL)
                show_menu();
            else
                edit_data(target);
            break;
        }
    }
}

/*
 * Load config from disk config file.
 */
void init_config()
{
    char config_folder[] = "StudentManager_Resources";
    char config_dat[] = "StudentManager_Resources/Config.dat";
    mkdir(config_folder);

    config = (struct config_model *)malloc(sizeof(struct config_model));
    if(get_file_size(config_dat) > 0)
    {
        FILE *config_file = fopen(config_dat,"rb+");
        fread(config, sizeof(struct config_model),1,config_file);
        fclose(config_file);
    }
    else
    {
        //Guide user to configure Student Manager System.
        system("cls");
        printf("\033[34;1m- STUDENT MANAGER SYSTEM · 学生信息管理系统 -\033[0m\n\n"); //Use ANSI Escape Code to control text color.
        printf("> 简体中文 [0]\n");
        printf("> English [1]\n");
        printf("\nPlease type a language number: ");
        scanf("%d",&(config -> is_english));

        system("cls");
        show_app_title();
        printf("%s\n\n", getl(FIRST_USING_CONFIGURATIONS));
        printf("%s ", getl(PLEASE_INPUT_SCHOOL_NAME));
        scanf("%s",config -> school_name);

        system("cls");
        show_app_title();
        printf("\033[?25l%s", getl(ALL_DONE_AND_THANK));
        //Write data to Config.dat and make user waiting.
        Sleep(3000);
        FILE *config_file = fopen(config_dat,"wb+");
        fwrite(config, sizeof(struct config_model),1,config_file);
        fclose(config_file);
        printf("\033[?25h");
    }

    read_data();
}
