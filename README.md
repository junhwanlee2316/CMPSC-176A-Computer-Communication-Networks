# Computer Communication Networks

## UCSB CMPSC 176A

## Overview

Implemented TCP and UDP server and client using C from simple application to more complicated application.

## Projects

1. **TCP**
   - Simple client and server establishing TCP connection
2. **UDP**
   - Simple UDP client and server
3. **Ping-Client**
   - Implements a simple ping program using UDP to send packets to a specified server and measure round-trip times (RTTs).
   - Sends 10 ping requests to the server, calculates RTT for each response, and calculates statistics such as packet loss percentage, minimum RTT, average RTT, and maximum RTT.
4. **Hangman**
   - Contains a client-side program for a simple hangman game. It connects to a server, communicates using TCP sockets, sends guesses for letters, and receives updates on the game state until the game ends.
   - Implementation of a server-side program for the hangman game. It listens for connections on a specified port, handles multiple client connections concurrently using threads, manages the hangman game logic, and communicates with clients to send game state updates and receive guesses
