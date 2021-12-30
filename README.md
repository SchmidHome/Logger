# Logger for Arduino env

This library is the base for logging.

It implements basic functionality and a log buffer

All messages get added to the buffer and can then be used.
The sending has to be implemented by other libraries to be compatible over various connection types (Serial, mqtt, ...), as this only defines default functions.

# USAGE
to log messages you can use one of these defined ways:
```c++
MSG("this message will be added to the buffer") 
WARN("ths message will begin with 'WARN: '")
ERR("ths message will begin with 'ERR: '")
// asserts check if the condition is not met, show the filename, line number and message. WARN can additionally execute something
ASSERT_ERR(example_buffer_full() == false, "Buffer is full, show as error")
ASSERT_WARN(example_buffer_full() == false, "Buffer is full, show as warning and clear buffer automatically", clear_example_buffer())
```

# IMPLEMENTATION
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
        WARN("Communication failed");               //     warn that communication failed
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
        WARN("Communication failed");               //     warn that communication failed
}
```

# CONSTANTS
## change buffer size
to change the buffer size, the following build_flag must be changed in the platform.ini environment.
The default size is 512.
```
build_flags = 
    -D LOGGER_BUFFER_SIZE=512
```
