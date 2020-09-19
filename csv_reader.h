#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"


#define DATA_LABEL_SIZE 129
#define DATA_LINE_SIZE 513

struct csv_record{
    char **col_name;
    char **col_value;
};

struct csv_reader{
    struct csv_record **data_array;
};

struct csv_meta_data{
    int records_count;
    int columns_count;
    char* file_name;
};

struct list* split(char* string){
    struct list* token_list = get_new_list();
    char *curr_string = (char*)malloc( DATA_LINE_SIZE*sizeof(char));
    for(int i = 0; i<strlen(string); i++){
        if(string[i]==','){
            char* new_str = (char*)malloc( DATA_LINE_SIZE*sizeof(char));
            strcpy(new_str,curr_string);
            append(token_list,(void*)new_str);
            strcpy(curr_string,"");
        }
        else if(string[i]=='\"'){
            i+=1;
            while(string[i]!='\"'){
                char c[2] = {string[i],'\0'};
                strcat(curr_string,c);
                i+=1;
            }
        }
        else{
            char c[2] = {string[i],'\0'};
            strcat(curr_string,c);
        } 
    }

    if(strlen(curr_string)>0){
        char* new_str = (char*)malloc( DATA_LINE_SIZE*sizeof(char));
        strcpy(new_str,curr_string);
        append(token_list,(void*)new_str);
    }

    return token_list;
}

struct csv_record* get_csv_record(int record_length){
    struct csv_record* record = (struct csv_record*) malloc(sizeof(struct csv_record));
    record->col_name = (char**) malloc(record_length*sizeof(char*));
    record->col_value = (char**) malloc(record_length*sizeof(char*));
    for(int i = 0; i<record_length; i++){
        record->col_name[i] = (char*) malloc(DATA_LABEL_SIZE*sizeof(char));
        record->col_value[i] = (char*) malloc(DATA_LABEL_SIZE*sizeof(char));
    }
    return record;
}

struct csv_reader* get_csv_reader(struct csv_meta_data* meta_data){
    int records_count = meta_data->records_count;
    int record_length = meta_data->columns_count;
    struct csv_reader *reader = (struct csv_reader*) malloc(sizeof(struct csv_reader));
    reader->data_array = (struct csv_record**) malloc(records_count*sizeof(struct csv_record*));
    for(int i = 0; i<records_count; i++){
        reader->data_array[i]  = get_csv_record(record_length);
    } 
    return reader;
}


struct csv_meta_data* get_csv_meta_data(char *file_name){
    struct csv_meta_data* meta_data = (struct csv_meta_data*) malloc(sizeof(struct csv_meta_data));
    FILE *data_file = fopen(file_name,"r+");
    int rows = 0, cols = 0;
    char data_line[DATA_LINE_SIZE];
    while(fgets(data_line,DATA_LINE_SIZE,data_file)){
        if(rows==0){
            struct list* tmp = split(data_line);
            cols = tmp->size;
        }
        rows+=1;
    }
    meta_data->columns_count = cols;
    meta_data->records_count = rows;
    meta_data->file_name = file_name;

    return meta_data;
}


void read_file(struct csv_reader *reader, struct csv_meta_data* meta_data){
    char* file_name = meta_data->file_name;
    int records_count = meta_data->records_count;
    int record_length = meta_data->columns_count;
    FILE *data_file = fopen(file_name,"r+");
    char **col_names = (char**)malloc(record_length*sizeof(char*));

    for(int i = 0; i<records_count; i++){
        char data_line[DATA_LINE_SIZE];

        if(fgets(data_line,DATA_LINE_SIZE,data_file)){
            int itr = 0;
            struct list_node* tmp = split(data_line)->head;

            /** Initialize column headers **/
            if(i==0) { 
                while(tmp){
                    col_names[itr] = (char*)tmp->data;
                    if(col_names[itr][strlen(col_names[itr])-1]=='\n'){
                        col_names[itr][strlen(col_names[itr])-1] = 0;
                    }
                    itr+=1;
                    tmp = tmp->next;
                }
            } 
            /** Initialize data array **/
            else { 
                while(tmp){
                    reader->data_array[i]->col_name[itr] = col_names[itr];
                    char* tmp_data = (char*) tmp->data;
                    if(tmp_data[strlen(tmp_data)-1]=='\n'){
                        tmp_data[strlen(tmp_data)-1] = 0;
                    }
                    reader->data_array[i]->col_value[itr] = tmp_data;

                    itr+=1;
                    tmp = tmp->next;
                }
            }
        }
    }
}

struct csv_reader* get_csv_data_object(char* file_name){
    struct csv_meta_data* meta_data = get_csv_meta_data(file_name);
    struct csv_reader *reader = get_csv_reader(meta_data);
    read_file(reader,meta_data);
    return reader;
}




