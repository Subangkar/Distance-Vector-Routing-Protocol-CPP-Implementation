//
// Created by subangkar on 12/6/18.
//

#ifndef DVR_SOCKET_H
#define DVR_SOCKET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

#define BUFFER_SIZE_SOCKET 1024


sockaddr_in getInetSocketAddress(const char *ipAddress, uint16_t port, sa_family_t sin_family = AF_INET) {
	sockaddr_in sockAddr{};
	sockAddr.sin_family = sin_family;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.s_addr = inet_addr(ipAddress);
	return sockAddr;
}

// returns bind flag
bool getSocket(int &sockfd, const sockaddr_in &inetSocketAddr, int domain = AF_INET, int type = SOCK_DGRAM,
               int protocol = 0) {
	sockfd = socket(domain, type, protocol);
	return !bind(sockfd, (struct sockaddr *) &inetSocketAddr, sizeof(sockaddr_in));
}


class Socket {
private:
	std::string localInetAddress;
	sockaddr_in socketAddr{};
	size_t buffer_size;
	int socketFileDescriptor{};
	bool bound = false;

	sockaddr_in lastRemoteAddress{};
	ssize_t lastBufferSize{};

	char *buffer{};

	void createSocket() {
		bound = getSocket(socketFileDescriptor, socketAddr);
		if (bound)
			buffer = new char[buffer_size];
	}

public:
	Socket() {
		bound = false;
		buffer = nullptr;
	}

//	Socket(const sockaddr_in &socketAddr, size_t buffer_size = BUFFER_SIZE_SOCKET) : socketAddr(socketAddr),
//	                                                                                 buffer_size(buffer_size) {
//		createSocket();
//	}

	Socket(const char *ipAddress, uint16_t port, size_t buffer_size = BUFFER_SIZE_SOCKET) :
			localInetAddress(ipAddress),
			buffer_size(buffer_size) {
		socketAddr = getInetSocketAddress(ipAddress, port);
		this->buffer_size = buffer_size;
		createSocket();
	}

	Socket(const Socket &socket) {
		localInetAddress = socket.localInetAddress;
		socketAddr = socket.socketAddr;
		buffer_size = socket.buffer_size;
		socketFileDescriptor = socket.socketFileDescriptor;
		bound = socket.bound;
		if (bound)buffer = new char[buffer_size];
	}

	const char *readBytes(sockaddr_in &remoteAddress) {
		socklen_t addrlen;
		lastBufferSize = recvfrom(socketFileDescriptor, buffer, buffer_size, 0, (struct sockaddr *) &lastRemoteAddress,
		                          &addrlen);
//		std::cout << lastBufferSize << std::endl;
		if (lastBufferSize < buffer_size) buffer[lastBufferSize] = '\0';
		return buffer;
	}

	long dataLength() {
		return lastBufferSize;
	}

	std::string readString(sockaddr_in &remoteAddress) {
		readBytes(remoteAddress);
		return lastBufferSize > 0 ? std::string(buffer, lastBufferSize) : "";
	}

	ssize_t writeString(const sockaddr_in &remote_address, const std::string &str) {
		return sendto(socketFileDescriptor, str.data(), str.size(), 0, (sockaddr *) &remote_address,
		              sizeof(sockaddr_in));
	}

	const sockaddr_in &getLastRemoteAddress() const {
		return lastRemoteAddress;
	}

	bool isBound() {
		return bound;
	}

	virtual ~Socket() {
		if (bound)
			delete[] buffer;
	}

	Socket &operator=(const Socket &socket) {
		localInetAddress = socket.localInetAddress;
		socketAddr = socket.socketAddr;
		buffer_size = socket.buffer_size;
		socketFileDescriptor = socket.socketFileDescriptor;
		bound = socket.bound;
		if (bound) {
			delete[] buffer;
			buffer = new char[buffer_size];
		}
		return *this;
	}


	const std::string getLocalIP() {
		return std::string(inet_ntoa(socketAddr.sin_addr));
//		return localInetAddress;
	}

	in_port_t getLocalPort() {
		return ntohs(socketAddr.sin_port);
//		return socketAddr.sin_port;
	}
};

#endif //DVR_SOCKET_H
