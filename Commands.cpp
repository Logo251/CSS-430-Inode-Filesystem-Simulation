#include "Commands.h"

static void Commands::FM(directoryFile directory, Inode *inodeArray, bool *disk)
{
    for (Inode nodes : inodeArray)
    {
        delete nodes;
        nodes = nullptr;
    }

    for (bool val : disk)
        val = 0;
}

void Commands::NF(std::string fileName, std::string blockCount, directoryFile directory, Inode *inodeArray, bool *disk) {
    //Create a new file
    while()
}

void Commands::MF(std::string fileName, std::string blockCount, directoryFile directory, Inode *inodeArray, bool *disk) {
}

void Commands::DF(std::string fileName, directoryFile directory, Inode *inodeArray, bool *disk) {

}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile directory, Inode *inodeArray, bool *disk) {

}

std::string Commands::PR(directoryFile directory, Inode *inodeArray, bool *disk) {
    return std::string();
}
