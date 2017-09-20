#include "Client.h"


#ifdef _WIN32
 void processing_Win(void* tmp)
{
	
	bool &working = ((Client*)tmp)->process;

	while (working)
	{
		WaitForSingleObject(((Client*)tmp)->mutex, INFINITE);
		send(((Client*)tmp)->mainSocket, "Processing", 512, 0);
		ReleaseMutex(((Client*)tmp)->mutex);
		Sleep(500);
	}
	
}

#elif unix
 void* processing_Linux(void* tmp)
 {
	 bool &working = ((Client*)tmp)->process;

	 while (working)
	 {
		 pthread_mutex_lock(&(((Client*)tmp)->mutex));
		 send(((Client*)tmp)->mainSocket, "Processing", 512, 0);
		 pthread_mutex_unlock(&(((Client*)tmp)->mutex));
		 usleep(500000);
	 }

	 return NULL;
 }
#endif




void Client::InitialiseWinSock()
{
#ifdef _WIN32
	WSADATA wsaData;
	//Initializing winsock

	if (WSAStartup(MAKEWORD(REQ_WINSOCK_VER, 2), &wsaData) == 0)
	{
		// check is max available WinSock is ok for me
		if (LOBYTE(wsaData.wVersion) >= REQ_WINSOCK_VER)
		{
			cout << "initialized.\n";

		}
		else
		{
			cerr << "required version of WinSock not supported!\n";

			WSACleanup();

		}
	}
#endif
	cout << "Starting to get message from server and start searching." << endl;
	
}


//--------------------------------------------------------------------------------
void Client::ShutSocket()
{
	shutdown(mainSocket, 2);
	mainSocket = INVALID_SOCKET;
}

//--------------------------------------------------------------------------------
void Client::CreateClient()
{

	mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mainSocket == INVALID_SOCKET)
	{
		cerr << "unable to create new socket!\n";
		ShutSocket();
        exit(1);

	}


}


//--------------------------------------------------------------------------------
bool Client::Connect(string ip)
{
        if (mainSocket == INVALID_SOCKET)
        exit(1);

	sockaddr_in sock_addr;

	//set server port and IP
	sock_addr.sin_family =  AF_INET; 
	sock_addr.sin_port= htons(10001); //Host TO Network Short
	sock_addr.sin_addr.s_addr = inet_addr(ip.c_str());

	cout << "Connection started\n";
	//connect mainSocket with serverSocket
	if (connect(mainSocket, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
	{
		cerr << "Unable to connect!\n";
		return false;

	}
	else
	{
		cout << "connected\n\n";
		return true;
	}
}



//--------------------------------------------------------------------------------
void Client::StartServerWork()
{
        if (mainSocket == INVALID_SOCKET)
        exit(1);

        if (send(mainSocket, "Start\0", DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
	{
		cout << "unable to send unable to send starting message\n " << endl;
		ShutSocket();

	}
	
	if (recv(mainSocket, recv_buf,DEFAULT_BUFLEN , 0) == SOCKET_ERROR)
	{
		
		
		cerr << "Work with server wasn`t start, client down!\n";
		ShutSocket();
	}
	
	cout << recv_buf<<endl;
	
}



// Methods that works with sockets

//--------------------------------------------------------------------------------
int Client::Send()
{
	return send(mainSocket, send_buf.c_str(), DEFAULT_BUFLEN, 0);
}


//--------------------------------------------------------------------------------
 void Client::SendFoundData()
 {
     //sending found file adress to server
     for(int i=0; i<data.size(); i++)
     {
         if (send(mainSocket, data[i].c_str(), DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
         {
             cout << "unable to send found data " <<endl;
			 ShutSocket();

         }
         if( recv(mainSocket, recv_buf, DEFAULT_BUFLEN, 0) == SOCKET_ERROR
                     || strncmp(recv_buf, "Received", 8))  //waiting for server to answer with "Received"
         {
			 ShutSocket(); //if message != "Recived" - break
             break;
        }
     }
 }

//--------------------------------------------------------------------------------
int Client::Recv()
{
	memset(recv_buf, 0, DEFAULT_BUFLEN);
	return recv(mainSocket, recv_buf, DEFAULT_BUFLEN, 0);
}

//--------------------------------------------------------------------------------
Client::Client(): DEFAULT_BUFLEN(512), REQ_WINSOCK_VER(2)
{
#ifdef _WIN32
	mutex = CreateMutex(NULL, FALSE, NULL);
#elif unix
	pthread_mutex_init(&mutex, NULL);
#endif
	process = true; //for processing func
	recv_buf = new char[DEFAULT_BUFLEN];
	memset(recv_buf, 0, DEFAULT_BUFLEN);

	find = new FindFile;
}

//--------------------------------------------------------------------------------
Client::~Client()
{

	shutdown(mainSocket, 2);
	delete find;
	delete[] recv_buf;

#ifdef _WIN32
	WSACleanup();
#endif
}

//--------------------------------------------------------------------------------
void Client::FindFile::StartSearching(Client &SC)
{
	if (SC.mainSocket != INVALID_SOCKET)
	{
		
		SC.process = true;
#ifdef unix
		//start sending "Processing"  every 500ms
		pthread_t thread; //id
		int thread_id = pthread_create(&thread, 0, processing_Linux, &SC);
		if (thread_id != 0)
		{
			cout << "Error, can`t create thread. Error num: " << errno << endl;
		}
		else
		  	cout << "Thread created, start processing"<<endl;

#elif _WIN32
		_beginthread(processing_Win, 0, &SC); //start sending "Processing"  every 500ms

#endif
		if (!search_file(filename_to_find, SC.data))//searching for file (collacting found data)
		{
			cout << "sent: FAILURE" << endl;
			SC.process = false;
			send(SC.mainSocket, "Failure", 512, 0);
           return;
		}

		SC.process = false;	//ending to send "Processing"
		SC.SendFoundData();
	}

}

void Client::FindFile::SetFileName(Client &SC)
{
	if (SC.Recv() == SOCKET_ERROR)  //geting file name to find
	{
		shutdown(SC.mainSocket, 2);
		SC.mainSocket = INVALID_SOCKET;
	}
	filename_to_find = SC.recv_buf;
	cout<<"Searching for: "<< filename_to_find << endl;
}



