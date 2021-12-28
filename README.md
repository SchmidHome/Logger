# Logger for Arduino

This library is the base for logging.

It implements basic funtionality and a log buffer

All messages get added to the buffer and can then be used
The sending has to be implemented by other libraries to be compatible over various connection types (Serial, mqtt, ...), as this only defines default functions

## usage
### ASSERT_ERR(condition, message)
if condition ist true *message* will be logged (deflogger.err is used)
ASSERT_ERR(buffer_full(), "Buffer is full")
### ASSERT_WARN(condition, message, execute)
if condition ist true *message* will be logged and *execute* will be executed (deflogger.warn is used)
ASSERT_WARN(buffer_full(), "Buffer is full", clear_buffer())
### MSG(message)
used to print messages using the default logger
### WARN(message)
used to print warnings using the default logger
### ERR(message)
used to print errors using the default logger

## implementaition
change *Communication* to own communication method
```c++
if (!buffer_empty() && Communication.available()) { // check if new message and connection are available
        finish_buffer();                                // finish buffer with \n and \0 at end
        Communication.print(logger_buffer);             // print logged messages
        if (buffer_full()) {                            // check if buffer had overflow
            create_overflow_message();                      // create overflow message 
            Communication.print(logger_buffer);             // print overflow message
        }
        clear_buffer();                                 // clear buffer as all content is sent
    }
```

## change buffer size
to change the buffer size, the following build_flag must be changed in the platform.ini environment.
The default size is 512.
```
build_flags = 
    -D LOGGER_BUFFER_SIZE=512
```
