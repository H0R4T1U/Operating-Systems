#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char** argv){
	int n=0;
	printf("n:");
	scanf("%d",&n);
	// pipes
	int p2a[2],a2b[2],b2p[2];
	pipe(p2a);
	pipe(a2b);
	pipe(b2p);
	
	if(fork() == 0){
		//PROCESS A
		close(b2p[0]);close(b2p[1]);
		close(p2a[1]);close(a2b[0]);
		while(1){
			if(read(p2a[0],&n,sizeof(int)) <= 0){
				break;
			}
			if(n<=0){
				break;
			}
			printf("A:%d, sending to B->%d\n",n,n-1);
			n = n-1;
			write(a2b[1],&n,sizeof(int));

		}
		close(a2b[1]);
		close(p2a[0]);
		exit(0);
	}
	if(fork() == 0){
		// PROCESS B
		close(p2a[0]);close(p2a[1]);
		close(a2b[1]);close(b2p[0]);
		while(1){
			if(read(a2b[0],&n,sizeof(int)) <= 0){
				break;
			}
			if(n<=0){
				break;
			}
			printf("B:%d, sending to parent ->%d\n",n,n-1);
			n = n-1;
			write(b2p[1],&n,sizeof(int));
		}
		close(a2b[0]);
		close(b2p[1]);
		exit(0);
	}
	//main
	close(a2b[0]);close(a2b[1]);
	close(p2a[0]);close(b2p[1]);
	while(1){
		printf("Parent:%d, sending to A -> %d\n",n,n-1);
		n = n-1;
		write(p2a[1],&n,sizeof(int));
		if(read(b2p[0],&n,sizeof(int)) <= 0){
			break;
		}
		if(n<=0){
			break;
		}

	}
	close(p2a[1]);
	close(b2p[0]);
	wait(0);wait(0);
	exit(0);
}
