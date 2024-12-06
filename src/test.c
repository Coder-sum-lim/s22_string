#include "s21_string.h"

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