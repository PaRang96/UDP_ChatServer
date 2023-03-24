#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN ServerSocketAddress;
	ZeroMemory(&ServerSocketAddress, sizeof(ServerSocketAddress));
	ServerSocketAddress.sin_addr.s_addr = INADDR_ANY;
	ServerSocketAddress.sin_port = htons(3000);
	ServerSocketAddress.sin_family = PF_INET;

	bind(ServerSocket, (SOCKADDR*)&ServerSocketAddress, sizeof(ServerSocketAddress));
	if (ServerSocket == SOCKET_ERROR)
	{
		cout << "Binding Failed: " << GetLastError() << endl;
		return -1;
	}

	char Buffer[1024] = { 0, };
	string ActualMessage;

	int RecvLen;

	while (true)
	{
		SOCKADDR_IN ClientSocketAddress;
		ZeroMemory(&ClientSocketAddress, sizeof(ClientSocketAddress));
		int ClientSocketAddressLen = sizeof(ClientSocketAddress);
		RecvLen = recvfrom(ServerSocket, Buffer, sizeof(Buffer)-1, 0,
			(SOCKADDR*)&ClientSocketAddress, &ClientSocketAddressLen);

		if (RecvLen < 0)
		{
			cout << "Faulty Input from Client" << endl;
		}

		ActualMessage = string(Buffer);

		string sender = string(inet_ntoa(ClientSocketAddress.sin_addr));

		cout << sender << " : " << ActualMessage << endl;

		ZeroMemory(&Buffer, 1024);
	}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}