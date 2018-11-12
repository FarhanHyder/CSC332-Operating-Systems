/*
author: Farhan Hyder
CSC 332 - Assignment 2
Date: 11/11/2018
*/

#include <stdio.h>	//FILE, size_t, fopen()
#include <stdlib.h>	//malloc

typedef struct node
{
	struct node *prev;
	struct node *next;
}node_t;

typedef struct list
{
	node_t *head;
	node_t *tail;
}list_t;

list_t *read_file(char *filepath)
{
	FILE *fp;
	char *line = NULL;
	size_t length = 0;
	ssize_t read;
	int number = 0;

	list_t *current_list = NULL;
	current_list = malloc(sizeof(list_t));
	current_list->head = NULL;

	fp = fopen(filepath,"r");
	if (fp == NULL)
	{
		printf("\nERROR: failed to open the file.\n");
	}

	fclose(fp);
}

int main()
{
	read_file("hudai");
}