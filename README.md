# fundamentals_of_networking
Simple client-server app

Client-Server Application for Linux that allows several clients to connect to server over TCP and send messages.

Client and Server can exchange multiple messages. Server can serve multiple clients at once (using fork()).

If the client sends 'hello' message, the server responds 'world'.
If the client sends 'disconnect' message, the server and client stop connection.
If any other message is sent, the server send it back without changing it.

There is CMakeLists.txt file that builds the project.

You need to specify whether you run "server" or "client" as a program argument:
./main <argument>

