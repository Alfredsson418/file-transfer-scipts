#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <windows.h>
#include <lmcons.h>


void CopyFiles(std::string local_path, std::string copy_path) {
    std::ifstream source(local_path, std::ios::binary);
    std::ofstream copy(copy_path, std::ios::binary);

    copy << source.rdbuf();

    source.close();
    copy.close();

}

void TraverseAndCopyFilesFiles(std::string path, std::string copy_path) {

    std::unordered_map<std::string, std::string> sub_files;

    try {
        for (const auto &entry : std::filesystem::directory_iterator(path)) {

            /* Returns to string*/
            std::string relative_path = entry.path().generic_string();

            /* Creates the path to store the file to. NOTE THAT RELATIVE_PAHT CHANGES THEN USING ERASE, ITS NOT TEMORARY*/
            std::string store_path = copy_path + relative_path.erase(0, path.size());



            /* If entry is a direcotry, then add that to vector (list) for later use
                else copy file to store_path */
            
            if (entry.is_directory()) {
                sub_files[entry.path().generic_string()] = store_path;
                if (!std::filesystem::is_directory(store_path)) {
                    std::filesystem::create_directory(store_path);
                }
            } else {
                try {
                    CopyFiles(entry.path().generic_string(), store_path);
                } catch (...) {
                    ;
                }
            }
            
        }

        for (auto sub_path : sub_files) {
            TraverseAndCopyFilesFiles(sub_path.first, sub_path.second);
    }
    } catch(...) {
        ;
    }
    

    
}


int main() {

    /* https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamea */
    /* UNLEN is defined in Lmcons.h */
    /* 
    A buffer size of (UNLEN + 1) characters will hold the maximum 
    length user name including the terminating null character. 
    UNLEN is defined in Lmcons.h. 
    */
    /* Creats a char with the lenghs of max username lenghs +1 for the username to be in */
    char username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    GetUserName(username, &username_len);

    std::string str_username = username;



    
    std::string copy_path = "C:/Users/" + str_username;
    std::string store_path = "C:/" + str_username;
    if (!std::filesystem::is_directory(store_path)) {
        std::filesystem::create_directories(store_path);

    }
    TraverseAndCopyFilesFiles(copy_path, store_path);
    return 1;
    
}
