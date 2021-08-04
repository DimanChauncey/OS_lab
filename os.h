#pragma once
#include"resource.h"
#include<string>

extern list<PCB*> readyList[3];
extern list<PCB*> blockList;
extern list<PCB*> allList;
extern resource** res;
extern PCB* now_executing;
extern int count_p;

void preempt(PCB* p, PCB* self);
void Scheduler();
resource* get_RCB(int rid);
void release_p(PCB* p, int rid, int n);
void release(int rid, int n);
void remove_from_waitinglist(PCB* p);
void kill_tree(PCB* p);
void create(string pname, PCB* father, int priority);
void destroy(string pname);
void init();
void request(int rid, int n);
void time_out();
