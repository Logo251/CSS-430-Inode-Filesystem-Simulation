#ifndef CSS_430_INODE_FILESYSTEM_SIMULATION_COMMANDS_H
#define CSS_430_INODE_FILESYSTEM_SIMULATION_COMMANDS_H

#include <string>

//TODO: here it make it work, is there a better way?
//Inode structure
struct Inode {
    char mode[10] = {'-', 'r', 'w', 'x', 'r', '-', '-', 'r', '-', '-'}; //Default as given by specifications.
    std::string uid = "CSS430";                                         //Default as given by specifications.
    std::string guid = "CSS430";                                        //Default as given by specifications.
    std::string atime; //TODO: set time to time created.
    std::string ctime; //TODO: set time to time created.
    std::string mtime; //TODO: set time to time created.
    int size = 0;
    int blockCount = 0;
    bool* directBlocks[10];
};

class Commands {
public:
    static void FM(Inode inodeArray[], bool disk[]);
    static void NF(std::string fileName, std::string blockCount, Inode inodeArray[], bool disk[]);
    static void MF(std::string fileName, std::string blockCount, Inode inodeArray[], bool disk[]);
    static void DF(std::string fileName, Inode inodeArray[], bool disk[]);
    static void DB(std::string fileName, std::string numBlocks, Inode inodeArray[], bool disk[]);
    static std::string PR(Inode inodeArray[], bool disk[]);

};


#endif //CSS_430_INODE_FILESYSTEM_SIMULATION_COMMANDS_H
