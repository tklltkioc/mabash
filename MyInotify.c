#include <stdio.h>
#include <sys/inotify.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>

//事件描述长度
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024*(EVENT_SIZE+1))
//创建一个inotify目录监视
void open_inotify(char* str){
    int i,len = 0;
    int fd;//文件描述符
    int wd;//监视点描述符
    char buffer[EVENT_BUF_LEN];
    int counter=0;
    //创建并且初始化
    fd = inotify_init();
    if(fd < 0){
        perror("inotify_init()");
        return;
    }
    //创建监视事件
    //第一个参数是inotify_init()返回值
	//第二个参数是被监视的目标的路径名
    //第三个参数是事件
    wd = inotify_add_watch(fd,str,IN_ACCESS|IN_CREATE|IN_DELETE|IN_MODIFY);
    while(1){//循环监听该目录发生的事件
        i = 0;
        //读取事件长度
        len = read(fd,buffer,EVENT_BUF_LEN);
        if(len < 0){
            perror("read");
            return;
        }
        while(len > i){
            //定义事件结构体指针
            struct inotify_event *event = (struct inotify_event*)&buffer[i];
            if(event->len){
                if(event->mask & IN_CREATE){
                    if(event->mask & IN_ISDIR){
                        printf("a new dir%s is created\n",event->name);
                    }
                    else{
                        printf("a new file%s is created\n",event->name);
                    }
                }
		else if(event->mask&IN_ACCESS){
		     if(event->mask&IN_ISDIR){
			printf("dir%sis accessed\n",event->name);			
		    }
		    else{
			printf("file%s is accessed\n",event->name);
			counter++;
                        printf("accessed is %d\n",counter);
		    }
		 }		
                else if(event->mask & IN_DELETE){
                    if(event->mask & IN_ISDIR){
                        printf("Dir%s is delete\n",event->name);
                    }
                    else{
                        printf("file%s is delete\n",event->name);
                    }
                }
                else if(event->mask & IN_MODIFY){
                    if(event-> mask & IN_ISDIR){
                        printf("Dir%s is modify\n",event->name);
                    }
                    else{
                        printf("File%s is modify\n",event->name);
                    }
                }
            }
            i += EVENT_SIZE + event->len;
	   // printf("accessed is %d\n",counter);
        }
    }
    //取消监视
    inotify_rm_watch(fd,wd);
    //关闭文件描述符
    close(fd);
    return;
}

int main(){
    open_inotify("./hello");
    return 0;
}
