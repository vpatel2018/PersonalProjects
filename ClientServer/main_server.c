#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>

#define PORT_NUM 1004
#define MAX_ROOMS 3 //maximum number of rooms that can be managed by the server 

/* Vihan Patel
 Daniel Ortega
 Jan Tameta
 CSC345-01
 Project 4 */

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

typedef struct _USR {
	int clisockfd;		// socket file descriptor
    char str[1000];     // used to store a user name of a client
    int room;           // used to store a client's room number
	struct _USR* next;	// for linked list queue
} USR;

USR *head = NULL;
USR *tail = NULL;

/* broadcasts a message to clients 
who belong to the same room */

void broadcast(int fromfd, char* message, int targetRoom, char* userName)
{
	// traverse through all connected clients
	USR* cur = head;
	while (cur != NULL) {
		// check if cur is not the one who sent the message 
		if (cur->clisockfd != fromfd && cur->room == targetRoom) {
			char buffer[512];

			// prepare message
			sprintf(buffer, "[%s]: %s", userName, message);
			int nmsg = strlen(buffer);

            //send the message
            char b[1010];
            sprintf(b, "%sto%s.txt", userName, cur->str);

            FILE* newFile = fopen(b, "w+");
            fprintf(newFile, "%s\n", buffer);
            fclose(newFile);

            FILE* fl = fopen(b, "r");
            char a[205];
            fgets(a, 205, fl);
            for(int o = 0; o < 205; o++) {
                if(a[o] == '\n') {
                    a[o] = '\0';
                    break;
                }
            }

            int nsen = send(cur->clisockfd, a, nmsg, 0);
			if (nsen != nmsg) error("ERROR send() failed");

		}
		cur = cur->next;
	}

}

typedef struct _ThreadArgs {
	int clisockfd;
    int room;
} ThreadArgs;

void* thread_main(void* args)
{
	// make sure thread resources are deallocated upon return
	pthread_detach(pthread_self());

	// get socket descriptor from argument
	int clisockfd = ((ThreadArgs*) args)->clisockfd;
    int roomNum = ((ThreadArgs*) args)->room;
	free(args);

    // get the user name associated with socket number
    USR* n = head;
    while(n->clisockfd != clisockfd) {
        n = n->next;
    }
    char* userName = n->str;

    //if a client joins a room, clients who belong to the same room as him/her need to know about it
	USR* cur = head;
	while (cur != NULL) {
		// check if cur is not the one who sent the message
		if (cur->clisockfd != clisockfd && cur->room == roomNum) {
			char buffer[512];

			// prepare message
			sprintf(buffer, "%s joined this chat room.", userName);
			int nmsg = strlen(buffer);

            // send the message
            char b[1010];
            sprintf(b, "%sto%s.txt", userName, cur->str);

            FILE* newFile = fopen(b, "w+");
            fprintf(newFile, "%s\n", buffer);
            fclose(newFile);

            FILE* fl = fopen(b, "r");
            char a[205];
            fgets(a, 205, fl);
            for(int o = 0; o < 205; o++) {
                if(a[o] == '\n') {
                    a[o] = '\0';
                    break;
                }
            }

            int nsen = send(cur->clisockfd, a, nmsg, 0);
			if (nsen != nmsg) error("ERROR send() failed");
		}
		cur = cur->next;
	}

	// Now, we receive/send messages
	char buffer[256];
	int nsen, nrcv;
	nrcv = recv(clisockfd, buffer, 255, 0);
	if (nrcv < 0) error("ERROR recv() failed");

	while (nrcv > 0) {
		// we send the message to everyone except the sender
		broadcast(clisockfd, buffer, roomNum, userName);
		nrcv = recv(clisockfd, buffer, 255, 0);
		if (nrcv < 0) error("ERROR recv() failed");
	}

    /* If client is done sending messages, print the up to date list of connected clients. 
       Also, make sure to update the number of people in each room that is available. */
    if(nrcv == 0) {

        if(head->clisockfd == clisockfd) {
            free(head);
            head = head->next;
            if(head == NULL) {
              printf("up-to-date list of connected clients\n");
              printf("* No client is currently connected.\n");
              printf("\n");
            }else{
              printf("up-to-date list of connected clients\n");
              USR* l = head;
              while(l != NULL) {
                printf("User = %s , Room number = %d\n", l->str, l->room);
                l = l->next;
              }
              printf("\n");
            }
        }else{
            USR* start = head;
            if(start->next != NULL) {
              while(start->next->clisockfd != clisockfd) {
                start = start->next;
              }
              USR* newNext = start->next->next;
              start->next->next = NULL;
              start->next = newNext;
              if(head == NULL) {
                printf("up-to-date list of connected clients\n");
                printf("* No client is currently connected.\n");
                printf("\n");
              }else{
                printf("up-to-date list of connected clients\n");
                USR* l = head;
                while(l != NULL) {
                    printf("User = %s , Room number = %d\n", l->str, l->room);
                    l = l->next;
                }
                printf("\n");
              }
            }
        }

        //remove client's username from file containing userNames
        FILE* b = fopen("b.txt", "w+");
        FILE* a = fopen("userNames.txt", "r");
        char theInput[50];
        sprintf(theInput, "numPeopleInRoom%d.txt", roomNum);
        FILE* c = fopen(theInput, "r");
        char str[205];

        if(a != NULL) {
            while(fgets(str, 205, a) != NULL) {
                int index = -1;
                for(int t = 0; t < 205; t++) {
                    if(str[t] == ' ') {
                        str[t] = '\0';
                        index = t;
                        break;
                    }
                }
                if(strcmp(str, userName) != 0) {
                    str[index] = ' ';
                    fprintf(b, "%s", str);
                }
            }
            system("mv b.txt userNames.txt");
            fclose(b);
            fclose(a);
        }

        if(c != NULL) {
            fgets(str, 205, c);
            for(int ab = 0; ab < 205; ab++) {
                if(str[ab] < 48 || str[ab] > 57) {
                   str[ab] = '\0';
                   break;
                }
            }
            int newInteger = atoi(str) - 1;
            c = fopen(theInput, "w+");
            fprintf(c, "%d\n", newInteger);
            fclose(c);
        }

        //remove information about what unique color is associated with client from file called userColors.txt
        FILE* d = fopen("userColors.txt", "r");
        FILE* e = fopen("c.txt", "w+");

        if(d != NULL) {
            while(fgets(str, 205, d) != NULL) {
                int index = -1;
                for(int t = 0; t < 205; t++) {
                    if(str[t] == ' ') {
                        str[t] = '\0';
                        index = t;
                        break;
                    }
                }
                if(strcmp(str, userName) != 0) {
                    str[index] = ' ';
                    fprintf(e, "%s", str);
                }   
            }
            fclose(e);   
            system("mv c.txt userColors.txt");
            fclose(d);
        }

        //remove all files that a client could use to communicate with other clients
        DIR* drc = opendir(".");
        struct dirent* item;
        while((item = readdir(drc)) != NULL) {
            char* fileName = item->d_name;
            if(strstr(fileName, "to") != NULL && strstr(fileName, userName) != NULL) {
                char command[205];
                sprintf(command, "rm %s", fileName);
                system(command);
            }
        }
        closedir(drc);

    }

	close(clisockfd);
	//-------------------------------

	return NULL;
}

