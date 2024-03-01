#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main()
{
	string command;

	inf_int first;
	inf_int second;
	char option;

	while (1)
	{
		cout << "Input: ";
		getline(cin, command);
		if (command == "0") return 0;

		istringstream ss(command);
		string stringBuffer;
		vector<string> com;
		com.clear();
		while (getline(ss, stringBuffer, ' '))
		{
			com.push_back(stringBuffer);
		}
		first = inf_int(com[0].c_str());
		option = com[1].front();
		second = inf_int(com[2].c_str());

		cout << "Output: ";
		if (option == '+')
		{
			cout << first + second << endl;
		}
		else if (option == '-')
		{
			cout << first - second << endl;
		}

		else if (option == '*')
		{
			cout << first * second << endl;
		}
	}

	return 0;
}