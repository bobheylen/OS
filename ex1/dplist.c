/**
 * \author Bob Heylen
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

//#define DEBUG

/*
 * definition of error codes
 */
#define DPLIST_NO_ERROR 0
#define DPLIST_MEMORY_ERROR 1   //error due to mem alloc failure
#define DPLIST_INVALID_ERROR 2  //error due to a list operation applied on a NULL list

#ifdef DEBUG
#define DEBUG_PRINTF(...) 									    \
        do {											    \
            fprintf(stderr,"\nIn %s - function %s at line %d: ", __FILE__, __func__, __LINE__);	    \
            fprintf(stderr,__VA_ARGS__);							    \
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
    element_t element;
};

struct dplist {
    dplist_node_t *head;
    // more fields will be added later
};

dplist_t *dpl_create() {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_MEMORY_ERROR);
    list->head = NULL;
    // pointer drawing breakpoint
    return list;
}

void dpl_free(dplist_t **list) {

    dplist_t *start = *list;
    if(*list == NULL){
    }
    else if(start->head == NULL){
        free(start);
        *list = NULL;
    }
    else{
        dplist_node_t *currentNode = start->head, *nextNode;
        while (currentNode->next!=NULL){
            nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }
        free(currentNode);
        free(start);
        *list = NULL;
    }
}

/* Important note: to implement any list manipulation operator (insert, append, delete, sort, ...), always be aware of the following cases:
 * 1. empty list ==> avoid errors
 * 2. do operation at the start of the list ==> typically requires some special pointer manipulation
 * 3. do operation at the end of the list ==> typically requires some special pointer manipulation
 * 4. do operation in the middle of the list ==> default case with default pointer manipulation
 * ALWAYS check that you implementation works correctly in all these cases (check this on paper with list representation drawings!)
 **/

dplist_t *dpl_insert_at_index(dplist_t *list, element_t element, int index) {
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) return NULL;

    list_node = malloc(sizeof(dplist_node_t));
    DPLIST_ERR_HANDLER(list_node == NULL, DPLIST_MEMORY_ERROR);
    list_node->element = element;
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

dplist_t *dpl_remove_at_index(dplist_t *list, int index) {

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

    // while-loop die checkt of next nog bestaat
    // Indien next=NULL, return size
    if(list == NULL){
        return -1;
    }

    int counter=0;
    dplist_node_t *current_node = list->head;
    while(current_node!=NULL)
    {
        counter++;
	current_node=current_node->next;
    }
    return counter;
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

element_t dpl_get_element_at_index(dplist_t *list, int index) {

    // list is NULL
    if(list == NULL){
        return 0;
    }

    dplist_node_t *current_node = list->head;
    // list is empty
    if(current_node==NULL){
        return 0;
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

int dpl_get_index_of_element(dplist_t *list, element_t element) {

    // list is NULL
    if(list == NULL){
        return -1;
    }
    
    dplist_node_t *current_node = list->head;
    for(int i=0; i<dpl_size(list); i++)
    {
        if(current_node->element==element)
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



