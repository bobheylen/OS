/**
 * \author Bob Heylen
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

/*
 * definition of error codes
 * */
#define DPLIST_NO_ERROR 0
#define DPLIST_MEMORY_ERROR 1 // error due to mem alloc failure
#define DPLIST_INVALID_ERROR 2 //error due to a list operation applied on a NULL list 

#ifdef DEBUG
#define DEBUG_PRINTF(...) 									                                        \
        do {											                                            \
            fprintf(stderr,"\nIn %s - function %s at line %d: ", __FILE__, __func__, __LINE__);	    \
            fprintf(stderr,__VA_ARGS__);								                            \
            fflush(stderr);                                                                         \
                } while(0)
#else
#define DEBUG_PRINTF(...) (void)0
#endif


#define DPLIST_ERR_HANDLER(condition, err_code)                         \
    do {                                                                \
            if ((condition)) DEBUG_PRINTF(#condition " failed\n");      \
            assert(!(condition));                                       \
        } while(0)


/*
 * The real definition of struct list / struct node
 */

struct dplist_node {
    dplist_node_t *prev, *next;
    void *element;
};

struct dplist {
    dplist_node_t *head;

    void *(*element_copy)(void *src_element);

    void (*element_free)(void **element);

    int (*element_compare)(void *x, void *y);
};


dplist_t *dpl_create(// callback functions
        void *(*element_copy)(void *src_element),
        void (*element_free)(void **element),
        int (*element_compare)(void *x, void *y)
) {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_MEMORY_ERROR);
    list->head = NULL;
    list->element_copy = element_copy;
    list->element_free = element_free;
    list->element_compare = element_compare;
    return list;
}

void dpl_free(dplist_t **list, bool free_element) {

    dplist_t *start = *list;
    if(*list == NULL){
    }
    else if(start->head == NULL){
        free(start);
        start = NULL;
        *list = NULL;
    }
    else{
        int size = dpl_size(start);
        for(int i = 0; i < size; i++){
            dpl_remove_at_index(start, 0, free_element);
        }
        free(start);
        start = NULL;
        *list = NULL;
    }
}

dplist_t *dpl_insert_at_index(dplist_t *list, void *element, int index, bool insert_copy) {

    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) return NULL;

    list_node = malloc(sizeof(dplist_node_t));
    DPLIST_ERR_HANDLER(list_node == NULL, DPLIST_MEMORY_ERROR);
    if(insert_copy){
        list_node->element=list->element_copy;
    }
    else{
        list_node->element = element;
    }
    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    } else if (index <= 0) { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        } else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }
    return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index, bool free_element) {

    // list is NULL
    if(list == NULL){
        return NULL;
    }
    
    // list is empty
    if(dpl_size(list)==0){
        return list;
    }

    dplist_node_t *current_node = list->head;
    // remove first (or negatif index)
    if(index<=0){
        list->head = current_node->next;
        //free(current_node->element);
        if(free_element){
            (list->element_free)(&(current_node->element));
        }
        free(current_node);
        current_node = NULL;
        return list;
    }

    // size(list) < index
    if(dpl_size(list)<=index){
        index=dpl_size(list)-1;
    }

    // remove random (or size(list) < index)
    for(int i=0; i<=index; i++)
    {
        if (i==index)
        {
            if(index!=dpl_size(list)-1)
            {
                dplist_node_t *temp_node;
                // fix previous
                temp_node=current_node->prev;
                temp_node->next=current_node->next;

                // fix next
                temp_node=current_node->next;
                temp_node->prev=current_node->prev;

                // remove current
                if(free_element){
                    (list->element_free)(&(current_node->element));
                }
                free(current_node);
                current_node = NULL;
            }
            else{
                dplist_node_t *temp_node;
                // fix previous
                temp_node=current_node->prev;
                temp_node->next=current_node->next;

                // no fix next, want last element in list
                /*temp_node=current_node->next;
                temp_node->prev=current_node->prev;*/

                // remove current
                if(free_element){
                    (list->element_free)(&(current_node->element));
                }
                free(current_node);
                current_node = NULL;
            }
        }
        else{
            current_node=current_node->next;
        }
    }
    return list;
}

int dpl_size(dplist_t *list) {

    // list is NULL
    if(list == NULL){
        return -1;
    }

    // normal situations
    int counter=0;
    dplist_node_t *current_node = list->head;
    while(current_node!=NULL)
    {
        counter++;
	    current_node=current_node->next;
    }
    return counter;
}

void *dpl_get_element_at_index(dplist_t *list, int index) {

    // list is NULL
    if(list == NULL){
        return NULL;
    }

    // list is empty
    dplist_node_t *current_node = list->head;
    if(current_node==NULL){
        return NULL;
    }
    
    // index negatif
    if(index<0){
        return current_node->element;
    }

    // index bigger than size of list
    if(dpl_size(list)<=index){
        index=dpl_size(list)-1;
    }

    for(int i=0; i<index; i++)
    {
        current_node=current_node->next;
    }

    return current_node->element;
}

int dpl_get_index_of_element(dplist_t *list, void *element) {
    
    // list is NULL
    if(list == NULL){
        return -1;
    }
    
    dplist_node_t *current_node = list->head;
    for(int i=0; i<dpl_size(list); i++)
    {
        if(list->element_compare(element, current_node->element) == 0)
        {
            return i;
        }
        else
        {
            current_node=current_node->next;
        }
    }
    //if element not in list
    return -1;
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {

    int count;
    dplist_node_t *dummy;
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    if (list->head == NULL) return NULL;
    for (dummy = list->head, count = 0; dummy->next != NULL; dummy = dummy->next, count++) {
        if (count >= index) return dummy;
    }
    return dummy;
}

void *dpl_get_element_at_reference(dplist_t *list, dplist_node_t *reference) {

    // list is NULL
    if(list == NULL){return NULL;}

    // list is empty
    if(list -> head == NULL){return NULL;}

    // reference is NULL
    if(reference == NULL){return NULL;}

    // normal situations
    dplist_node_t *current_node = list -> head;
    while(current_node != NULL){
        if(current_node == reference){
            return current_node -> element;
        }
        current_node = current_node-> next;
    }
    // reference not in list
    return NULL;
}

