#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>



#ifdef unix

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h> //to use inet_ntoa()
#include <cstring> //to use memset
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1\

#include <pthread.h>

#elif _WIN32

#include <WinSock2.h>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")

#include <process.h>
#else

#endif
using namespace std;


#ifdef unix
	void* WorkWithNewClient(void* a);
	
#elif _WIN32
	void WorkWithNewClient(void* a);
	
#endif


static const int DEFAULT_BUFLEN=512;





class Server
{
public:

	//Methods that let the user to interact with client
	void InitialiseWinSock();
	static bool StartClientWork(SOCKET remoteSocket, char* recv_buf);
	void CreateBindServer();
	void HandleConnection(const sockaddr_in &sockAddr);
	void SendNameToFind(SOCKET remoteSocket);
	void StartGetingData(SOCKET remoteSocket, char* recv_buf, vector<string> &data);
	void Listen();
	void Accept();

	//Method that works with data
	void set_find_name(const string file_name);
	SOCKET get_remoteSocket();
	void show_found_data(vector<string> data);

	//Method that works with sockets
	Server();
	~Server();

private:
	deque<SOCKET> remoteSocket;
	int socket_counter;
	SOCKET serverSocket; //here lays descriptor
	string filename_to_find; //file that we looking for

	int recv_buf_len;
	char recv_buf[512];

	

	const int REQ_WINSOCK_VER;
	sockaddr_in sock_addr;

	sockaddr_in remoteAddr;
#ifdef _WIN32
	int remoteAddr_len;
#elif unix
	unsigned remoteAddr_len;
#endif
	
}; //class Server creates, binds and put socket in listening mode

