#include <iostream>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <limits>
#include <string.h>

using namespace std;

/*
This function receives the path of the directory as a parameter and lists all the files contained in the directory.
It takes the path as a string by referencing it and specifying it as a constant so it cannot be modified.
*/
void listFilesInDirectory(const std::string &path)
{
    DIR *dir;
    struct dirent *entry;

    // If the path is not null, it reads the entries from the path's content
    if ((dir = opendir(path.c_str())) != NULL)
    {
        std::cout << "Contents of '" << path << "':" << std::endl; // header of the tree
        while ((entry = readdir(dir)) != NULL)
        {                                                    // while there are files
            std::cout << "- " << entry->d_name << std::endl; // writes the name of the file/directory
        }
        closedir(dir); // closes the directory
    }
    else
    { // error handler
        std::cerr << "Error opening the directory: " << strerror(errno) << std::endl;
    }
}

/*
This function creates a file in the specified directory. The function receives two string type parameter references
(one for the directory and one for the content).
*/
void writeFile(const std::string &path)
{
    std::ofstream file(path, std::ios::out); // creates an output stream in write mode to write the content
    if (file.is_open())
    {                        // if the file has been created correctly
        std::string content; // variable for the content
        std::string line;    // variable to control the inputs per line

        std::cout << "Enter the content for the file. Enter '#' on a new line to finish:\n";

        while (true)
        {
            std::getline(std::cin, line); // reads the line
            if (line == "#")
            { // if a # is detected on a new line, it breaks and exits the input
                break;
            }
            content += line + "\n"; // Add line break at the end of each line
        }

        file << content; // writes the content in the file
        file.close();    // closes the file
        std::cout << "File '" << path << "' created successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error creating the file: " << strerror(errno) << std::endl;
    }
}

/*
This function receives as a parameter a constant reference of the path of the file to be deleted.
It checks if the remove operation of the specified path is correct; if it is correct, the file is deleted
and it is notified that it has been successfully deleted; otherwise, it indicates that an error has occurred.
*/
void deleteFile(const std::string &path)
{
    if (remove(path.c_str()) == 0)
    {                                                                            // checks if it has been deleted correctly
        std::cout << "File '" << path << "' successfully deleted." << std::endl; // prints that the file has been deleted
    }
    else
    {
        std::cerr << "Error deleting the file: " << strerror(errno) << std::endl; // prints the error handler
    }
}

/*
This function receives as parameters the constant references of the path of the file to be copied and
the destination path where the file is to be copied. Two streams are created, one for input with ifstream (read mode) and another for
output (write mode) to copy the content from one to the other. It checks if both files have been opened correctly, and if so,
copies the content from one to the other with the function rdbuf(). Once this is done, the files are closed and it prints that the file has been
correctly copied. In the opposite case, it prints that there has been an error.
*/
void copyFile(const std::string &sourcePath, const std::string &destinationPath)
{
    std::ifstream fileSource(sourcePath, std::ios::binary);           // creates an input stream in read mode to copy the content
    std::ofstream fileDestination(destinationPath, std::ios::binary); // creates an output stream in write mode to write the content
    if (fileSource.is_open() && fileDestination.is_open())
    {                                                                                                                // checks if the files have been opened correctly
        fileDestination << fileSource.rdbuf();                                                                       // copies the content of the source file into the destination file
        fileSource.close();                                                                                          // closes the file
        fileDestination.close();                                                                                     // closes the file
        std::cout << "File '" << sourcePath << "' copied to '" << destinationPath << "' successfully." << std::endl; // Prints that the operation has been successfully carried out
    }
    else
    {
        std::cerr << "Error copying the file: " << strerror(errno) << std::endl; // Error handler
    }
}

/*
This function receives as a parameter the path of the file that wants to be modified. It's a constant reference of the same.
First, it creates an input stream in read mode to extract the existing content of the file. Then it checks if
the file has been opened correctly; if so, it reads line by line the content and stores it in the variable
existingContent. Then, it asks for the new content to be added to the file. It will read the inputs until
a # is detected on a new line. Then, the new content is concatenated with the existing one and written to the file.
*/
void writeToFile(const std::string &path)
{
    std::ifstream file(path);
    std::string existingContent;
    std::string line;

    if (file.is_open())
    {
        // Read the existing content of the file
        while (std::getline(file, line))
        {
            existingContent += line + "\n"; // Add a newline after each line
        }
        file.close();

        // Allow the user to input new content
        std::string newContent;
        std::cout << "Enter the new content for the file. Enter '#' on a new line to finish:\n";

        while (true)
        {
            std::getline(std::cin, line);
            if (line == "#")
            {
                break;
            }
            newContent += line + "\n"; // Add a newline after each line
        }

        // Concatenate the new content to the existing content
        existingContent += newContent;

        // Write the modified content back to the file
        std::ofstream outFile(path, std::ios::trunc);
        if (outFile.is_open())
        {
            outFile << existingContent;
            outFile.close();
            std::cout << "File '" << path << "' successfully modified." << std::endl;
        }
        else
        {
            std::cerr << "Error modifying the file: " << strerror(errno) << std::endl;
        }
    }
    else
    {
        std::cerr << "Error opening the file for modification: " << strerror(errno) << std::endl;
    }
}

