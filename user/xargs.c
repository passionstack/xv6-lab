#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
	//一次读入输入长度
	int length;
	//存储一行的参数
	char buffer[64];
	//buffer数组的下标
	int curPlace = 0;
	//一个参数的起始地址
	//（初始情况下是buffer的第一个元素，因为第一个位置必为参数的起始位置）
	char* paraStart = buffer;
	//新顺序的参数的数组
	char* newPara[64];

	//除了xargs以外其余的输入参数
	char input[64];

	//标记在新的参数数组的结尾下标
	//是最后一个参数之后的一个下标
	int paraEnd;

	int i;
	//将xargs的参数都先放进新的参数数组中
	for (i = 1; i < argc; i++) {
		newPara[i - 1] = argv[i];
	}
	paraEnd = argc - 1;

	while ((length = read(0, input, sizeof(input))) > 0) {
		for (i = 0; i < length; i++) {
			//如果是参数的结尾
			if (input[i] == ' ') {
				buffer[curPlace++] = 0;
				//将当前的参数的首地址赋给新参数数组
				newPara[paraEnd++] = paraStart;

				//记录下一个参数的首地址
				paraStart = &(buffer[curPlace]);
			}
			//如果是一行的结尾
			else if (input[i] == '\n') {
				buffer[curPlace] = 0;
				newPara[paraEnd] = paraStart;

				paraStart = buffer;
				//标记为结尾
				newPara[++paraEnd] = 0;

				paraEnd = argc - 1;

				curPlace = 0;
				if (fork() == 0) {
					exec(argv[1], newPara);
				}

				wait(0);

			}
			//如果是参数的元素
			else {
				buffer[curPlace++] = input[i];
			}
		}
	}

	exit(0);


}
