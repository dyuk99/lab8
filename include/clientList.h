/** @file
    @brief Generic list abstract data type
*/
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/**
    @struct List
    @brief defines a generic List Songs data type
*/

typedef struct Songs Songs;

/**
    @brief List constructor
*/
Songs * SongsNew (int id, char name[100], char performer[100]);

/**
    @brief List constructor with fictive node
*/
Songs * SongsNewFict ();

/**
    @brief adds new item to the end of List
    @param node - pointer to memory from where to read new value
    @returns pointer to List head
*/
Songs * SongsAddLast (Songs *self, Songs *node);

/** 
    @brief List destructor
*/
void freeSongs (Songs** self);

/**
    @brief remove item at index
    @param index - index of element to remove
    @returns pointer to List head
*/
Songs *DeleteSong (Songs * self, int index);

/** 
    @brief Node destructor
*/
void freeSong (Songs** node);

/** 
    @returns a number of items in list
*/
int countSongs (Songs *self);

/**
    @brief List items getter
    @param index - index of element
    @returns poinyer to List head
*/
Songs *SongsGetNode (Songs *self, int index);

/**
    @brief get Node Performer Name field data
    @returns pointer to memory from where to read data 
*/
char * SongGetPerformerName (Songs * self);

/**
    @brief get Node Name field data
    @returns pointer to memory from where to read data 
*/
char * SongGetName (Songs * self);

/**
    @brief get Node Id field data
    @returns pointer to memory from where to read data 
*/
int * SongGetPerformerId (Songs * self);
#endif