int main()
{

    std::string path;            // path of the directory
    std::string content;         // file content in case of wanting to create one and insert something
    int option;                  // menu option
    bool repeat = true;          // control variable for the menu
    std::string destinationPath; // variable for the destination path when copying a file
    std::string newContent;      // variable for the new content in case of wanting to write in a file

    while (repeat)
    {

        /* Print the menu options */
        std::cout << "------------------------------------------------------------------------------------------------------------ \n";
        std::string asciiArt =

            "                       _____ .__ .__                                                                         \n"
            " _____   __  _  __   _/ ____\\|__||  |    ____       _____  _____     ____  _____      ____    ____  _______  \n"
            " \\__  \\  \\ \\/ \\/ /   \\   __\\ |  ||  |  _/ __ \\     /     \\ \\__  \\   /    \\ \\__  \\    / ___\\ _/ __ \\ \\_  __ \\ \n"
            "  / __ \\_ \\     /     |  |   |  ||  |__\\  ___/    |  Y Y  \\ / __ \\_|   |  \\ / __ \\_ / /_/  >\\  ___/  |  | \\/ \n"
            " (____  /  \\/\\_/      |__|   |__||____/ \\___  >   |__|_|  /(____  /|___|  /(____  / \\___  /  \\___  > |__|    \n"
            "      \\/                                    \\/          \\/      \\/      \\/      \\/ /_____/       \\/          \n\n";
        std::cout << asciiArt;
        std::cout << "Terminal Based File Manager                                                                  By AbyssWatcher\n";
        std::cout << "------------------------------------------------------------------------------------------------------------\n";
        std::cout << "\nFILE EXPLORER MENU:" << std::endl;
        std::cout << "------------------------------------\n";
        std::cout << "\n1. List files in a directory" << std::endl;
        std::cout << "2. Write a file" << std::endl;
        std::cout << "3. Delete a file" << std::endl;
        std::cout << "4. Copy a file" << std::endl;
        std::cout << "5. Add content to a file" << std::endl;
        std::cout << "6. Exit\n" << std::endl;
        std::cout << "-----------------------------------\n";
        std::cout << "Enter your option: ";
        std::cin >> option;                                                 // read the user's option
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear the input buffer after reading a numerical value with std::cin, ignore the content after the newline \n

        switch (option)
        {

        case 1:
            std::cout << "Enter the path of the directory: "; // request the desired directory path
            std::getline(std::cin, path);                     // store it in the variable path using cin
            listFilesInDirectory(path);                       // call the function and pass the path as a parameter
            break;

        case 2:
            std::cout << "Enter the path for the file: ";
            std::getline(std::cin, path); // IMPORTANT better to use getline, cin only reads up to the first space or newline
            // std::cout << "Enter the file content: ";
            // std::getline(std::cin, content); // store the file content
            writeFile(path); // call the function passing the path where to create the file and the content
            break;

        case 3:
            std::cout << "Enter the path of the file you wish to delete: ";
            std::getline(std::cin, path); // path of the file to be deleted
            deleteFile(path);             // function call
            break;

        case 4:
            std::cout << "Enter the path of the file you wish to copy: ";
            std::getline(std::cin, path); // path of the file to be copied
            std::cout << "Enter the destination path for the desired file: ";
            std::getline(std::cin, destinationPath); // destination path for the file
            copyFile(path, destinationPath);         // function call
            break;

        case 5:
            std::cout << "Enter the path of the file where you want to write: "; // path of the file to write in
            std::getline(std::cin, path);
            writeToFile(path); // function call
            break;

        case 6:
            std::cout << "Thank you for using AWFileManager! See you later!" << std::endl; // goodbye message
            repeat = false;                                                                // break the while loop to end execution
            return 0;

        default:
            break;
        }
    }
}
