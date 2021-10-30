//============================================================================
// Name        : OS.cpp
// Author      : Hana Parece
// Version     :
// Copyright   : Your copyright notice
// Description : OS simulator for CMSC 312
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "Process.h"
#include "Scheduler.h"
using namespace std;


int main() {
	vector<Process> processes;
	int input,i, saveMin, saveMax;
	int savedCount = 0;
	int checker = 1;
	static int count;
	string fileName = "template1";
	string stringIn, saveType, line;
	ifstream in;

	cout << "Welcome to OS Simulator\n_______________________\n\nPress [1] to continue. Press [0] to exit." << endl;
	cin >> input;
	while(input == 1){
		cout << "_______________________\n\nPress [2] to load a job\nPress [3] to display statistics\n"
				<< "Press [4] for help \nPress [0] to exit." << endl << endl;
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
								Process process(saveType,(count +1), saveMin, saveMax);
								processes.push_back(process);
							}
							if(checker == 1){
								count++;
								processes = scheduler(processes); //whenever a process is done we
							}
					}
					in.close();
				}
				savedCount = count;
			}


		//input == 3, so we would like to know the current statistics of running programs
		else if(input == 3){
			cout
					<< left
					<<setw(12)
					<<"Process ID"
					<< left
					<< setw(12)
					<< "Operation"
					<< left
					<< setw(12)
					<< "Min Cycles"
					<< left
					<< setw(12)
					<< "Max Cycles"
					<< left
					<< setw(12)
					<< "Actual Cycles"
					<< endl;
			for(i=0; i < processes.size(); i++){
						cout
								<< left
								<<setw(12)
								<< processes[i].getID()
								<< left
								<< setw(12)
								<< processes[i].getType()
								<< left
								<< setw(12)
								<< processes[i].getMinCycle()
								<< left
								<< setw(12)
								<< processes[i].getMaxCycle()
								<< left
								<< setw(12)
								<< processes[i].getActualCycle()
								<< endl;
			}


		}

		//input == 4, so we would like to display the help menu
		else if(input == 4){

		}
		//input == 0, so we would like to exit the program
		else if(input == 0)
			break;
		else
				cerr <<"Invalid input\n";

		cout << "Press [1] to continue. Press[0] to exit.\n\n";
		cin >> input;
	}
	return 0;
}
