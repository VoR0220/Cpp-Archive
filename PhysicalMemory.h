#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class PhysicalMemory
{
    public:
        PhysicalMemory(int memorySize, int algorithmIn); //Initialize all the data and data structure in the class (e.g. set clock to 0). The second argument is for
							//testing different page replacement algorithms. For this assignment, the input will be 0, which is LRU.
        void access(int frameID);   //access the frame, update the time stamp and time list 
        void printMemory() const;  //print out the physical memory layout
        int swapIn(std::string item);  //returns the frame ID of the item just swapped in

    private:
	  std::vector <std::string> memoryList;   //The physical memory!
	  std::vector <int> timeList; //data structure for the replacement algorithm
          int currentTimeIndex;  //current clock for timestamp
	  int algorithmFlag; //indicate if using FIFO or LRU etc.
          int getNextAvailableFrame();  //get a frame, either available or via replacement
          bool isFull();   //check if the memory is full
};


/****************************************************************

   FUNCTION:   PhysicalMemory

   ARGUMENTS:  memory size and an algorithm flag

   RETURNS:    an object constructor for a physical memory table

   NOTES:      constructor
****************************************************************/
PhysicalMemory::PhysicalMemory (int memorySize, int algorithmIn)
{
currentTimeIndex = 0;
timeList.resize(memorySize);		//make timeList full from get go. Makes it easier. Make memoryList empty. Makes more sense.
memoryList.reserve(memorySize);
algorithmFlag = algorithmIn;
}


/****************************************************************

   FUNCTION:   access

   ARGUMENTS:  frameID

   RETURNS:    nothing

   NOTES:      accesses the physical memory and updates the time index and time list accordingly
****************************************************************/
void PhysicalMemory::access(int frameID)
{
std::cout << "Physical: Accessed frameID: " << frameID << " contains " 
	  << memoryList[frameID] << std::endl;	//declare accessed memory

currentTimeIndex++;				//update time stamps accordingly
timeList[frameID] = currentTimeIndex;

}

/****************************************************************

   FUNCTION:   getNextAvailableFrame

   ARGUMENTS:  none

   RETURNS:    the frame for the next chunk of memory to be inserted

   NOTES:      if full, execute minimum element in timeList algorithm, else,
		return size of the memoryList vector
****************************************************************/
int PhysicalMemory::getNextAvailableFrame()
{
 if (isFull() == false)				//if not full, return size of memory list, else return the index of the smallest value in the time list
	return memoryList.size();
 std::vector<int>::iterator it = min_element(timeList.begin(),timeList.end());
 int frameID = std::distance (timeList.begin(), it);

 return frameID;
}

/****************************************************************

   FUNCTION:   isFull

   ARGUMENTS:  none

   RETURNS:    boolean value whether it is full or not

   NOTES: 
****************************************************************/
bool PhysicalMemory::isFull()
{
 if (memoryList.capacity() > memoryList.size()) //check to see if memory list is full.
	return false;

return true;
}

/****************************************************************

   FUNCTION:   swapIn

   ARGUMENTS:  a string item

   RETURNS:    the frameID that the new item is taking

   NOTES:      initializes the changing of the physical memory
****************************************************************/
int PhysicalMemory::swapIn(std::string item)
{
 int frameID = getNextAvailableFrame();		//get the next frame

 	if (isFull() == false)
 		memoryList.push_back(item);	//if it's not full, insert into the list, else swap the value for the new item
 	else
		memoryList.at(frameID) = item;

 currentTimeIndex++;				//update the time stamps accordingly
 timeList[frameID] = currentTimeIndex;

 std::cout << "Physical: Stored: " << item << std::endl; //access the value

return frameID;
}

/****************************************************************

   FUNCTION:   printMemory

   ARGUMENTS:  none

   RETURNS:    none

   NOTES:      printed Physical memory
****************************************************************/
void PhysicalMemory::printMemory() const
{
	for (size_t i = 0; i < memoryList.size(); i++){
		std::cout << "Physical: Memory Spot: " << i << " contains: " << memoryList[i] << std::endl;
	}
}
