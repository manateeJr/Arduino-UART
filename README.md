# Arduino-UART
UART reader with non-blocking delays

This is a simple project about reading the UART port on an Arduino. I'm really happy with how it turned out, because it works really well in terms of stability at speeds
up to 9600 baud. Also, using macros I made it so that you only have to change the baudrate in the BAUDRATE define (line 10) , meaning the code is easily maintainable.
To show the reading in action, I decided to print out the user input in the serial port, which you can access with a 3rd party app like PuTTy, Termite or the Arduino IDE 
built-in serial monitor. The program can read and write full unlimited strings at up to 9600 baud rate.
