#include "Server.h"



#ifdef unix
	
	pthread_mutex_t mutex_L;
#elif _WIN32
	
	HANDLE mutex_W;
#endif
//--------------------------------------------------------------------------------
void Server::InitialiseWinSock()
{
#ifdef _WIN32

	WSADATA wsaData;

	//Initializing winsock
	if (WSAStartup(MAKEWORD(REQ_WINSOCK_VER, 0), &wsaData) == 0)
	{
		// Is max version of WinSock is ok for me
		if (LOBYTE(wsaData.wVersion) >= REQ_WINSOCK_VER)
		{
			cout << "initialized.\n";

		}
		else
		{
			cerr << "required version not supported!\n";
			WSACleanup();
		}

	}
#endif
}


//--------------------------------------------------------------------------------
void Server::CreateBindServer()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET)
	{
		cerr << "unable to create new socket!\n";
	}

	cout << serverSocket << endl;


	sockaddr_in sock_addr = { AF_INET, htons(10001), INADDR_ANY };



	if (bind(serverSocket, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
	{
		cerr << "unable to bind socket!\n";
		shutdown(serverSocket, 2);
	}

}

//--------------------------------------------------------------------------------
//Methods that let the user to interact with client
bool Server::StartClientWork(SOCKET remoteSocket, char* recv_buf)
{
	
	if (recv(remoteSocket, recv_buf, DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
	{
		shutdown(remoteSocket, 2);
		cerr << "Work with server wasn`t start, can`t receive start message! (StartClientWork)\n";
		remoteSocket = INVALID_SOCKET;
	}
	cout << recv_buf<<endl;
	
	if (strncmp(recv_buf, "Start", 5)) //if first message after connection !="Start"
	{
		memset(recv_buf, 0, 512);
		shutdown(remoteSocket, 2);
		if (send(remoteSocket, "Work server isnt started", DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
		{
			cerr << "unable to send message about stoping connection (StartClientWork) " << endl;
			shutdown(remoteSocket, 2);
			remoteSocket = INVALID_SOCKET;

		}

		cout << "Work with client isn`t started "<< endl;
		return false;
	}
	else
	{

		memset(recv_buf, 0, 512);
		if (send(remoteSocket, "Work with server started", DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
		{
			cout << "unable to send message about succsess connection! (StartClientWork) "<< endl;
			shutdown(remoteSocket, 2);
			remoteSocket = INVALID_SOCKET;

		}

		cout << "Work with client started "<< endl;
		return true;
	}

}
//--------------------------------------------------------------------------------

void Server::HandleConnection(const sockaddr_in &sockAddr)
{
	//  cout info about connection 
	ostringstream stream;
	stream << inet_ntoa(sockAddr.sin_addr) << ":" << ntohs(sockAddr.sin_port);

	cout << endl << "Connected with " << stream.str() << ".\n";

}
//-------------------------------------------------------------------------------



void Server::SendNameToFind(SOCKET remoteSocket)
{
	if (send(remoteSocket, filename_to_find.c_str(), DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
	{
		cout << "unable to send name to find !!!"<< " Socket: " << remoteSocket << "\n";
		shutdown(remoteSocket, 2);

	}
	else
		cout << "sended:  " << filename_to_find << endl;
}
//-------------------------------------------------------------------------------



void Server::StartGetingData(SOCKET remoteSocket, char* recv_buf, vector<string> &data)
{	
	while (1)
	{
	
		int recv_bytes = recv(remoteSocket, recv_buf, DEFAULT_BUFLEN, 0);//getting messages from client
		if ( recv_bytes== SOCKET_ERROR || recv_bytes == 0) 
		{
			//if client is off
			cout << "\n Connection is over: (Client is off). Socket: " << remoteSocket << endl<<flush;
			break;
		}
		else //if received data = processing --> put '!', else --> put data
		{

			if (!strncmp(recv_buf, "Processing", 10))
			{
				cout << '!' <<  flush;

				memset(recv_buf, 0, DEFAULT_BUFLEN);
			}
			else
			{

				if (send(remoteSocket, "Received", DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
				{
					cout << "unable to send: \"Received\" message!  Socket: " << remoteSocket << endl<<flush;
					shutdown(remoteSocket, 2);
					break;
				}
				string out(recv_buf);

				data.push_back(out); //add found data path 
				memset(recv_buf, 0, DEFAULT_BUFLEN);
			}
		}
	}
}
//--------------------------------------------------------------------------------
void Server::Listen()
{
	if (listen(serverSocket, SOMAXCONN) != 0)
	{
		cerr << "unable to set listening socket mode!\n";
		shutdown(serverSocket, 2);
	}
}


//--------------------------------------------------------------------------------
void Server::Accept()
{
	//if(!remoteSocket.empty())
	  //remoteSocket.pop_front();

	remoteSocket.push_back( (accept(serverSocket, (sockaddr*)(&remoteAddr), &remoteAddr_len)) );
	if (remoteSocket.back() == INVALID_SOCKET)
	{
		cerr << "unable to set REMOTE socket!\n";
		HandleConnection(remoteAddr);
		shutdown(remoteSocket.back(), 2);
		remoteSocket.pop_front();
	}
	

	HandleConnection(remoteAddr);	//log info about client	
#ifdef unix

		//start working with client in thread
		pthread_t thread; //id
		int thread_id = pthread_create(&thread, 0, WorkWithNewClient, this);
		if (thread_id != 0)
		{
			cout << "Error, can`t create thread. Error num: " << errno << endl;
		}
		else
		  	cout << "New Client! Thread created, start processing"<<endl;

#elif _WIN32

		_beginthread(WorkWithNewClient, 0, this); 	//start working with client in thread

#endif	
	
	
}
	




//Method that works with sockets


//--------------------------------------------------------------------------------

void Server::set_find_name(const string file_name)
{
	filename_to_find = file_name;
}


//--------------------------------------------------------------------------------
SOCKET Server::get_remoteSocket()
{
	return remoteSocket.back();
}

//--------------------------------------------------------------------------------

//Methods that works with sockets
Server::Server(): REQ_WINSOCK_VER(2)
{
	remoteAddr_len = sizeof(remoteAddr);
	recv_buf_len = DEFAULT_BUFLEN;

#ifdef _WIN32
	mutex_W = CreateMutex(NULL, FALSE, NULL);
#elif unix
	pthread_mutex_init(&mutex_L, NULL);
#endif
}
//--------------------------------------------------------------------------------
Server::~Server()
{
	shutdown(serverSocket, 2);
#ifdef _WIN32
	WSACleanup();
#endif
}

//--------------------------------------------------------------------------------

void Server::show_found_data(vector<string> data)
{	
	int num_of_files = data.size();
	if (num_of_files && data[0]!="Failure")
	{
		cout << endl << "All files with: " << filename_to_find << "(There are "<<num_of_files <<" files with this name!)"<< endl << endl <<flush;
		for (int i=0; i<num_of_files; i++)
		{
			cout << data[i] << endl<<flush;
		}
	}
	else
	{
		cout << endl << "No files with: " << filename_to_find << endl;
	}
	cout << endl;
}




//--------------------------------------------------------------------------------

#ifdef unix
void* WorkWithNewClient(void *a)
{
	pthread_mutex_lock(&mutex_L);

	char recv_buf[512];
	vector<string> data;//finded file pathes
	
	Server *Thread_client =   ( (Server*)a ); //my copy constructor

	SOCKET threadSocket = Thread_client->get_remoteSocket(); 

	if (Thread_client->StartClientWork(threadSocket, recv_buf)) // client sending request to 
							//start work corectly
	{
									
	Thread_client->SendNameToFind(threadSocket); //sending file name to find
	
	Thread_client->StartGetingData(threadSocket, recv_buf, data);
	

	Thread_client->show_found_data(data);

	pthread_mutex_unlock(&mutex_L);
	
	close(threadSocket);
	
	return NULL;
	
	}



}

#elif _WIN32

void WorkWithNewClient(void *a)
{	
	WaitForSingleObject(mutex_W, INFINITE);

	char recv_buf[512];
	vector<string> data;//finded file pathes
	
	Server *Thread_client =    (Server*)a ; 

	SOCKET threadSocket = Thread_client->get_remoteSocket();

	if (Thread_client->StartClientWork(threadSocket, recv_buf)) // client sending request to 
							//start work corectly
	{
									
	Thread_client->SendNameToFind(threadSocket); //sending file name to find
	

	Thread_client->StartGetingData(threadSocket, recv_buf, data);
	

	Thread_client->show_found_data(data);

	ReleaseMutex(mutex_W);
	closesocket(threadSocket);
	
	
	}



}

#endif
