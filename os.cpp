#include"os.h"

list<PCB*> readyList[3];
list<PCB*> blockList;
list<PCB*> allList;
resource** res;
PCB* now_executing;
static int count_p = 0;

void preempt(PCB* p, PCB* self) {
	p->status = running;
	p->status_list = &readyList[p->get_priority()];//可有可无
	now_executing = p;
}
void Scheduler() {
	PCB* highest;//因为之前把新的进程已经加入了最高优先级队列，在此函数里，使用get_running函数就是把该进程挑了出来，是错的。只有
	//设立一个全局变量或者  在加入之前就使用getruning才可以
	if (readyList[2].empty())
	{
		if (readyList[1].empty())
		{
			highest = readyList[0].front();
		}
		else
		{
			highest = readyList[1].front();
		}
	}
	else {
		highest = readyList[2].front();
	}

	if (now_executing->get_priority() < highest->get_priority())
	{
		PCB* temp = now_executing;
		preempt(highest, now_executing);
		cout << "process " << highest->get_name() << " is running. process " << temp->get_name() << " is ready" << endl;
	}
	else if (now_executing->status != running) {
		preempt(highest, now_executing);
	}
	else {
		cout << "process " << now_executing->get_name() << " is running." << endl;
	}
}

resource* get_RCB(int rid) {
	return res[rid - 1];
}

void remove_p(PCB* p, resource* r, int n) {
	r->inc_resource(n);
	int rid = r->get_id();
	cout << "release R" << rid << ". ";

}
void release_p(PCB* p, int rid, int n) {
	resource* r = get_RCB(rid);
	/*remove r from self->resources_occupy, and u= u + n，将资源r 从当前进
	程占用的资源列表里移除，并且资源r 的可用数量从u 变为u+n*/
	remove_p(p, r, n);
	/*如果阻塞队列不为空, 且阻塞队列首部进程需求的资源数req 小于
	等于可用资源数量u，则唤醒这个阻塞进程，放入就绪队列*/
	while (!r->get_waitinglist()->empty())//进程请求资源数为pair中的第二项
	{
		PCB* q = r->get_waitinglist()->front().first;
		int req_num = r->get_waitinglist()->front().second;
		int empty_num = r->get_empty();
		if (empty_num >= req_num)
		{
			r->dec_resource(req_num); //可用资源数量减少
			r->get_waitinglist()->pop_front();// 从资源r 的阻塞队列中移除
			q->status = ready;
			q->status_list = &readyList[q->priority];
			q->resource_occupied.insert(make_pair(r->get_id(), req_num));//插入r 到q 所占用的资源中
			readyList[q->priority].push_back(q); // 插入q 到就绪队列
			blockList.remove(q);
			cout << "wake up process " << q->get_name() << endl;
		}
	}
}
void release(int rid, int n) //rid 为资源ID，n 为释放的资源数量
{
	resource* r = get_RCB(rid);
	/*remove r from self->resources_occupy, and u= u + n，将资源r 从当前进
	程占用的资源列表里移除，并且资源r 的可用数量从u 变为u+n*/
	remove_p(now_executing, r, n);
	/*如果阻塞队列不为空, 且阻塞队列首部进程需求的资源数req 小于
	等于可用资源数量u，则唤醒这个阻塞进程，放入就绪队列*/
	while (!r->get_waitinglist()->empty() && r->get_empty() >= r->get_waitinglist()->front().second)//进程请求资源数为pair中的第二项
	{
		PCB* q = r->get_waitinglist()->front().first;
		int req_num = r->get_waitinglist()->front().second;
		r->dec_resource(req_num); //可用资源数量减少
		r->get_waitinglist()->pop_front();// 从资源r 的阻塞队列中移除
		q->status = ready;
		q->status_list = &readyList[q->priority];
		q->resource_occupied.insert(make_pair(r->get_id(), req_num));//插入r 到q 所占用的资源中
		readyList[q->priority].push_back(q); // 插入q 到就绪队列
		blockList.remove(q);
		cout << "wake up process " << q->get_name() << endl;
	}
	Scheduler(); //基于优先级的抢占式调度策略，因此当有进程获得资源时，需要查看当前的优先级情况并进行调度
}
void remove_from_waitinglist(PCB* p) {
	auto it = p->blocked_list->begin();
	for (; it != p->blocked_list->end();) {
		if (it->first == p)
		{
			it = p->blocked_list->erase(it);
		}
		else {
			it++;
		}
	}

}

