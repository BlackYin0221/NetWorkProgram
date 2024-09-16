#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
void error_handling(char* message);
void hello_world(int argc, char* argv[]);
void endian_conv();

int main(int argc, char* argv[])
{
	endian_conv();
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void hello_world(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;

	char message[] = "Hello World";

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		error_handling("WSAStartup error()!");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) {
		error_handling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		error_handling("bind() error");
	}

	if (listen(hServSock, 5) == -1) {
		error_handling("listen() error");
	}

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET) {
		error_handling("accpt() error");
	}

	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void endian_conv()
{
	WSADATA wsaData;
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned long host_addr = 0x12345678;
	unsigned long net_addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		error_handling("WSAStartup() error!");
	}
	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("Host orderd port: %#x \n", host_port);
	printf("Network orderd port: %#x \n", net_port);
	printf("Host orderd addrs: %#lx \n", host_addr);
	printf("Network orderd port: %#lx \n", net_addr);
	WSACleanup();
	return;

}