#include "Server.h"


int main()
{

		Server SS; //creating, biding 

		SS.InitialiseWinSock();
		SS.CreateBindServer();
		

		string to_find;
		cout << "Please enter file name to find: " << endl;
		cin >> to_find;
         cout << "Waiting for connection... ( ! -- means processing)" << endl;

		SS.set_find_name(to_find.c_str()); //
	
		SS.Listen();//and seting listening mode for socket
			

while(1)
{
		SS.Accept(); // accept(), StartClientWork(), HandleConnection(), SendNameToFind(), StartGetingData()
}
		
	
	return 0;
}


