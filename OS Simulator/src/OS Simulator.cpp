//============================================================================
// Name        : OS.cpp
// Author      : Hana Parece
// Created On  : Oct 15 2021
// Description : OS simulator for CMSC 312
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <iomanip>
#include <thread>

#include "Process.h"
#include "Scheduler1.h" // Priority Scheduler
#include "Scheduler2.h" //
using namespace std;
using namespace std::chrono;


vector<PCB> cycle(vector<PCB> pcb, int *memoryInUse, clock_t *time, int schedulerVersion, int *resource); //to simulate 1 cycle
int randomIO(); //to create a random I/O Event (1/100 chance per cycle)
void cascadingTermination(vector<PCB> *pcb); //to close all child process if the parent is no longer running
void messageComms1(vector<PCB> *pcb); //first interprocess communcation method
bool BankersAlgo(vector<PCB> pcb); //Deadlock Avoidance


/***************************/

//main method
int main() {

	clock_t time = clock();
	vector<Process> jobQueue, temp;
	vector<vector<Process>> getTest;
	vector<PCB> control;
	int input, saveMin, saveMax, input2;
	unsigned int i;
	int savedCount = 0;
	int j;
	int checker = 1;
	static int count, counter2;
	string fileName = "template1"; //default to template1 just in case
	string stringIn, saveType, line;
	ifstream in;
	const unsigned int TOTAL_MEMORY = 1024;
	int memoryInUse = 0;
	int resources = 50; //each process may want to request a resource, we have 50 available

	cout << "Welcome to OS Simulator\n_______________________\n\nPress [1] to continue. Press [0] to exit." << endl;
	cin >> input;
	cout << "Which scheduler would you like to use for this run? \n[1] for Priority [2] for Shortest Time Remaining" << endl;
	cin >> input2;
	while(input == 1){
		cout << "Memory in Use: " << memoryInUse << endl;
		cout << "Memory Available: " << TOTAL_MEMORY - memoryInUse << endl;
		cout << "_______________________\n\nPress [2] to load a job\nPress [3] to display statistics\n"
				<< "Press [4] for help\nPress [5] to simulate a cycles and display no output\nPress [0] to exit." << endl << endl;

		cin >> input;

		//input == 2, so we would like to load a job/process
		if(input == 2){
			cout << "Enter template name:" << endl;
			cin >> fileName;
			fileName = "templates/" + fileName + ".txt";
			in.open(fileName);
			if (!in) {
				while(!in){
					cerr << "Unable to open file. Please check spelling." << endl;
					cout << "Enter template name:" << endl;
					cin >> fileName;
					fileName = "templates/" + fileName + ".txt";
					in.open(fileName);
				}
			}
			in.close();

			cout << "How many processes do you wish to make?" << endl;
			cin >> input;
			jobQueue.clear();

			//read the input of the file
			while(count < (input + savedCount)){
				in.open(fileName);
				while(!in.eof() && count < (input + savedCount)){
					in >> line;
					if(checker == 1){
						saveType = line;
						checker=2;
					}
					else if(checker == 2){
						saveMin = stoi(line);
						checker=3;
					}
					else{
						saveMax = stoi(line);
						checker = 1;
						Process process(saveType, saveMin, saveMax, false);
						jobQueue.push_back(process);
					}
					if(checker == 1){
						count++;
					}

				}
				in.close();
			}
			//randomly assign one of the processes to be critical, every program will have 1 critical process
			jobQueue[rand() % jobQueue.size()].setIsCritical(true);
			counter2++;
			time = clock () - time;
			control.push_back(PCB(jobQueue, counter2, (float)time/CLOCKS_PER_SEC, input2));
			savedCount = count;
			control = cycle(control, &memoryInUse, &time, input2, &resources);
		}


		//input == 3, so we would like to know the current statistics of running programs
		else if(input == 3){
			if(control.size() == 0){
				cerr << "No processes running.";
			}
			for(i=0; i < control.size(); i++){
				cout  << "JOB NUMBER: " << control[i].getPid() << endl;
				cout << "PRIORITY: " << control[i].getPriority() << endl;
				cout << "STATUS: " << control[i].getStatus() << endl;
				cout << "ARRIVAL TIME: " << control[i].getArrivalTime() << endl;
				cout << "MEMORY NEEDED: " << control[i].getMemoryUse() << endl;
				if(control[i].getparentID() != 0){
					cout << "THIS CHILD'S PARENT'S ID: " << control[i].getparentID() << endl;
				}
				getTest = control[i].getTest();
				print(getTest[0]);
			}
			control = cycle(control, &memoryInUse, &time, input2, &resources);
		}

		//input == 4, so we would like to display the help menu
		else if(input == 4){
			cout << "Help Menu\n\nOS Simulator designed by Hana Parece for CMSC 312: VCU Fall 2021.\nThe OS Simulator is designed"
					"to simulate the creation and running of programs and processes through loading processes from 5 program templates.\n"
					"After the OS Simulator finishes 1 'action' lie loading processes or displaying an output, 1 CPU cycle is completed.\n"
					"To load processes, select [2] from the menu. The valid template names are: template1, template2, template3, template4, and template5.\n"
					"More .txt file templates can be added to the 'templates' folder to allow for additional templates. No template naming scheme is required other"
					"than the file being .txt\n"
					"To display statistics onto the console, select [3] from the menu. These statistics only show the processes currently in CPU memory.\n"
					"To display the help menu, select [4] from the menu.\n"
					"The menu selection [5] will simulate 1 cycle without displaying an input.\n"
					"To continue the program after any menu selection, the user will be prompted to select [1]. To exit the program after any menu selection,"
					"the user will be prompted to select [0].\n\n";
		}

		//input == 5, so we would like to simulate a cycle without displaying an output
		else if(input ==5){
			cout << "Simulate how many cycles?\n";
			cin >> input;
			for(j = 0; j < input; j++)
				control = cycle(control, &memoryInUse, &time, input2, &resources);
		}


		//input == 0, so we would like to exit the program
		// in theory we would want to free up all memory when we exit the program
		else if(input == 0){
			cout << "Closing processes..." << endl;
			control.erase(control.begin(), control.end()); //shut down all processes
			cout << "Freeing memory..." << endl;
			memoryInUse = 0; // free up memory
			break;
		}

		else{
			cerr <<"Invalid input\n";
		}


		cout << "Press [1] to continue. Press[0] to exit." << endl << endl;
		cin >> input;
		if(input > 1 || input == 0){
			if(input > 1)
				cout << "Invalid input, closing program..." << endl;;
			cout << "Closing processes..." << endl;
			control.erase(control.begin(), control.end()); //shut down all processes
			cout << "Freeing memory..." << endl;
			memoryInUse = 0; // free up memory
		}


	}
	return 0;
}


