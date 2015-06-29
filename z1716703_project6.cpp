#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
//the header files of the related classes and data structure
//you can modify based on your filenames
#include "MemoryChunk.h"
#include "PhysicalMemory.h"
#include "PageTable.h"

using namespace std;

vector<string> virtualMemoryList; //layout of the virtual memory
vector<string> runList; //page reference string

//load the virtual memory with each item
void loadVirtualMemory()
{
	virtualMemoryList.push_back("main");
	virtualMemoryList.push_back("load");
	virtualMemoryList.push_back("tax");
	virtualMemoryList.push_back("update");
	virtualMemoryList.push_back("print");
	virtualMemoryList.push_back("holding");
	virtualMemoryList.push_back("activity");
	virtualMemoryList.push_back("net");
}

//load each item to be referenced
void loadRunList()
{
	//load the reference string
	runList.push_back("main");
	runList.push_back("load");
	runList.push_back("holding");
	runList.push_back("main");
	runList.push_back("activity");
	runList.push_back("net");
	runList.push_back("main");
	runList.push_back("update");
	runList.push_back("holding");
	runList.push_back("net");
	runList.push_back("activity");
	runList.push_back("main");
	runList.push_back("tax");
	runList.push_back("net");
	runList.push_back("main");
	runList.push_back("print");
	runList.push_back("net");
}

//this function accepts a string item name and returns the item's
//matching virtual page index
int itemToPageID(string itemIn)
{
	//for each item in the virtual memory list
	for(unsigned int i = 0; i < virtualMemoryList.size(); i++)
	{
		//get the current virutal memory list item

		string cItem = virtualMemoryList.at(i);
		//cout << "String cItem: " << cItem << endl;

		//compare the requested run list item with the current virtual memory list item
		int sComp = cItem.compare(itemIn);
		//cout << "int sComp: " << sComp << endl;

		//if the two items match
		if (sComp == 0)
		{
			//return the current virtual memory list index
			return i;
		}
	}

	//if we get to here, no match was found
	return -1;
}

int main()
{
	cout << "Welcome!" << endl;

	loadVirtualMemory();
	loadRunList();

	//0 for LRU..
	PhysicalMemory physicalMem(4, 0);
	PageTable pTable(&physicalMem, virtualMemoryList.size());
	cout << "LRU with 4 physical frames" << endl;

//	cout << "Run list size: " << runList.size() << endl;
	//for each item in the run list
	for(int i = 0; i < runList.size(); i++)
	{
		//fetch the virtual index for the string item
		int virtualIdx = itemToPageID(runList.at(i));

		//if the virtual frame index was found
		if(virtualIdx != -1)
		{
//			cout << "virtualIdx: " << virtualIdx << endl;
			//cout << "virtualMemoryList.at(virtualIdx): " <<< virtualMemoryList.at(virtualIdx) << endl;
			//reference the item at the specified virtual index
			pTable.reference(virtualIdx, virtualMemoryList.at(virtualIdx));
		}
		//otherwise the item to virtual frame index failed
		else
		{
			//print an error message and break
			cout << "Failed to convert reference to virtual memory: " << runList.at(i) << endl;
			break;
		}
	}

	cout << "Main: now print" << endl << endl;

	//print physical memory and page table
	physicalMem.printMemory();
	pTable.printTable();
//*/
    return 0;
}
