/**
 * Givens:
 * A disk contains 1000 512K blocks (0-999)
 * Permissions and file type(first byte) should be -rwxr--r--
 * Use only direct blocks (no indirect blocks)
 * uid = CSS430 gid = CSS430
 *
 * Commands used are:
 * Create new file: NF filename numblocks
 *      Creates file of 4 blocks.
 * Add to file: MF filename numblocks
 *      Add 3 blocks to file.
 * Delete file: DF filename
 *      Delete file.
 * Delete blocks: DB filename numblocks
 *      Delete numblocks from file.
 * Print: PR
 *      Print all file system contents, including inode,
 *      disk map, and disk directory.
 */

#include <string> //Needed for string.
#include <iostream> //Needed for cin/cout/getline.
#include <vector> //Needed for vector for function return.
#include <fstream> //Needed to read from file.
#include "Commands.h" //Commands the OS uses.

const std::string FILENAME = "command.txt";

//Function Prototypes
std::vector<std::string> parseInput(std::string filename, int lineCount);           //Parses commands given to the program.
																	//Returns vector because its not easy
																	//to return arrays.
int main() {
	std::cout << "Written by:\n";
	std::cout << "Logan Petersen\n";
	std::cout << "Jayden Fullerton\n";
	std::cout << "Makoto Booth\n";

	//Local Variables
	std::string consoleInput;
	std::vector<std::string> givenCommand;
	directoryFile empty;
	empty.filename = "";
	empty.inodeNum = 0;
	Inode* inodeArray = new Inode[25];
	directoryFile* directory = new directoryFile[25]{ empty };
	bool* disk = new bool[1000]{ false };
	int lineCount = 1;

	while (true) {
		givenCommand = parseInput(FILENAME, lineCount);
		lineCount++;

		//Switch statement doesn't work effectively on strings unless you convert each string
		//to int, so therefore if is being used. :(
		if (givenCommand.size() == 0)
			break;
		if (givenCommand[0] == "FM") {
			Commands::FM(directory, inodeArray, disk);
		}
		else if (givenCommand[0] == "NF") {
			Commands::NF(givenCommand[1], givenCommand[2], directory, inodeArray, disk);
		}
		else if (givenCommand[0] == "MF") {
			Commands::MF(givenCommand[1], givenCommand[2], directory, inodeArray, disk);
		}
		else if (givenCommand[0] == "DF") {
			Commands::DF(givenCommand[1], directory, inodeArray, disk);
		}
		else if (givenCommand[0] == "DB") {
			Commands::DB(givenCommand[1], givenCommand[2], directory, inodeArray, disk);
		}
		else if (givenCommand[0] == "PR") {
			Commands::PR(directory, inodeArray, disk);
		}
		else {
			std::cout << "Invalid Command.\n";
		}
	}

	delete[] directory;
	delete[] inodeArray;
	delete[] disk;
	return 0;
}

std::vector<std::string> parseInput(std::string filename, int lineCount) {  //Parses commands given to the program.
																			//Returns vector because its not easy
																			//to return arrays.
	std::ifstream file(filename);

	//Local Variables
	std::vector<std::string> returnVector;
	std::string line;
	std::string temporaryStorage;

	for (int i = 0; i < lineCount; i++)
		if (!std::getline(file, line)) return std::vector<std::string>(0);

	for (char i : line) {
		if (i != ' ') {
			temporaryStorage.push_back(i);
		}
		else {
			returnVector.push_back(temporaryStorage);
			temporaryStorage.clear();
		}
	}

	returnVector.push_back(temporaryStorage);

	return returnVector;
}