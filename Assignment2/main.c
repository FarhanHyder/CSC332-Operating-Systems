/*
author: Farhan Hyder
CSC 332 - Assignment 2
Date: 11/12/2018
*/

#include <stdio.h>		//FILE, size_t, fopen()
#include <stdlib.h>		//malloc
#include <pthread.h>	//for all pthread stuffs

#define DATA_FILE "datafile.dat"
#define ANSWER_FILE "ans.dat"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	//initialize mutex
//other files must be named in new#.dat format. For example: new1.dat, new2.dat, new3.dat

typedef struct node {
    int val;
    struct node *prev;
    struct node *next;
} node_t;

typedef struct list {
	node_t *head;
	node_t *tail;
} list_t;

list_t *read_file(char *);
void *thread_job(void *);
void compare_lists(list_t *, list_t *);

int main()
{
	int NUM_OF_THREADS = 3;
	int returnval, t;
	pthread_t thread[NUM_OF_THREADS];
	pthread_attr_t attr;
	void *status;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//create threads
	for (t=0; t<NUM_OF_THREADS; t++)
	{
		returnval = pthread_create(&thread[t],&attr,thread_job,(void *)t);
		if (returnval)
		//failed to create thread
		{
			printf("[ERROR]: error code from pthread_create() : %d",returnval);
			exit(-1);
		}
	}

	pthread_attr_destroy(&attr);
	for (t=0; t<NUM_OF_THREADS; t++)
	{
		returnval = pthread_join(thread[t],&status);
		if(returnval)
		//failed to join thread
		{
			printf("[ERROR]: error code from pthread_join() : %d",returnval);
			exit(-1);
		}
	}

	//create lists for answer file and datafile
	list_t *list1 = read_file(ANSWER_FILE);
	list_t *list2 = read_file(DATA_FILE);

	//compare the lists to check if they are equal or not
	compare_lists(list1, list2);
	
	return 0;

}

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
		//printf("\nERROR: failed to open the file.\n");
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &length, fp)) != -1)
	{
		number = atoi(line);
		if (current_list->head == NULL)
		// first element. set head and tail.
		{
			// set head
			current_list->head = malloc(sizeof(node_t));
			current_list->head->val = number;
			current_list->head->prev = NULL;

			//set tail
			current_list->tail = malloc(sizeof(node_t));
			current_list->tail->val = number;
			current_list->tail->prev = NULL;
			current_list->tail->next = NULL;
			current_list->head->next = current_list->tail;
		}
		else
		//update tail
		{
			current_list->tail->next = malloc(sizeof(node_t));
			current_list->tail->next->val = number;
			current_list->tail->next->prev = current_list->tail;
			current_list->tail->next->next = NULL;
			current_list->tail = current_list->tail->next;
		}
	}

	fclose(fp);
	return current_list;
}


void *thread_job(void *t)
{
	int index = (int)t + 1;
	char filename[50];
	sprintf(filename, "new%d.dat", index);

	list_t *current_list = read_file(filename);	//current thread's list
	pthread_mutex_lock(&mutex);	//create critical region
    list_t *main_list = read_file(DATA_FILE);	//main file

    //merge both lists
    main_list->tail->next = current_list->head->next;
    current_list->head->next->prev = main_list->tail;

	//open datafile as write
    FILE *fp = fopen(DATA_FILE, "w");
	if (fp == NULL)
	{
		printf("\nERROR: failed to open the file.\n");		
	    exit(1);
	}

    /*
	at each iteration find the min # of the merged list,
	and write it into the data file
	*/
    node_t *current_node = NULL;
    node_t *min_node = NULL;
    main_list->head = main_list->head->next;

    while (1)
	{
    	//find the node with the min val
	    current_node = main_list->head;
	    min_node = current_node;

		while (current_node != NULL)
		{
			if (current_node->val < min_node->val){
				min_node = current_node;
			}

			current_node = current_node->next;
		}

		if (min_node == NULL)
		//reached the end of the list, no proper min found
		{
			break;
		}
		else
		//update the min number in the datafile
		{
			fprintf(fp, "%d\n", min_node->val);

			if (min_node->prev == NULL)
			//head node
			{
				if (min_node->next == NULL)
				//last node
				{
					break;
				}
				else
				//update head
				{
					min_node->next->prev = NULL;
					main_list->head = min_node->next;
				}
			}
			else
			{
				//remove min node from the list
				min_node->prev->next = min_node->next;
				if (min_node->next != NULL){
					min_node->next->prev = min_node->prev;
				}
			}
		}
	}

	fclose(fp);	//end file
    pthread_mutex_unlock(&mutex);	//end critical
}


void compare_lists(list_t *list1, list_t *list2)
/*
compare two lists and prints result if they have equal values or not
in this project: this func has been used to compare the newly updated data file-
	with the provided answer file by the instructor.
*/
{
	node_t *list1_current_node = NULL;
	node_t *list2_current_node = NULL;
	list1_current_node = list1->head->next;
    list2_current_node = list2->head->next;

	while(1)
	{
		if(list1_current_node == NULL)
		{
			if (list2_current_node ==NULL)
			{
				printf("\n[EQUAL] Two lists are equal.\n");
			}
			else
			//list1 ended but list2 didn't
			{
				printf("\n[NOT equal] : Two lists are not equal.\n");
			}
			break;
		}

		//compare values
		if (list1_current_node->val != list2_current_node->val)
		{
			printf("\n[NOT equal] : Two lists are not equal.\n");
			break;
		}

		//update nodes
		list1_current_node = list1_current_node->next;
		list2_current_node = list2_current_node->next;
	}
}