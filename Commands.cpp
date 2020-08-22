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
    for (int i = 0; i < 25; i++)
    {
        directory[i].filename = "";
        directory[i].inodeNum = 0;
    }
}

void Commands::NF(std::string fileName, std::string blockCount, directoryFile* directory, Inode* inodeArray, bool* disk) {
    //Local Variables
    Inode newInode;
    int blockNumber = std::stoi(blockCount);
    if (blockNumber > 10) blockNumber = 10;

    //Set up the new Inode.
    newInode.blockCount = blockNumber;
    newInode.size = 524288 * std::stoi(blockCount);
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
          inodeNumber = directory[i].inodeNum;
        }
    }

    int blockNumber = std::stoi(blockCount);
    int blocksLeft = 10 - inodeArray[inodeNumber].blockCount;
    if (blockNumber > blocksLeft) blockNumber = blocksLeft;

    //Set accessed and modified date to be now.
    inodeArray[inodeNumber].atime = FormattedCurrentTime();
    inodeArray[inodeNumber].mtime = FormattedCurrentTime();

    //Add more to inode
    //TODO: reuse code from NF.
    int numBlocksOnDisk = 0;
    //Get the Inode some disk.
    for (int i = 0; i < 1000; i++) {
        if (disk[i] == 0)
        {
            disk[i] = 1;
            inodeArray[inodeNumber].directBlocks[numBlocksOnDisk + inodeArray[inodeNumber].blockCount] = &disk[i];
            numBlocksOnDisk++;
        }
        if (numBlocksOnDisk == blockNumber)
            break;
    }

    inodeArray[inodeNumber].blockCount += blockNumber;
    inodeArray[inodeNumber].size += 512000 * blockNumber;
}

void Commands::DF(std::string fileName, directoryFile* directory, Inode* inodeArray, bool* disk) {
    //Find the Inode
    for (int i = 0; i < 25; i++) {
        if (directory[i].filename == fileName) {
          for(int j = 0; j <= inodeArray[i].blockCount; j++){
            //inode direct block points to index on disk, which is set back to 0
            inodeArray[i].directBlocks[j] = 0;
          }
          //bitmap updated, directory entry updated, but still need to update inode array entry to null
          directory[i].inodeNum = 0;
          directory[i].filename = "";
          return;
        }
    }
}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode* inodeArray, bool* disk) {
  int blockNumber = std::stoi(numBlocks);
  for (int i = 0; i < 25; i++) {
    if (directory[i].filename == fileName) {
      for(int j = blockNumber - 1; j >= inodeArray[i].blockCount - blockNumber; j--){
        //inode direct block points to index on disk, which is set back to 0
        *inodeArray[i].directBlocks[j] = 0;
        //decrement block count and size
        inodeArray[i].blockCount--;
        inodeArray[i].size -= 512000;
      }
      //update other inode fields
      inodeArray[i].atime = FormattedCurrentTime();
      inodeArray[i].mtime = FormattedCurrentTime();
      return;
    }
  }
}

void Commands::PR(directoryFile* directory, Inode *inodeArray, bool *disk) {
    //Local Variables
    std::cout << std::setw(15) << "filename";
    std::cout << std::setw(12) << "mode";
    std::cout << std::setw(10) << "inode #";
    std::cout << std::setw(8) << "uid";
    std::cout << std::setw(8) << "guid";
    std::cout << std::setw(16) << "ctime";
    std::cout << std::setw(16) << "atime";
    std::cout << std::setw(16) << "mtime";
    std::cout << std::setw(10) << "size";
    std::cout << std::setw(13) << "block count" << std::endl;
    for (int i = 0; i < 25; i++) {
        if (!directory[i].filename.empty()) {
            std::cout << std::setw(15) << directory[i].filename;
            std::cout << "  ";
            for (char i : inodeArray[directory[i].inodeNum].mode) {
                std::cout << i;
            }
            std::cout << std::setw(10) << directory[i].inodeNum;
            std::cout << std::setw(8) << inodeArray[directory[i].inodeNum].uid;
            std::cout << std::setw(8) << inodeArray[directory[i].inodeNum].guid;
            std::cout << std::setw(16) << inodeArray[directory[i].inodeNum].ctime;
            std::cout << std::setw(16) << inodeArray[directory[i].inodeNum].atime;
            std::cout << std::setw(16) << inodeArray[directory[i].inodeNum].mtime;
            std::cout << std::setw(10) << inodeArray[directory[i].inodeNum].size ;
            std::cout << std::setw(13) << inodeArray[directory[i].inodeNum].blockCount << std::endl;
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
