#include <check.h>

#include "s21_string.h"

START_TEST(test_s21_strerror) {
  ck_assert_str_eq(s21_strerror(1), strerror(1));
  ck_assert_str_eq(s21_strerror(2), strerror(2));
  ck_assert_str_eq(s21_strerror(3), strerror(3));
}
END_TEST

START_TEST(test_s21_strpbrk) {
  ck_assert_ptr_eq(s21_strpbrk("gtr", "abc"), strpbrk("gtr", "abc"));
  ck_assert_ptr_eq(s21_strpbrk("a", "abc"), strpbrk("a", "abc"));
  ck_assert_ptr_eq(s21_strpbrk("v", "abc"), strpbrk("v", "abc"));
}
END_TEST

START_TEST(test_s21_strlen) {
  ck_assert_int_eq(s21_strlen("12345"), strlen("12345"));
  ck_assert_int_eq(s21_strlen("\n"), strlen("\n"));
  ck_assert_int_eq(s21_strlen(""), strlen(""));
}
END_TEST

START_TEST(test_memchr) {
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

  tcase_add_test(tc_core, test_s21_strlen);
  tcase_add_test(tc_core, test_s21_strerror);
  tcase_add_test(tc_core, test_memchr);
  tcase_add_test(tc_core, test_s21_strpbrk);
  suite_add_tcase(suite, tc_core);

  return suite;
}

int main(void) {
  int number_failed;
  Suite *suite;
  SRunner *runner;

  suite = s21_string_suite();
  runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);

  srunner_free(runner);

  return (number_failed == 0) ? 0 : 1;
}