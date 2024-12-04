#include "s21_string.h"
#include <check.h>

START_TEST(test_s21_strerror1) {
    ck_assert_str_eq(s21_strerror(1), strerror(1));
    ck_assert_str_eq(s21_strerror(2), strerror(2));
    ck_assert_str_eq(s21_strerror(3), strerror(3));
}
END_TEST

START_TEST(test_s21_strerror2) {
    ck_assert_str_eq(s21_strerror(0), strerror(0));
    ck_assert_str_eq(s21_strerror(100), strerror(100));
    ck_assert_str_eq(s21_strerror(150), strerror(150));
}
END_TEST

Suite *s21_error_suite(void) {
    Suite *suite;
    TCase *tc_core;

    suite = suite_create("S21 Error Tests");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_s21_strerror1);
    tcase_add_test(tc_core, test_s21_strerror2);
    suite_add_tcase(suite, tc_core);

    return suite;
}

int main(void) {
    int number_failed;
    Suite *suite;
    SRunner *runner;

    suite = s21_error_suite();
    runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    
    srunner_free(runner);
    
    return (number_failed == 0) ? 0 : 1;
}