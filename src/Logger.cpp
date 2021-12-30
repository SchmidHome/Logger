#include <Logger.h>

Logger _DefaultLogger;
Logger &deflogger = _DefaultLogger;

char logger_buffer[LOGGER_BUFFER_SIZE];
uint16_t logger_buffer_index = 0;

// -------------------------------------------------------------------------------- Logger
void Logger::add_to_buffer(String message) {
    if (logger_buffer_index + 1 < LOGGER_BUFFER_SIZE) {
        // copy message to buffer
        message.toCharArray(&logger_buffer[logger_buffer_index], LOGGER_BUFFER_SIZE - logger_buffer_index - 1);
        // set \n
        if (logger_buffer_index + message.length() < LOGGER_BUFFER_SIZE - 2)
            logger_buffer[logger_buffer_index + message.length()] = '\n';
        else
            logger_buffer[LOGGER_BUFFER_SIZE - 2] = '\n';
        logger_buffer_index++;
        // set \0
        if (logger_buffer_index + message.length() < LOGGER_BUFFER_SIZE - 1)
            logger_buffer[logger_buffer_index + message.length()] = '\0';
        else
            logger_buffer[LOGGER_BUFFER_SIZE - 1] = '\0';
    }
    // always increment index to show buffer overflow amount
    logger_buffer_index += message.length();
};

void Logger::clear_buffer() {
    logger_buffer_index = 0;
};

void Logger::create_overflow_message() {
    uint16_t tmp = logger_buffer_index;
    clear_buffer();
    warn("Logger buffer overflowed by " + String(tmp + 1 - LOGGER_BUFFER_SIZE) + " characters");
};
