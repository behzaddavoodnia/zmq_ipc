#include "zmq_basic.h"

static void init_prog(prog_name_t* prog){
    
    prog->tcp_socket_addr =     TCP_SOCKET;
    prog->ipc_socket_addr =     IPC_SOCKET;
    prog->inproc_socket_addr =  INPROC_SOCKET;
    prog->ready = true;
    prog->id = 0;
    prog->message = strdup("Hello!");
    prog->name = strdup("process0");
    prog->contex = NULL;

}



static int init_socket(prog_name_t* prog){

    prog->contex = zmq_ctx_new();
    if(prog->contex == NULL){
        printf ("init_socket on contex creation error: %s\n", zmq_strerror(errno));
        return -1;
    }
    return 0;

}

static int create_out_socket(prog_name_t* prog, char* connection_mode){

    prog->socket = zmq_socket(prog->contex, ZMQ_DEALER);

    if (prog->socket == NULL){
        printf ("create out zmq_socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_connect(prog->socket, prog->ipc_socket_addr) <0){
            printf ("create ipc connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    if (!strcmp(connection_mode, "inproc")){
        if(zmq_connect(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("create inproc connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    if (!strcmp(connection_mode, "tcp")){
        if (zmq_connect(prog->socket, prog->tcp_socket_addr) <0){
            printf ("create tcp connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    return 0;

}

static int create_in_socket(prog_name_t* prog, char* connection_mode){

    prog->socket = zmq_socket(prog->contex, ZMQ_DEALER);
    
    if (prog->socket == NULL){
        printf ("create in zmq_socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_bind(prog->socket, prog->ipc_socket_addr) < 0){
            printf ("create ipc bind socket error: %s\n", zmq_strerror(errno));
            return -1;
        }    
    }

    if (!strcmp(connection_mode, "inproc")){
        if(zmq_bind(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("create inproc bind socket error: %s\n", zmq_strerror(errno));
            return -1;}
    }

    if (!strcmp(connection_mode, "tcp")){
        if (zmq_bind(prog->socket, prog->tcp_socket_addr) < 0){
            printf ("create tcp bind socket error: %s\n", zmq_strerror(errno));
             return -1;
        }
    }
    return 0;

}


static int destroy_out_socket(prog_name_t* prog, char* connection_mode){

   
    if (prog->socket == NULL){
        printf ("destroy zmq out-socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_disconnect(prog->socket, prog->ipc_socket_addr) <0){
            printf ("destroy ipc connect socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "inproc")){
        if(zmq_disconnect(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("destroy inproc connect socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "tcp")){
        if (zmq_disconnect(prog->socket, prog->tcp_socket_addr) <0){
            printf ("destroy tcp connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    return 0;

}

static int destroy_in_socket(prog_name_t* prog, char* connection_mode){


    if (prog->socket == NULL){
        printf ("destroy zmq in_socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_unbind(prog->socket, prog->ipc_socket_addr) < 0){
            printf ("destroy ipc bind socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "inproc")){
        if(zmq_unbind(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("destroy inproc bind socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "tcp")){
        if (zmq_unbind(prog->socket, prog->tcp_socket_addr) < 0){
            printf ("destroy tcp bind socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    return 0;

}


static int send_message(prog_name_t* prog, char* msg, int msg_len, bool multi_part_msg){

    int flag = -1;

    if (multi_part_msg)
        flag = ZMQ_SNDMORE;
    else
        flag = 0;

    if (zmq_send(prog->socket, msg, msg_len, flag) < 0){
        printf ("send error: %s\n", zmq_strerror(errno));
        return -1;
    }
    printf("Sent message: %s\n", msg);

    return 0;
}

static int recv_message(prog_name_t* prog, char* buf, int msg_len, int flag){
   
    int _flag = -1;
    
    if (flag == 0)  _flag = 0;
    else if (flag == 1)  _flag = ZMQ_RCVMORE;
    else if (flag == 2)  _flag = ZMQ_NOBLOCK;
    else{
        perror("Invalid flag argument!\n");
        exit(EXIT_FAILURE);
    }
        

    if (zmq_recv(prog->socket, buf, msg_len, flag) < 0){
        printf ("recv error: %s\n", zmq_strerror(errno));
        return -1;
    }
    
    printf("Received message: %s\n", buf);
    return 0;
    
}

static int term_socket(prog_name_t* prog){
    
    zmq_close(prog->socket);
    int term = zmq_ctx_destroy (prog->contex);
    if(term < 0)
        printf ("socket contex termination error: %s\n", zmq_strerror(errno));

    
    return 0;
}



int main(){
system("mkdir "SOCKET_DIR);
    prog_name_t* p1 = (prog_name_t*) calloc (100, sizeof(prog_name_t));
    init_prog(p1);
    init_socket(p1);
    create_out_socket(p1, "ipc");
    create_out_socket(p1, "tcp");
    p1->message = strdup("Only God can judge me!");
    char buffer [50] = "";
    send_message(p1, p1->message, strlen(p1->message), false);

    prog_name_t* p2 = (prog_name_t*) calloc (100, sizeof(prog_name_t));
    init_prog(p2);
    init_socket(p2);
    create_in_socket(p2, "ipc");
    create_in_socket(p2, "tcp");
    recv_message(p2, buffer, sizeof buffer, 0);
    
    //term_socket(my_prog);
    safe_free(p1);
    safe_free(p2);

    return 0;
}
