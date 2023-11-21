/**
 * \author Bob Heylen
 */

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}
START_TEST(test_ListFree)
    {
        // Test free NULL
        dplist_t *list = NULL;
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list
        list = dpl_create();
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free with one element
        list = dpl_create();
        dplist_t *result = dpl_insert_at_index(list, 'A', 0);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free with multiple element
        list = dpl_create();
        result = dpl_insert_at_index(list, 'A', 0);
        result = dpl_insert_at_index(list, 'B', 1);
        result = dpl_insert_at_index(list, 'C', 2);
        result = dpl_insert_at_index(list, 'D', 3);
        result = dpl_insert_at_index(list, 'E', 4);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

    }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
    {
        // Test inserting at index -1
        dplist_t *result = dpl_insert_at_index(NULL, 'A', -1);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        // Test inserting at index 0
        result = dpl_insert_at_index(NULL, 'B', 0);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        // Test inserting at index 99
        result = dpl_insert_at_index(NULL, 'C', 99);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
    }
END_TEST

START_TEST(test_ListInsertAtIndexListEmpty)
{
    // Test inserting at index -1
    dplist_t *list = dpl_create();
    dplist_t *result = dpl_insert_at_index(list, 'A', -1);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    result = dpl_insert_at_index(list, 'B', -1);
    ck_assert_msg(dpl_size(result) == 2, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);

    // Test inserting at index 0
    list = dpl_create();
    result = dpl_insert_at_index(list, 'C', 0);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    // Test inserting at index 99
    result = dpl_insert_at_index(list, 'D', 99);
    ck_assert_msg(dpl_size(result) == 2, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);
}
END_TEST

//START_TEST(test_nameOfYourTest)
//  Add other testcases here...