void kill_tree(PCB* p) {
	if (p != NULL)
	{
		auto child = p->children;
		list<PCB*>::iterator iter;
		for (iter = child.begin(); iter != child.end();) {
			kill_tree(*iter);
			iter = child.erase(iter);
		}
		//release资源
		map<int, int>::iterator it;
		for (it = p->resource_occupied.begin(); it != p->resource_occupied.end();) {
			release_p(p, it->first, (*it).second);
			it = p->resource_occupied.erase(it);
			//if (p->resource_occupied.size() == 0)//在对map的遍历中涉及对元素的删除修改，可能会报错
			//{
			//	break;
			//}
		}
		if (p->status == block)
		{
			remove_from_waitinglist(p);
		}
		allList.remove(p);
		readyList[p->priority].remove(p);
		blockList.remove(p);
		
		p->father->children.remove(p);//从父亲节点的孩子列表中删除该节点的pcb信息
		delete p;
	}

}

void create(string pname, PCB* father, int priority) {
	//create PCB
	map<int, int> resource;
	int pid = count_p;
	PCB* pcb_p = new PCB(pid, pname, father, priority);
	pcb_p->status_list = &readyList[priority];
	father->children.push_back(pcb_p);
	PCB* p = pcb_p;
	count_p++;
	//插入就绪队列中
	allList.push_back(p);
	readyList[p->get_priority()].push_back(p);//可能有错
	Scheduler();
}

void destroy(string pname) {
	list<PCB*>::iterator it;
	for (it = allList.begin(); it != allList.end(); it++) {
		if ((**it).get_name() == pname) {
			break;
		}
	}
	if (it == allList.end()&&allList.size()!=0) {
		throw exception();
	}
	PCB* temp = *it;
	kill_tree(temp);
	Scheduler();
}


void init() {
	//初始化四个资源
	res = new resource*[4];
	res[0] = new resource(1, 1);
	res[1] = new resource(2, 2);
	res[2] = new resource(3, 3);
	res[3] = new resource(4, 4);
	PCB* in = new PCB(0, "init", NULL, 0);
	in->status = running;
	readyList[0].push_back(in);
	now_executing = in;
	count_p++;
}

void request(int rid, int n) {
	resource* r = get_RCB(rid);
	if (r->get_empty() >= n) {
		r->dec_resource(n);
		//PCB将资源占用情况写入当前执行进程的pcb当中
		//insert；
		now_executing->resource_occupied.insert(make_pair(rid, n));
	}
	else {
		if (n > r->get_total())
		{
			cout << "error" << endl;
		}
		else {
			PCB* now_running_process = now_executing;
			now_running_process->status = block;
			for (auto it = readyList[now_executing->get_priority()].begin(); it != readyList[now_executing->get_priority()].end();) {
				if ((*it) == now_running_process) {
					it = readyList[now_executing->get_priority()].erase(it);
				}
				else {
					it++;
				}
			}
			now_running_process->status_list = &blockList; //point to block list, self is blocked by r
			now_running_process->blocked_list = r->get_waitinglist();
			blockList.push_back(now_running_process);
			r->push_to_queue(now_running_process, n);//r的waitinglist需要记录该进程

			Scheduler();
		}

	}
}

void time_out() {
	PCB* q = now_executing;
	readyList[q->get_priority()].remove(q);
	q->status = ready;
	readyList[q->get_priority()].push_back(q);
	Scheduler();
}
