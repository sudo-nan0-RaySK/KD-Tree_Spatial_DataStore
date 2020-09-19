#include <stdlib.h>
#include <string.h>

/**
 * 
 * A generic Linked List implementation in C
 * 
 * */

#ifndef _LINKED_LIST_
#define  _LINKED_LIST_

struct list_node{
    void *data;
    struct list_node *next;
    struct list_node *prev;
};

struct list{
    struct list_node* head;
    struct list_node* tail;
    int size;
};

struct list_node* get_new_list_node(void* data){
    struct list_node* node = (struct list_node*) malloc(sizeof(struct list_node));
    node->prev = NULL;
    node->next = NULL;
    node->data = data;
    return node;
}

struct list* get_new_list(){
    struct list* list = (struct list*) malloc(sizeof(struct list));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void append(struct list* list, void* data){
    if (!list->head){
        list->head = get_new_list_node(data);
        list->tail = list->head;
    } else {
        struct list_node* new_node = get_new_list_node(data); 
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = list->tail->next;
    }
    list->size += 1;
}

void print_list(struct list* list, double x, double y, char* outputfile){
    FILE* fd = fopen(outputfile,"a+");
    struct list_node* tmp = list->head;
    while(tmp){
        fprintf(fd,"%lf %lf --> %s\n",x,y,(char*) tmp->data);
        tmp = tmp->next;
    }
    fclose(fd);
}

#endif


