#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <iostream>

using namespace std;

int main()
{
pid_t pid, child1, child2;
int status;

	// create first fork
	pid1 = fork();
	// check if first fork failed
	if (pid1 < 0) {
		cout << "Error when using first fork()" << endl;
		return 1;
	}
	// do second fork, this will fork both the parent process and the intermediate parent process
	pid2 = fork();

	// check if second fork failed
	if (pid2 < 0){
	        cout << "Error when using second fork()" << endl;
		return 1;
	}

	// if pid2 == 0 then do child process
	if (pid2 == 0) {
		// child2
		cout << "CHILD: My PID is " << getpid() << ", my parent's PID is " << getppid() << endl;
		// sleep 
		sleep(3);
		// print message when awake
		cout << "CHILD: " << getpid() << " is awake." << endl;
		// close process
		exit(0);
	}
	
	// if pid == 0, do intermediate parent
	if (pid1 == 0){
		// intermediate parent code
		cout << "INTERMEDIATE PERSON: My PID is " << getpid() << ", my parent's PID is " << getppid() << ", my child is " << pid2 << endl;
		// sleep
		sleep(3);
		// print message
		cout << "INTERMEDIATE CHILD: " << getpid() << " is awake." << endl;
		// close process
		exit(0);
	} else{ 	// parent code

		cout << "PARENT: My PID is " << getpid() << ", my parent's PID is " << getppid() << ", my children are " << pid << ", " << child2 << endl; 
		cout << "PARENT: Issuing command: /bin/ps -f --ppid " << getppid() << "," << getpid() << "," << pid << "," << child2 << endl;

		stringstream ss;		//preparing to execute the command
                ss << "/bin/ps -f --ppid " << getppid() << "," << getpid() << "," <<  pid << "," << child2;
                string temp = ss.str();
                const char * cmd = temp.c_str();
                //cout << "COMMAND: " << cmd << endl;
                system(cmd); 

			if(wait(&status) != child2 && wait(&status) != pid){		//wait for the child processes to finish
				cout << "PARENT: Children Processes are finished" << endl;
				exit(0);
			}
	}


return 0;
}
