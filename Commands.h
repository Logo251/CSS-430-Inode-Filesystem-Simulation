#ifndef CSS_430_INODE_FILESYSTEM_SIMULATION_COMMANDS_H
#define CSS_430_INODE_FILESYSTEM_SIMULATION_COMMANDS_H

#include <string>
#include <ctime> //Needed for time for inode.

//TODO: here it make it work, is there a better way?
//Inode structure
struct Inode {
    //Setting some to const because assignment says never change them.
    const char mode[10] = {'-', 'r', 'w', 'x', 'r', '-', '-', 'r', '-', '-'}; //Default as given by specifications.
    const std::string uid = "CSS430";                                         //Default as given by specifications.
    const std::string guid = "CSS430";                                        //Default as given by specifications.
    std::string atime;
    std::string ctime;
    std::string mtime; 
    int size = 0;
    int blockCount = 0;
    bool* directBlocks[10];
};

//Directory Structure
struct directoryFile {
    std::string filename;
    int Inode;
};

class Commands {
public:
    static void FM(directoryFile* directory, Inode inodeArray[], bool disk[]);
    static void NF(std::string fileName, std::string blockCount, directoryFile* directory, Inode inodeArray[], bool disk[]);
    static void MF(std::string fileName, std::string blockCount, directoryFile* directory, Inode inodeArray[], bool disk[]);
    static void DF(std::string fileName, directoryFile* directory, Inode inodeArray[], bool disk[]);
    static void DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode inodeArray[], bool disk[]);
    static std::string PR(directoryFile* directory, Inode inodeArray[], bool disk[]);

};


#endif //CSS_430_INODE_FILESYSTEM_SIMULATION_COMMANDS_H
