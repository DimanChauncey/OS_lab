#pragma once
#include<list>
#include"PCB.h"
class resource
{
public:
	resource() {

	}
	resource(int id, int total) {
		this->id = id;
		this->total = total;
		this->empty = total;
	}
	int get_empty() {
		return this->empty;
	}
	int get_total() {
		return this->total;
	}
	void dec_resource(int num) {
		this->empty -= num;
	}
	void inc_resource(int num) {
		this->empty += num;
	}
	void push_to_queue(PCB* p, int num) {
		this->waitingList.push_back(make_pair(p, num));
	}
	void pop_from_queue() {
		this->waitingList.pop_front();
	}
	int get_id() {
		return this->id;
	}
	std::list<std::pair<PCB*, int>>* get_waitinglist() {
		return &this->waitingList;
	}
private:
	int id;
	int total;
	int empty;
	std::list<std::pair<PCB*, int>> waitingList;
};