int main(int argc, char** argv) {

    //remove text files within project 4 directory if there are any
    system("rm *.txt 2> /dev/null");

    //create a file that can let clients know about the maximum amount of rooms that can be created
	FILE* f2 = fopen("maxRoom.txt", "w+");
	fprintf(f2, "%d", MAX_ROOMS);
	fclose(f2);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");

	struct sockaddr_in serv_addr;
	socklen_t slen = sizeof(serv_addr);
	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;	
	serv_addr.sin_port = htons(PORT_NUM);

	int status = bind(sockfd, 
			(struct sockaddr*) &serv_addr, slen);
	if (status < 0) error("ERROR on binding");

	listen(sockfd, 5); // maximum number of connections = 5

    while(1) {

		struct sockaddr_in cli_addr;
		socklen_t clen = sizeof(cli_addr);
		int newsockfd = accept(sockfd, 
			(struct sockaddr *) &cli_addr, &clen);
		if (newsockfd < 0) error("ERROR on accept");

        //read a file that tells which client wants to the server right now
        FILE* newFile = fopen("currUser.txt", "r");
        char str[205];

        if(fgets(str, 205, newFile) != NULL) {

            char* user;
            int room;
            int index = -1;

            for(int j = 0; j < 205; j++) {
               if(str[j] == ' ') {
                  index = j;
                  str[j] = '\0';
                  break;
               }
            }
            if(index != -1) {
               user = str;
               char* temp = str + index + 1;
               room = atoi(temp);
            }

            newFile = fopen("currUser.txt", "w+");
            fclose(newFile);
            
            //update linked list containing all users connected to the server
            //add a new client to the linked list
            if (head == NULL) {
                head = (USR*) malloc(sizeof(USR));
                head->clisockfd = newsockfd;
                strcpy(head->str, user);
                head->room = room;
                head->next = NULL;
                tail = head;
            } else {
                tail->next = (USR*) malloc(sizeof(USR));
                tail->next->clisockfd = newsockfd;
                strcpy(tail->next->str, user);
                tail->next->room = room;
                tail->next->next = NULL;
                tail = tail->next;
            }

            //print the up to date list of all connected clients
            USR* cur = head;
            printf("up-to-date list of connected clients\n");
            while(cur != NULL) {
                printf("User = %s , Room number = %d\n", cur->str, cur->room);
                cur = cur->next;
            }
            printf("\n");

            // prepare ThreadArgs structure to pass client socket
            ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
            if (args == NULL) error("ERROR creating thread argument");
            
            args->clisockfd = newsockfd;
            args->room = room;

            pthread_t tid;
            if (pthread_create(&tid, NULL, thread_main, (void*) args) != 0) error("ERROR creating a new thread");

        }

	}

}