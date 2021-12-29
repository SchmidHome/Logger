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
    Communication.print(logger_buffer);             //   print logged messages
    if (buffer_full()) {                            //   check if buffer had overflow
        create_overflow_message();                  //     clear buffer and create overflow message 
        Communication.print(logger_buffer);         //     print overflow message
    }                                               //
    clear_buffer();                                 //   clear buffer as all content is sent
}
```
In Case the communication print function can fail use:
```c++
if (!buffer_empty() && Communication.available()) { // check if new message and connection are available
    if (Communication.print(logger_buffer)) {       //   print logged messages, continue on success
        if (buffer_full()) {                        //     check if buffer had overflow
            create_overflow_message();              //       clear buffer and create overflow message 
            if (Communication.print(logger_buffer)) //       print overflow message, continue on success
                clear_buffer();                     //         clear buffer as overflow message is sent    
            else                                    //  
                WARN("Communication failed")        //         warn that communication failed
        } else                                      //  
            clear_buffer();                         //       clear buffer as all content is sent
    } else                                          //  
        WARN("Communication failed")                //     warn that communication failed
}
```
If it is enougth for the next loop to send the overflow message, you can also use this:
```c++
if (!buffer_empty() && Communication.available()) { // check if new message and connection are available
    if (Communication.print(logger_buffer))         //   print logged messages, continue on success
        if (buffer_full())                          //     check if buffer had overflow
            create_overflow_message();              //       clear buffer and create overflow message
                                                    //       message will be sent on the next call of this function
        else                                        // 
            clear_buffer();                         //       clear buffer as all content is sent
    else                                            //    
        WARN("Communication failed")                //     warn that communication failed
}
```

## change buffer size
to change the buffer size, the following build_flag must be changed in the platform.ini environment.
The default size is 512.
```
build_flags = 
    -D LOGGER_BUFFER_SIZE=512
```
