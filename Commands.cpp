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

    return *this;
}

void Commands::FM(directoryFile* directory, Inode *inodeArray, bool *disk)
{
    delete[] directory;
    delete[] inodeArray;
    delete[] disk;

    directoryFile temp;
    temp.filename = "";
    temp.inodeNum = 0;

    directory = new directoryFile[25]{ temp };
    inodeArray = new Inode[25];
    disk = new bool[1000]{ false };
}

void Commands::NF(std::string fileName, std::string blockCount, directoryFile* directory, Inode *inodeArray, bool *disk) {
    //Local Variables
    Inode newInode;
    int blockNumber = std::stoi(blockCount);
    if (blockNumber > 10) blockNumber = 10;

    //Set up the new Inode.
    newInode.blockCount = blockNumber;
    newInode.size = 512000 * std::stoi(blockCount);
    newInode.atime = FormattedCurrentTime();
    newInode.ctime = FormattedCurrentTime();
    newInode.mtime = FormattedCurrentTime();

    int numBlocksOnDisk = 0;
    //Get the Inode some disk.
    for (int i = 0; i < 1000 && numBlocksOnDisk <= std::stoi(blockCount); i++) {
        if (disk[i] == 0)
        {
            newInode.directBlocks[numBlocksOnDisk] = &disk[i];
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
    //Find the Inode
    for (int i = 0; i < 25; i++) {
        if (directory[i].filename == fileName) {
            directory[i].inodeNum = 0;
            directory[i].filename = "";
            return;
        }
    }
}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode *inodeArray, bool *disk) {

}

std::string Commands::PR(directoryFile* directory, Inode *inodeArray, bool *disk) {
    //Local Variables
    std::string returnString;
    returnString += "Filename\tInode #\tMode\tUser ID\tGroup ID\tCreate Time\t\tAccess Time\t\tModify Time\t\tSize\tBlock Count\n\n";
    for (int i = 0; i < 25; i++) {
        if (directory[i].inodeNum != 0) {
            returnString += directory[i].filename + "\t";
            returnString += directory[i].inodeNum + "\t";
            returnString += inodeArray[directory[i].inodeNum].uid + "\t";
            returnString += inodeArray[directory[i].inodeNum].guid + "\t";
            returnString += inodeArray[directory[i].inodeNum].ctime + "\t";
            returnString += inodeArray[directory[i].inodeNum].atime + "\t";
            returnString += inodeArray[directory[i].inodeNum].mtime + "\t";
            returnString += inodeArray[directory[i].inodeNum].size + "\t";
            returnString += inodeArray[directory[i].inodeNum].blockCount + "\t";
            returnString += '\n';
        }
    }
    returnString += "bitmap array:\n";
    for (int i = 0; i < 1000; i++) {
        if (i % 100 == 0) {
            returnString += '\n';
        }
        if (disk[i] == true) {
            returnString += '1';
        }
        else if (disk[i] == false) {
            returnString += '0';
        }
    }
    return returnString;
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
