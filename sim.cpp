#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>
using namespace std;

struct page
{
	int frame;
	int mbit;
};

void nru(int size);
void fifo(int size);

int main()
{
	string algo;
	int size;
	cout << "알고리즘을 입력하시오 (fifo/nru) :" << endl;
	cin >> algo;
	cout << "원하는 frame의 수를 입력하시오 :" << endl;
	cin >> size;

	if (algo == "fifo")
	{
		fifo(size);
	}
	else
	{
		nru(size);
	}
}

void nru(int size)
{

	int fault = 0;
	int hit = 0;

	list<page> pages(size);

	int read = 0;
	int write = 0;

	ifstream input_file("access.list");
	string line;
	while (getline(input_file, line))
	{
		int index = line.find(' ');
		int address = stoi(line.substr(0, index));
		string ref = line.substr(index + 1, sizeof(address));

		if (ref == "read")
			read++;
		else
			write++;

		page temp;
		temp.frame = address;
		if (ref == "read")
			temp.mbit = 0;
		else
			temp.mbit = 1;

		bool check = false;
		list<page>::iterator it;
		for (it = pages.begin(); it != pages.end();)
		{
			if ((*it).frame == address)
			{
				hit++;
				check = true;
				it = pages.erase(it);
				pages.insert(pages.end(), temp);
				break;
			}
			else
				it++;
		}

		if (check == false)
		{
			list<page>::iterator it;
			bool find = false;
			fault++;
			for (it = pages.begin(); it != pages.end();)
			{
				if ((*it).mbit == 0)
				{
					it = pages.erase(it);
					find = true;
					pages.insert(pages.end(), temp);
					break;
				}
				else
					it++;
			}
			if (find == false)
			{
				pages.erase(pages.begin());
				pages.insert(pages.end(), temp);
			}
		}
	}
	int access = hit + fault;
	int frate = ((float)fault / access) * 100;


	cout << "Total number of access: " << access << endl;
	cout << "Total number of read: " << read << endl;
	cout << "Total number of write: " << write << endl;
	cout << "Total number of hits: " << hit << endl;
	cout << "Total number of faults: " << fault << endl;
	cout << "Page fault rate: " << fault << "/" << access << " = " << frate << "%" << endl;


	input_file.close();


}

void fifo(int size)
{
	int fault = 0;
	int hit = 0;

	list<int> frame(size, 0);

	int read = 0;
	int write = 0;

	ifstream input_file("access.list");
	string line;
	while (getline(input_file, line))
	{
		int index = line.find(' ');
		int address = stoi(line.substr(0, index));
		string ref = line.substr(index + 1, sizeof(address));

		if (ref == "read")
			read++;
		else
			write++;

		bool check = false;
		list<int>::iterator it;
		for (it = frame.begin(); it != frame.end(); it++)
		{
			if ((*it) == address)
			{
				hit++;
				check = true;
				break;
			}
		}

		if (check == false)
		{
			frame.pop_front();
			frame.insert(frame.end(), address);
			fault++;
		}

	}
	int access = hit + fault;
	int frate = ((float)fault / access) * 100;

	cout << "Total number of access: " << access << endl;
	cout << "Total number of read: " << read << endl;
	cout << "Total number of write: " << write << endl;
	cout << "Total number of hits: " << hit << endl;
	cout << "Total number of faults: " << fault << endl;
	cout << "Page fault rate: " << fault << "/" << access << " = " << frate << "%" << endl;


	input_file.close();
}
