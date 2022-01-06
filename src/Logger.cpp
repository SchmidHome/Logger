#include <Logger.h>

Logger _DefaultLogger(true);
Logger *deflogger = &_DefaultLogger;

char deflogger_buffer[LOGGER_BUFFER_SIZE];

// -------------------------------------------------------------------------------- Logger
Logger::Logger(bool set_as_default) : logger_buffer(nullptr), logger_buffer_index(0) {
    if (set_as_default) {
        logger_buffer = deflogger_buffer;
        if (&_DefaultLogger == this)
            return;
        Serial.println("Logger::Logger(bool set_as_default) A");
        logger_buffer_index = deflogger->logger_buffer_index;
        if (deflogger == &_DefaultLogger) {
            // unassign _DefaultLogger
            deflogger->logger_buffer = new char[1];
        } else {
            // unassign other logger
            deflogger->logger_buffer = new char[LOGGER_BUFFER_SIZE];
        }
        deflogger->logger_buffer_index = 0;
        deflogger = this; 
    } else {
        logger_buffer = new char[LOGGER_BUFFER_SIZE];
        logger_buffer_index = 0;
    }
}

Logger::~Logger() {
    if (deflogger != this) {
        delete[] logger_buffer;
    } else if (&_DefaultLogger != this) {
        // reassign _DefaultLogger
        delete[] _DefaultLogger.logger_buffer;
        _DefaultLogger.logger_buffer = logger_buffer;
        _DefaultLogger.logger_buffer_index = logger_buffer_index;
        deflogger = &_DefaultLogger;
    }
}

void Logger::unassign() {
    if (&_DefaultLogger == this)
        logger_buffer = new char[1];
    else
        logger_buffer = new char[LOGGER_BUFFER_SIZE];

    logger_buffer_index = 0;
}

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
