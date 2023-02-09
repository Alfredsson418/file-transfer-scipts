#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>


void CopyFiles(std::string local_path, std::string copy_path) {
    std::ifstream source(local_path, std::ios::binary);
    std::ofstream copy(copy_path, std::ios::binary);

    copy << source.rdbuf();

    source.close();
    copy.close();

}

void TraverseAndCopyFilesFiles(std::string path, std::string copy_path) {

    std::unordered_map<std::string, std::string> sub_files;

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
            CopyFiles(entry.path().generic_string(), store_path);
        }
        
    }
    
    for (auto sub_path : sub_files) {
        TraverseAndCopyFilesFiles(sub_path.first, sub_path.second);
    }
    
}


int main() {
    
    std::string path = "C:/Users/User/Desktop/Dummy Files";
    std::string copy_path = "C:/Users/User/Desktop/Copy";
    if (!std::filesystem::is_directory(copy_path)) {
        std::filesystem::create_directory(copy_path);
    }
    TraverseAndCopyFilesFiles(path, copy_path);
    return 1;
    
}