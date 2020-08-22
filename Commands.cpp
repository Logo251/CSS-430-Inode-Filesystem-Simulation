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
	for (int i = 0; i < 1000; i++)
	{
		disk[i] = 0;
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
	int index;

	//Find the Inode
	for (int i = 0; i < 25; i++) {
		if (directory[i].filename == fileName) {
			index = i;
		}
	}

	if (inodeArray[index].blockCount + std::stoi(blockCount) > 10)
		std::cout << "You can't add that many blocks to " << fileName << std::endl;

	//Set accessed and modified date to be now.
	inodeArray[index].atime = FormattedCurrentTime();
	inodeArray[index].mtime = FormattedCurrentTime();

	//Add more to inode
	int numBlocksOnDisk = 0;
	//Get the Inode some disk.
	for (int i = 0; i < 1000; i++) {
		if (disk[i] == 0)
		{
			disk[i] = 1;
			inodeArray[index].directBlocks[numBlocksOnDisk + inodeArray[index].blockCount] = &disk[i];
			numBlocksOnDisk++;
		}
		if (numBlocksOnDisk == std::stoi(blockCount))
			break;
	}

	inodeArray[index].blockCount += std::stoi(blockCount);
	inodeArray[index].size = inodeArray[index].blockCount * 524288;
}

void Commands::DF(std::string fileName, directoryFile* directory, Inode* inodeArray, bool* disk) {
	//Find the Inode
	for (int i = 0; i < 25; i++) {
		if (directory[i].filename == fileName) {
			for (int j = 0; j < inodeArray[i].blockCount; j++) {
				//inode direct block points to index on disk, which is set back to 0
				*inodeArray[i].directBlocks[j] = 0;
			}
			//update directory to indicate open spot
			directory[i].inodeNum = 0;
			directory[i].filename = "";
			return;
		}
	}
}

void Commands::DB(std::string fileName, std::string numBlocks, directoryFile* directory, Inode* inodeArray, bool* disk) {
	//Local Variables
	int index;

	//Find the Inode
	for (int i = 0; i < 25; i++) {
		if (directory[i].filename == fileName) {
			index = i;
		}
	}

	if (inodeArray[index].blockCount - std::stoi(numBlocks) < 1)
		std::cout << "You can't delete that many blocks from " << fileName << std::endl;

	//Set accessed and modified date to be now.
	inodeArray[index].atime = FormattedCurrentTime();
	inodeArray[index].mtime = FormattedCurrentTime();

	for (int i = inodeArray[index].blockCount - 1; i >= inodeArray[index].blockCount - std::stoi(numBlocks); i--) {
		*inodeArray[index].directBlocks[i] = 0;
		inodeArray[index].directBlocks[i] = nullptr;
	}

	inodeArray[index].blockCount -= std::stoi(numBlocks);
	inodeArray[index].size = inodeArray[index].blockCount * 524288;
}

void Commands::PR(directoryFile* directory, Inode* inodeArray, bool* disk) {
	//Local Variables
	std::cout << std::endl << "filename";
	std::cout << std::setw(9) << "mode";
	std::cout << std::setw(15) << "inode #";
	std::cout << std::setw(5) << "uid";
	std::cout << std::setw(9) << "guid";
	std::cout << std::setw(9) << "ctime";
	std::cout << std::setw(21) << "atime";
	std::cout << std::setw(21) << "mtime";
	std::cout << std::setw(20) << "size";
	std::cout << std::setw(16) << "block count" << std::endl;
	for (int i = 0; i < 25; i++) {
		if (!directory[i].filename.empty()) {
			std::cout << directory[i].filename;
			std::cout << "  ";
			for (char i : inodeArray[i].mode) {
				std::cout << i;
			}
			std::cout << std::setw(9) << directory[i].inodeNum;
			std::cout << std::setw(8) << inodeArray[i].uid;
			std::cout << std::setw(8) << inodeArray[i].guid;
			std::cout << std::setw(21) << inodeArray[i].ctime;
			std::cout << std::setw(21) << inodeArray[i].atime;
			std::cout << std::setw(21) << inodeArray[i].mtime;
			std::cout << std::setw(9) << inodeArray[i].size;
			std::cout << std::setw(13) << inodeArray[i].blockCount << std::endl;
		}
	}
	std::cout << "\nbitmap array:";
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
	std::cout << std::endl << "-------------------------------------------------------------------------------------------------------------------------------------";
	std::cout << std::endl;
	return;
}

std::string Commands::FormattedCurrentTime() {
	std::string returnString;
	struct tm timeInfo;
	time_t t = time(NULL);

#ifdef _WIN32
	localtime_s(&timeInfo, &t);
#else
	localtime_r(&t, &timeInfo);
#endif

	std::string day = (timeInfo.tm_mday < 10) ? "0" + std::to_string(timeInfo.tm_mday) : std::to_string(timeInfo.tm_mday);
	std::string mon = (timeInfo.tm_mon + 1 < 10) ? "0" + std::to_string(timeInfo.tm_mon) : std::to_string(timeInfo.tm_mon);
	std::string yr = std::to_string(timeInfo.tm_year + 1900);
	std::string hr = (timeInfo.tm_hour < 10) ? "0" + std::to_string(timeInfo.tm_hour) : std::to_string(timeInfo.tm_hour);
	std::string min = (timeInfo.tm_min < 10) ? "0" + std::to_string(timeInfo.tm_min) : std::to_string(timeInfo.tm_min);
	std::string sec = (timeInfo.tm_sec < 10) ? "0" + std::to_string(timeInfo.tm_sec) : std::to_string(timeInfo.tm_sec);

	returnString = day + "/" + mon + "/" + yr + " " + hr + ":" + min + ":" + sec;
	return returnString;
}