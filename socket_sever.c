#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>

#define EHCO_PORT 8080
#define MAX_CLIENT_NUM 10

int main()
{
	int	sock_fd;
	struct	sockaddr_in serv_addr;
	int	clientfd;
	struct	sockaddr_in clientAdd;
	char	buff[101];
	socklen_t	len;
	int 	n;
	

	/*创建socket*/

	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd == -1){
		perror("creat socket error!");
		return 0 ; 
	}
	else{
		printf("Success to create socket %d\n",sock_fd);
	}


	/*设置server地址结构*/

	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(EHCO_PORT);
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	bzero(&(serv_addr.sin_zero),8);

	
	/*把地址和套接字绑定*/

	if(bind(sock_fd, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) != 0){
		printf("bind address fail! %d\n",errno);
		close(sock_fd);
		return 0;
	} else{
		printf("Success to bind address!\n");
	}


	/*设置套接字监听*/

	if(listen(sock_fd,MAX_CLIENT_NUM) != 0){
		perror("listen socket error!\n");
		close(sock_fd);
		return 0;
	} else{
		printf("Success to listen!\n");
	}


	/*创建新连接对应的套接字*/

	len = sizeof(clientAdd);
	clientfd = accept(sock_fd, (struct sockaddr*)& clientAdd, &len);
	if(clientfd <= 0){
		perror("accept() error!\n");
		close(sock_fd);
		return 0;
	}


	/*接收用户发来的数据*/

	while((n = recv(clientfd,buff, 100, 0)) > 0){
		buff[n] = '\0';
		printf("number of receive bytes = %d data = %s\n", n, buff);
	fflush(stdout);		//强制将缓冲区的内容立刻输出
	send(clientfd , buff , n , 0);
	if(strncmp(buff,"quit",4) == 0)
		break;
	}

	close(clientfd);
	close(sock_fd);

	return 0 ; 
	
}


