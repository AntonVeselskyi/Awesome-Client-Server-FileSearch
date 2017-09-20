#pragma once
#include <iostream>
#include <string>
#include <vector>

#ifdef unix

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>	//to use close() and usleep()
#include <arpa/inet.h> //to use inet_ntoa()

#include <stdlib.h>
#include <cstring> //to use memset
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#include "SearchFile_L.h"
#include <pthread.h>
 
#elif _WIN32

#include <WinSock2.h>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")

#include "SearchFile.h"
#include <process.h>
#else

#endif



using namespace std;

class Client
{
public:

	class FindFile
	{
		string filename_to_find; //file that we looking for
	public:
		void StartSearching(Client &SC);
		void SetFileName(Client &SC); //to find
		
	};
	FindFile *find;


	//Method that let the user to interact with server connection
	void InitialiseWinSock();
	void CreateClient();
	void StartServerWork();
	bool Connect(string ip);
	void ShutSocket();

#ifdef unix
	friend void* processing_Linux(void* tmp);
#elif _WIN32
	friend void processing_Win(void* tmp);
#endif

	//Methods that works with sockets
        int Send();
        void SendFoundData();

	int Recv();

	Client();
	~Client();

private:

#ifdef unix
	pthread_mutex_t mutex;
#elif _WIN32
	HANDLE mutex;
#endif
        vector<string> data;//found data lays here
	sockaddr_in sock_addr;
	char* recv_buf;
	string send_buf;
	SOCKET mainSocket; //here lays descriptor
	friend class FindFile;
	
	bool process;

	const int  DEFAULT_BUFLEN;
	const int REQ_WINSOCK_VER;	//Min WinSock version

}; //class Client creates and connects socket

