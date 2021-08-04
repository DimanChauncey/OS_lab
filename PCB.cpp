#include"PCB.h"
PCB::PCB(int pid, string pname, PCB* father, int priority)
{
	this->pid = pid;
	this->pname = pname;
	this->father = father;
	this->status = ready;
	this->priority = priority;
}