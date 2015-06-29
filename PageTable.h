/****************************************************************
   PROGRAM:   Assignment 6
   AUTHOR:    RJ Catalano
   LOGON ID:  Z1716703  (your Z number here)
   DUE DATE:  Way after today

   FUNCTION:  A class defining a page table.

   INPUT:     Pointer to physical memory and an integer denoting page table
	      size

   OUTPUT:    numerous methods for interacting between virtual memory and 
	      physical memory.

   NOTES:     any relevant information that would be of
              additional help to someone looking at the program.
****************************************************************/
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class PageTable
{
    public:
        PageTable(PhysicalMemory* pmIn, int tableSize); //initialize all entries to invalid
        void reference(int pageID, std::string item); //reference a logical page,  if not in memory,  call pageFault() 
        int getFaults() const;  //return number of faults
        void printTable() const;  //print the layout of page table
        void reset();//reset the validity flag to false, and  numberoffaults  to 0.
 
    private:
	  std::vector<PageTableEntry> entryList;  //the page table!
	  int numFaults;
	  PhysicalMemory* mainPhysicalMemory;
          void pageFault(int pageID, std::string item);  // increment numFaults;  need to swap in the item into physical memory by calling the swapIn() of the 
							//PhysicalMemory class 
	  void updateReplacedEntry(int replacedIndex, int pageID); // after page replacement, update the page table correspondingly by make the original mapping for 
								  //the frame invalid
};


/****************************************************************

   FUNCTION:   Page Table constructor

   ARGUMENTS:  pointer to physical memory, table size

   RETURNS:    an object signifying a page table

   NOTES:      initialize all values and data structures
****************************************************************/

PageTable::PageTable(PhysicalMemory* pmIn, int tableSize)
{
entryList.resize(tableSize);					//allocate page table size

	for (size_t i = 0; i < entryList.size(); i++){ 		//initialize everything to false
		entryList[i].valid = false;
		entryList[i].physicalMemoryIndex = -1;
	}

numFaults = 0;							//set physical memory and initialize faults to 0
mainPhysicalMemory = pmIn;
}

/****************************************************************

   FUNCTION:   reference

   ARGUMENTS:  a page ID, and a string item representing a command

   RETURNS:    nothing

   NOTES:      Checks to see if currently in the page table, else initializes page fault
****************************************************************/
void PageTable::reference (int pageID, std::string item)
{
		if (entryList[pageID].valid == true)		//if page is in the physical memory, access, else there is a page fault
			mainPhysicalMemory->access(entryList[pageID].physicalMemoryIndex);
		else
			pageFault(pageID, item);
}

/****************************************************************

   FUNCTION:   getFaults()

   ARGUMENTS:  none

   RETURNS:    number of faults in the page table

   NOTES:
****************************************************************/
int PageTable::getFaults() const
{
return numFaults;
}


/****************************************************************

   FUNCTION:   pageFault

   ARGUMENTS:  takes a pageID and a string item

   RETURNS:    nothing

   NOTES:      increments page faults, swaps in virtual memory into physical memory
		and then cleans the current page table for validity among the entries
		and sets the most recently used memory to valid
****************************************************************/
void PageTable::pageFault(int pageID, std::string item)
{
numFaults++; 								//increment and record that the page fault occurred
std::cout << "PageTable: page fault occured" << std::endl;
int frameID = mainPhysicalMemory->swapIn(item); 			//access physical memory to swap in the new item

	for (size_t i = 0; i < entryList.size(); i++){
		if (entryList[i].physicalMemoryIndex == frameID){	//delete traces of old page entry
			entryList[i].physicalMemoryIndex = -1;
			entryList[i].valid = false;
			break;
		}
	}

entryList[pageID].physicalMemoryIndex = frameID;			//set the index of the new page entry and set it to valid
entryList[pageID].valid = true;
}

/****************************************************************

   FUNCTION:   printTable()

   ARGUMENTS:  none

   RETURNS:    none

   NOTES:      print the table
****************************************************************/
void PageTable::printTable() const
{
	for (std::size_t i = 0; i < entryList.size(); i++)
		std::cout << "PageTable: Index: " << i << " Physical Index: " << entryList[i].physicalMemoryIndex
			  << " In Use: " << std::boolalpha << entryList[i].valid << std::endl;

	std::cout << "PageTable: Current number of page faults: " << getFaults() << std::endl;

}
/****************************************************************

   FUNCTION:   reset

   ARGUMENTS:  none

   RETURNS:    none

   NOTES:      essentially a destructor
****************************************************************/
void PageTable::reset()
{
 numFaults = 0;							//optional destructor

	for (size_t i = 0; i < entryList.size(); i++){
                entryList[i].valid = false;
                entryList[i].physicalMemoryIndex = -1;
        }

}


