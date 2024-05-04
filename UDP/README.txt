CS176A HW 2 Programming
Code Instructions:
Junhwan Lee
4/28/2023


TCP(server_c_tcp.c & client_c_tcp.c):
    1. Server creates a socket and bind to the designated port number.
    2. Server then begins listening.
    3. Client creates a socket and connect to the server after checking if the port is on.
    4. Server accepts the client and creates a new socket. Establish a connection.
    5. User inputs a string in client and the client writes the string into the buffer.
    6. Server reads the buffer.
    7. If a valid string of integers, server computes the sum of digits and writes each sum for the client to read.
    8. Repeat until client reads a single digit string.
    9. Close the client socket.

UDP(server_c_udp.c & server_c_udp.c):
    1. Server creates a socket and bind to the designated port number.
    2. Client creates a socket and send the string to the desginated port number and server ip address.
    3. If the server receives the client message, server computes the sum and send the sum values to client.
    4. Client is constantly polling for any incoming messages from server and if received, write the string.
    5. Client closes socket after last digit is received. 