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
        element_t ap;
        ap = malloc(sizeof(element_t));
        dplist_t *result = dpl_insert_at_index(list, ap, 0);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free with multiple element
        list = dpl_create();
        ap = malloc(sizeof(element_t));
        result = dpl_insert_at_index(list, ap, 0);
        element_t bp;
        bp = malloc(sizeof(element_t));
        result = dpl_insert_at_index(list, bp, 1);
        element_t cp;
        cp = malloc(sizeof(element_t));
        result = dpl_insert_at_index(list, cp, 2);
        element_t dp;
        dp = malloc(sizeof(element_t));
        result = dpl_insert_at_index(list, dp, 3);
        element_t ep;
        ep = malloc(sizeof(element_t));
        result = dpl_insert_at_index(list, ep, 4);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

    }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
    {
        // Test inserting at index -1
        element_t ap;
        ap = malloc(sizeof(element_t));
        dplist_t *result = dpl_insert_at_index(NULL, ap, -1);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        // Test inserting at index 0
        element_t bp;
        bp = malloc(sizeof(element_t));
        result = dpl_insert_at_index(NULL, bp, 0);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        // Test inserting at index 99
        element_t cp;
        cp = malloc(sizeof(element_t));
        result = dpl_insert_at_index(NULL, cp, 99);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // You malloc element_t, but no dpl_free(&list) because list==NULL
        free(ap);
        free(bp);
        free(cp);
    }
END_TEST

START_TEST(test_ListInsertAtIndexListEmpty)
{
    // Test inserting at index -1
    dplist_t *list = dpl_create();
    element_t ap;
    ap = malloc(sizeof(element_t));
    dplist_t *result = dpl_insert_at_index(list, ap, -1);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    element_t bp;
    bp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, bp, -1);
    ck_assert_msg(dpl_size(result) == 2, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);

    // Test inserting at index 0
    list = dpl_create();
    element_t cp;
    cp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, cp, 0);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    // Test inserting at index 99
    element_t dp;
    dp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, dp, 99);
    ck_assert_msg(dpl_size(result) == 2, "Failure: expected list to have size of 2, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);
}
END_TEST

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
    element_t ap;
        ap = malloc(sizeof(element_t));
    dplist_t *result = dpl_insert_at_index(list, ap, 0);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    // Test size with multiple element
    element_t bp;
    bp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, bp, 1);
    element_t cp;
    cp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, cp, 2);
    element_t dp;
    dp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, dp, 3);
    ck_assert_msg(dpl_size(result) == 4, "Failure: expected list to have size of 4, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);
}
END_TEST

START_TEST(test_getElementAtIndex)
{
    dplist_t *list = dpl_create();

    // Test list NULL
    ck_assert_msg(dpl_get_element_at_index(NULL,0) == 0, "Failure: expected element has to be 0, but instead it is %p",
                                         dpl_get_element_at_index(NULL,0));

    // Test empty list
    ck_assert_msg(dpl_get_element_at_index(list,0) == 0, "Failure: expected element has to be 0, but instead it is %p",
                                         dpl_get_element_at_index(list,0));

    element_t ap;
    ap = malloc(sizeof(element_t));
    dplist_t *result = dpl_insert_at_index(list, ap, 0);
    element_t bp;
    bp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, bp, 1);
    element_t cp;
    cp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, cp, 2);
    element_t dp;
    dp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, dp, 3);

    // Test index negatif
    ck_assert_msg(dpl_get_element_at_index(result,-3) == ap, "Failure: expected element has to be ap, but instead it is %p",
                                         dpl_get_element_at_index(result,-3));

    // Test index > size(list)
    ck_assert_msg(dpl_get_element_at_index(result,8) == dp, "Failure: expected element has to be dp, but instead it is %p",
                                         dpl_get_element_at_index(result,8));
    
    // Test normal situations
    ck_assert_msg(dpl_get_element_at_index(result,2) == cp, "Failure: expected element has to be cp, but instead it is %p",
                                         dpl_get_element_at_index(result,2));
    element_t ep;
    ep = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, ep, 4);
    ck_assert_msg(dpl_get_element_at_index(result,4) == ep, "Failure: expected element has to be ep, but instead it is %p",
                                         dpl_get_element_at_index(result,4));

    dpl_free(&list);
}
END_TEST

