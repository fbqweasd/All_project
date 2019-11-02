#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fs.h>
#include <time.h>
#include <string.h>

#define BUF_SIZ 1024

int main(){
	pid_t pid;
	int i;    int fd;
	
	time_t a_time;
	time_t b_time;
	struct tm *t;

	char buf[BUF_SIZ];
	ssize_t nr;

	pid = fork();
	if(pid == -1){
		printf("fork\n");	
		return -1; 
	} 

	if(pid != 0){
		exit(EXIT_SUCCESS);
	}
	
	if(setsid() == -1){
		return -1; 
	}
	if(chdir("/") == -1){
		printf("chdir()\n");
		return -1;
	}

	open("/dev/null", O_RDWR);
	dup(0);
	dup(0);

	// 데몬 작업
	fd = open("/opt/log/time.log", O_WRONLY | O_CREAT, 0644);
	a_time = time(NULL);

	while(1){
		b_time = time(NULL);
		if(b_time - a_time >= 1){
			t = localtime(&b_time);
			sprintf(buf,"%d %d, %d : %d %2d\n", t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			nr = write(fd, buf, strlen(buf));

			if(nr == -1){
				return -1;
			}
			a_time = b_time;
		}
	}

	return (0);

}
