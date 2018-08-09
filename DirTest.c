#include<stdio.h>
#include<dirent.h>
#include<fcntl.h>
#include<limits.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
typedef int Myfunc(const char *, const struct stat*, int);
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static long nreg, ndir, nblk,nchr, nfifo, nslink, nsock, ntot;
char *path_alloc(int* size);
int main(int argc, char *argv[])
{
	int ret;
	if (argc != 2)
	{
		printf("Please input correct paralam!\n");//��������
		return 1;
	}
	ret = myftw(argv[1], myfunc);
	ntot = nreg +  ndir +nblk+ nchr + nfifo + nslink + nsock;
	if (ntot == 0)
	{
		ntot = 1;
	}
	printf("normal=%71d,%5.2f%%\n", nreg, nreg*100.0 / ntot);
	printf("dirfile=%71d,%5.2f%%\n", ndir, ndir*100.0 / ntot);
	printf("blockdev=%71d,%5.2f%%\n", nblk, nblk*100.0 / ntot);
	printf("bitdev=%71d,%5.2f%%\n", nchr, nchr*100.0 / ntot);
	printf("FIFOS=%71d,%5.2f%%\n", nfifo, nfifo*100.0 / ntot);
	printf("slink=%71d,%5.2f%%\n", nslink, nslink*100.0 / ntot);
	printf("sock=%71d,%5.2f%%\n", nsock, nsock*100.0 / ntot);
	return ret;
}
char *path_alloc(int*size)
{
	char *p = NULL;
	if (!size)
	{
		return NULL;
	}
	p = malloc(256);
	if (p)
	{
		*size = 256;
	}
	else
	{
		*size = 0;
	}
	return p;
}
#define FTW_F 1
#define FTW_D 2            //Ŀ¼
#define FTW_DNR 3          //���ܶ���Ŀ¼
#define FTW_NS 4           //���ܻ��״̬���ļ�
static char *fullpath;     //���ÿ���ļ�������·��
static int myftw(char *pathname, Myfunc *func)
{
	int len;
	fullpath = path_alloc(&len);//��·������������һ������
	strncpy(fullpath, pathname, len);//�����ļ�����
	fullpath[len - 1] = 0;
	return(dopath(func));
}
//����ļ���״̬
static int dopath(Myfunc *func)
{
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	int ret;
	char *ptr;

	if (lstat(fullpath, &statbuf) < 0)//��ȡʧ��
	{
		return(func(fullpath, &statbuf, FTW_NS));
	}
	if (S_ISDIR(statbuf.st_mode) == 0)//�������Ŀ¼
	{
		return(func(fullpath, &statbuf, FTW_F));
	}
	if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
	{
		return(ret);
	}
	ptr = fullpath + strlen(fullpath);//ָ��·��β��
	*ptr++ = '/';
	*ptr = 0;
	if ((dp = opendir(fullpath)) == NULL)//������ܶ�Ŀ¼
	{
		return(func(fullpath, &statbuf, FTW_DNR));
	}
	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") == 0 ||
			strcmp(dirp->d_name, "..") == 0)
			continue;
		strcpy(ptr, dirp->d_name);
		if ((ret = dopath(func)) != 0)
			break;
	}
	ptr[-1] = 0;
	if (closedir(dp) < 0)
	{
		printf("can't close directory%s\n", fullpath);
	}
	return(ret);
}
static int myfunc(const char *pathname, const struct stat*statptr, int type)
{
	switch (type) {
	case FTW_F:
		switch (statptr->st_mode&S_IFMT){
	case S_IFREG: nreg++; break;
	case S_IFBLK: nblk++; break;
	case S_IFCHR: nchr++; break;
	case S_IFIFO: nfifo++; break;
	case S_IFLNK: nslink++; break;
	case S_IFSOCK: nsock++; break;
	case S_IFDIR:
		printf("for S_IFDIR for %s\n", pathname);
	}
	break;
	case FTW_D:
		ndir++;
		break;
	case FTW_DNR:
		printf("can't read directory %s\n", pathname);
		break;
	case FTW_NS:
		printf("stat error for %s\n", pathname);
		break;
	default:
		printf("unkonwn type %d for pathname %s\n", type, pathname);
    }
return(0);
}
