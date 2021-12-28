# Logger for Arduino

This library is the base for logging.

It implements basic funtionality and a log buffer

All messages get added to the buffer and can then be used
The sending has to be implemented by other libraries to be compatible over various connection types (Serial, mqtt, ...), as this only defines default functions

## implementaition:
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

## change buffer size:
to change the buffer size, the following build_flag must be changed in the platform.ini environment.
The default size is 512.
```
build_flags = 
    -D LOGGER_BUFFER_SIZE=512
```
