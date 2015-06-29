/*CSCI 480 - Project 1 - Spring 2015
Name: RJ Catalano
Section: 1
TA: Vijaya Paparaju
Date Due: Friday, Jan 30th
Purpose:  Gather information from files about the hopper system. Answer questions using this information.
Only use one loop to gather the data.*/
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sys/utsname.h>
#include <time.h>
#include <stdlib.h>


using namespace std;

void partAB (string, string);
void partCD (string, string, string, string);

int main()
{
	string cpuInfo = "/proc/cpuinfo", upTime  = "/proc/uptime"; //part A and B files

	string hostname = "/proc/sys/kernel/hostname", ostype = "/proc/sys/kernel/ostype", 
		   osrelease = "/proc/sys/kernel/osrelease", version = "/proc/sys/kernel/version"; //part C 

	partAB(cpuInfo, upTime);
	partCD(ostype, osrelease, hostname, version);


	return 0;
}

/*
Function: PartAB
Use: Processes two files, reads in the information about system info and up time and answers questions
Arguments: the two files needing to be read in string format
Returns: Nothing
*/


void partAB (string cpuInfo, string upTime)
{
	string  fpoint, 
		vendor, 
		model, 
		cache_size, 
		address_size, 
		buffer, 
		sharedCache;	//Declare variables 

	int 	num_proc = 0, 
		phyID = 0, 
		cores = 0, 
		cache_alignment = 0, 
		procID = 0,
		virtual_size = 0, 
		physical_size = 0;

	float 	uptime_sec = 0, 
		cpu_speed = 0;



	ifstream opsysinfo(cpuInfo.c_str());

	while(!opsysinfo.eof())
	{
		getline (opsysinfo, buffer);
		size_t found = buffer.find("processor");

		if (found != string::npos){
			num_proc++;
			int pos = buffer.find_first_of(":");
			string sub = buffer.substr(pos + 1);
			procID = atoi(sub.c_str());
		/*	searches for the string processor    : adds 1 to the counter, 
		 *  and then using the lengthof the string processor    : finds 
		 *  the procID */
		}


		//find physical ID and count up multi-core chips

		if (buffer.find("physical id") != string::npos){
			phyID++; 
		} 

		//find number of cores, if number > 1, then they share a cache

		if (buffer.find("cpu cores") != string::npos){
			int pos = buffer.find_first_of(':');
			string sub = buffer.substr(pos + 1);
			cores += atoi(sub.c_str());

			if (cores > 1)
				sharedCache = "do";
			else
				sharedCache = "do not";

		}


		// find out whether floating point or not
		if (buffer.substr(0,3) == "fpu"){
			int len = buffer.find_first_of(':');
			string truth = buffer.substr(len + 2);
			if (truth == "yes")
				fpoint = "does";
			else
				fpoint = "does not";
			}

		//find size of the cache
		if (procID == 0){
			if (buffer.find("cache size") != string::npos ){
				int pos = buffer.find_first_of(":");
				cache_size = buffer.substr(pos + 1);
			}
		}

		//Find the vendor
		found = buffer.find("vendor_id");
		if (found != string::npos ){
			int pos = buffer.find_first_of(":");
			vendor = buffer.substr(pos + 1);
		}

		//Find the model name
		found = buffer.find("model name");
		if (found != string::npos){
			int pos = buffer.find_first_of(":");
			model = buffer.substr(pos + 1);
		}
		
		//Find the CPU speed
		found = buffer.find("cpu MHz");
		if (found != string::npos){
			int pos = buffer.find_first_of(":");
			string sub = buffer.substr(pos + 1);
			cpu_speed = ::atof(sub.c_str());
		}

		//Find cache alignment
		found = buffer.find("cache_alignment");
		if (found != string::npos){
			int pos = buffer.find_first_of(":");
			string sub = buffer.substr(pos + 1);
			cache_alignment = ::atof(sub.c_str());
		}

		//Find address sizes
		found = buffer.find("address sizes");
		if (found != string::npos){
			int pos = buffer.find_first_of(":");
			int len = buffer.find_first_of(",");
			string physical_address = (buffer.substr(pos + 1, len));
			string virtual_address = (buffer.substr(len + 1));

			pos = physical_address.find_first_of("0123456789");
			len = physical_address.find_last_of ("0123456789");
			string sub = physical_address.substr(pos, len);
			physical_size = atoi(sub.c_str());

			pos = virtual_address.find_first_of("0123456789");
			len = virtual_address.find_last_of ("0123456789");
			sub = virtual_address.substr(pos, len);
			virtual_size = atoi(sub.c_str());
		}
		
		
	}
	opsysinfo.close();
	
	opsysinfo.open(upTime.c_str()); getline (opsysinfo, buffer); opsysinfo.close(); //get one line in uptime

	char * sz;
	const char * sub = buffer.c_str();	//convert from strings to floats and add up
	uptime_sec += strtof(sub, &sz);
	uptime_sec += strtof(sz, NULL);

	time_t seconds(uptime_sec); // you have to convert your input_seconds into time_t
	tm *p = gmtime(&seconds); // convert to broken down time

	cout << "A: Questions about hopper: " << endl;
	cout << "1. Hopper has " << num_proc << " processors." << endl;
	cout << "2. Hopper has " << phyID << " physical multi-core chip(s)." << endl;
	cout << "3. Hopper's CPUs " << sharedCache << " share the L2/L3 cache" << endl;
	cout << "4. Hopper has been up for " << uptime_sec << " seconds" << endl;
	cout << "5. Hopper has been up for " << p->tm_yday << " days, " << p->tm_hour << " hours, " << p->tm_min  << " minutes, and " << p->tm_sec
    	 << " seconds" << endl << endl;
	
	cout << "B: Questions about Processor 0: " << endl;
	cout << "1. Processor 0 " << fpoint << " have a floating point unit" << endl;
	cout << "2. Processor 0's cache is " << cache_size << endl;
	cout << "3. Processor 0's vendor is " << vendor << endl;
	cout << "4. Processor 0's model name is " << model << endl;
	cout << "5. Processor 0's CPU speed is " << cpu_speed << " MHz" << endl;
	cout << "6. Processor 0's cache alignment is " << cache_alignment << endl;
	cout << "7. Processor 0's physical address size is " << physical_size << " bits" << endl;
	cout << "8. Processor 0's virtual address size is " << virtual_size << " bits" << endl << endl;
}

/*Function: partCD
Use:	    Process 4 files and post the output, then use utsname to find most of that information more efficiently
Arguments:  4 filenames in the form of strings
Returns:    nothing
*/

void partCD(string ostype, string osrelease, string hostname, string version)
{
	string A, B, C, D;
	
	ifstream file(ostype.c_str());
	getline (file, A);
	file.close();
	
	file.open(osrelease.c_str());
	getline (file, B);
	file.close();
	
	file.open(hostname.c_str());
	getline(file, C);
	file.close();
	
	file.open(version.c_str());
	getline(file, D);
	file.close();
	cout << "Part C: " << endl;	
	cout << "Ostype information: " << A << endl;
	cout << "Hostname information: " << B << endl;
	cout << "OSrelease information: " << C << endl;
	cout << "Version information: " << D << endl;
	cout << endl;

	struct utsname hopper;
	uname(&hopper);

	cout << "Part D: " << endl;
	cout << "Sysname is:  " << hopper.sysname << endl;
	cout << "Nodename is:  " <<hopper.nodename << endl;
	cout << "Release is:  " <<hopper.release << endl;
	cout << "Version is: " << hopper.version << endl;
	cout << "Machine is: " << hopper.machine << endl;

}