START_TEST(test_getIndexOfElement)
{
    dplist_t *list = dpl_create();
    element_t ap;
    ap = malloc(sizeof(element_t));
    dplist_t *result = dpl_insert_at_index(list, ap, 0);
    element_t bp;
    bp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, bp, 1);
    element_t cp;
    cp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, cp, 2);
    element_t dp;
    dp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, dp, 3);

    // Test list NULL
    ck_assert_msg(dpl_get_index_of_element(NULL,ap) == -1, "Failure: expected index has to be -1, but instead it is %d",
                                         dpl_get_index_of_element(NULL,ap));

    // Test element not in list
    element_t zp;
    zp = malloc(sizeof(element_t));
    ck_assert_msg(dpl_get_index_of_element(result,zp) == -1, "Failure: expected index has to be -1, but instead it is %d",
                                         dpl_get_index_of_element(result,zp));

    // Test normal situations
    ck_assert_msg(dpl_get_index_of_element(result,cp) == 2, "Failure: expected index has to be 2, but instead it is %d",
                                         dpl_get_index_of_element(result,cp));
    element_t ep;
    ep = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, ep, 4);
    ck_assert_msg(dpl_get_index_of_element(result,ep) == 4, "Failure: expected index has to be 4, but instead it is %d",
                                         dpl_get_index_of_element(result,ep));

    dpl_free(&list);
}
END_TEST

START_TEST(test_removeAtIndex)
{
    dplist_t *list = dpl_create();

    // Test remove list NULL
    dpl_remove_at_index(NULL,0); // return NULL

    // Test remove empty list
    dpl_remove_at_index(list, 0); // return unmodified list

    element_t ap;
    ap = malloc(sizeof(element_t));
    dplist_t *result = dpl_insert_at_index(list, ap, 0);
    element_t bp;
    bp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, bp, 1);
    element_t cp;
    cp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, cp, 2);
    element_t dp;
    dp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, dp, 3);

    // Test remove negatif index
    result = dpl_remove_at_index(result,-3);
    ck_assert_msg(dpl_get_element_at_index(result,0) == bp, "Failure: expected element has to be bp, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == cp, "Failure: expected element has to be cp, but instead it is %p",
                                         dpl_get_element_at_index(result,1));
    ck_assert_msg(dpl_get_element_at_index(result,2) == dp, "Failure: expected element has to be dp, but instead it is %p",
                                         dpl_get_element_at_index(result,2));

    // Test remove first element
    result = dpl_remove_at_index(result,0);
    ck_assert_msg(dpl_get_element_at_index(result,0) == cp, "Failure: expected element has to be cp, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == dp, "Failure: expected element has to be dp, but instead it is %p",
                                         dpl_get_element_at_index(result,1));

    // Test remove size(list)<index
    result = dpl_remove_at_index(result,8);
    ck_assert_msg(dpl_get_element_at_index(result,0) == cp, "Failure: expected element has to be cp, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    
    // Test remove random element
    element_t ep;
    ep = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, ep, 1);
    element_t fp;
    fp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, fp, 2);
    element_t gp;
    gp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, gp, 3);
    ck_assert_msg(dpl_get_element_at_index(result,0) == cp, "Failure: expected element has to be cp, but instead it is %p",
                                         dpl_get_element_at_index(result,0));
    ck_assert_msg(dpl_get_element_at_index(result,1) == ep, "Failure: expected element has to be ep, but instead it is %p",
                                         dpl_get_element_at_index(result,1));
    ck_assert_msg(dpl_get_element_at_index(result,2) == fp, "Failure: expected element has to be fp, but instead it is %p",
                                         dpl_get_element_at_index(result,2));
    ck_assert_msg(dpl_get_element_at_index(result,3) == gp, "Failure: expected element has to be gp, but instead it is %p",
                                         dpl_get_element_at_index(result,3));

    // Test add negatif, remove negatif
    element_t zp;
    zp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, zp, -45);
    ck_assert_msg(dpl_get_element_at_index(list,-45) == zp, "Failure: expected element is zp, but instead it is %p",
                                         dpl_get_element_at_index(list,-45));

    // Test add high, remove high
    zp = malloc(sizeof(element_t));
    result = dpl_insert_at_index(list, zp, 45);
    ck_assert_msg(dpl_get_element_at_index(list,45) == zp, "Failure: expected element is zp, but instead it is %p",
                                         dpl_get_element_at_index(list,45));
    dpl_free(&list);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX2");
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

    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
