#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char* argv[])
{
	if (argc != 2) {
		fprintf(2, "参数数量错误\n");
		exit(1);
	}
	else {
		//得到睡眠时间
		int sleepTime = atoi(argv[1]);
		if (sleepTime < 0) {
			fprintf(2, "睡眠时间非法\n");
		}
		sleep(sleepTime);
		exit(0);
	}
}
