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
		int chdirtemp = 0;//ƫ���������м���
		int fd;//�ļ�������
		char filetimebuf[3];//�ļ�ʱ����Ϣ
		char dirtimebuf[5];//Ŀ¼ʱ����Ϣ
		char dirnamebuf[10] = "DIR";//ʱ���֡�����Ϣ������
		char filenamebuf[10] = "File";//�ļ�������
		char npath[100];//����·��
		sleep(60);
		time(&timetemp);
		printf("current time is%s", asctime(gmtime(&timetemp)));
		p = localtime(&timetemp);//���ʱ����������Թ��������ļ�
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
		//struct timeval timenow, timeold;//ʱ�����
		//struct timezone timez;
		//int j = 0;
		//int writeCounter = 0;//������
		//gettimeofday(&timeold, &timez);//��ȡʱ����Ϣ
		//if (argc != 2)//����������
		//{
		//	printf("Plz as'./exam39 string'!\n");
		//	return 1;
		//}
		//fd = open(*(argv + 1), O_RDWR | O_CREAT, S_IRWXU);//���ļ���û���򴴽�
		//while (1)//������ѭ��
		//{
		//	while (1)//1������ʱ
		//	{
		//		gettimeofday(&timenow, &timez);//��ȡ��ǰʱ�����
		//		time(&filetime);
		//		p = localtime(&filetime);//���ʱ����������Թ��������ļ�
		//		sprintf(timebuf, "%02d%02d%02d", p->tm_hour, p->tm_min, p->tm_sec);
		//		printf("%d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
		//		gettimeofday(&timenow, &timez);
		//		if ((timenow.tv_sec - timeold.tv_sec) == 1)//��1��
		//		{
		//			timeold = timenow;//���±����ʱ����Ϣ
		//			break;//1��ʱ�䵽���˳�
		//		}
		//	}
		//	if (timeold.tv_sec == 0)//�����0��
		//	{
		//		strcat(filenamebuf, timebuf);//�����ļ���
		//		fd = open(filenamebuf, O_RDWR | O_CREAT, S_IRWXU);
		//	}
		//	time(&timetemp);
		//	sprintf(writebuf, "%s", ctime(&timetemp));//����ǰʱ���������д����
		//	printf("%s", &writebuf);
		//	if (writeCounter == 0)//�״�д��
		//	{
		//		temp = write(fd, writebuf, strlen(writebuf));//д������
		//		seektemp = lseekP(fd, 0, SEEK_CUR);//��õ�ǰ��ƫ����
		//		writeCounter++;//д�������
		//	}
		//	else
		//	{
		//		j = strlen(writebuf)*writeCounter;//���ƫ����
		//		seektemp = lseek(fd, j, SEEK_SET);
		//		temp = write(fd, writebuf, strlen(writebuf));
		//		writeCounter++;
		//	}
		//}
		//close(fd);
		//return 0;

}
