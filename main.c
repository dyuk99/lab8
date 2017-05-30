#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <progbase/net.h>
#include "include/clientList.h"
#include <jansson.h>

#define BUFFER_LEN 10000

char *parseRequest(char *str);
void processResponse(char *str, Songs*Songs);

int main(int argc, char * argv[]) {
    Songs *songs = SongsNewFict();
    SongsAddLast(songs, SongsNew(1, "Wind of change", "Scorpions"));
    SongsAddLast(songs, SongsNew(2, "Show must go on", "Queen"));
    SongsAddLast(songs, SongsNew(3, "Believer", "Imagine dragons"));
    SongsAddLast(songs, SongsNew(1, "Stressed out", "Twenty one pilots"));
	const int port = 3000;

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
        processResponse(resStr, songs);
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

char* readFromFileToString(char* buffer) {
	FILE *fin;
	char name[30] = "data/data.txt";
	fin = fopen(name,"r");
	char buf[1000] = "";
	if (fin == NULL)
	{
		printf("Error opening file %s\n", name);
	}
	else
	{
		while (!feof(fin))
		{
            if(fgets(buf, 1000, fin)) {
                strcat(buffer, buf);
                buffer[strlen(buffer) - 1] = '\0';
            }	
		}
	}
    //buffer[strlen(buffer)/2] = '\0';
	fclose(fin);
    printf("**%s**", buffer);
	return buffer;
}

char *parseRequest(char *str){
    strcpy(str, strstr(str, "/"));
    for (int i = 0; i < strlen(str); i++) {
        if(isspace(str[i]) && i != 0) {
            str[i] = '\0';
        }
    }
    return str;
}

void processResponse(char *str, Songs*songs){
    if (strstr("/", str) && strlen("/") == strlen(str)) {
        json_t * ar = json_array();
	    // for (int i = 0; i < 1; i++)
        // {
            json_t * json = json_object();
            json_object_set_new(json, "titleab", json_string("dyukServer"));
            json_object_set_new(json, "developer", json_string("Dyuk"));
            json_object_set_new(json, "time", json_string("serverTime"));
            json_array_append(ar, json);
        // }
        char * jsonString = json_dumps(ar, JSON_INDENT(2));
        strcpy(str, (char *)jsonString);
        free(jsonString);
        json_decref(ar);
    }
    else if (strstr(str, "/favourites?")) {
        char key[100] = "";
        strcpy(key, strstr(str, "{")+1);
        char value[100] = "";
        strcpy(value, strstr(key, "{")+1);
        for (int i = 0; i < strlen(key); i++) {
            if (key[i] == '}') {
                key[i] = '\0';
                break;
            }
        }
        value[strlen(value)-1] = '\0';
        json_t * ar = json_array();
	    for (int i = 0; i < countSongs(songs); i++)
        {
            Songs *node = SongsGetNode(songs, i);
            if (strcmp(SongGetPerformerName(node),value) == 0) {
                json_t * json = json_object();
                json_object_set_new(json, "name", json_string(SongGetName(node)));
                json_object_set_new(json, "performer", json_string(SongGetPerformerName(node)));
                json_object_set_new(json, "id", json_integer(*SongGetPerformerId(node)));
                json_array_append(ar, json);
            }
        }
        char * jsonString = json_dumps(ar, JSON_INDENT(2));
        strcpy(str, (char *)jsonString);
        free(jsonString);
        json_decref(ar);
    }
    else if (strstr(str, "/favourites/")) {
        char id[100] = "";
        strcpy(id, strstr(str, "{")+1);
        for (int i = 0; i < strlen(id); i++) {
            if (id[i] == '}') {
                id[i] = '\0';
                break;
            }
        }
        bool isId = false;
        for (int i = 0; i < countSongs(songs); i++) {
            Songs *node = SongsGetNode(songs, i);
            if (atoi(id) == *SongGetPerformerId(node)){
                isId = true;
                json_t * ar = json_array();
                // for (int i = 0; i < 1; i++)
                // {
                    json_t * json = json_object();
                    json_object_set_new(json, "name", json_string(SongGetName(node)));
                    json_object_set_new(json, "performer", json_string(SongGetPerformerName(node)));
                    json_object_set_new(json, "id", json_integer(*SongGetPerformerId(node)));
                    json_array_append(ar, json);
                // }
                char * jsonString = json_dumps(ar, JSON_INDENT(2));
                strcpy(str, (char *)jsonString);
                free(jsonString);
                json_decref(ar);
                break;
            }
        }
        if (!isId) {
            strcpy(str, "404 Not Found");
        }
    }
    else if (strstr(str, "/data") && strlen("/data") == strlen(str)) {
        char buffer[10000] = "";
        readFromFileToString(buffer);
        json_t * ar = json_array();
	    // for (int i = 0; i < 1; i++)
        // {
            json_t * json = json_object();
            json_object_set_new(json, "name", json_string("data.txt"));
            json_object_set_new(json, "size(bytes)", json_integer(strlen(buffer)));
            json_object_set_new(json, "data", json_string(buffer));
            json_array_append(ar, json);
        // }
        char * jsonString = json_dumps(ar, JSON_INDENT(2));
        strcpy(str, (char *)jsonString);
        free(jsonString);
        json_decref(ar);
    }
    else if (strstr(str, "/file/data") && strlen("/file/data") == strlen(str)){
        char buffer[10000] = "";
        readFromFileToString(buffer);
        int min = atoi(buffer);
        int count = 1;
        for (int i = 0; i < strlen(buffer); i++) {
            if (isspace(buffer[i])) {
                int number = atoi(buffer+i+1);
                if (number < min) min = number;
                count++;
            }
        }
        json_t * ar = json_array();
	    // for (int i = 0; i < 1; i++)
        // {
            json_t * json = json_object();
            json_object_set_new(json, "name", json_string("data.txt"));
            json_object_set_new(json, "min number", json_integer(min));
            json_object_set_new(json, "quantity of numbers", json_integer(count));
            json_array_append(ar, json);
        // }
        char * jsonString = json_dumps(ar, JSON_INDENT(2));
        strcpy(str, (char *)jsonString);
        free(jsonString);
        json_decref(ar);
    }
}