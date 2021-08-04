#include<vector>
#include"os.h"
#include<fstream>
void split_string(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void cr(vector<string> args) {
	if (args.size() > 3 || args.size() == 1)
	{
		cout << "invalid input" << endl;
		return;
	}
	else if (args.size() == 2) {
		PCB* p = now_executing;
		//默认创建优先级为1的进程
		create(args[1], p, 1);
		return;
	}
	else
	{
		if (args[2] >= "1"&&args[2] <= "2")
		{
			PCB* p = now_executing;
			create(args[1], p, atoi(args[2].c_str()));
		}
		else
		{
			cout << "invalid input::Priority Error" << endl;
		}
		return;
	}
}

void de(vector<string> args) {
	if (args.size() != 2) {
		cout << "invalid input::deficient or redundant parameter" << endl;
		return;
	}
	try
	{
		//先把该进程占用的资源列出来
		//找到资源现在等待队列 的第一名
		destroy(args[1]);

		//对比第一名，如果不一样则：  cout；一样则不打印什么

	}
	catch (const std::exception&)
	{
		cout << "now, no process named " << args[1] << endl;
	}
}

void rel(vector<string> args) {
	if (args.size() != 3)//输入有误
	{
		if (args.size() < 3) cout << "invalid input::No detail what you want to release" << endl;
		else cout << "invalid input::Redundant words" << endl;
		return;
	}
	else
	{
		if (args[1] == "R1")
		{
			try
			{
				release(1, atoi(args[2].c_str()));
			}
			catch (const std::exception&)//思考一下如果删除的资源数目大于本身的数目时会怎么样？会删除后才触发catch么还是回退后再出发catch；
			{
				cout << "invalid input::wrong number input" << endl;
			}

		}
		else if (args[1] == "R2") {
			try
			{
				release(2, atoi(args[2].c_str()));
			}
			catch (const std::exception&)//思考一下如果删除的资源数目大于本身的数目时会怎么样？会删除后才触发catch么还是回退后再出发catch；
			{
				cout << "invalid input::wrong number input" << endl;
			}
		}
		else if (args[1] == "R3") {
			try
			{
				release(3, atoi(args[2].c_str()));
			}
			catch (const std::exception&)//思考一下如果删除的资源数目大于本身的数目时会怎么样？会删除后才触发catch么还是回退后再出发catch；
			{
				cout << "invalid input::wrong number input" << endl;
			}
		}
		else if (args[1] == "R4") {
			try
			{
				release(4, atoi(args[2].c_str()));
			}
			catch (const std::exception&)//思考一下如果删除的资源数目大于本身的数目时会怎么样？会删除后才触发catch么还是回退后再出发catch；
			{
				cout << "invalid input::wrong number input" << endl;
			}
		}
		else {
			cout << "invalid input::no resource named " << args[1] << endl;
		}
	}
}

void ls(vector<string> args) {
	if (args.size() != 2)//输入有误
	{
		if (args.size() == 1) cout << "invalid input::No detail what you want to list" << endl;
		else cout << "invalid input::Redundant words" << endl;
		return;
	}
	else
	{
		if (args[1] == "res")
		{
			cout << "res:" << endl;
			for (int i = 0; i < 4; i++) {
				cout << i + 1 << ":" << res[i]->get_empty() << endl;
			}
		}
		else if (args[1] == "ready") {
			std::list<PCB*>::iterator it;
			cout << "ready:" << endl;
			cout << "2:";
			for (it = readyList[2].begin(); it != readyList[2].end(); ) {
				auto it_before = it;
				if (++it == readyList[2].end())
				{
					cout << (*it_before)->get_name();
				}
				else {
					cout << (*it_before)->get_name() << "->";
				}
			}
			cout << "\n1:";
			for (it = readyList[1].begin(); it != readyList[1].end(); ) {
				auto it_before = it;
				if (++it == readyList[1].end())
				{
					cout << (*it_before)->get_name();
				}
				else {
					cout << (*it_before)->get_name() << "->";
				}
			}
			cout << "\n0:";
			for (it = readyList[0].begin(); it != readyList[0].end(); ) {
				auto it_before = it;
				if (++it == readyList[0].end())
				{
					cout << (*it_before)->get_name() << endl;
				}
				else {
					cout << (*it_before)->get_name() << "->";
				}
			}
		}
		else if (args[1] == "block") {
			for (int i = 0; i < 4; i++) {
				cout << "R" << i + 1 << "： ";
				auto temp = res[i]->get_waitinglist();
				//queue < pair<PCB*, int>> temp_queue;
				int j = 0;
				while (j < temp->size())
				{
					auto thing = temp->front();
					cout << thing.first->get_name() << " ";
					//temp_queue.push(temp.front());
					temp->pop_front();
					temp->push_back(thing);
					j++;
				}
				cout << endl;

			}
		}
		else {
			cout << "invalid input::Please Check Words" << endl;
		}
	}

}

void to(vector<string> args) {
	if (args.size() > 1)
	{
		cout << "invalid input::redundant words" << endl;
	}
	else {
		string before_p = now_executing->get_name();
		time_out();
		string now_p = now_executing->get_name();
		if (before_p == now_p) {
			cout << "process " << now_p << " is running." << endl;
		}
		else
		{
			cout << "process " << now_p << " is running. process " << before_p << " is ready" << endl;
		}
	}
}

void ini(vector<string> args) {
	init();
	cout << "init process is running" << endl;
}

void pr(vector<string> args) {
	if (args.size() != 2) {
		cout << "invalid input" << endl;
	}
	else {
		PCB* temp=NULL;
		for (auto item : allList) {
			if (item->get_name() == args[1]) {
				temp = item;
				break;
			}
		}
		if (temp == NULL) {
			cout << "no process named " << args[1] << endl;
			return;
		}
		else {
			cout << "pid:" << temp->pid<<endl;
			string s;
			if (temp->status==running)
			{
				s = "running";
			}
			else if (temp->status=block)
			{
				s = "block";
			}
			else {
				s = "ready";
			}
			cout << "status:" << s << endl;
			cout << "priority:" << temp->priority << endl;
			cout << "father:" << temp->father->get_name() << endl;
			for (auto i : temp->resource_occupied) {
				cout << "占有 R" << i.first << " " << i.second << "个" << endl;
			}
			cout << "children:";
			for (auto i : temp->children) {
				cout << i->get_name()<<" ";
			}
			cout << endl;
		}

		
	}
}

void req(vector<string> args) {
	if (args.size() != 3)
	{
		cout << "invalid input" << endl;
	}
	else {
		PCB* pre = now_executing;
		if (args[1] == "R1")
		{
			try
			{
				request(1, atoi(args[2].c_str()));
			}
			catch (const std::exception&)
			{
				cout << "invalid input::wrong number input" << endl;
				return;
			}
			if (now_executing == pre)
			{
				cout << "process " << now_executing->get_name() << " requests " << atoi(args[2].c_str()) << " R1" << endl;
			}
			else {
				cout << "process " << now_executing->get_name() << " is running. process " << pre->get_name() << " is blocked." << endl;
			}

		}
		else if (args[1] == "R2") {
			try
			{
				request(2, atoi(args[2].c_str()));
			}
			catch (const std::exception&)
			{
				cout << "invalid input::wrong number input" << endl;
				return;
			}
			if (now_executing == pre)
			{
				cout << "process " << now_executing->get_name() << " requests " << atoi(args[2].c_str()) << " R2" << endl;
			}
			else {
				cout << "process " << now_executing->get_name() << " is running. process " << pre->get_name() << " is blocked." << endl;
			}
		}
		else if (args[1] == "R3") {
			try
			{
				request(3, atoi(args[2].c_str()));
			}
			catch (const std::exception&)
			{
				cout << "invalid input::wrong number input" << endl;
				return;
			}
			if (now_executing == pre)
			{
				cout << "process " << now_executing->get_name() << " requests " << atoi(args[2].c_str()) << " R3" << endl;
			}
			else {
				cout << "process " << now_executing->get_name() << " is running. process " << pre->get_name() << " is blocked." << endl;
			}
		}
		else if (args[1] == "R4") {
			try
			{
				request(4, atoi(args[2].c_str()));
			}
			catch (const std::exception&)
			{
				cout << "invalid input::wrong number input" << endl;
				return;
			}
			if (now_executing == pre)
			{
				cout << "process " << now_executing->get_name() << " requests " << atoi(args[2].c_str()) << " R4" << endl;
			}
			else {
				cout << "process " << now_executing->get_name() << " is running. process " << pre->get_name() << " is blocked." << endl;
			}
		}
		else {
			cout << "invalid input::no resource named " << args[1] << endl;
		}
	}
}

int main() {
	string s[17];
	s[0] = "init";
	s[1] = "c x";
	s[2] = "c p";
	s[3] = "c q";
	s[4] = "c r";
	s[5] = "t";
	s[6] = "r 2 1";
	s[7] = "t";
	s[8] = "r 3 3";
	s[9] = "t";
	s[10] = "r 4 3";
	s[11] = "t";
	s[12] = "t";
	s[13] = "r 3 1";
	s[14] = "r 4 2";
	s[15] = "r 2 2";
	s[16] = "t";
	ifstream file;
	file.open("./input.txt");
	string str;
	while (!file.eof())
	{
		getline(file, str);
		vector<string> args;
		split_string(str, args, " ");
		if (args[0] == "cr")
		{
			cr(args);
		}
		else if (args[0] == "de")
		{
			de(args);
		}
		else if (args[0] == "list") {
			ls(args);
		}
		else if (args[0] == "to") {
			to(args);
		}
		else if (args[0] == "req") {
			req(args);
		}
		else if (args[0] == "init") {
			ini(args);
		}
		else if (args[0] == "rel") {
			rel(args);
		}
		else if (args[0]=="pr")
		{
			pr(args);
		}
		else {
			cout << "invalid input, please check! " << endl;
		}
	}

	//for (int i = 0; i < 17; i++) {
	//	vector<string> args;
	//	split_string(s[i], args, " ");
	//	if (args[0] == "c")
	//	{
	//		cr(args);
	//	}
	//	else if (args[0] == "de")
	//	{
	//		de(args);
	//	}
	//	else if (args[0] == "list") {
	//		ls(args);
	//	}
	//	else if (args[0] == "t") {
	//		to(args);
	//	}
	//	else if (args[0] == "r") {
	//		req(args);
	//	}
	//	else if (args[0] == "init") {
	//		ini(args);
	//	}
	//	else if (args[0] == "rel") {
	//		rel(args);
	//	}
	//	else {
	//		cout << "invalid input, please check! " << endl;
	//	}
	//}
	while (true)
	{
		string instruction;
		getline(cin, instruction, '\n');
		vector<string> args;
		split_string(instruction, args, " ");
		try
		{
			if (!args.size())
			{
				throw exception("error");
			}
			if (args[0] == "cr")
			{
				cr(args);
			}
			else if (args[0] == "de")
			{
				de(args);
			}
			else if (args[0] == "list") {
				ls(args);
			}
			else if (args[0] == "to") {
				to(args);
			}
			else if (args[0] == "req") {
				req(args);
			}
			else if (args[0] == "init") {
				ini(args);
			}
			else if (args[0] == "rel") {
				rel(args);
			}
			else if (args[0] == "pr")
			{
				pr(args);
			}
			else {
				cout << "invalid input, please check! " << endl;
			}
		}
		catch (const std::exception&)
		{
			cout << "invalid input, please check! " << endl;
		}
		

	}
	return 0;
}