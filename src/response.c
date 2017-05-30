#include "../include/response.h"

void getServerInfo(char *str, const int port) {
    long int ttime;
    ttime = time (NULL);
    char *timeStr = ctime (&ttime);
    timeStr[strlen(timeStr)-1] = '\0';
    json_t * ar = json_array();
    json_t * json = json_object();
    char name[100] = "127.0.0.1:";
    sprintf(name+strlen(name), "%i", port);
    json_object_set_new(json, "title", json_string(name));
    json_object_set_new(json, "developer", json_string("Dyukarev Oleksandr"));
    json_object_set_new(json, "time", json_string(timeStr));
    json_array_append(ar, json);
    char * jsonString = json_dumps(ar, JSON_INDENT(2));
    strcpy(str, (char *)jsonString);
    free(jsonString);
    json_decref(ar);
}

void getListOfSongs(char *str, Songs *songs) {
    json_t * ar = json_array();
    for (int i = 0; i < countSongs(songs); i++)
    {
        Songs *node = SongsGetNode(songs, i);
        json_t * json = json_object();
        json_object_set_new(json, "name", json_string(SongGetName(node)));
        json_object_set_new(json, "performer", json_string(SongGetPerformerName(node)));
        json_object_set_new(json, "id", json_integer(*SongGetPerformerId(node)));
        json_array_append(ar, json);    
    }
    char * jsonString = json_dumps(ar, JSON_INDENT(2));
    strcpy(str, (char *)jsonString);
    free(jsonString);
    json_decref(ar);
}

void getFilteredListOfSongs(char *str, Songs *songs) {
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

void getSongById(char *str, Songs *songs) {
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
            json_t * json = json_object();
            json_object_set_new(json, "name", json_string(SongGetName(node)));
            json_object_set_new(json, "performer", json_string(SongGetPerformerName(node)));
            json_object_set_new(json, "id", json_integer(*SongGetPerformerId(node)));
            json_array_append(ar, json);
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

void getFileInfo(char *str) {
    char buffer[10000] = "";
    readFromFileToString(buffer);
    json_t * ar = json_array();
    json_t * json = json_object();
    json_object_set_new(json, "name", json_string("data.txt"));
    json_object_set_new(json, "size(bytes)", json_integer(strlen(buffer)));
    json_object_set_new(json, "data", json_string(buffer));
    json_array_append(ar, json);
    char * jsonString = json_dumps(ar, JSON_INDENT(2));
    strcpy(str, (char *)jsonString);
    free(jsonString);
    json_decref(ar);
}

void getFileData(char *str) {
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
    json_t * json = json_object();
    json_object_set_new(json, "name", json_string("data.txt"));
    json_object_set_new(json, "min number", json_integer(min));
    json_object_set_new(json, "quantity of numbers", json_integer(count));
    json_array_append(ar, json);
    char * jsonString = json_dumps(ar, JSON_INDENT(2));
    strcpy(str, (char *)jsonString);
    free(jsonString);
    json_decref(ar);
}

void readFromFileToString(char* buffer) {
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
	fclose(fin);
}

void processResponse(char *str, Songs*songs, const int port) {
    if (strstr("/", str) && strlen("/") == strlen(str)) {
        getServerInfo(str, port);
    }
    else if (strstr(str, "/favourites") && strlen("/favourites") == strlen(str)) {
        getListOfSongs(str, songs);
    }
    else if (strstr(str, "/favourites?")) {
        getFilteredListOfSongs(str, songs);
    }
    else if (strstr(str, "/favourites/")) {
        getSongById(str, songs);
    }
    else if (strstr(str, "/data") && strlen("/data") == strlen(str)) {
        getFileInfo(str);
    }
    else if (strstr(str, "/file/data") && strlen("/file/data") == strlen(str)){
        getFileData(str);
    }
    else strcpy(str, "404 Not Found");
}