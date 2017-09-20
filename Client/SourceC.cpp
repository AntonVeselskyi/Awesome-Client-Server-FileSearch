#include "Client.h"
void SaveConnect(Client &SC);

int main()
{
		Client SC;			
		SC.InitialiseWinSock();
		SC.CreateClient();	//Creating and

		SaveConnect(SC);

		SC.StartServerWork(); //sending to server request ("start message") and geting reply 
		SC.find->SetFileName(SC);
		SC.find->StartSearching(SC);
		
		return 0;
}

void SaveConnect(Client & SC)
{
	while (1)
	{
		cout << "Please enter the IP to connect ( ***.***.***.*** ):" << endl;
		string ip;
		cin >> ip;

		int counter=0;
		for (int i = 0; i < ip.size(); i++)
		{
			if (isdigit(ip[i]))
				continue;
			else if (ip[i] == '.')
				counter++;
			else
			{
				counter = -1;
				break;
			}
		}

		if (counter != 3)
		{
			cout << "Wrong format! Try again." << endl;
			continue;
		}
		
		if (SC.Connect(ip)) //connecting mainSocket
			break;
		else
		{
			char answer;
			cout << "Try again? (Y/N)" << endl;
			cin >> answer;
			if (answer != 'Y' && answer != 'y')
			{
				SC.ShutSocket();
				break;
			}
		}
	}

}
