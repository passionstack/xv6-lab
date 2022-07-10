#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void outputPath(char* path)
{
	printf("%s\n", path);
}

//比较路径对应的文件是否和目标文件名一致
int compareFileName(char* path, char* fileName)
{
	//先找到路径末尾
	while (*path != 0) {
		path++;
	}
	//找到最后一个'/'
	while (*path != '/') {
		path--;
	}
	//用于存储文件名起始指针
	char* targetName;
	targetName = path + 1;
	if (strcmp(targetName, fileName) == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

//查找文件，参数为当前路径和目标文件名
void find(char* path, char* fileName)
{
	//文件号
	int fid;
	//文件状态
	struct stat fileState;
	//下一个文件的路径，文件路径结尾指针
	char nextPath[512], * pathEnd;
	//文件记录
	struct dirent fileRecord;
	//打开文件
	if ((fid = open(path, O_RDONLY)) < 0) {
		fprintf(2, "文件%s打开失败\n", path);
		return;
	}
	//取文件状态
	if (fstat(fid, &fileState) < 0) {
		fprintf(2, "文件%s取状态失败\n", path);
		close(fid);
		return;
	}
	//如果文件是非文件夹文件
	if (fileState.type == T_FILE) {
		if (compareFileName(path, fileName) == 1) {
			outputPath(path);
		}
	}
	//如果是文件夹
	else if(fileState.type == T_DIR){
		strcpy(nextPath, path);

		pathEnd = nextPath;
		//找到字符串末尾
		while (*pathEnd != 0) {
			pathEnd++;
		}
		*pathEnd = '/';
		pathEnd++;
		//从文件夹中每次读取一个文件
		while (read(fid, &fileRecord, sizeof(fileRecord)) == sizeof(fileRecord)) {
			//不能进入.或..的递归
			if (fileRecord.inum == 0 || fileRecord.inum == 1 || strcmp(fileRecord.name, ".") == 0 || strcmp(fileRecord.name, "..") == 0) {
				continue;
			}
			else {
				//将文件名接入下一文件路径的结尾
				memmove(pathEnd, fileRecord.name, strlen(fileRecord.name));
				pathEnd[strlen(fileRecord.name)] = 0;
				find(nextPath, fileName);
			}
		}
	}
	close(fid);

}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("参数数量错误\n");
		exit(0);
	}
	else {
		find(argv[1], argv[2]);
		exit(0);
	}
}
