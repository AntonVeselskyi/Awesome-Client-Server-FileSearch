#ifdef _WIN32
#include "SearchFile.h"


void get_files_on_disk(const string &DATA_DIR, const string &to_find, bool &something_found, vector<string> &vec_data)
{
        HANDLE hFind;
        WIN32_FIND_DATAA data;		//info about found file
        string main_dir = DATA_DIR;
        string mesg_for_next_folder; //adress of subfolder

        hFind = FindFirstFileA(LPCSTR(DATA_DIR.c_str()), &data);

        if (hFind != INVALID_HANDLE_VALUE) //is handle valid
        {
                do {

                        //in every folder we have "." and ".." folders, we shoulden`t touch them
                        if (!strncmp((char*)data.cFileName, ".", 1) || !strncmp((char*)data.cFileName, "..", 2))
                                continue;

                        //if file with correct filename is FOUND
                        if (!strncmp((char*)data.cFileName, to_find.c_str(), to_find.size()))
                        {
                                something_found = 1;
                                string str_to_send = main_dir; //string with adress of file
                                for (int i = 3; i; --i)
                                        str_to_send.pop_back();
                                str_to_send += data.cFileName;
                                printf("Success %s - %s\n", data.cFileName, main_dir.c_str());

                                vec_data.push_back(str_to_send); //adding new found file path
                        }



                        //if it`s a folder I would call search function for this folder
                        if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
                        {


                                mesg_for_next_folder = main_dir;
                                for (int i = 3; i; --i)
                                        mesg_for_next_folder.pop_back();
                                mesg_for_next_folder += data.cFileName;
                                mesg_for_next_folder += "\\*.*";

                                get_files_on_disk(mesg_for_next_folder, to_find,something_found, vec_data);
                        }

                } while (FindNextFileA(hFind, &data));
                FindClose(hFind);
        }
}
//--------------------------------------------------------------------------------

//search_file -- searching for the file on every existing disk
bool search_file(const string &to_find, vector<string> &data)
{
        //insted of using GetGlobalDevices() I check every disk by my self

        string DATA_DIR = "c:\\*.*";
        bool something_found = 0;
        for (int i = 0; i < 26; i++)   //cheking all possible disks (a-z)
		{
			cout << "Searching in " << char(i + 'a') << endl;
                DATA_DIR[0] = ('a' + i);
               
                //if disk doesn`t exist FindFileNext() will ignore it
                get_files_on_disk(DATA_DIR, to_find, something_found, data);
                
        }

		if (!something_found)//no file found, sending FAILURE
		{
			return false;
		}
		else
			return true;

}

#endif
