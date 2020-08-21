#include "Commands.h"
#include "Commands.h"

Inode Inode::operator=(const Inode& inode)
{
    atime = inode.atime;
    mtime = inode.mtime;
    size = inode.size;
    blockCount = inode.blockCount;
    for (int i = 0; i < 10; i++) {
        directBlocks[i] = inode.directBlocks[i];
    }
}

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
    if (blockNumber > 10) blockNumber = 10;

    //Set up the new Inode.
    newInode.blockCount = blockNumber;
    newInode.size = 0; //TODO: set this to something.

    int numBlocksOnDisk = 0;
    //Get the Inode some disk.
    for (int i = 0; i < 1000 && numBlocksOnDisk <= std::stoi(blockCount); i++) {
        if (disk[i] == 0)
        {
            disk[i] = 1;
            numBlocksOnDisk++;
        }
    }

    //Iterate through Inodearray till be find an open slot.
    int iterator = 0;

}

void Commands::MF(std::string fileName, std::string blockCount, directoryFile* directory, Inode *inodeArray, bool *disk) {
    //Local Variables
    int inodeNumber;
    
    //Find the Inode
    for (int i = 0; i < 25; i++) {
        if (directory[i].filename == fileName) {
            inodeNumber = directory->inodeNum;
        }
    }

    //Set accessed and modified date to be now.
    inodeArray[inodeNumber].atime = FormattedCurrentTime();
    inodeArray[inodeNumber].mtime = FormattedCurrentTime();
    
    //Add more to inode
    //TODO: reuse code from NF.
}

void Commands::DF(std::string fileName, directoryFile* directory, Inode *inodeArray, bool *disk) {
    //Local Variables
    int inodeNumber;
    int directoryNumber;

    //Find the Inode
    for (int i = 0; i < 25; i++) {
        if (directory[i].filename == fileName) {
            inodeNumber = directory->inodeNum;
            directoryNumber = i;
        }
    }

    //Delete data of the inode.
    for (bool* i : inodeArray[inodeNumber].directBlocks) {
        if (i != nullptr) { //I think this is needed?
            *i = false;
        }
    }

    //Delete everything now.
    inodeArray[inodeNumber] = NULL; //TODO: fix this?
    directory[directoryNumber] = NULL;
}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode *inodeArray, bool *disk) {

}

std::string Commands::PR(directoryFile* directory, Inode *inodeArray, bool *disk) {
    return std::string();
}

std::string Commands::FormattedCurrentTime() {
    std::string returnString;
    struct tm timeInfo;
    time_t t = time(NULL);
    localtime_s(&timeInfo, &t);
    returnString = std::to_string(timeInfo.tm_mday) + "/"
        + std::to_string(timeInfo.tm_mon) + "/"
        + std::to_string((1900 + timeInfo.tm_year)) + " "
        + std::to_string((1 + timeInfo.tm_hour)) + "/" + std::to_string(timeInfo.tm_sec);
    return returnString;
}
