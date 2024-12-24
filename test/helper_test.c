#include "../src/helper.h"
#include "../external/unity/unity.h"
#include <string.h>


void setUp(void) {
}

void tearDown(void) {
}

void test_break_path_into_array(void) {
    int counter = 0;
    char *path = "/custom/path:/other/path:/final/path";
    char **tokens = parse_path(&counter, path);
    TEST_ASSERT_NOT_NULL(tokens);
    TEST_ASSERT_EQUAL(3, counter);
    TEST_ASSERT_EQUAL(0, strcmp("/custom/path", tokens[0]));
    TEST_ASSERT_EQUAL(0, strcmp("/other/path", tokens[1]));
    TEST_ASSERT_EQUAL(0, strcmp("/final/path", tokens[2]));
}

void test_parse_exact_values(void) {
    int counter = 0;
    char *path = "/custom/path:/other/path";
    char **tokens = parse_path(&counter, path);
    TEST_ASSERT_NOT_NULL(tokens);
    TEST_ASSERT_EQUAL(2, counter);
    TEST_ASSERT_EQUAL(0, strcmp("/custom/path", tokens[0]));
    TEST_ASSERT_EQUAL(0, strcmp("/other/path", tokens[1]));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parse_exact_values);
    RUN_TEST(test_break_path_into_array);
    return UNITY_END();
}
