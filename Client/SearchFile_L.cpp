
#ifdef unix
#include "SearchFile_L.h"




void get_files_on_disk(const string &DATA_DIR, const string &to_find, bool &something_found,vector<string> &data)
{	
	
	string main_dir = DATA_DIR; //current address
	string mesg_for_next_folder; //address of subfolder
	 
	DIR *current= opendir(DATA_DIR.c_str());
	 if(!current) 
		{
		cout<<"Can`t open directory "<< DATA_DIR<<endl;
		return;		
		}
	struct dirent* entry=readdir(current);
	struct stat info;	//info about found file

	if (entry != NULL) //is handle valid
	{
		do { 
			//in every folder we have "." and ".." folders, we shouden`t touch them
			if (!strncmp(entry->d_name, ".", 1) || !strncmp(entry->d_name, "..", 2)	)
				continue;

			//if file with correct filename is FOUND
			if (!strncmp(entry->d_name, to_find.c_str(), to_find.size()))
			{
				something_found = 1;
				string str_to_send = main_dir; //string with address of file

				if(main_dir=="/")
				str_to_send +=entry->d_name;
				else
				{
				str_to_send +='/';
				str_to_send += entry->d_name;
				}

				cout<<"Success "<< entry->d_name<< " - "<< main_dir.c_str()<<endl;

                                 data.push_back(str_to_send); //adding new found file path

			}

			mesg_for_next_folder = main_dir;
			if(main_dir=="/")
			mesg_for_next_folder += entry->d_name;
			else
			{
			mesg_for_next_folder += "/";
			mesg_for_next_folder += entry->d_name;
			}
			
			if(stat(mesg_for_next_folder.c_str(), &info) !=0)
			{
                              cout<<"Can`t use stat() for "<< entry->d_name<<"  entry: "<< entry<<endl;
			}
      			else
			{
			//if it`s a folder I would call search function for this folder
			if (S_ISDIR(info.st_mode))
                              get_files_on_disk(mesg_for_next_folder, to_find, something_found, data);
			}

		} while (entry=readdir(current));
		closedir(current);
	}
	else //if directory not opened
	{
                printf("Can`t open directory\n");
	}
}
//--------------------------------------------------------------------------------

//search_file -- searching for the file on every existing disk
bool search_file(const string &to_find, vector<string> &data)
{

	string DATA_DIR = "/home";
	bool something_found = 0;

        get_files_on_disk(DATA_DIR, to_find, something_found, data);


		if (!something_found)//no file found, sending FAILURE
		{
			return false;
		}
		else
			return true;

}

#endif
