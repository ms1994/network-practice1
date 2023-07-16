#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080


int main (int argc,char* argv[]) {
    
    
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    // Hacemos la conexion al socket y lo guardamos en el client_fd
     if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\n Socket creation error \n");
        return -1;
    }

     /*
      *Cuando no se usa el getaddrinfo se hace manual el set up de los address
      * */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
  
    // Convert IPv4 and IPv6 addresses from text to binary
    // form y lo guarda en el internet address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        perror(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    // COnectamos y vemos si ocurrio algun error
    if ((status
         = connect(client_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        close(client_fd);
        perror("\nConnection Failed \n");
        return -1;
    }
    // Eviamos un msg al server
    send(client_fd, hello, strlen(hello), 0);
    printf("mensaje hello enviado al server\n");
    valread = read(client_fd, buffer, 1024);
    printf("Buffer es %s\n", buffer);
    close(client_fd);

    return 0;
}
