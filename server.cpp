#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define sz(a) ((int)((a).size()))
#define BACKLOG 10
#define PORT 8080
 
//using namespace std;
// mt19937 rnd(239);

typedef long long ll;
typedef long double ld;
 

 
int32_t main() {
    int server_fd,new_socket,val_read;
    struct sockaddr_in address;// viene del include netinet/in.h
    int option = 1;
    int addr_len = sizeof(address);
    char buffer[1024] = {0};
    char* msg = "Hello from server"; //Se usa esto en lugar de string porque esto es C no C++
    
    printf("Creando el socket\n");
    // Creamos el socket hard code la familia el protocolo etc pero se puede hacer dinamico
    server_fd = socket(AF_INET, SOCK_STREAM, 0);// El socket,AF_INET y el Sock_stream vienen del sys/socket.h header

    /*
     * la funcion del socket acepta un dominio, el tipo y el protocolo, sirve para identificar el protocolo si es tcp/udp, data stream, etc, AF_INET se refiere address family pero es mas correcto usar PF_INET, el tipo es si es sock_stream (comunicacion continua) o sock_dagram (que no tienen que estar necesariamente conectados) Se puede colocar por hard code, pero lo recomendable es utilizar el getaddres info para poblar esta funcion. el protocolo de 0 permite a el eligir la mejor opcion segun los datos dados (tcp o upd).
     * */

    if (server_fd < 0) {
        perror("Socket connection failed");
        return 1;
    }
    printf("Todo correcto con el set up del socket\n");
    // forzar asignacion del puerto al socket es opcional esta parte
    
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &option,
                   sizeof(option))) {
        perror("setsockopt error");
        return 1;
    }
    // Anteriormente se hacia manualmente la asignancion en el struct sockaddr_in
    address.sin_family = AF_INET; // puede ser AF_UNSPEC para dejarlo dinamico
    address.sin_addr.s_addr = INADDR_ANY; // structura internet address que contiene la direccion del socket de internet ese valor asignado indica que debe tomar el ip de la computadora a la cual esta corriendo el programa
    address.sin_port = htons(PORT); // asignar puerto

    /*
     *Al bind se le pasa el socket file descriptor una struct de tipo sockaddr (puede ser sin_addr que estamos usando) o se puede automatizar con la funcion getaddrinfo, el ultimo argumento en el lenght del ip
     * */
    // Vieja forma de hacerlo
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        return 1;
    }
   
    // Luego del bind dejamos al socket como listen() para escuchar conexiones entrantes
    // los parametros son el socket file descriptor y el numero de conexiones entrantes que es escuchara
     if (listen(server_fd, 5) < 0) {
        perror("en el listen");
        return 1;
    }
    printf("server: waiting for connections...\n");
    /*
     *El accept te crea un nuevo socket el cual se utilizara para enviar y recibir datos mientras que el anterior esta en espera esperando conexiones, el accept acepta el socket en listen mode, la struck sockaddr donde guarda la informacion de la conexion entrante y el size de esta struct sockaddr
     se puede usar el sockaddr_in o crear la struct sockaddr_storage para guardar la data
     * */
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t *)  &addr_len);

    if (new_socket < 0) {
        perror("Error en el accept");
        return 1;
    }
    // Entra el socket que recibe, el buffer y su tamano y flag es 0, si es -1 hay error, si es 0 no hay conexion
    if ( (val_read = recv(new_socket, buffer, 1024, 0)) == -1 ) {
        perror("Error recibiendo datos del client");
        return 1;
    }
    printf("received %s\n",buffer);
    if (send(new_socket, msg, strlen(msg), 0) == -1 ) {
        perror("Error al send");
        return 1;
    }
    printf("mensaje enviado al client");
    // Cerramos el socket conectado
    close(new_socket);
    // Shutdown para evitar mas leidas y envios al socket principal
    shutdown(server_fd, SHUT_RDWR);

    /*
     * Leemos el valor si hay de la conexion, en algunos casos la funcion read es igual a recv
     * usar esta mejor para probar.
     *valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    // Enviamos un mensaje desde el server
    // los parametros son el socket que estas trabajando, puede ser el que esta en listen o el accept
    // el mensaje que quieres enviar (char o string) el size de ese mensaje en bytes y las flags que por
    // lo general son 0
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
  
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
     * */

    return 0;
}
