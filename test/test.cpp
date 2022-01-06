#ifndef UNIT_TEST
#define UNIT_TEST  // needed for intellisense to work
#endif

#include <Arduino.h>
#include <Logger.h>
#include <unity.h>

void test_size_0(void) {
    TEST_ASSERT_EQUAL(0, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
}

void test_message_1(void) {
    deflogger->msg("Hello World!");
    TEST_ASSERT_EQUAL(false, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
    TEST_ASSERT_EQUAL(13, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World!\n", deflogger->logger_buffer, deflogger->logger_buffer_index);
}

void test_message_2(void) {
    deflogger->msg("hello world?");
    TEST_ASSERT_EQUAL(false, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
    TEST_ASSERT_EQUAL(13 + 13, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World!\nhello world?\n\0", deflogger->logger_buffer, deflogger->logger_buffer_index);
}

void test_send_msg(void) {
    TEST_ASSERT_EQUAL_STRING("Hello World!\nhello world?\n", deflogger->logger_buffer);
    deflogger->clear_buffer();
    TEST_ASSERT_EQUAL(0, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
}

void test_no_overflow(void) {
    // needed for overflow test
    TEST_ASSERT_EQUAL(50, LOGGER_BUFFER_SIZE);
    deflogger->clear_buffer();

    deflogger->msg("this is a 50 char string which schould work fine");
    TEST_ASSERT_EQUAL(false, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
    TEST_ASSERT_EQUAL(49, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR('\n', deflogger->logger_buffer[48]);
    TEST_ASSERT_EQUAL_CHAR('\0', deflogger->logger_buffer[49]);
    TEST_ASSERT_EQUAL_STRING("this is a 50 char string which schould work fine\n", deflogger->logger_buffer);

    deflogger->clear_buffer();
    TEST_ASSERT_EQUAL(0, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
}

void test_exact_overflow(void) {
    // needed for overflow test
    TEST_ASSERT_EQUAL(50, LOGGER_BUFFER_SIZE);
    deflogger->clear_buffer();

    deflogger->msg("this is a 51char string which will cause overflow");
    TEST_ASSERT_EQUAL(false, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(true, deflogger->buffer_full());
    TEST_ASSERT_EQUAL(50, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL_CHAR('\n', deflogger->logger_buffer[48]);
    TEST_ASSERT_EQUAL_CHAR('\0', deflogger->logger_buffer[49]);
    TEST_ASSERT_EQUAL_STRING("this is a 51char string which will cause overflo\n", deflogger->logger_buffer);

    deflogger->create_overflow_message();
    TEST_ASSERT_EQUAL(false, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
    TEST_ASSERT_EQUAL_STRING("WARN: Logger buffer overflowed by 1 characters\n", deflogger->logger_buffer);

    deflogger->clear_buffer();
    TEST_ASSERT_EQUAL(0, deflogger->logger_buffer_index);
    TEST_ASSERT_EQUAL(true, deflogger->buffer_empty());
    TEST_ASSERT_EQUAL(false, deflogger->buffer_full());
}

void test_use_two_logger(void) {
    deflogger->clear_buffer();

    deflogger->msg("A");
    TEST_ASSERT_EQUAL_STRING("A\n", deflogger->logger_buffer);

    Logger* other_logger = new Logger(false);
    TEST_ASSERT_NOT_EQUAL(deflogger, other_logger);
    other_logger->msg("B");
    deflogger->msg("C");
    TEST_ASSERT_EQUAL_STRING("A\nC\n", deflogger->logger_buffer);
    TEST_ASSERT_EQUAL_STRING("B\n", other_logger->logger_buffer);

    delete other_logger;
    TEST_ASSERT_EQUAL_STRING("A\nC\n", deflogger->logger_buffer);
}

void test_set_other_to_default(void) {
    deflogger->clear_buffer();

    deflogger->msg("A");
    TEST_ASSERT_EQUAL_STRING("A\n", deflogger->logger_buffer);

    Logger* other_logger = new Logger(true);
    TEST_ASSERT_EQUAL(deflogger, other_logger);
    other_logger->msg("B");
    deflogger->msg("C");
    TEST_ASSERT_EQUAL_STRING("A\nB\nC\n", deflogger->logger_buffer);

    delete other_logger;
    TEST_ASSERT_EQUAL_STRING("A\nB\nC\n", deflogger->logger_buffer);

    deflogger->msg("D");
    TEST_ASSERT_EQUAL_STRING("A\nB\nC\nD\n", deflogger->logger_buffer);

    deflogger->clear_buffer();
    TEST_ASSERT_EQUAL(0, deflogger->logger_buffer_index);
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_size_0);
    RUN_TEST(test_message_1);
    RUN_TEST(test_message_2);
    RUN_TEST(test_send_msg);

    RUN_TEST(test_no_overflow);
    RUN_TEST(test_exact_overflow);

    RUN_TEST(test_use_two_logger);
    RUN_TEST(test_set_other_to_default);

    UNITY_END();
}

void setup() {
    delay(2000);
    process();
}

void loop() {
    delay(1000);
}
