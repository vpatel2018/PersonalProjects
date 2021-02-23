#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <pthread.h>
#include <dirent.h>
#include <time.h>

#define PORT_NUM 1004
char* choice = "";
char* theUser = "";
int red = 0;
int green = 0;
int blue = 0;

/* Vihan Patel
 Daniel Ortega
 Jan Tameta
 CSC345-01
 Project 4 */

void error(const char *msg)
{
	perror(msg);
	exit(0);
} //

typedef struct _ThreadArgs {
	int clisockfd;
} ThreadArgs;

//look into a file called userColors
//it will tell you the color that is assigned to a user
void getColorForUser(char* userName, int* r, int* g, int* b) {

	FILE* file = fopen("userColors.txt", "r");
	char str[205];

	while(fgets(str, 205, file) != NULL) {
		int ind = -1;
		for(int k = 0; k < 205; k++) {
			if(str[k] == ' ') {
				str[k] = '\0';
				ind = k;
				break;
			}
		}
		if(strcmp(str, userName) == 0) {
			char* word = str + ind + 1;
			for(int k = 0; k < strlen(word); k++) {
				if(word[k] == ',') {
					word[k] = '\0';
					*(r) = atoi(word);
					word[k] = ',';
					word = word + k + 1;
					break;
				}
			}
			for(int k = 0; k < strlen(word); k++) {
				if(word[k] == ',') {
					word[k] = '\0';
					*(g) = atoi(word);
					word[k] = ',';
					word = word + k + 1;
					break;
				}
			}
			for(int k = 0; k < strlen(word); k++) {
				if(word[k] == '\n') {
					word[k] = '\0';
					*(b) = atoi(word);
					break;
				}
			}
			break;
		}
	}

}

//display colored messages from other clients to you
void* thread_main_recv(void* args)
{
	pthread_detach(pthread_self());

	int sockfd = ((ThreadArgs*) args)->clisockfd;
	free(args);

	char buffer[512];
	int n;

	int rd;
	int gr;
	int bl;

	n = recv(sockfd, buffer, 512, 0);
	printf("\n");
	while (n > 0) {
		char* string;
		int ind = -1;
		for(int k = 0; k < strlen(buffer); k++) {
			if(buffer[k] == ']') {
				buffer[k] = '\0';
				ind = k;
				break;
			}
		}
		string = buffer + 1;
		getColorForUser(string, &rd, &gr, &bl);
		buffer[ind] = ']';
		printf("\x1b[38;5;%d;%d;%dm%s\x1b[0m\n", rd, gr, bl, buffer);
		memset(buffer, 0, 512);
		n = recv(sockfd, buffer, 512, 0);
		if (n < 0) error("ERROR recv() failed");
	}

	return NULL;
}


