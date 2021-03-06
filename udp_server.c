#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
int main(void) {
	char received_message[512], s_port[5];
	int received_bytes_count, i_port, size_remote_addr;
	WSADATA wsa_data;
	SOCKET socket_descriptor;
	SOCKADDR_IN address_data, remote_address_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) { //Loads Winsock v2.2.
		printf("ERROR: WSAStartup failed with error %d.\n", WSAGetLastError());
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	printf("Port: ");
	scanf_s("%s", &s_port, sizeof s_port);
	i_port = atoi(s_port);
	if (i_port <= 0 || i_port > 65563) {
		printf("ERROR: Invalid port number.\n");
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	address_data.sin_port = htons(i_port); //Host-byte ordering to network-byte ordering (least-significant-byte ordering to most-significant-byte ordering) in short type.
	address_data.sin_family = AF_INET;
	address_data.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY is for any IP address.
	socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //Creates an UDP socket.
	if (socket_descriptor == SOCKET_ERROR) {
		printf("ERROR: Failed in calling socket() with error %d.\n", WSAGetLastError());
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	if (bind(socket_descriptor, (struct sockaddr *) &address_data, sizeof(address_data)) == SOCKET_ERROR) { //Binds to address and port.
		printf("ERROR: Failed in calling bind() with error %d.\n", WSAGetLastError());
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	printf("Waiting for message (Press Ctrl-C to exit)...");
	size_remote_addr = sizeof(remote_address_data);
	received_bytes_count = recvfrom(socket_descriptor, received_message, sizeof received_message, 0, (struct sockaddr *) &remote_address_data, &size_remote_addr); //Receives datagram.
	if (received_bytes_count == SOCKET_ERROR) {
		printf("\nERROR: Failed in calling recvfrom() with error %d.\n", WSAGetLastError());
		printf("Press anything to exit.");
		_getch();
		exit(1);
	}
	received_message[received_bytes_count] = '\0';
	printf("\n%s: %s\n", inet_ntoa(remote_address_data.sin_addr), received_message);
	closesocket(socket_descriptor);
	WSACleanup();
	printf("Press anything to exit.");
	_getch();
	return 0;
}
