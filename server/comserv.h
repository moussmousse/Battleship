/*    protoserv.h
 * All functions and includes required for network
 */




typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PORT 1977
#define MAX_CLIENTS 100

#define BUF_SIZE 1024


int init_connection(void);
