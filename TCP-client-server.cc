#include <iostream>
#include <sys/socket.h> 		
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

void chomp(char *s) {
	for (char *p = s + strlen(s)-1; // start at the end of string
			*p == '\r' || *p == '\n'; // while there is a trailing \r or \n
			p--) // check next character from back

		*p = '\0'; // change \r or \n to \0
}

void GET (int connSock, char * path, char * home)
{
	struct dirent *dirEntry;
	DIR *dirp;
	int rs;
	struct stat buffer;
	bool file = true;
	char filestuff [256];
	const char * final =  strcat(home, path);
	//char * path = argv[1];

	cout << endl;
	cout << "Path: " << final << endl;

	if (path[0] == '/')
	{
		rs = stat (final, &buffer); //get status
		if (rs < 0)
		{
			perror ("failed to use stat in get function");
			exit(rs);
		}

		if (S_ISREG(buffer.st_mode)) //check if file or directory
			file = true;
		else if (S_ISDIR(buffer.st_mode))
			file = false;
		else
		{
			perror ("You cannot access symbolic links!");
			exit(EXIT_FAILURE);
		}

		if (file == true) //if file
		{
			int fd = open(final, O_RDONLY);
			if (fd == -1){
				perror ("duplicate open");
				exit(EXIT_FAILURE);
			}
			int recieved = read (fd, filestuff, 256);
			write (connSock, filestuff, recieved);
			close(fd);
		}

		else if (file == false) //if directory
		{
			dirp = opendir(final);
			if (dirp == 0)
			{
				perror(path); 
				exit(EXIT_FAILURE);
			}

			while ((dirEntry = readdir(dirp)) != NULL) {
				cout << dirEntry->d_name << endl;
			}
			closedir(dirp);
		}
	}
	else 
	{
		perror("Must start path with a '/' !");
		exit(EXIT_FAILURE);
	}

}

void INFO ()
{
	cout << endl;
	time_t timer = time(NULL);
	cout << asctime(localtime(&timer));
}

void processClientRequest(int connSock, struct sockaddr_in echoclient, char * home) //char *argv[] )
{
	char buffer[256];
	unsigned int recieved, count = 0;
	//char * get = "GET";
	char * path;

	if (dup2(connSock, 1) < 0)
	{
		perror ("Failed to process dup2");
		exit (EXIT_FAILURE);
	}


	if ((recieved = read(connSock, buffer, 256)) < 0) { //read a message from the client
		perror ("Failed to recieve message");
		exit (EXIT_FAILURE);
	}

	buffer[recieved] = '\0'; //ensure string is terminated 
	chomp(buffer); //remove trailing \r and \n

	if (write(connSock, buffer, recieved) != recieved) {
		perror ("Mismatch in number of echo'd bytes");
		exit(EXIT_FAILURE);
	}

	// prepare argv array of strings
	char *argv[] = { (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0 };

	// parse command into words
	for (char *p = strtok(buffer, " "); p; p = strtok(NULL, " "))
	{
		argv[count] = p;
		count++;
	}

	path = argv[1];
	if (strcmp (argv[0], "GET") == 0)
		GET(connSock, path, home);

	else if (strcmp(argv[0], "INFO") == 0)
		INFO();

	else
	{
		perror("Invalid first argument!");
		exit(EXIT_FAILURE);
	}

	// run command and its arguments via execvp
	/*	if (execvp(argv[0], argv) < 0) {
		perror("exec in child after fork");
		exit(EXIT_FAILURE);
		}*/

	cerr << "Client(" << inet_ntoa(echoclient.sin_addr) << ") sent: " << buffer << endl;

}

int main (int argc, char *argv[])
{
	int sock, connSock;
	struct sockaddr_in echoserver
		; //structure for address of server
	unsigned int serverlen;
	socklen_t clientlen;
	struct sockaddr_in echoclient;
	char * home = argv[2];

	//base this on UDP server in previous assignment up to the while loop

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) //create TCP socket //NOTE: TURN THIS TO AF_INET after done
	{
		perror ("Failed to bind server socket");
		exit (EXIT_FAILURE);
	}

	//construct server sockaddr_in structure
	memset(&echoserver, 0, sizeof(echoserver)); /*Clear struct*/
	echoserver.sin_family = AF_INET;			/*Internet/IP*/
	echoserver.sin_addr.s_addr = htonl(INADDR_ANY); //htonl(INADDR_ANY); /*Any IP address */
	echoserver.sin_port = htons(atoi(argv[1]));	/*server port */


	//bind the socket
	serverlen = sizeof(echoserver);

	if (bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0)
	{
		perror("Failed to bind server socket" );
		exit(EXIT_FAILURE);
	}

	//listen; make socket passive and set length of queue
	if (listen(sock, 64) < 0) {
		perror("listen failure!");
		exit (EXIT_FAILURE);
	}


	//Run until cancelled
	while (true) {
		connSock = accept(sock, (struct sockaddr *) &echoclient, &clientlen);
		if (connSock < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		if (fork()) // parent process
			close(connSock);

		else { 	  // child process

			// read a message from the client
			processClientRequest (connSock, echoclient, home); //argv);

			close(connSock);
			exit(EXIT_SUCCESS);
		}

	}

	return 0;
}
