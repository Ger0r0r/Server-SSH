# Server-SSH

This program is presented in the form of two parts: client and server.

Realization:

The server is forked into two processes:
1) one is waiting for a broadcast special message, upon receipt it extracts information from the message and passes it to the second.
2) the second of the received information starts the child process (corresponding session).

Before the session starts, the following actions take place:
1) the child process and the client learn about each other (before that, the client was waiting for the creation of the child process).
2) using the Diffie-Hellman algorithm, private keys for symmetric encryption will be created.
3) through a secure channel (from replacing by default, from viewing due to encryption), you enter the system account.
4) the regular session continues.

During the session, the user has full non-graphical access to the terminal, as well as the ability to transfer files from the server to his device.

In addition to the above actions, the server knows in advance all initialized users, maintains an input/output history, a chronology of events, and records all errors in a log file.