//*************************************************************************************************************************
START_TEST(test_size)
{
    dplist_t *list = dpl_create();

    // Test size NULL
    ck_assert_msg(dpl_size(NULL) == -1, "Failure: expected list to have size of -1, got a size of %d",
                                         dpl_size(NULL));
    // Test size empty list
    ck_assert_msg(dpl_size(list) == 0, "Failure: expected list to have size of 0, got a size of %d",
                                         dpl_size(list));
    // Test size with one element
    dplist_t *result = dpl_insert_at_index(list, 'A', 0);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    // Test size with multiple element
    result = dpl_insert_at_index(list, 'B', 1);
    result = dpl_insert_at_index(list, 'C', 2);
    result = dpl_insert_at_index(list, 'D', 3);
    ck_assert_msg(dpl_size(result) == 4, "Failure: expected list to have size of 4, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);
}
END_TEST
//*************************************************************************************************************************

//*************************************************************************************************************************
START_TEST(test_getElementAtIndex)
{
    dplist_t *list = dpl_create();

    // Test list NULL
    ck_assert_msg(dpl_get_element_at_index(NULL,0) == 0, "Failure: expected element has to be 0, but instead it is %c",
                                         dpl_get_element_at_index(NULL,0));

    // Test empty list
    ck_assert_msg(dpl_get_element_at_index(list,0) == 0, "Failure: expected element has to be 0, but instead it is %c",
                                         dpl_get_element_at_index(list,0));

    dplist_t *result = dpl_insert_at_index(list, 'A', 0);
    result = dpl_insert_at_index(list, 'B', 1);
    result = dpl_insert_at_index(list, 'C', 2);
    result = dpl_insert_at_index(list, 'D', 3);

    // Test index negatif
    ck_assert_msg(dpl_get_element_at_index(result,-3) == 'A', "Failure: expected element has to be A, but instead it is %c",
                                         dpl_get_element_at_index(result,-3));

    // Test index > size(list)
    ck_assert_msg(dpl_get_element_at_index(result,8) == 'D', "Failure: expected element has to be D, but instead it is %c",
                                         dpl_get_element_at_index(result,8));
    
    // Test normal situations
    ck_assert_msg(dpl_get_element_at_index(result,2) == 'C', "Failure: expected element has to be C, but instead it is %c",
                                         dpl_get_element_at_index(result,2));
    result = dpl_insert_at_index(list, 'E', 4);
    ck_assert_msg(dpl_get_element_at_index(result,4) == 'E', "Failure: expected element has to be E, but instead it is %c",
                                         dpl_get_element_at_index(result,4));

    // Test free and empty list
    dpl_free(&list);
    list = dpl_create();
    ck_assert_msg(dpl_get_element_at_index(result,0) == 0, "Failure: expected element has to be 0, but instead it is %c",
                                         dpl_get_element_at_index(result,0));
    dpl_free(&list);
}
END_TEST
//*************************************************************************************************************************

//*************************************************************************************************************************
START_TEST(test_getIndexOfElement)
{
    dplist_t *list = dpl_create();
    dplist_t *result = dpl_insert_at_index(list, 'A', 0);
    result = dpl_insert_at_index(list, 'B', 1);
    result = dpl_insert_at_index(list, 'C', 2);
    result = dpl_insert_at_index(list, 'D', 3);

    // Test list NULL
    ck_assert_msg(dpl_get_index_of_element(NULL,'A') == -1, "Failure: expected index has to be -1, but instead it is %d",
                                         dpl_get_index_of_element(NULL,'A'));

    // Test element not in list
    ck_assert_msg(dpl_get_index_of_element(result,'Z') == -1, "Failure: expected index has to be -1, but instead it is %d",
                                         dpl_get_index_of_element(result,'Z'));

    // Test normal situations
    ck_assert_msg(dpl_get_index_of_element(result,'C') == 2, "Failure: expected index has to be 2, but instead it is %d",
                                         dpl_get_index_of_element(result,'C'));
    result = dpl_insert_at_index(list, 'E', 4);
    ck_assert_msg(dpl_get_index_of_element(result,'E') == 4, "Failure: expected index has to be 4, but instead it is %d",
                                         dpl_get_index_of_element(result,'E'));

    // Test free and empty list
    dpl_free(&list);
    list = dpl_create();
    ck_assert_msg(dpl_get_index_of_element(result, 'A') == -1, "Failure: expected index has to be -1, but instead it is %d",
                                         dpl_get_index_of_element(result,'A'));
    dpl_free(&list);
}
END_TEST
//*************************************************************************************************************************

//*************************************************************************************************************************
START_TEST(test_removeAtIndex)
{
    dplist_t *list = dpl_create();

    // Test remove list NULL
    dpl_remove_at_index(NULL,0); // return NULL

    // Test remove empty list
    dpl_remove_at_index(list, 0); // return unmodified list

    
    dplist_t *result = dpl_insert_at_index(list, 'A', 0);
    result = dpl_insert_at_index(list, 'B', 1);
    result = dpl_insert_at_index(list, 'C', 2);
    result = dpl_insert_at_index(list, 'D', 3);

    // Test remove negatif index
    result = dpl_remove_at_index(result,-3);
    ck_assert_msg(dpl_get_element_at_index(result,0) == 'B', "Failure: expected element has to be 'B', but instead it is %c",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == 'C', "Failure: expected element has to be 'C', but instead it is %c",
                                         dpl_get_element_at_index(result,1));
    ck_assert_msg(dpl_get_element_at_index(result,2) == 'D', "Failure: expected element has to be 'D', but instead it is %c",
                                         dpl_get_element_at_index(result,2));

    // Test remove first element
    result = dpl_remove_at_index(result,0);
    ck_assert_msg(dpl_get_element_at_index(result,0) == 'C', "Failure: expected element has to be 'C', but instead it is %c",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == 'D', "Failure: expected element has to be 'D', but instead it is %c",
                                         dpl_get_element_at_index(result,1));

    // Test remove size(list)<index
    result = dpl_remove_at_index(result,8);
    ck_assert_msg(dpl_get_element_at_index(result,0) == 'C', "Failure: expected element has to be 'C', but instead it is %c",
                                         dpl_get_element_at_index(result,0));
    
    // Test remove random element
    result = dpl_insert_at_index(list, 'E', 1);
    result = dpl_insert_at_index(list, 'F', 2);
    result = dpl_insert_at_index(list, 'G', 3);
    ck_assert_msg(dpl_get_element_at_index(result,0) == 'C', "Failure: expected element has to be 'C', but instead it is %c",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == 'E', "Failure: expected element has to be 'E', but instead it is %c",
                                         dpl_get_element_at_index(result,1));
    ck_assert_msg(dpl_get_element_at_index(result,2) == 'F', "Failure: expected element has to be 'F', but instead it is %c",
                                         dpl_get_element_at_index(result,2));
    ck_assert_msg(dpl_get_element_at_index(result,3) == 'G', "Failure: expected element has to be 'G', but instead it is %c",
                                         dpl_get_element_at_index(result,3));

    // Test add negatif, remove negatif
    result = dpl_insert_at_index(list, 'Z', -45);
    ck_assert_msg(dpl_get_element_at_index(list,-45) == 'Z', "Failure: expected element is Z, but instead it is %c",
                                         dpl_get_element_at_index(list,-45));

    // Test add high, remove high
    result = dpl_insert_at_index(list, 'Z', 45);
    ck_assert_msg(dpl_get_element_at_index(list,45) == 'Z', "Failure: expected element is Z, but instead it is %c",
                                         dpl_get_element_at_index(list,45));
    dpl_free(&list);
}
END_TEST
//*************************************************************************************************************************
//END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX1");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_checked_fixture(tc1_1, setup, teardown);
    tcase_add_test(tc1_1, test_ListFree);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListNULL);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListEmpty);
    // Add other tests here...
//*********************
    tcase_add_test(tc1_1, test_size);
    tcase_add_test(tc1_1, test_getElementAtIndex);
    tcase_add_test(tc1_1, test_getIndexOfElement);
    tcase_add_test(tc1_1, test_removeAtIndex);
//*********************
    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
