#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <progbase/net.h>
#include "include/clientList.h"
#include "include/response.h"
#include <jansson.h>

#define BUFFER_LEN 10000

void parseRequest(char *str);

int main(int argc, char * argv[]) {
    Songs *songs = SongsNewFict();
    SongsAddLast(songs, SongsNew(1, "Wind of change", "Scorpions"));
    SongsAddLast(songs, SongsNew(2, "Show must go on", "Queen"));
    SongsAddLast(songs, SongsNew(3, "Believer", "Imagine dragons"));
    SongsAddLast(songs, SongsNew(1, "Stressed out", "Twenty one pilots"));
	const int port = 2000;

	TcpListener * server = TcpListener_init(&(TcpListener){});
    IpAddress * address = IpAddress_initAny(&(IpAddress){}, port);
    if(!TcpListener_bind(server, address)) {
        perror("tcp bind");
        return 1;
    }
    if (!TcpListener_start(server)) {
        perror("tcp server start");
        return 1;
    }
    printf("TCP Server is listening on port %d\n", 
        IpAddress_port(TcpListener_address(server)));
    
    NetMessage * message = NetMessage_init(
        &(NetMessage){},  // value on stack
        (char[BUFFER_LEN]){},  // array on stack 
        BUFFER_LEN);
	//
    // to store information about current client
    TcpClient client;
    while (1) {
        puts(">> Waiting for connection...");
		//
        // wait for someone to connect to server
        TcpListener_accept(server, &client);
        // wait for data from client
        if(!TcpClient_receive(&client, message)) {
			perror("recv");
			return 1;
		}
        IpAddress * clientAddress = TcpClient_address(&client);
        printf(">> Received message from %s:%d (%d bytes): `%s`\n",
            IpAddress_address(clientAddress),  // client IP-address
            IpAddress_port(clientAddress),  // client port
            NetMessage_dataLength(message),
            NetMessage_data(message));
        char *resStr = (char*)NetMessage_data(message);
		parseRequest(resStr);
        processResponse(resStr, songs, port);
        // send data back
        
        if(!TcpClient_send(&client, NetMessage_setDataString(message,resStr))) {
			perror("send");
			return 1;
		}
        printf(">>String sent to client:\r\n%s\r\n",NetMessage_data(message));
        // close tcp connection
        TcpClient_close(&client);
    }
    // close listener
    TcpListener_close(server);
	return 0;   
}

void parseRequest(char *str){
    strcpy(str, strstr(str, "/"));
    for (int i = 0; i < strlen(str); i++) {
        if(isspace(str[i]) && i != 0) {
            str[i] = '\0';
        }
    }
}
