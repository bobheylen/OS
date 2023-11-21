#define _GNU_SOURCE

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    int id;
    char* name;
} my_element_t;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);

void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->name); //asprintf requires _GNU_SOURCE
    assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
    copy->name = new_name;
    return (void *) copy;
}

void element_free(void ** element) {
    free((((my_element_t*)*element))->name);
    free(*element);
    *element = NULL;
}

int element_compare(void * x, void * y) {
    return ((((my_element_t*)x)->id < ((my_element_t*)y)->id) ? -1 : (((my_element_t*)x)->id == ((my_element_t*)y)->id) ? 0 : 1);
}

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}
START_TEST(test_ListFree)
    {
        // Test free NULL, don't use callback
        dplist_t *list = NULL;
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free NULL, use callback
        list = NULL;
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, don't use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free with one element, also test if inserted elements are set to NULL
        list = dpl_create(element_copy, element_free, element_compare);

        my_element_t *element = malloc(sizeof(my_element_t));
        element->name = malloc(sizeof(char));
        *(element->name) = 'A';

        list = dpl_insert_at_index(list, element, 0, false);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");
        //ck_assert_msg(element == NULL, "Failure: expected result to be NULL, but instead it is %p", element);

        // Test free with multiple element, also test if inserted elements are set to NULL
        list = dpl_create(element_copy, element_free, element_compare);

        my_element_t *element2 = malloc(sizeof(my_element_t));
        element2->name = malloc(sizeof(char));
        *(element2->name) = 'B';

        my_element_t *element3 = malloc(sizeof(my_element_t));
        element3->name = malloc(sizeof(char));
        *(element3->name) = 'C';

        my_element_t *element4 = malloc(sizeof(my_element_t));
        element4->name = malloc(sizeof(char));
        *(element4->name) = 'D';

        my_element_t *element5 = malloc(sizeof(my_element_t));
        element5->name = malloc(sizeof(char));
        *(element5->name) = 'E';

        list = dpl_insert_at_index(list, element2, 0, false);
        list = dpl_insert_at_index(list, element3, 0, false);
        list = dpl_insert_at_index(list, element4, 0, false);
        list = dpl_insert_at_index(list, element5, 0, false);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");
    }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
    {
        // Test inserting at index -1
        my_element_t *element = malloc(sizeof(my_element_t));
        element->name = malloc(sizeof(char));
        *(element->name) = 'A';
        
        dplist_t *result = dpl_insert_at_index(NULL, element, -1, false);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        result = dpl_insert_at_index(NULL, element, -1, true);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // Test inserting at index 0
        my_element_t *element2 = malloc(sizeof(my_element_t));
        element2->name = malloc(sizeof(char));
        *(element2->name) = 'B';
        
        result = dpl_insert_at_index(NULL, element2, 0, false);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        result = dpl_insert_at_index(NULL, element2, 0, true);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // Test inserting at index 99
        my_element_t *element3 = malloc(sizeof(my_element_t));
        element3->name = malloc(sizeof(char));
        *(element3->name) = 'C';

        result = dpl_insert_at_index(NULL, element3, 99, false);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        result = dpl_insert_at_index(NULL, element3, 99, true);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // You malloc my_element_t and element->name, but no dpl_free(&list, true) because list==NULL
        free(element->name);
        free(element);
        free(element2->name);
        free(element2);
        free(element3->name);
        free(element3);
    }
END_TEST

