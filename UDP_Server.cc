#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <netdb.h>

using namespace std;

int main()
{
        string name, ccnum, expDate, money, message;
        int place = 0, error, num = 0;
        char send[256];
        char buffer[256];
        int sockfd;
        unsigned int echolen, received = 0, addrlen, decimal;
        struct sockaddr_in echoserver;
        struct addrinfo *res;
        const char * port = "4445";

//BEGIN!
        cout << "Welcome to the Credit Card transaction utility" << endl;
        cout << "Enter card holder name (or quit): ";
        getline(cin, name);

//Collect all the inputs
        while (name != "quit")
        {
                while (name.length() > 64)
                {
                        cout << "ERROR! Your name is too large." << endl;
                        cout << "Please enter a shorter name (or quit): ";
                        getline(cin, name);
                }
                if (name == "quit")
                        break;

                cout << "Enter CC number: ";
                getline(cin, ccnum);

						for (unsigned j = 0; j < ccnum.length(); j++)
                       {
						if (isdigit(ccnum.at(j)))
						num++;
						else if (isalpha(ccnum.at(j)))
						{cout << "ERROR! No letters in a credit card!";
						exit (EXIT_FAILURE);}
						
					   }
						
				if (num < 15 || num > 16){
				cout << "ERROR! Wrong amount of characters in credit card!";
				exit (EXIT_FAILURE);}


                cout << "Enter expiration (MM/YYYY): ";
                getline(cin, expDate);

                while (!ispunct(expDate.at(2)) || expDate.length() < 7) // checks if position 2 is a /
                {
                        cout << "ERROR! Invalid expiration date. Exiting." << endl;
                        exit (EXIT_FAILURE);
                }
                cout << "Enter amount to deposit ($ xx.xx): ";
                cin >> money;

                for (unsigned j = 0; j < money.length(); j++)
                {
                        if (!ispunct(money.at(j)))
                                decimal = money.length();

                        if (ispunct(money.at(j)))
                                {decimal = j + 1;
                                break;}
                }
                if (decimal < money.length())
                {
                        for (unsigned j = decimal - 1; j < money.length(); j++)
                                if (isdigit(money.at(j)))
                                        place++;

				}
                while (place < 2 || place > 2 || decimal == money.length())
                {
                        cout <<"ERROR! Invalid dollar amount. Exiting." << endl;
                        exit (EXIT_FAILURE);
                }

                message = ccnum + ":" + expDate + ":" + money + ":" + name;
            	unsigned i = 0;
                for (i = 0; i < message.length(); i++)
                        send[i] = message[i];
                
                send[i] = '\0';

                cout << message.length() << " " << sizeof(send) << endl;

                // set up socket

                sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                if (sockfd < 0)
                {
                        perror("Failed to create socket");
                        exit(EXIT_FAILURE);
                }
                error = getaddrinfo("graphics.cs.niu.edu", port, NULL, &res);
                if (error)
                {
                        cout << "Invalid domain name";
                        exit(EXIT_FAILURE);
                }
                struct sockaddr_in * addr = (struct sockaddr_in *) res->ai_addr;
                memset(&echoserver, 0, sizeof(echoserver));
                echoserver.sin_family = AF_INET;
                echoserver.sin_addr= addr->sin_addr;
                echoserver.sin_port = htons(atoi(port));

                //send info to server

                echolen = sizeof(send);
                if (sendto(sockfd, send, sizeof(send), 0, (struct sockaddr *) &echoserver, sizeof(echoserver)) != echolen)
                {
                        cout << sendto(sockfd, send, sizeof(send), 0, (struct sockaddr *) &echoserver, sizeof(echoserver)) << endl;
                        perror("Mismatch in number of sent bytes");
                        exit(EXIT_FAILURE);
                }

                // receive from server

                addrlen = sizeof(echoserver);
                received = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &echoserver, &addrlen);
                buffer[received] = '\0'; // add NULL character to end of sent message
                cout << buffer << endl;

                // close sock

                close(sockfd);
                cout << "Enter card holder name (or quit): ";
                getline(cin, name); // used to stop "Enter card holder name (or quit): Enter CC number: "
                getline(cin, name);
        }
        return 0;
}
