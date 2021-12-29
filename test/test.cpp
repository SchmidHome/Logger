#ifndef UNIT_TEST
#define UNIT_TEST  // needed for intellisense to work
#endif

#include <Arduino.h>
#include <Logger.h>
#include <unity.h>

void test_size_0(void) {
    TEST_ASSERT_EQUAL(0, logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
}

void test_message_1(void) {
    deflogger.msg("Hello World!");
    TEST_ASSERT_EQUAL(false, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
    TEST_ASSERT_EQUAL(13, logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World!\n", logger_buffer, logger_buffer_index);
}

void test_message_2(void) {
    deflogger.msg("hello world?");
    TEST_ASSERT_EQUAL(false, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
    TEST_ASSERT_EQUAL(13 + 13, logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World!\nhello world?\n\0", logger_buffer, logger_buffer_index);
}

void test_send_msg(void) {
    TEST_ASSERT_EQUAL_STRING("Hello World!\nhello world?\n", logger_buffer);
    deflogger.clear_buffer();
    TEST_ASSERT_EQUAL(0, logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
}

void test_no_overflow(void) {
    // needed for overflow test
    TEST_ASSERT_EQUAL(50, LOGGER_BUFFER_SIZE);
    deflogger.clear_buffer();

    deflogger.msg("this is a 50 char string which schould work fine");
    TEST_ASSERT_EQUAL(false, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
    TEST_ASSERT_EQUAL(49, logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR('\n', logger_buffer[48]);
    TEST_ASSERT_EQUAL_CHAR('\0', logger_buffer[49]);
    TEST_ASSERT_EQUAL_STRING("this is a 50 char string which schould work fine\n", logger_buffer);

    deflogger.clear_buffer();
    TEST_ASSERT_EQUAL(0, logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
}

void test_exact_overflow(void) {
    // needed for overflow test
    TEST_ASSERT_EQUAL(50, LOGGER_BUFFER_SIZE);
    deflogger.clear_buffer();

    deflogger.msg("this is a 51char string which will cause overflow");
    TEST_ASSERT_EQUAL(false, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(true, deflogger.buffer_full());
    TEST_ASSERT_EQUAL(50, logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR('\n', logger_buffer[48]);
    TEST_ASSERT_EQUAL_CHAR('\0', logger_buffer[49]);
    TEST_ASSERT_EQUAL_STRING("this is a 51char string which will cause overflo\n", logger_buffer);

    deflogger.create_overflow_message();
    TEST_ASSERT_EQUAL(false, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
    TEST_ASSERT_EQUAL_STRING("WARN: Logger buffer overflowed by 1 characters\n", logger_buffer);

    deflogger.clear_buffer();
    TEST_ASSERT_EQUAL(0, logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger.buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger.buffer_full());
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_size_0);
    RUN_TEST(test_message_1);
    RUN_TEST(test_message_2);
    RUN_TEST(test_send_msg);

    RUN_TEST(test_no_overflow);
    RUN_TEST(test_exact_overflow);
    UNITY_END();
}

void setup() {
    delay(2000);
    process();
}

void loop() {
    delay(1000);
}