START_TEST(test_ListInsertAtIndexListEmpty)
{
    // Test inserting at index -1
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    
    my_element_t *element = malloc(sizeof(my_element_t));
    element->name = malloc(sizeof(char));
    *(element->name) = 'A';

    dplist_t *result = dpl_insert_at_index(list, element, -1, false);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    
    my_element_t *element2 = malloc(sizeof(my_element_t));
    element2->name = malloc(sizeof(char));
    *(element2->name) = 'B';
    
    result = dpl_insert_at_index(list, element2, -1, false);
    ck_assert_msg(dpl_size(result) == 2, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list, true);

    // Test inserting at index 0
    list = dpl_create(element_copy, element_free, element_compare);

    my_element_t *element3 = malloc(sizeof(my_element_t));
    element3->name = malloc(sizeof(char));
    *(element3->name) = 'C';

    result = dpl_insert_at_index(list, element3, 0, false);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    
    // Test inserting at index 99
    my_element_t *element4 = malloc(sizeof(my_element_t));
    element4->name = malloc(sizeof(char));
    *(element4->name) = 'D';

    result = dpl_insert_at_index(list, element4, 99, false);
    ck_assert_msg(dpl_size(result) == 2, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list, true);
}
END_TEST

START_TEST(test_size)
{
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);

    // Test size NULL
    ck_assert_msg(dpl_size(NULL) == -1, "Failure: expected list to have size of -1, got a size of %d",
                                         dpl_size(NULL));
    // Test size empty list
    ck_assert_msg(dpl_size(list) == 0, "Failure: expected list to have size of 0, got a size of %d",
                                         dpl_size(list));
    
    // Test size with one element
    my_element_t *element = malloc(sizeof(my_element_t));
    element->name = malloc(sizeof(char));
    *(element->name) = 'A';

    dplist_t *result = dpl_insert_at_index(list, element, 0, false);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));

    // Test size with multiple element
    my_element_t *element2 = malloc(sizeof(my_element_t));
    element2->name = malloc(sizeof(char));
    *(element2->name) = 'B';

    result = dpl_insert_at_index(list, element2, 1, false);

    my_element_t *element3 = malloc(sizeof(my_element_t));
    element3->name = malloc(sizeof(char));
    *(element3->name) = 'C';

    result = dpl_insert_at_index(list, element3, 2, false);

    my_element_t *element4 = malloc(sizeof(my_element_t));
    element4->name = malloc(sizeof(char));
    *(element4->name) = 'D';

    result = dpl_insert_at_index(list, element4, 3, false);
    ck_assert_msg(dpl_size(result) == 4, "Failure: expected list to have size of 4, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list, true);
}
END_TEST

START_TEST(test_getElementAtIndex)
{
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);

    // Test list NULL
    ck_assert_msg(dpl_get_element_at_index(NULL,0) == NULL, "Failure: expected element has to be NULL, but instead it is %p",
                                         dpl_get_element_at_index(NULL,0));

    // Test empty list
    ck_assert_msg(dpl_get_element_at_index(list,0) == NULL, "Failure: expected element has to be NULL, but instead it is %p",
                                         dpl_get_element_at_index(list,0));

    my_element_t *element = malloc(sizeof(my_element_t));
    element->name = malloc(sizeof(char));
    *(element->name) = 'A';
    dplist_t *result = dpl_insert_at_index(list, element, 0, false);

    my_element_t *element2 = malloc(sizeof(my_element_t));
    element2->name = malloc(sizeof(char));
    *(element2->name) = 'B';
    result = dpl_insert_at_index(list, element2, 1, false);

    my_element_t *element3 = malloc(sizeof(my_element_t));
    element3->name = malloc(sizeof(char));
    *(element3->name) = 'C';
    result = dpl_insert_at_index(list, element3, 2, false);

    my_element_t *element4 = malloc(sizeof(my_element_t));
    element4->name = malloc(sizeof(char));
    *(element4->name) = 'D';
    result = dpl_insert_at_index(list, element4, 3, false);

    // Test index negatif
    ck_assert_msg(dpl_get_element_at_index(result,-3) == element, "Failure: expected element has to be element, but instead it is %p",
                                         dpl_get_element_at_index(result,-3));

    // Test index > size(list)
    ck_assert_msg(dpl_get_element_at_index(result,8) == element4, "Failure: expected element has to be element4, but instead it is %p",
                                         dpl_get_element_at_index(result,8));
    
    // Test normal situations
    // First
    ck_assert_msg(dpl_get_element_at_index(result,0) == element, "Failure: expected element has to be element, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    // Middle
    ck_assert_msg(dpl_get_element_at_index(result,2) == element3, "Failure: expected element has to be element3, but instead it is %p",
                                         dpl_get_element_at_index(result,2));
    // Last
    my_element_t *element5 = malloc(sizeof(my_element_t));
    element5->name = malloc(sizeof(char));
    *(element5->name) = 'E';
    result = dpl_insert_at_index(list, element5, 4, false);

    ck_assert_msg(dpl_get_element_at_index(result,4) == element5, "Failure: expected element has to be element5, but instead it is %p",
                                         dpl_get_element_at_index(result,4));

    dpl_free(&list, true);
}
END_TEST

