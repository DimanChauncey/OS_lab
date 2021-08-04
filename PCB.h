#pragma once
#include<string.h>
#include<map>
#include<list>
#include<utility>
#include<iostream>
using namespace std;
enum Status
{
	ready, block, running
};

class PCB
{
public:
	int pid;
	string pname;
	Status status;
	list<PCB*>* status_list;
	list<pair<PCB*, int>>* blocked_list;
	int priority;
	map<int, int> resource_occupied;
	PCB* father;
	list<PCB*> children;
	PCB(int pid, string pname, PCB* father, int priority);
	int get_priority() {
		return this->priority;
	}
	string get_name() {
		return this->pname;
	}
};

