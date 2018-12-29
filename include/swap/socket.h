/*
	Functions for performing socket operations
	Copyright (C) 2017 John Powell

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef JEP_SOCKET_H
#define JEP_SOCKET_H

#ifdef _WIN32
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#define JEP_INVALID_SOCKET INVALID_SOCKET
#define JEP_SOCKET_ERROR SOCKET_ERROR

#define JEP_SD_READ SD_RECEIVE
#define JEP_SD_WRITE SD_SEND
#define JEP_SD_READWRITE SD_BOTH

typedef SOCKET jep_socket;
typedef struct sockaddr jep_addr;
typedef struct addrinfo jep_addrinf;

#endif // _WIN32

#if defined(__unix__) || defined(__linux__) || defined(__MACH__)

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>

#define JEP_INVALID_SOCKET -1
#define JEP_SOCKET_ERROR -1

#define JEP_SD_READ SHUT_RD
#define JEP_SD_WRITE SHUT_WR
#define JEP_SD_READWRITE SHUT_RDWR

typedef int jep_socket;
typedef struct sockaddr jep_addr;
typedef struct addrinfo jep_addrinf;

#endif // __unix__ || __linux__

/**
 * initializes sockets
 */
int jep_socket_init();

/**
 * uninitializes sockets
 */
int jep_socket_quit();

/**
 * gets address info
 */
int jep_get_addr_info(jep_addrinf** inf, const char* address, const char* port);

/**
* releases the resources used by an address info structure
*/
void jep_free_addrinf(jep_addrinf* inf);

/**
 * creates a new socket
 */
jep_socket jep_socket_create(jep_addrinf* inf);

/**
 * binds a socket
 */
int jep_socket_bind(jep_socket s, jep_addrinf* inf);

/**
 * listens for connections on a socket
 */
int jep_socket_listen(jep_socket s, int backlog);

/**
 * connects to a socket
 */
int jep_socket_connect(jep_socket s, jep_addrinf* inf);

/**
 * accepts a connection to a socket
 */
jep_socket jep_socket_accept(jep_socket s, jep_addr* addr, int* addrlen);

/**
 * sends data over a socket connection
 */
int jep_socket_send(jep_socket s, char* buffer, size_t len, int flags);

/**
 * receives data over a socket connection
 */
int jep_socket_receive(jep_socket s, unsigned char* buffer, size_t len, int flags);

/**
 * closes a socket
 */
int jep_socket_close(jep_socket s);

/**
 * shuts down socket communication
 */
int jep_socket_shutdown(jep_socket s, int how);

/**
 * gets the most recent socket error code
 */
int jep_socket_get_error();

#endif // !JEP_SOCKET_H
