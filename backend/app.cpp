#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    hostname[255] = '\0';

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Server listening on port 8080 (hostname: " << hostname 
<< ")" << std::endl;

    while(true) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) continue;

        std::string body = "Served by backend: " + std::string(hostname) + 
"\n";

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            body;

        write(client_fd, response.c_str(), response.size());
        close(client_fd);
    }

    return 0;
}
