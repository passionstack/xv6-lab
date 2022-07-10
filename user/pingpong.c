#include "kernel/types.h"
#include "user/user.h"

int main()
{
	//分别对应父->子和子->父的pipe
	int pipe1[2], pipe2[2];
	//读取的信息
	char buffer[] = { 'A' };
	//记录传输信息的长度
	long length = sizeof(buffer);
	//父进程写
	pipe(pipe1);
	//子进程写
	pipe(pipe2);

	int pid = fork();

	if (pid < 0) {
		printf("进程创建失败\n");
		exit(1);
	}
	//如果是父进程
	else if (pid > 0) {
		close(pipe1[0]);
		close(pipe2[1]);
		//父进程写
		if (write(pipe1[1], buffer, length) != length) {
			printf("a->b 父进程写错误\n");
			exit(1);
		}
		//父进程读
		if (read(pipe2[0], buffer, length) != length) {
			printf("a<-b 父进程读错误\n");
			exit(1);
		}
		printf("%d: received pong\n", getpid());
		//等待子进程退出，防止僵死
		wait(0);
		exit(0);

	}
	//如果是子进程
	else {
		close(pipe1[1]);
		close(pipe2[0]);
		if (read(pipe1[0], buffer, length) != length) {
			printf("a->b 子进程读错误");
			exit(1);
		}
		printf("%d: received ping\n", getpid());
		if (write(pipe2[1], buffer, length) != length) {
			printf("a<-b 子进程写错误");
			exit(1);
		}
		exit(0);
	}

}
