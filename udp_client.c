#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
int main(void) {
	WSADATA wsa_data;
	SOCKET socket_descriptor;
	SOCKADDR_IN address_data;
	char message_text[80], s_ip[64], s_port[5];
	int iMessageLen, sent_bytes_count, i_port;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) { //Loads Winsock v2.2.
		printf("ERROR: WSAStartup failed with error %d.\n", WSAGetLastError());
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	printf("IP: ");
	scanf_s("%s", &s_ip, sizeof s_ip);
	address_data.sin_addr.s_addr = inet_addr(s_ip);
	if (address_data.sin_addr.s_addr == INADDR_NONE) {
		printf("ERROR: Invalid IP: %s\n", s_ip);
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	printf("Port: ");
	scanf_s("%s", &s_port, sizeof s_port);
	getchar(); //Removes newline from buffer.
	i_port = atoi(s_port);
	if (i_port <= 0 || i_port > 65563) {
		printf("ERROR: Invalid port number.\n");
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	address_data.sin_port = htons(i_port);
	address_data.sin_family = AF_INET;
	socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //Creates an UDP socket.
	if (socket_descriptor == SOCKET_ERROR) {
		printf("ERROR: Failed in calling socket() with error %d.\n", WSAGetLastError());
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	printf("Message: ");
	scanf_s("%[^\n]%*c", &message_text, sizeof(message_text));
	//fgets(message_text, sizeof(message_text), stdin);
	iMessageLen = strlen(message_text);
	sent_bytes_count = sendto(socket_descriptor, message_text, iMessageLen, 0, (struct sockaddr *) &address_data, sizeof(address_data)); //Sends message.
	if (sent_bytes_count == SOCKET_ERROR) {
		printf("ERROR: Failed in calling sendto() with error %d.\n", WSAGetLastError());
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	printf("Message sent to %s.\n", s_ip);
	closesocket(socket_descriptor);
	WSACleanup();
	printf("Press anything to exit.");
	_getch();
	return 0;
}