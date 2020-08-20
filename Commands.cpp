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
    if (blockNumber > 10) blockNumber = 10;

    //Set up the new Inode.
    newInode.blockCount = blockNumber;
    newInode.size = 0; //TODO: set this to something.

    struct tm timeInfo;
    time_t t = time(NULL);
    localtime_s(&timeInfo, &t);
    newInode.ctime = std::to_string(timeInfo.tm_mday) + "/"
        + std::to_string(timeInfo.tm_mon) + "/"
        + std::to_string((1900 + timeInfo.tm_year)) + " "
        + std::to_string((1 + timeInfo.tm_hour)) + "/" + std::to_string(timeInfo.tm_sec);

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
}

void Commands::DF(std::string fileName, directoryFile* directory, Inode *inodeArray, bool *disk) {

}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode *inodeArray, bool *disk) {

}

std::string Commands::PR(directoryFile* directory, Inode *inodeArray, bool *disk) {
    return std::string();
}
