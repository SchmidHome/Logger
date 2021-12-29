#pragma once

#ifndef LOGGER_BUFFER_SIZE
#define LOGGER_BUFFER_SIZE 512
#endif

#define ASSERT_ERR(condition, message) \
    if (!(condition)) deflogger.err(__BASE_FILE__ + (String) " at " + __LINE__ + ": " + message)
#define ASSERT_WARN(condition, message, execute)                                     \
    if (!(condition)) {                                                              \
        deflogger.warn(__BASE_FILE__ + (String) " at " + __LINE__ + ": " + message); \
        execute;                                                                     \
    }

#define MSG(message) deflogger.msg(message)
#define WARN(message) deflogger.warn(message)
#define ERR(message) deflogger.err(message)

extern char logger_buffer[LOGGER_BUFFER_SIZE];
extern uint16_t logger_buffer_index;

class Logger {
   public:
    virtual void msg(String message) { add_to_buffer(message); };
    virtual void warn(String message) { add_to_buffer("WARN: " + message); };
    virtual void err(String message) { add_to_buffer("ERR: " + message); };

   protected:
    void add_to_buffer(String message);
    void clear_buffer();
    void create_overflow_message();
    inline bool buffer_full() { return logger_buffer_index >= LOGGER_BUFFER_SIZE; };
    inline bool buffer_empty() { return logger_buffer_index == 0; };
#ifdef UNIT_TEST
    friend void test_size_0(void);
    friend void test_message_1(void);
    friend void test_message_2(void);
    friend void test_send_msg(void);
    friend void test_no_overflow(void);
    friend void test_exact_overflow(void);
    friend void process();
#endif
};

extern Logger &deflogger;  // default logger