/*function to perform one cycle
 during each cycle we want there to be a random chance of an I/O event happening.
 Currently, there is a 1 in 100 chance of a random I/O even occuring.
 If a random I/0 event occurs, all processes are halted for 3 cycles.
 */

vector<PCB> cycle(vector<PCB> pcb, int *memoryInUse, clock_t *time, int schedulerVersion, int *resource){
	static string memoryMessage;
	vector<PCB> hold = pcb;
	int *holdmem = memoryInUse;
	clock_t *timer = time;

	vector<vector<Process>> level1 = pcb[0].getTest();
	vector<Process> level2 = level1[0];
	Process level3 =level2[0];


	if(BankersAlgo(pcb)){ //check if there are enough resources using banker's algorithm
		cascadingTermination(&pcb);

		if(randomIO() > 3){
			//no randomIO event is triggered, we continue with a cycle as normal

			//we are using the Shortest Time Remaining First Scheduler
			if(schedulerVersion == 2){

				//send a copy of the values to run the other scheduler for comparison
				auto start = high_resolution_clock::now();

				hold = scheduler1(hold, &holdmem, &timer);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);

				cout << "Time taken by Priority Scheduler: "
						<< duration.count() << " microseconds" << endl;


				//actually run the scheduler we want
				start = high_resolution_clock::now();

				pcb = scheduler2(pcb, &memoryInUse, &time);

				stop = high_resolution_clock::now();
				duration = duration_cast<microseconds>(stop - start);

				cout << "Time taken by THIS (Shortest Time Remaining) Scheduler: "
						<< duration.count() << " microseconds" << endl;
			}

			//we aer using the Priority Scheduler

			else if(schedulerVersion == 1){

				//send a copy of the values to run the other scheduler for comparison
				auto start = high_resolution_clock::now();

				hold = scheduler2(hold, &holdmem, &timer);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);

				cout << "Time taken by Shortest Time Remaining Scheduler: " << duration.count() << " microseconds" << endl;

				//actually run the scheduler we want
				start = high_resolution_clock::now();

				pcb = scheduler1(pcb, &memoryInUse, &time);

				stop = high_resolution_clock::now();
				duration = duration_cast<microseconds>(stop - start);

				cout << "Time taken by THIS (Priority) Scheduler: " << duration.count() << " microseconds" << endl;
			}

			messageComms1(&pcb);

			for(unsigned int j =0; j < pcb.size() && j < 5; j++){
				level1 = pcb[j].getTest();
				level2 = level1[0];
				level3 =level2[0];
				if(level3.currentCycle <= 0){
					//when a process finishes, we need to free the memory and release resources
					*resource = *resource + level2[0].getResource();
					*memoryInUse = *memoryInUse - level2[0].getMemoryNeeded();
					pcb[j].setMemoryNeeded(pcb[j].getMemoryNeeded() - level2[0].getMemoryNeeded());
					level2.erase(level2.begin());

					if(level2.size() == 0){
						pcb.erase(pcb.begin() + j);
						level1 = pcb[0].getTest();
						level2 = level1[0];
						level3 = level2[0];
						if(pcb.size() == 0){
							cerr << "No Processes Running";
						}
					}
					level1[0] = level2;
					pcb[j].setTest(level1);
					if(schedulerVersion == 2){
						CPU2(&level2);
					}
					else if(schedulerVersion == 1){
						CPU1(&level2);
					}
				}
			}
		}
		else{
			// a randomIO event has been triggered, all processes are required to wait.
			for(unsigned int i = 0; i < pcb.size(); i++){
				pcb[i].setStatus("Waiting");
			}
		}

		//1 in 30 chance of releasing a resource each cycle.
		if(rand()% 30 == 5){
			level1 = pcb[0].getTest();
			level2 = level1[0];
			level2[0].setResource(level2[0].getResource() - 1);
			level1[0] = level2;
			pcb[0].setTest(level1);
			*resource--;

		}


		//inter process communication via sharing memory, each process is paired with another process to share memory
		//and share a message with
		if(pcb.size() > 1 && pcb.size()%2 == 0){
			for(unsigned int p; p < pcb.size(); p = p + 2){
				pcb[p].setMessage("We Share Memory!");
				memoryMessage = pcb[p].getMessage();
				pcb[p + 1].setMessage(memoryMessage);
			}
		}

		return pcb;
	}

	//there is a problem with resource allocation, so instead we leave and wait for resources to be freed

	//1 in 30 chance of releasing a resource each cycle.
	if(rand()% 30 == 5){
		level1 = pcb[0].getTest();
		level2 = level1[0];
		level2[0].setResource(level2[0].getResource() - 1);
		level1[0] = level2;
		pcb[0].setTest(level1);
		*resource--;

	}
	return pcb;
}

