#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cstring>

using namespace std;
namespace fs = std::filesystem;

void listFiles(const string& path) {
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            cout << (entry.is_directory() ? "[DIR] " : "[FILE] ") << entry.path().filename().string() << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error listing files: " << e.what() << endl;
    }
}

void createDirectory(const string& path) {
    try {
        if (fs::create_directory(path)) {
            cout << "Directory created: " << path << endl;
        } else {
            cout << "Directory already exists or failed to create." << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error creating directory: " << e.what() << endl;
    }
}

void copyFile(const string& source, const string& destination) {
    try {
        fs::copy(source, destination, fs::copy_options::overwrite_existing);
        cout << "File copied from " << source << " to " << destination << endl;
    } catch (const fs::filesystem_error& e) {
        cerr << "Error copying file: " << e.what() << endl;
    }
}

void moveFile(const string& source, const string& destination) {
    try {
        fs::rename(source, destination);
        cout << "File moved from " << source << " to " << destination << endl;
    } catch (const fs::filesystem_error& e) {
        cerr << "Error moving file: " << e.what() << endl;
    }
}

void navigateDirectory(string& currentPath) {
    string command;
    while (true) {
        cout << "Current directory: " << currentPath << endl;
        cout << "Enter command (ls, mkdir <dir_name>, cp <source> <dest>, mv <source> <dest>, cd <dir>, exit): ";
        getline(cin, command);

        if (command == "exit") {
            break;
        } else if (command == "ls") {
            listFiles(currentPath);
        } else if (command.rfind("mkdir ", 0) == 0) {
            string dirName = command.substr(6);
            createDirectory(currentPath + "/" + dirName);
        } else if (command.rfind("cp ", 0) == 0) {
            size_t spacePos = command.find(" ", 3);
            string source = command.substr(3, spacePos - 3);
            string destination = command.substr(spacePos + 1);
            copyFile(source, destination);
        } else if (command.rfind("mv ", 0) == 0) {
            size_t spacePos = command.find(" ", 3);
            string source = command.substr(3, spacePos - 3);
            string destination = command.substr(spacePos + 1);
            moveFile(source, destination);
        } else if (command.rfind("cd ", 0) == 0) {
            string dirName = command.substr(3);
            if (fs::exists(currentPath + "/" + dirName) && fs::is_directory(currentPath + "/" + dirName)) {
                currentPath += "/" + dirName;
            } else {
                cerr << "Directory not found!" << endl;
            }
        } else {
            cerr << "Unknown command!" << endl;
        }
    }
}

int main() {
    string currentPath = fs::current_path().string();

    cout << "Welcome to the Command-Line File Manager powered by Syed Amir!" << endl;

    navigateDirectory(currentPath);

    return 0;
}