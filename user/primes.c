#include "kernel/types.h"
#include "user/user.h"


void prime(int* data, int num)
{
	//如果data中只剩一个数字，那么必是质数
	if (num == 1) {
		printf("prime %d\n", *data);
		return;
	}
	int myPipe[2], currentPrime, input;

	//记录当前的第一个数字，即当前输出的质数
	currentPrime = *data;
	printf("prime %d\n", currentPrime);
	pipe(myPipe);

	//子进程
	if (fork() == 0) {
		for (int i = 0; i < num; i++) {
			//将data中的数据传到管道中
			write(myPipe[1], (char*)(data + i), 4);
		}
		exit(0);
	}

	//父进程
	//将写通道关闭
	close(myPipe[1]);
	//子进程
	if (fork() == 0) {
		//记录不被currentPrime整除的数字的个数
		int primeNum = 0;
		//用于读取pipe中的数据
		char buffer[4];
		//将data中不被currentPrime整除的数字重新组成一个data数组，
		//将其余的数字覆盖掉，确保data的第一项必是质数
		while (read(myPipe[0], buffer, 4) > 0) {
			//将buffer中的内容转化为int
			input = *((int*)buffer);
			//如果不被整除
			if (input % currentPrime != 0) {
				//使用input的值覆盖data中原有的值
				//如果有出现被currentPrime整除的，说明不是质数或者为currentPrime本身，那么就会被覆盖掉
				*(data + primeNum) = input;
				primeNum++;
			}
		}
		//继续找下一个质数
		prime(data, primeNum);
		exit(0);

	}

	wait(0);
	wait(0);

}


int main()
{
	int data[34];
	for (int i = 0; i < 34; i++) {
		//因为从2开始
		data[i] = i + 2;
	}
	prime(data, 34);
	exit(0);
}