//each cycle there needs to be a chance of a random IO event happening
int randomIO(){
	static int count;
	int num;
	num = rand() % 100 + 1; //there is a 1 in 100 chance of a randomIO event occuring each cycle
	if(num == 50){
		cout << "Random I/O Event Triggered, Waiting " << 3 - count <<  " Cycles" << endl;
		if(count == 3){
			count = 0;
			return count;
		}
		else
			count++;
		return count;
	}
	count = 50;
	return count;

}

//if somehow a parent finishes before a child, we need to kill all child processes
//go over the pcb and search for children
//once a child is found, check if the parent's PID is still in the pcb
//if it is not, delete the child (and all other children that share that parent PID)
//This process should work for a child who has a child and so on.
void cascadingTermination(vector<PCB> *pcb){
	for(unsigned int i = 0; i < pcb->size(); i++){
		if(pcb->at(i).getparentID() != 0){
			for(unsigned int j = 0; j < pcb->size(); j++){
				if(pcb->at(i).getparentID() != pcb->at(j).getparentID() && j == pcb->size()){ //we can't find the Parent's ID
					pcb->erase(pcb->begin() + i); //erase the child
				}
			}
		}


	}
}


//basically stores a message from a process into the inbox
//and then the first process of the PCB will pick up that message
//the inbox is probably only going to have 1 message in it at a time, until FORKS start happening
void messageComms1(vector<PCB> *pcb){
	static vector<string> inbox;
	string message;
	for(unsigned int i = 0; i < pcb->size(); i++){
		if(pcb->at(i).getMessage() == "This is a message! :D"){
			message = " This is a message from Process " + to_string(pcb->at(i).getPid());
			pcb->at(i).setMessage(message);
			inbox.push_back(pcb->at(i).getMessage());
		}

	}
	//the first process will pick up the first message in the inbox
	if(inbox.size() > 1){
		pcb->at(0).setMessage(inbox[0]);
		inbox.erase(inbox.begin());
	}
}

//we want to avoid deadlock, so we need to check that the resources are available
//and that is is safe for us to run
bool BankersAlgo(vector<PCB> pcb){
	vector<vector<Process>> level1 = pcb[0].getTest();
	vector<Process> level2 = level1[0];
	Process level3 =level2[0];
	int resourceNeed;


	for (unsigned int i =0; i < pcb.size(); i++){
		level1 = pcb[i].getTest();
		level2 = level1[0];
		for(unsigned int j = 0; j <pcb.size(); j++){
			level3 = level2[j];
			resourceNeed += level3.getResource(); //calculate the resource need
		}

		//check if the resource need is less than what we have available
		if(resourceNeed > 50){
			return false;
		}


		return true;

	}

	return false;
}