START_TEST(test_getIndexOfElement)
{
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    
    my_element_t *element = malloc(sizeof(my_element_t));
    element->name = malloc(sizeof(char));
    *(element->name) = 'A';
    dplist_t *result = dpl_insert_at_index(list, element, 0, false);

    my_element_t *element2 = malloc(sizeof(my_element_t));
    element2->name = malloc(sizeof(char));
    *(element2->name) = 'B';
    result = dpl_insert_at_index(list, element2, 1, false);

    my_element_t *element3 = malloc(sizeof(my_element_t));
    element3->name = malloc(sizeof(char));
    *(element3->name) = 'C';
    result = dpl_insert_at_index(list, element3, 2, false);

    my_element_t *element4 = malloc(sizeof(my_element_t));
    element4->name = malloc(sizeof(char));
    *(element4->name) = 'D';
    result = dpl_insert_at_index(list, element4, 3, false);

    // Test list NULL
    ck_assert_msg(dpl_get_index_of_element(NULL,element) == -1, "Failure: expected index has to be -1, but instead it is %d",
                                         dpl_get_index_of_element(NULL,element));

    // Test element not in list
    my_element_t *element99 = malloc(sizeof(my_element_t));
    element99->name = malloc(sizeof(char));
    *(element99->name) = 'A';
    ck_assert_msg(dpl_get_index_of_element(result,element99) == -1, "Failure: expected index has to be -1, but instead it is %d",
                                         dpl_get_index_of_element(result,element99));
    // Element not in list, dus dpl_free is not working on element99
    free(element99->name);
    free(element99);

    // Test normal situations
    // First
    ck_assert_msg(dpl_get_index_of_element(result,element) == 0, "Failure: expected index has to be 0, but instead it is %d",
                                         dpl_get_index_of_element(result,element3));
    // Middle
    ck_assert_msg(dpl_get_index_of_element(result,element3) == 2, "Failure: expected index has to be 2, but instead it is %d",
                                         dpl_get_index_of_element(result,element3));
    // Last
    my_element_t *element5 = malloc(sizeof(my_element_t));
    element5->name = malloc(sizeof(char));
    *(element5->name) = 'E';
    result = dpl_insert_at_index(list, element5, 4, false);
    ck_assert_msg(dpl_get_index_of_element(result,element5) == 4, "Failure: expected index has to be 4, but instead it is %d",
                                         dpl_get_index_of_element(result,element5));

    dpl_free(&list, true);
}
END_TEST

