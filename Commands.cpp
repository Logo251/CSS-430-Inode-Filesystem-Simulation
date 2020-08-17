#include "Commands.h"

static void FM(Inode *inodeArray, bool *disk)
{
    for (Inode nodes : inodeArray)
    {
        delete nodes;
        nodes = nullptr;
    }

    for (bool val : disk)
        val = 0;
}

void Commands::NF(std::string fileName, std::string blockCount, Inode *inodeArray, bool *disk) {
    //Create a new inode in the inodeArray
}

void Commands::MF(std::string fileName, std::string blockCount, Inode *inodeArray, bool *disk) {

}

void Commands::DF(std::string fileName, Inode *inodeArray, bool *disk) {

}

void Commands::DB(std::string fileName, std::string numBlocks, Inode *inodeArray, bool *disk) {

}

std::string Commands::PR(Inode *inodeArray, bool *disk) {
    return std::string();
}
