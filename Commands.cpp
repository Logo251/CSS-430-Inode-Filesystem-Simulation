#include "Commands.h"
#include "Commands.h"

Inode Inode::operator=(const Inode& inode)
{
    atime = inode.atime;
    mtime = inode.mtime;
    ctime = inode.ctime;
    size = inode.size;
    blockCount = inode.blockCount;
    for (int i = 0; i < 10; i++) {
        directBlocks[i] = inode.directBlocks[i];
    }

    return *this;
}

void Commands::FM(directoryFile* directory, Inode* inodeArray, bool* disk)
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

void Commands::NF(std::string fileName, std::string blockCount, directoryFile* directory, Inode* inodeArray, bool* disk) {
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
    for (int i = 0; i < 1000; i++) {
        if (disk[i] == 0)
        {
            disk[i] = 1;
            newInode.directBlocks[numBlocksOnDisk] = &disk[i];
            numBlocksOnDisk++;
        }
        if (numBlocksOnDisk == blockNumber)
            break;
    }

    for (int i = 0; i < 25; i++)
    {
        if (directory[i].filename == "" && directory[i].inodeNum == 0) // the spot is available
        {
            inodeArray[i] = newInode;
            directory[i].filename = fileName;
            directory[i].inodeNum = i + 1;
            break;
        }
    }

}

void Commands::MF(std::string fileName, std::string blockCount, directoryFile* directory, Inode* inodeArray, bool* disk) {
    //Local Variables
    int inodeNumber;

    //Find the Inode
    for (int i = 0; i < 25; i++) {
        if (directory[i].filename == fileName) {
            inodeNumber = directory->inodeNum;
        }
    }

    int blockNumber = std::stoi(blockCount);
    int blocksLeft = 10 - inodeArray[inodeNumber].blockCount;
    if (blockNumber > blocksLeft) blockNumber = blocksLeft;

    //Set accessed and modified date to be now.
    inodeArray[inodeNumber].atime = FormattedCurrentTime();
    inodeArray[inodeNumber].mtime = FormattedCurrentTime();
    //increment block number and size
    inodeArray[inodeNumber].blockCount += blockNumber;
    inodeArray[inodeNumber].size += 512000 * blockNumber;

    //Add more to inode
    //TODO: reuse code from NF.
    int numBlocksOnDisk = 0;
    //Get the Inode some disk.
    for (int i = 0; i < 1000; i++) {
        if (disk[i] == 0)
        {
            disk[i] = 1;
            inodeArray[inodeNumber].directBlocks[numBlocksOnDisk] = &disk[i];
            numBlocksOnDisk++;
        }
        if (numBlocksOnDisk == blockNumber)
            break;
    }
}

void Commands::DF(std::string fileName, directoryFile* directory, Inode* inodeArray, bool* disk) {
    //Find the Inode
    for (int i = 0; i < 25; i++) {
        if (directory[i].filename == fileName) {
            directory[i].inodeNum = 0;
            directory[i].filename = "";
            for(int j = 0; j <= inodeArray[i].blockCount; j++){
            //inode direct block points to index on disk, which is set back to 0
            *inodeArray[i].directBlocks[j] = 0;
          }
          //bitmap updated, directory entry updated, but still need to update inode array entry to null
            return;
        }
    }
}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode* inodeArray, bool* disk) {

}

void Commands::PR(directoryFile* directory, Inode *inodeArray, bool *disk) {
    //Local Variables
    //std::cout << "Filename\tInode #\tMode\tUser ID\tGroup ID\tCreate Time\t\tAccess Time\t\tModify Time\t\tSize\tBlock Count\n\n";
    for (int i = 0; i < 25; i++) {
        if (!directory[i].filename.empty()) {
            std::cout << directory[i].filename + '\t';
            for (char i : inodeArray[directory[i].inodeNum].mode) {
                std::cout << i;
            }
            std::cout << std::endl;
            std::cout << directory[i].inodeNum << std::endl;
            std::cout << inodeArray[directory[i].inodeNum].uid << std::endl;
            std::cout << inodeArray[directory[i].inodeNum].guid << std::endl;
            std::cout << inodeArray[directory[i].inodeNum].ctime << std::endl;
            std::cout << inodeArray[directory[i].inodeNum].atime << std::endl;
            std::cout << inodeArray[directory[i].inodeNum].mtime << std::endl;
            std::cout << inodeArray[directory[i].inodeNum].size << std::endl;
            std::cout << inodeArray[directory[i].inodeNum].blockCount << std::endl;
        }
    }
    std::cout << "bitmap array:\n";
    for (int i = 0; i < 1000; i++) {
        if (i % 100 == 0) {
            std::cout << '\n';
        }
        if (disk[i] == true) {
            std::cout << '1';
        }
        else if (disk[i] == false) {
            std::cout << '0';
        }
    }
    return;
}

std::string Commands::FormattedCurrentTime() {
    std::string returnString;
    struct tm timeInfo;
    time_t t = time(NULL);
    localtime_s(&timeInfo, &t);
    returnString = std::to_string(timeInfo.tm_mday) + "/"
        + std::to_string(1 + timeInfo.tm_mon) + "/"
        + std::to_string((1900 + timeInfo.tm_year)) + " "
        + std::to_string((1 + timeInfo.tm_hour)) + ":" + std::to_string(timeInfo.tm_sec);
    return returnString;
}
