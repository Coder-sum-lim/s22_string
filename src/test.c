#include "s21_string.h"
#include <check.h>

START_TEST(test_memchr){
    char test_1[] = "12345";
    char test_2 = '3';
    char test_3 = '7';
    char test_4[] = "Hello, world!";
    char test_5 = 'g';
    char test_6 = ' ';
    char test_7[] = "W";
    char test_8 = '|';
    char test_9 = 't';
    char test_10 = '4';

    ck_assert_uint_eq((unsigned long)s21_memchr(test_1, test_2, 3),
                        (unsigned long)memchr(test_1, test_2, 3));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_1, test_3, 5),
                        (unsigned long)memchr(test_1, test_3, 3));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_4, test_5, 13),
                        (unsigned long)memchr(test_4, test_5, 13));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_4, test_6, 13),
                        (unsigned long)memchr(test_4, test_6, 13));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_4, test_10, 13),
                        (unsigned long)memchr(test_4, test_10, 3));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_7, test_8, 1),
                        (unsigned long)memchr(test_7, test_8, 1));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_7, test_9, 1),
                        (unsigned long)memchr(test_7, test_9, 1));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_7, test_10, 1),
                        (unsigned long)memchr(test_7, test_10, 1));
    ck_assert_uint_eq((unsigned long)s21_memchr(test_1, test_8, 5),
                        (unsigned long)memchr(test_1, test_8, 3));                                        
    }
END_TEST

Suite *s21_string_suite(void) {
    Suite *suite;
    TCase *tc_core;

    suite = suite_create("S21_string Tests");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_memchr);
    suite_add_tcase(suite, tc_core);

    return suite;
}

int main(void) {
    int number_failed;
    Suite *suite;
    SRunner *runner;

    runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    
    srunner_free(runner);
    
    return (number_failed == 0) ? 0 : 1;
}