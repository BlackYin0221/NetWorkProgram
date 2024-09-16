#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
void error_handling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int index = 0, readLen = 0;

	if (argc != 3)
	{
		printf("Usage : %s <port> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		error_handling("WSAStartup error()!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		error_handling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		error_handling("connect() error");
	}

	while (readLen = recv(hSocket, &message[index++], 1, 0)) {
		if (readLen == -1) {
			error_handling("read() error!");
		}
		strLen += readLen;
		if (message[index - 1] == '\0') {
			break;
		}
	}

	printf("Message from server : %s \n", message);
	printf("Function read call count %d \n", strLen);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}