START_TEST(test_removeAtIndex)
{
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);

    // Test remove list NULL
    ck_assert_msg(dpl_remove_at_index(NULL, 0, true) == NULL, "Failure: expected list has to be NULL, but instead it is %p",
                                         dpl_remove_at_index(NULL, 0, true));

    // Test remove empty list
    ck_assert_msg(dpl_remove_at_index(list, 0, true) == list, "Failure: expected list has to be NULL, but instead it is %p",
                                         dpl_remove_at_index(NULL, 0, true));

    my_element_t *element = malloc(sizeof(my_element_t));
    element->name = malloc(sizeof(char));
    *(element->name) = 'A';
    dplist_t *result = dpl_insert_at_index(list, element, 0, false);

    my_element_t *element2 = malloc(sizeof(my_element_t));
    element2->name = malloc(sizeof(char));
    *(element2->name) = 'B';
    result = dpl_insert_at_index(list, element2, 1, false);

    my_element_t *element3 = malloc(sizeof(my_element_t));
    element3->name = malloc(sizeof(char));
    *(element3->name) = 'C';
    result = dpl_insert_at_index(list, element3, 2, false);

    my_element_t *element4 = malloc(sizeof(my_element_t));
    element4->name = malloc(sizeof(char));
    *(element4->name) = 'D';
    result = dpl_insert_at_index(list, element4, 3, false);

    // Test remove negatif index
    result = dpl_remove_at_index(list, -3, true);
    ck_assert_msg(dpl_get_element_at_index(result,0) == element2, "Failure: expected element has to be element2, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == element3, "Failure: expected element has to be element3, but instead it is %p",
                                         dpl_get_element_at_index(result,1));
    ck_assert_msg(dpl_get_element_at_index(result,2) == element4, "Failure: expected element has to be element4, but instead it is %p",
                                         dpl_get_element_at_index(result,2));

    // Test remove first element
    result = dpl_remove_at_index(list, 0, true);
    ck_assert_msg(dpl_get_element_at_index(result,0) == element3, "Failure: expected element has to be element3, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == element4, "Failure: expected element has to be element4, but instead it is %p",
                                         dpl_get_element_at_index(result,1));

    // Test remove size(list)<index
    result = dpl_remove_at_index(list, 8, true);
    ck_assert_msg(dpl_get_element_at_index(result,0) == element3, "Failure: expected element has to be element3, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    
    // Test remove random element
    my_element_t *element5 = malloc(sizeof(my_element_t));
    element5->name = malloc(sizeof(char));
    *(element5->name) = 'E';
    result = dpl_insert_at_index(list, element5, 1, false);

    my_element_t *element6 = malloc(sizeof(my_element_t));
    element6->name = malloc(sizeof(char));
    *(element6->name) = 'F';
    result = dpl_insert_at_index(list, element6, 2, false);

    my_element_t *element7 = malloc(sizeof(my_element_t));
    element7->name = malloc(sizeof(char));
    *(element7->name) = 'G';
    result = dpl_insert_at_index(list, element7, 3, false);

    ck_assert_msg(dpl_get_element_at_index(result,0) == element3, "Failure: expected element has to be element3, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == element5, "Failure: expected element has to be element5, but instead it is %p",
                                         dpl_get_element_at_index(result,1));
    ck_assert_msg(dpl_get_element_at_index(result,2) == element6, "Failure: expected element has to be element6, but instead it is %p",
                                         dpl_get_element_at_index(result,2));
    ck_assert_msg(dpl_get_element_at_index(result,3) == element7, "Failure: expected element has to be element7, but instead it is %p",
                                         dpl_get_element_at_index(result,3));

    // First
    result = dpl_remove_at_index(list, 0, true);
    // Middle
    result = dpl_remove_at_index(list, 1, true);
    // Last
    result = dpl_remove_at_index(list, 1, true);
    ck_assert_msg(dpl_get_element_at_index(result,0) == element5, "Failure: expected element has to be element5, but instead it is %p",
                                         dpl_get_element_at_index(result,0));

    // Test add negatif, remove negatif
    my_element_t *element99 = malloc(sizeof(my_element_t));
    element99->name = malloc(sizeof(char));
    *(element99->name) = 'Z';
    result = dpl_insert_at_index(list, element99, -45, false);
    ck_assert_msg(dpl_get_element_at_index(result, -45) == element99, "Failure: expected element is element99, but instead it is %p",
                                         dpl_get_element_at_index(result,-45));
    dpl_remove_at_index(list, -45, true);
    ck_assert_msg(dpl_get_element_at_index(result,-45) == element5, "Failure: expected element is element5, but instead it is %p",
                                         dpl_get_element_at_index(result,-45));

    // Test add high, remove high
    my_element_t *element66 = malloc(sizeof(my_element_t));
    element66->name = malloc(sizeof(char));
    *(element66->name) = 'Z';
    result = dpl_insert_at_index(list, element66, 45, false);    
    ck_assert_msg(dpl_get_element_at_index(result,45) == element66, "Failure: expected element is element66, but instead it is %p",
                                         dpl_get_element_at_index(result,45));

    dpl_free(&list, true);
}
END_TEST

