#include <stdlib.h>
#include <stdio.h>
#include "kd_tree.h"
#include "csv_reader.h"

#define MAX_OUTPUT_ROW_SIZE 1024 
#define MAX_ROW_SIZE 513
#define READ_SPACES_FORMATTER "%[^\n]s"

char* trim(char* str){
    if(str[strlen(str)-1]=='\n' || str[strlen(str)-1]=='\r'){
        str[strlen(str)-1] = '\0';
    }
    return str;
}

int main(int argc, char** argv){
    char* datafile = argv[1];
    char* outputfile = argv[2];
    if(argc<2){
        printf("Usage ./map1 datafile outputfile < inputfile\n");
        exit(0);
    }
    struct csv_meta_data* meta_data = get_csv_meta_data(datafile);
    struct csv_reader* reader = get_csv_data_object(datafile);
    
    KDTree *tree = get_new_kd_tree(2);

    for(int i = 0; i<meta_data->records_count; i++){
        char* data_line = (char*) malloc(MAX_OUTPUT_ROW_SIZE*sizeof(char));
        
        for(int j = 0; j<meta_data->columns_count; j++){
            char* formated_str = (char*) malloc(MAX_ROW_SIZE*sizeof(char));
            sprintf(formated_str,"%s: %s || "
                ,trim(reader->data_array[i]->col_name[j])
                ,trim(reader->data_array[i]->col_value[j]));
            strcat(data_line,formated_str);
        }
        char *endptr;
        double *key = (double*)malloc(2*(sizeof(double))) ;
        key[0] = strtod(reader->data_array[i]->col_value[8],&endptr); // X Co-ordinate
        key[1] = strtod(reader->data_array[i]->col_value[9],&endptr); // Y Co-ordinate

        kd_tree_insert(tree,key,(void*)data_line);
    }
    
    double x,y;
   

    while(scanf("%lf %lf",&x,&y)==2){
        double key[] = {x,y};
        KDTreeNode *node = find_closest(tree,key);
        print_list(node->data,x,y,outputfile);
    }
    
}