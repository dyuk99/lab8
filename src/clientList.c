#include "../include/clientList.h"
#include <string.h>
#include <math.h>
struct Songs{
	int id;
    char name[100];
    char performer[100];
	struct Songs *next;
};

Songs * SongsNew (int id, char name[100], char performer[100])
{
    Songs * node = (Songs *)malloc(sizeof(Songs));
    node->next = NULL;
    node->id = id;
    strcpy(node->name, name);
    strcpy(node->performer, performer);
    return node;
}

Songs * SongsNewFict () {
    Songs * node = (Songs *)malloc(sizeof(Songs));
    node->next = NULL;
    node->id = 0;
    strcpy(node->name, "");
    strcpy(node->performer, "");
    return node;
}

Songs * SongsAddLast (Songs *self, Songs *node) {
    if (self == NULL)
    {
        self = node;
        return self;
    }
    Songs * cur = self;
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = node;
    return self;
}

void freeSong (Songs** node) 
{
    free(*node);
    *node = NULL;
}

void freeSongs (Songs** self) {
    int size = countSongs(*self) - 1;
	while (size >= 0) {
		DeleteSong(*self, size);
		size--;
	}
}

Songs *DeleteSong (Songs * self, int index)
{
    if (index >= countSongs(self) || index < 0 || self == NULL)
    {
        return NULL;
    }
    if (index == 0)
    {
        Songs * node = self;
        self = self->next;
        freeSong(&node);
        return self;
    }
    Songs * cur = self;
    while (index - 1 != 0)
    {
        cur = cur->next;
        index--;
    }
    Songs * node = cur->next;
    cur->next = cur->next->next;
    freeSong(&node);
    return self;
}

int countSongs (Songs *self)
{
    if (self != NULL)
    {
        int counter = 0;
        Songs *cur = self;
        while (cur != NULL)
        {
            counter++;
            cur = cur->next;
        }
        return counter;
    }
    else
    {
        return 0;
    }
}

Songs *SongsGetNode (Songs *self, int index){
    if (self != NULL && index < countSongs(self) && index >= 0)
    {
        Songs *cur = self;
        for (int i = 0; i < index; i++)
        {
            cur = cur->next;
        }
        return cur;
    }
    else
    {
        return self;
    }
}

char * SongGetPerformerName (Songs * self){
    return self->performer;
}

char * SongGetName (Songs * self){
    return self->name;
}

int * SongGetPerformerId (Songs * self){
    return &self->id;
}


 