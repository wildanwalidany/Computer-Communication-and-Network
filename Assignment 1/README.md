# Socket Programming and Packet Analysis

In this assignment, an experiment was conducted to study the basic concepts of `Socket Programming`using a communication channel between the client and server using the TCP protocol and `Packet Analysis` performed with Wireshark.

## Socket Programming

The [server.c](./Scripts/server.c) and [client.c](./Scripts/client.c) programs demonstrate a simple client-server architecture implemented using socket programming in C. The server acts as a centralized communication hub, waiting for client connections and responding to their requests, while the client initiates a connection, sends messages, and receives responses. Both programs rely on the `TCP protocol`, ensuring reliable data transmission between the two endpoints.

### 1. Sockets

A **socket** is an endpoint for sending or receiving data across a network. Both the [server.c](./Scripts/server.c) and the [client.c](./Scripts/client.c) create sockets to establish a communication channel.

+ **Server Socket Creation:**

``` c
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
```

+ `AF_INET`: Specifies IPv4.
+ `SOCK_STREAM`: Indicates a TCP connection,

+ **Client Socket Creation:**

``` c
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
```

### 2. Sockets

Sockets are bound to an IP address and a port to identify communication endpoints:

+ **Server:**
  + The server binds to **0.0.0.0** (`INADDR_ANY`) and listens on **port 5001**:

    ``` c
    socket_address->sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address->sin_port = htons(port_number);
    bind(server_fd, (const struct sockaddr *) server_sockaddr, server_socklen);
    ```

  + This allows the server to accept connections from any network interface.

+ **Client:**
  + The client connects to the server using **127.0.0.1** (`localhost`) and **port 5001**:

    ``` c
    server = gethostbyname("127.0.0.1");
    serv_addr.sin_port = htons(PORTNO);
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    ```

### 3. Listening and Accepting Connections

The `server` enters a listening state to wait for incoming client connections:

``` c
    if (listen(server_fd, 3) < 0)

            int client_fd = accept(server_fd, (struct sockaddr *) &client_sockaddr, &client_socklen);
```

+ `listen`: Enables the server to queue up to 3 pending connections.
+ `accept`: Accepts an incoming connection and creates a new socket (`client_fd`) for interacting with the specific client.

### 4. Establishing a Connection

The `client` initiates a connection to the server using the connect function:

``` c
    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR while connecting");
        exit(1);
    }
```

This establishes the communication channel between the client socket (`sockfd`) and the `server`.

### 5. Reading and Writing

Both programs use the `read` and `write` system calls for sending and receiving messages:

+ **Server:**

``` c
read(client_fd, buffer, buffer_len);
send(client_fd, response, strlen(response), 0);
```

+ **Client:**

``` c
write(sockfd, buffer, strlen(buffer));
read(sockfd, buffer, BUFFER_SIZE - 1);
```

### 6. Sample Execution

The [MakeFile](./Scripts/Makefile) automates the compilation of the server.c and client.c programs. It contains a single rule, `all`, which compiles both source files into their respective executables.

![Make](./Images/make.png)

+ **Output:**

![Server](./Images/server.png)
![Client](./Images/client.png)

the connection between client and server is monitored and captured on loopback interface using wireshark and saved in [capture](./Captured_Packets/capture.pcapng) for further analysis below

## Packet Analysis



## Resources

+ [C Socket Programming: Simple Server and Client](<https://github.com/ferryastika/socket-programming-simple-server-and-client?tab=readme-ov-file>)
