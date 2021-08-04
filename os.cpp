#include"os.h"

list<PCB*> readyList[3];
list<PCB*> blockList;
list<PCB*> allList;
resource** res;
PCB* now_executing;
static int count_p = 0;

void preempt(PCB* p, PCB* self) {
	p->status = running;
	p->status_list = &readyList[p->get_priority()];//���п���
	now_executing = p;
}
void Scheduler() {
	PCB* highest;//��Ϊ֮ǰ���µĽ����Ѿ�������������ȼ����У��ڴ˺����ʹ��get_running�������ǰѸý������˳������Ǵ�ġ�ֻ��
	//����һ��ȫ�ֱ�������  �ڼ���֮ǰ��ʹ��getruning�ſ���
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
	/*remove r from self->resources_occupy, and u= u + n������Դr �ӵ�ǰ��
	��ռ�õ���Դ�б����Ƴ���������Դr �Ŀ���������u ��Ϊu+n*/
	remove_p(p, r, n);
	/*����������в�Ϊ��, �����������ײ������������Դ��req С��
	���ڿ�����Դ����u����������������̣������������*/
	while (!r->get_waitinglist()->empty())//����������Դ��Ϊpair�еĵڶ���
	{
		PCB* q = r->get_waitinglist()->front().first;
		int req_num = r->get_waitinglist()->front().second;
		int empty_num = r->get_empty();
		if (empty_num >= req_num)
		{
			r->dec_resource(req_num); //������Դ��������
			r->get_waitinglist()->pop_front();// ����Դr �������������Ƴ�
			q->status = ready;
			q->status_list = &readyList[q->priority];
			q->resource_occupied.insert(make_pair(r->get_id(), req_num));//����r ��q ��ռ�õ���Դ��
			readyList[q->priority].push_back(q); // ����q ����������
			blockList.remove(q);
			cout << "wake up process " << q->get_name() << endl;
		}
	}
}
void release(int rid, int n) //rid Ϊ��ԴID��n Ϊ�ͷŵ���Դ����
{
	resource* r = get_RCB(rid);
	/*remove r from self->resources_occupy, and u= u + n������Դr �ӵ�ǰ��
	��ռ�õ���Դ�б����Ƴ���������Դr �Ŀ���������u ��Ϊu+n*/
	remove_p(now_executing, r, n);
	/*����������в�Ϊ��, �����������ײ������������Դ��req С��
	���ڿ�����Դ����u����������������̣������������*/
	while (!r->get_waitinglist()->empty() && r->get_empty() >= r->get_waitinglist()->front().second)//����������Դ��Ϊpair�еĵڶ���
	{
		PCB* q = r->get_waitinglist()->front().first;
		int req_num = r->get_waitinglist()->front().second;
		r->dec_resource(req_num); //������Դ��������
		r->get_waitinglist()->pop_front();// ����Դr �������������Ƴ�
		q->status = ready;
		q->status_list = &readyList[q->priority];
		q->resource_occupied.insert(make_pair(r->get_id(), req_num));//����r ��q ��ռ�õ���Դ��
		readyList[q->priority].push_back(q); // ����q ����������
		blockList.remove(q);
		cout << "wake up process " << q->get_name() << endl;
	}
	Scheduler(); //�������ȼ�����ռʽ���Ȳ��ԣ���˵��н��̻����Դʱ����Ҫ�鿴��ǰ�����ȼ���������е���
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
		//release��Դ
		map<int, int>::iterator it;
		for (it = p->resource_occupied.begin(); it != p->resource_occupied.end();) {
			release_p(p, it->first, (*it).second);
			it = p->resource_occupied.erase(it);
			//if (p->resource_occupied.size() == 0)//�ڶ�map�ı������漰��Ԫ�ص�ɾ���޸ģ����ܻᱨ��
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
		
		p->father->children.remove(p);//�Ӹ��׽ڵ�ĺ����б���ɾ���ýڵ��pcb��Ϣ
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
	//�������������
	allList.push_back(p);
	readyList[p->get_priority()].push_back(p);//�����д�
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
	//��ʼ���ĸ���Դ
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
		//PCB����Դռ�����д�뵱ǰִ�н��̵�pcb����
		//insert��
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
			r->push_to_queue(now_running_process, n);//r��waitinglist��Ҫ��¼�ý���

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
