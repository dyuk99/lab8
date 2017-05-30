/** @file
    @brief Response from server
*/
#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "clientList.h"
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <progbase/net.h>
#include <jansson.h>

/**
    @brief get informetion about server
    @param str - pointer to memory where response string can be read
    @param port - port where server is binded
*/
void getServerInfo(char *str, const int port);

/**
    @brief get List of favourites songs
    @param str - pointer to memory where response string can be read
    @param songs - pointer to head of List of songs
*/
void getListOfSongs(char *str, Songs *songs);

/**
    @brief get Filtered List of songs
    @param str - pointer to memory where response string can be read
    @param songs - pointer to head of List of songs
*/
void getFilteredListOfSongs(char *str, Songs *songs);

/**
    @brief get Song by it's id
    @param str - pointer to memory where response string can be read
    @param songs - pointer to head of List of songs
*/
void getSongById(char *str, Songs *songs);

/**
    @brief get information about file 
    @param str - pointer to memory where response string can be read
*/
void getFileInfo(char *str);

/**
    @brief get processed data from file
    @param str - pointer to memory where response string can be read
*/
void getFileData(char *str);

/**
    @brief read file to string
    @param buffer - pointer to memory where string can be read
*/
void readFromFileToString(char* buffer);

/**
    @brief process response
    @param str - pointer to memory where response string can be read
    @param songs - pointer to head of List of songs
    @param port - port where server is binded
*/
void processResponse(char *str, Songs*songs, const int port);

#endif