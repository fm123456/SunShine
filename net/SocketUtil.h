#ifndef MUDUO_NET_SOCKET_H
#define MUDUO_NET_SOCKET_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>


#define CONNECT_TIMEOUT -1

struct tcp_info;
class InetAddress;
class SocketUtil 
{
 public:
 
	static int32_t CreateSocket(bool is_noblock = true, bool is_tcp = true);

	static int32_t Bind(int32_t fd, const std::string& addr, int32_t port);

	static bool TcpConnect(const std::string& addr, int32_t port, int32_t& socket_fd);

	static bool TcpListen(const std::string& addr, int32_t port, int32_t& socket_fd);

	static bool Accept(int32_t fd, int32_t& connect_fd, std::string& connect_addr, int32_t& connect_port);

	static void SetTcpNoDelay(int32_t fd, bool on);
   
	static void SetReuseAddr(int32_t fd, bool on);
   
	static void SetReusePort(int32_t fd, bool on);
   
	static void SetKeepAlive(int32_t fd, bool on);

	static void SetTcpLinger(int32_t fd, short u_onoff, short u_linger);

	static void BindAddr(const std::string& addr, int32_t port, struct sockaddr_in& sock_addr);
};

#endif  
