#ifndef ZMQ_BASIC_H
#define ZMQ_BASIC_H

/* UDP is not supported by 0MQ. Also, IPC is only supported on OSes which
 have a POSIX-conforming implementation of named pipes; so, on Windows,
 you can really only use 'inproc' or 'tcp' for endpoints.
*/

/*ZMQ_REQ does not throw away any messages. If there are no available services to send
the message or if the all services are busy, all send operations i.e. zmq_send(), are
blocked until a service becomes available to send the message*/


// System headers
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <msgpack.h>
#include <zmq.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <sys/time.h>


// Deallocating a memory pointed by ptr causes dangling and non-wild pointer
#ifndef safe_free
#define safe_free(ptr) ({free(ptr); ptr=NULL;})
#endif
 

// +----------------------- Socket Paths ------------------------+ //

#define SOCKET_DIR      			"/tmp/feeds/"
#define IPC_SOCKET			        "ipc:///tmp/feeds/0"
#define IPC_SOCKET_INPUT		    "ipc://%s"
#define TCP_SOCKET		            "tcp://0.0.0.0:5555"
#define TCP_SOCKET_INPUT	        "tcp://%s:%s"
#define TCP_SOCKET_ALL	            "tcp://*:5555"
#define INPROC_SOCKET               "inproc://my-inproc-endpoint"
#define INPROC_SOCKET_INPUT			"inproc://%s"

#define MAX_SIZE_OF_NAME            256


typedef struct _prog_name_t
{
    char* name;
    char* message;
    char* ipc_socket_addr;
    char* inproc_socket_addr;
    char* tcp_socket_addr;
    unsigned id;
    void* contex;
    void* socket;
    bool ready;
    
}prog_name_t;




static void init_prog(prog_name_t* prog);

static int init_socket(prog_name_t* prog);


static int create_out_socket(prog_name_t* prog, char* connection_mode);

static int create_in_socket(prog_name_t* prog, char* connection_mode);

static int destroy_out_socket(prog_name_t* prog, char* connection_mode);

static int destroy_in_socket(prog_name_t* prog, char* connection_mode);

static int send_message(prog_name_t* prog, char* msg, int msg_len, bool multi_part_msg);

static int recv_message(prog_name_t* prog, char* msg, int msg_len, int flag);

static int term_socket(prog_name_t* prog);


#endif // !ZMQ_BASIC_H