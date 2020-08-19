#include "Commands.h"

void Commands::FM(directoryFile* directory, Inode *inodeArray, bool *disk)
{
    delete[] directory;
    delete[] inodeArray;
    delete[] disk;
}

void Commands::NF(std::string fileName, std::string blockCount, directoryFile* directory, Inode *inodeArray, bool *disk) {
    //Local Variables
    Inode newInode;
    int blockNumber = std::stoi(blockCount);
    

    //Set up the new Inode.
    newInode.blockCount = blockNumber;
    newInode.size = 0; //TODO: set this to something.
    //TODO: time still.
    //Get the Inode some disk.
    for (int i = 0; i < blockNumber; i++) {
        for (bool j : disk) { //Inefficient because we restart at 0 every bit we look for, but unimportant.
            if (j == false) {
                j = true;
                newInode.directBlocks[i] = &j;
            }
        }
    }


    //Iterate through Inodearray till be find an open slot.
    int iterator = 0;

    

}

void Commands::MF(std::string fileName, std::string blockCount, directoryFile* directory, Inode *inodeArray, bool *disk) {
}

void Commands::DF(std::string fileName, directoryFile* directory, Inode *inodeArray, bool *disk) {

}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode *inodeArray, bool *disk) {

}

std::string Commands::PR(directoryFile* directory, Inode *inodeArray, bool *disk) {
    return std::string();
}
