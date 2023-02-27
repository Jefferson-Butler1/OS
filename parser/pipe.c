#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    
    int fds [2];
    int rc = pipe(fds);
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "pipe failed\n");
	exit(1);
    }
    else{
    	rc = fork();
    	if (rc < 0) {
        	// fork failed; exit
        	fprintf(stderr, "fork failed\n");
        	exit(1);
    	} else if (rc == 0) {
		// child: redirect standard output to a file
		close(1);
		dup(fds[1]);
		close(fds[0]);
		close(fds[1]);	

		// now exec "wc"...
       		char *myargs[3];
        	myargs[0] = strdup("ls");   // program: "wc" (word count)
        	myargs[1] = strdup("/");    // argument: file to count
        	myargs[2] = NULL;           // marks end of array
        	execvp(myargs[0], myargs);  // runs word count
    	} else {
        	// parent goes down this path (original process)
		rc = fork();
		if(rc < 0){fprintf(stderr, "bad fork"); exit(1);}
		if(rc == 0){
	    		close(0);
	    		dup(fds[0]);
			close(fds[0]);
			close(fds[1]);
			     
           	 	char *myargs[4];
            		myargs[0] = strdup("tail");   // program: "wc" (word count)
            		myargs[1] = strdup("-n"); // argument: file to count
            		myargs[2] = strdup("5"); // argument: file to count
            		myargs[3] = NULL;           // marks end of array
            		execvp(myargs[0], myargs);  // runs word count

		}
		else{
        		int wc = wait(NULL);
			assert(wc >= 0);
		}
	}
    }
    return 0;
}
