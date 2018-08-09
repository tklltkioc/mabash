#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>

int main(int argc, char *argv)
{
	while (1)
	{
		time_t filetime;
		time_t timetemp;
		struct tm *p;
		DIR *dp;
		int temp = 0;
		int chdirtemp = 0;//偏移量计算中间量
		int fd;//文件描述符
		char filetimebuf[3];//文件时间信息
		char dirtimebuf[5];//目录时间信息
		char dirnamebuf[10] = "DIR";//时、分、秒信息缓冲区
		char filenamebuf[10] = "File";//文件名缓冲
		char npath[100];//完整路径
		sleep(60);
		time(&timetemp);
		printf("current time is%s", asctime(gmtime(&timetemp)));
		p = localtime(&timetemp);//获得时分秒参数，以供创建新文件
		printf("%d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
		sprintf(dirtimebuf, "%02d%02d", p->tm_hour, p->tm_min);
		sprintf(filetimebuf, "%02d", p->tm_sec);
		strcat(filenamebuf, filetimebuf);
		strcat(dirnamebuf, dirtimebuf);
		printf("%s\n", filenamebuf);
		printf("%s\n", dirnamebuf);
		dp = opendir(dirnamebuf);
		if (dp == NULL)
		{
			printf("dir %s not existence\n", dirnamebuf);
			temp = mkdir(dirnamebuf, S_IRWXU | S_IRGRP | S_IXOTH);
			if (temp == -1)
			{
				printf("failue\n");
				return 1;
			}
			else
			{
				printf("creat dir%s success\n", dirnamebuf);
				chdirtemp = chdir(dirnamebuf);
				if (chdirtemp == -1)
				{
					printf("chdir wrong\n");
					return 2;
				}
				else
				{
					if (getcwd(npath, 100) == NULL)
					{
						printf("can't get path\n");
						return 3;
					}
					else
					{
						printf("dirpath is%s\n", npath);
					}
					fd = open(filenamebuf, O_RDWR | O_CREAT, S_IRWXU);
					if (fd != -1)
					{
						printf("creat file%s success\n", filenamebuf);
						close(fd);
					}
					else
					{
						printf("file fauile\n");
						return 4;
					}
				}
			}
		}
		else
		{
			printf("dir %s already estence\n", dirnamebuf);
			closedir(dp);
			chdirtemp = chdir(dirnamebuf);
			if (chdirtemp == -1)
			{
				printf("change dir wrong\n");
				return 2;
			}
			else
			{
				if (getcwd(npath, 100) == NULL)
				{
					printf("can't get dirpath\n");
					return 3;
				}
				else
				{
					printf("fullpath is%s\n", npath);
				}
				fd = open(filenamebuf, O_RDWR | O_CREAT, S_IRWXU);
				if (fd != -1)
				{
					printf("creat file%s success\n", filenamebuf);
					close(fd);
				}
				else
				{
					printf("creat file fauile\n");
					return 4;
				}
			}
		}
	}
		return 0;
		//struct timeval timenow, timeold;//时间变量
		//struct timezone timez;
		//int j = 0;
		//int writeCounter = 0;//计数器
		//gettimeofday(&timeold, &timez);//获取时间信息
		//if (argc != 2)//若参数错误
		//{
		//	printf("Plz as'./exam39 string'!\n");
		//	return 1;
		//}
		//fd = open(*(argv + 1), O_RDWR | O_CREAT, S_IRWXU);//打开文件，没有则创建
		//while (1)//进入主循环
		//{
		//	while (1)//1毫秒延时
		//	{
		//		gettimeofday(&timenow, &timez);//获取当前时间参数
		//		time(&filetime);
		//		p = localtime(&filetime);//获得时分秒参数，以供创建新文件
		//		sprintf(timebuf, "%02d%02d%02d", p->tm_hour, p->tm_min, p->tm_sec);
		//		printf("%d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
		//		gettimeofday(&timenow, &timez);
		//		if ((timenow.tv_sec - timeold.tv_sec) == 1)//差1秒
		//		{
		//			timeold = timenow;//更新保存的时间信息
		//			break;//1秒时间到，退出
		//		}
		//	}
		//	if (timeold.tv_sec == 0)//如果是0秒
		//	{
		//		strcat(filenamebuf, timebuf);//创建文件名
		//		fd = open(filenamebuf, O_RDWR | O_CREAT, S_IRWXU);
		//	}
		//	time(&timetemp);
		//	sprintf(writebuf, "%s", ctime(&timetemp));//将当前时间参数放入写缓冲
		//	printf("%s", &writebuf);
		//	if (writeCounter == 0)//首次写入
		//	{
		//		temp = write(fd, writebuf, strlen(writebuf));//写入数据
		//		seektemp = lseekP(fd, 0, SEEK_CUR);//获得当前的偏移量
		//		writeCounter++;//写入计数器
		//	}
		//	else
		//	{
		//		j = strlen(writebuf)*writeCounter;//获得偏移量
		//		seektemp = lseek(fd, j, SEEK_SET);
		//		temp = write(fd, writebuf, strlen(writebuf));
		//		writeCounter++;
		//	}
		//}
		//close(fd);
		//return 0;

}