void* thread_main_send(void* args)
{
	pthread_detach(pthread_self());

	int sockfd = ((ThreadArgs*) args)->clisockfd;
	free(args);

	fflush(stdin);
	char buffer[256];
	int n;

	struct sockaddr_in cliaddr;
	socklen_t clen = sizeof(cliaddr);
	getpeername(sockfd, (struct sockaddr*)&cliaddr, &clen);

	while(1) {
		char buffer[512];
		printf("\x1b[38;5;%d;%d;%dmPlease enter a message: \x1b[0m\n", red, green, blue);
		memset(buffer, '\0', 256);
		fgets(buffer, 255, stdin);

		if (strlen(buffer) == 1) {
			buffer[0] = '\0';
		}

		n = send(sockfd, buffer, strlen(buffer), 0);
		if (n < 0) error("ERROR writing to socket");

		if (n == 0) {
			fflush(stdin);
			exit(0);
		} // we stop transmission when user type empty string

		fflush(stdin);
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//print the list of rooms available and the number of people in each of them
void printServerOptions() {

	FILE* f1 = fopen("rooms.txt", "r");

	printf("Server says following options are available:\n");
	char str[205];
	int count = 0;
	while(fgets(str, 205, f1) != NULL) {
		count++;
	}

	for(int k = 1; k < count + 1; k++) {
		char array[50];
		sprintf(array, "numPeopleInRoom%d.txt", k);
		char lineInFile[205];
		FILE* qwe = fopen(array, "r");
		fgets(lineInFile, 205, qwe);
		fclose(qwe);
		for(int jh = 0; jh < 205; jh++) {
			if(lineInFile[jh] < 48 || lineInFile[jh] > 57) {
				lineInFile[jh] = '\0';
				break;
			}
		}
		int number = atoi(lineInFile);
		if(number == 1) {
			printf("    Room %d: 1 person\n", k);
		}else{
			printf("    Room %d: %d people\n", k, number);
		}
	}

}

int validateInput(int MAX_ROOMS) {

	//count total number of rooms that are available right now
	FILE* f = fopen("rooms.txt", "r");
	int lastInteger = -1;
	if(f != NULL) {
		char str[205];
		while(fgets(str, 205, f) != NULL) {
			for(int k = 0; k < 205; k++) {
				if(str[k] == '\n') {
					str[k] = '\0';
					break;
				}
			}
			int result = atoi(str);
			char in[50];
			sprintf(in, "%d", result);
			if(strcmp(in, str) == 0) {
				lastInteger = result;
			}
		}
	}else{
		//if no rooms exist and no new room is to be created, then return -1
		//otherwise return 0
		if(strcmp(choice, "new") != 0) {
			return -1;
		}else{
			return 0;
		}
	}
	fclose(f);

	//if number of rooms is not negative number
	if(lastInteger != -1) {

		//do not create a new room if number of rooms used reach a limit
		if(strcmp(choice, "new") == 0 && lastInteger == MAX_ROOMS) {
		   return -1;
		}

		/* If no new room will be created, 
		   check if input given by user is a number.
		   If input is not a number, return 0.
		   If input is a number, check if it is
		   less than 1 or greater than number of
		   available rooms. If so, return 0;
		*/
		if(strcmp(choice, "new") != 0) {
			if(choice[0] == '-') {
				return -1;
			}
		    for(int k = 0; k < strlen(choice); k++) {
			    if(choice[k] < 48 || choice[k] > 57) {
				    return -1;
			    }
		    }
			int number = atoi(choice);
			if(number < 1 || number > lastInteger) {
			    return -1;
			}
		}

	}

	return lastInteger;

}

int main(int argc, char *argv[]) 
{
	//main_server should give information about max rooms available to client
	int MAX_ROOMS = -1;
	FILE* f = fopen("maxRoom.txt", "r");
	if(f != NULL) {
	   char str[205];
	   char* num = fgets(str, 205, f);
	   for(int k = 0; k < 205; k++) {
		   if(str[k] < 48 || str[k] > 57) {
			  str[k] = '\0';
			  break;
		   }
	   }
	   MAX_ROOMS = atoi(str);
	   fclose(f);
	}

	if (argc < 2) error("Please specify hostname");

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");

	struct sockaddr_in serv_addr;
	socklen_t slen = sizeof(serv_addr);
	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(PORT_NUM);

	int result;
	int isNew = 0;

	//allow client to choose if he wants to join a new room or join a room that is currently available
	if(argc == 3) {
		choice = argv[2];
		result = validateInput(MAX_ROOMS);
		if(result == -1) {
			return 0;
		}
	}else if(argc < 3) {
		FILE* v = fopen("rooms.txt", "r");
		if(v == NULL) {
			choice = "new";
		}else{
			printServerOptions();
			fflush(stdin);
			printf("Choose the room number or type [new] to create a new room:");
			char input[5];
			memset(input, '\0', 5);
			fgets(input, 4, stdin);
			for(int k = 0; k < 5; k++) {
				if(input[k] == '\n') {
					input[k] = '\0';
				}
			}
			char* res = input;
			choice = input;
		}
		result = validateInput(MAX_ROOMS);
		if(result == -1) {
			return 0;
		}
	}

	//create a new room if client wants a new room and if it is possible
	if(strcmp(choice, "new") == 0) {
		result = result + 1;
		choice = "";
		char theInteger[50];
		sprintf(theInteger, "%d", result);
		char* finResult = theInteger;
		choice = finResult;
		isNew = 1;
	}

	char numb[205];
	sprintf(numb, "%s", choice);
	choice = numb;

	if(isNew == 1) {
		FILE* f1 = fopen("rooms.txt", "r");
		if(f1 == NULL) {
		    f1 = fopen("rooms.txt", "w+");
			fprintf(f1, "%s\n", choice);
		}else{
			f1 = fopen("rooms.txt", "a");
			fprintf(f1, "%s\n", choice);
		}
		fclose(f1);

		char buf[50];
		sprintf(buf, "numPeopleInRoom%s.txt", choice);
		FILE* aFile = fopen(buf, "w+");
		int integer = 1;
		fprintf(aFile, "%d\n", integer);
		fclose(aFile);
	}else{
		char buf[50];
		sprintf(buf, "numPeopleInRoom%s.txt", choice);
		FILE* aFile = fopen(buf, "r");
		char str3[205];
		fgets(str3, 205, aFile);
		for(int v = 0; v < 205; v++) {
			if(str3[v] < 48 || str3[v] > 57) {
				str3[v] = '\0';
				break;
			}
		}
		int integer = atoi(str3) + 1;
		aFile = fopen(buf, "w+");
		fprintf(aFile, "%d\n", integer);
		fclose(aFile);
	}

	//ask client to specify a username of their choice

	FILE* current = fopen("currUser.txt", "w+");
	f = fopen("userNames.txt", "r");

	if(f == NULL) {
		f = fopen("userNames.txt", "w+");
		fflush(stdin);
		printf("Enter a user name of your choice: ");
		char str[205];
		memset(str, '\0', 205);
		fgets(str, 205, stdin);
		int isNewLine = 0;
		for(int k = 0; k < 205; k++) {
			if(str[k] == '\n') {
				str[k] = '\0';
				if(k == 0) {
					isNewLine = 1;
				}
				break;
			}
		}

		int isValid = 1;
		if(strlen(str) == 0) {
			fflush(stdin);
			if(isNewLine == 1) {
				printf("\n");
			}
			printf("User name should be at least 1 character, please enter another one: ");
			isValid = 0;
		}else if(strstr(str, " ") != NULL) {
			fflush(stdin);
			printf("No spaces allowed in user name, please enter another one: ");
			isValid = 0;
		}

		while(isValid == 0) {
			memset(str, '\0', 205);
			fgets(str, 205, stdin);
			for(int k = 0; k < 205; k++) {
				if(str[k] == '\n') {
					str[k] = '\0';
					break;
				}
			}
			if(strlen(str) == 0) {
				fflush(stdin);
				printf("User name should be at least 1 character, please enter another one: ");
				isValid = 0;
			}else if(strstr(str, " ") != NULL){
				fflush(stdin);
				printf("No spaces allowed in user name, please enter another one: ");
				isValid = 0;
			}else{
				isValid = 1;
			}
		}

		fprintf(f, "%s %s\n", str, choice);
		fprintf(current, "%s %s", str, choice);
		fflush(stdin);
		theUser = str;

	}else{
		f = fopen("userNames.txt", "a");
		fflush(stdin);
		printf("Enter a user name of your choice: ");
		char str[205];
		memset(str, '\0', 205);
		fgets(str, 205, stdin);
		int isNewLine = 0;
		for(int k = 0; k < 205; k++) {
			if(str[k] == '\n') {
				str[k] = '\0';
				if(k == 0) {
					isNewLine = 1;
				}
				break;
			}
		}

		int isValid = 1;
		if(strlen(str) == 0) {
			fflush(stdin);
			if(isNewLine == 1) {
				printf("\n");
			}
			printf("User name should be at least 1 character, please enter another one: ");
			isValid = 0;
		}else if(strstr(str, " ") != NULL) {
			fflush(stdin);
			printf("No spaces allowed in user name, please enter another one: ");
			isValid = 0;
		}

		while(isValid == 0) {
			memset(str, '\0', 205);
			fgets(str, 205, stdin);
			for(int k = 0; k < 205; k++) {
				if(str[k] == '\n') {
					str[k] = '\0';
					break;
				}
			}
			if(strlen(str) == 0) {
				fflush(stdin);
				printf("User name should be at least 1 character, please enter another one: ");
				isValid = 0;
			}else if(strstr(str, " ") != NULL){
				fflush(stdin);
				printf("No spaces allowed in user name, please enter another one: ");
				isValid = 0;
			}else{
				isValid = 1;
			}
		}

		FILE* m = fopen("userNames.txt", "r");
		char str2[205];
		int userNameTaken = 0;

		do{
			m = fopen("userNames.txt", "r");
			int stopped = 0;
			while(fgets(str2, 205, m) != NULL) {
				for(int k = 0; k < 205; k++) {
					if(str2[k] == ' ') {
						str2[k] = '\0';
					}
				}
				if(strcmp(str2, str) == 0) {
					userNameTaken = 1;
					fflush(stdin);
					printf("User name already taken, please enter another one: ");
					memset(str, '\0', 205);
					fgets(str, 205, stdin);
					for(int k = 0; k < 205; k++) {
						if(str[k] == '\n') {
							str[k] = '\0';
							break;
						}
					}
					stopped = 1;
					break;
				}
			}
			if(stopped == 0) {
				userNameTaken = 0;
			}
		}while(userNameTaken == 1);

		fclose(m);
		fprintf(f, "%s %s\n", str, choice);
		fprintf(current, "%s %s", str, choice);
		fflush(stdin);
		theUser = str;
	}

	fclose(f);
	fclose(current);

	srand(time(0));

	//assign random and unique color for client
	red = rand() % 256;
	green = rand() % 256;
	blue = rand() % 256;

	char arr[205];
	sprintf(arr, "%s %d,%d,%d\n", theUser, red, green, blue);
	FILE* col = fopen("userColors.txt", "r");
	if(col == NULL) {
		col = fopen("userColors.txt", "w+");
	}else{
		col = fopen("userColors.txt", "a");
	}
	fprintf(col, "%s", arr);
	fclose(col);

	//if client goes to a new room, let them know about it
	if(isNew == 1) {
		printf("Connected to %s with new room number %s\n", inet_ntoa(serv_addr.sin_addr), choice);
	}

	int status = connect(sockfd, 
		(struct sockaddr *) &serv_addr, slen);
	if (status < 0) error("ERROR connecting");

	pthread_t tid1;
	pthread_t tid2;

	ThreadArgs* args;
		
	args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
	args->clisockfd = sockfd;
	pthread_create(&tid1, NULL, thread_main_send, (void*) args); //need to look at this method

	args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
	args->clisockfd = sockfd;
	pthread_create(&tid2, NULL, thread_main_recv, (void*) args); //need to look at this method

	// parent will wait for sender to finish (= user stop sending message and disconnect from server)
	pthread_join(tid1, NULL);

	close(sockfd);

	return 0;
}