START_TEST(test_getElementAtReference)
{
    // Test list NULL
    ck_assert_msg(dpl_get_element_at_reference(NULL, 0) == NULL, "Failure: expected element has to be NULL, but instead it is %p",
                                         dpl_get_element_at_reference(NULL, 0));

    // Test empty list
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_element_at_reference(list, 0) == NULL, "Failure: expected element has to be NULL, but instead it is %p",
                                         dpl_get_element_at_reference(list, 0));

    // Test reference NULL
    my_element_t *element = malloc(sizeof(my_element_t));
    element->name = malloc(sizeof(char));
    *(element->name) = 'A';
    dplist_t *result = dpl_insert_at_index(list, element, 0, false);
    ck_assert_msg(dpl_get_element_at_reference(result, NULL) == NULL, "Failure: expected element has to be NULL, but instead it is %p",
                                         dpl_get_element_at_reference(result, NULL));

    // Test reference not in list
    my_element_t *element2 = malloc(sizeof(my_element_t));
    element2->name = malloc(sizeof(char));
    *(element2->name) = 'B';
    result = dpl_insert_at_index(list, element2, 1, false);
    dplist_node_t *reference = dpl_get_reference_at_index(result, 1);
    result = dpl_remove_at_index(list, 1, true);
    ck_assert_msg(dpl_get_element_at_reference(result, reference) == NULL, "Failure: expected element has to be NULL, but instead it is %p",
                                         dpl_get_element_at_reference(result, reference));

    // Test normal situations
    reference = dpl_get_reference_at_index(result, 0);

    element2 = malloc(sizeof(my_element_t));
    //*(element2->name) = 'B';
    result = dpl_insert_at_index(list, element2, 1, false);
    dplist_node_t *reference2 = dpl_get_reference_at_index(result, 1);

    my_element_t *element3 = malloc(sizeof(my_element_t));
    element3->name = malloc(sizeof(char));
    *(element3->name) = 'C';
    result = dpl_insert_at_index(list, element3, 2, false);
    dplist_node_t *reference3 = dpl_get_reference_at_index(result, 2);

    // First
    ck_assert_msg(dpl_get_element_at_reference(result, reference) == element, "Failure: expected element has to be element, but instead it is %p",
                                         dpl_get_element_at_reference(result, reference));
    // Middle
    ck_assert_msg(dpl_get_element_at_reference(result, reference2) == element2, "Failure: expected element has to be element2, but instead it is %p",
                                         dpl_get_element_at_reference(result, reference));
    // Last
    ck_assert_msg(dpl_get_element_at_reference(result, reference3) == element3, "Failure: expected element has to be element3, but instead it is %p",
                                         dpl_get_element_at_reference(result, reference));

    dpl_free(&list, true);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX3");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_checked_fixture(tc1_1, setup, teardown);
    tcase_add_test(tc1_1, test_ListFree);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListNULL);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListEmpty);
    tcase_add_test(tc1_1, test_size);
    tcase_add_test(tc1_1, test_getElementAtIndex);
    tcase_add_test(tc1_1, test_getIndexOfElement);
    tcase_add_test(tc1_1, test_removeAtIndex);
    tcase_add_test(tc1_1, test_getElementAtReference);

    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
