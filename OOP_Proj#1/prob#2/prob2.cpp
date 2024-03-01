#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
/* struct used when we store the informations from file1.txt */
typedef struct Student
{
	string Name;
	string SID;
	string Dept;
	string Byear;
	string Tel;
};
/* Sort the informations from file1.txt by the name (Alphabetically, Increasing order) */
bool SortByName(Student x, Student y)
{
	return x.Name < y.Name;
}
/* Sort the informations from file1.txt by the student number (Increasing order) */
bool SortBySID(Student x, Student y)
{
	return x.SID < y.SID;
}
/* Sort the informations from file1.txt by the birth year (Increasing order) */
bool SortByAdmissionyear(Student x, Student y)
{
	return x.SID < y.SID;
}
/* Sort the informations from file1.txt by the department (Increasing order) */
bool SortByDept(Student x, Student y)
{
	return x.Dept < y.Dept;
}
int main()
{
	/* variables for the informations of students to store in file1.txt */
	string name;
	string sID;
	string dept;
	string byear;
	string tel;

	int sel;	// variables for the choice of options in main menu

	string search;
	string line;
	string word;

	fstream myfile;	// create file stream object variable

	while (1)
	{	
		cout << "- Main Menu -" << endl;
		cout << "1. Insertion" << endl;
		cout << "2. Search" << endl;
		cout << "3. Sorting Option" << endl;
		cout << "4. Exit" << endl;
		
		cin >> sel;	// insert the selection
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (sel == 1)
		{
			/* insert informations and store in file1.txt */
			int i = 0;
			
			cout << "Name? ";
			getline(cin, name);

			cout << "Student ID (10 digits)? ";
			getline(cin, sID);

			cout << "Departmant? ";
			getline(cin, dept);

			cout << "Birth Year (4 digits)? ";
			getline(cin, byear);

			cout << "Tel (11 digits)? ";
			getline(cin, tel);

			myfile.open("file1.txt", ios::in);
			/* explore all contents, and count duplicate informations with student ID */
			while (getline(myfile, line))
			{
				if (line.find(sID) != line.npos)
				{
					i++;
				}
			}
			if (i >= 1)	// if the duplicate informations exist
			{
				cout << "Error: Already Inserted!" << endl;
			}
			myfile.close();
			if (i < 1)	// if the duplicate informations don't exist
			{
				myfile.open("file1.txt", ios::out | ios::app);	// store inserted informations
				myfile << name << '\t';
				myfile << sID << '\t';
				myfile << dept << '\t';
				myfile << byear << '\t';
				myfile << tel << '\n';
				myfile.close();
			}

			int nol = 0;	// variable that stores number of lines 
			vector <Student> st;	// vector struct that stores the information from file1.txt and it is used when sorting

			myfile.open("file1.txt", ios::in);

			/* count number of lines */
			while (getline(myfile, line))
			{
				nol++;
			}
			myfile.close();
			
			myfile.open("file1.txt", ios::in);
			
			Student get;	// struct temporarily storing the informations
			
			/* store the informations of all members from file1.txt to struct get */
			for (int i = 0; i < nol; i++)
			{
				getline(myfile, word, '\t');
				get.Name = word;

				getline(myfile, word, '\t');
				get.SID = word;

				getline(myfile, word, '\t');
				get.Dept = word;

				getline(myfile, word, '\t');
				get.Byear = word;

				getline(myfile, word, '\n');
				get.Tel = word;

				st.push_back(get);	// push informations to vector
			}
			myfile.close();

			sort(st.begin(), st.end(), SortByName);	// sort by name is default option

			myfile.open("file1.txt", ios::out);

			/* store sorted informations of all members to file1.txt from vector */
			for (int i = 0; i < nol; i++)
			{
				myfile << st[i].Name << '\t';
				myfile << st[i].SID << '\t';
				myfile << st[i].Dept << '\t';
				myfile << st[i].Byear << '\t';
				myfile << st[i].Tel << '\n';
			}
			myfile.close();
		}

		else if (sel == 2)
		{
			int option;	// variable used when we select searching option

			myfile.open("file1.txt", ios::in);

			cout << "- Search -" << endl;
			cout << "1. Search by Name" << endl;
			cout << "2. Search by Student ID (10 numbers)" << endl;
			cout << "3. Search by Admission year (4 numbers)" << endl;
			cout << "4. Search by Department name" << endl;
			cout << "5. List All" << endl;

			cin >> option;	// insert option
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (option == 1)
			{
				cout << "Please insert the name you want to search ";
				getline(cin, search);
				cout << "Name\t\tStudentID\tDept\t\tBirth Year\tTel" << endl;

				/* explore all the contents of the file and output matching values */
				while (getline(myfile, line))
				{
					if (line.find(search) != line.npos)
					{
						cout << line << endl;
					}
				}
			}

			else if (option == 2)
			{
				cout << "Please insert the student ID you want ";
				getline(cin, search);
				cout << "Name\t\tStudentID\tDept\t\tBirth Year\tTel" << endl;

				/* explore all the contents of the file and output matching values */
				while (getline(myfile, line))
				{
					if (line.find(search) != line.npos)
					{
						cout << line << endl;
					}
				}
			}

			else if (option == 3)
			{
				cout << "Please insert the admission year you want ";
				getline(cin, search);
				cout << "Name\t\tStudentID\tDept\t\tBirth Year\tTel" << endl;

				/* explore all the contents of the file and output matching values */
				while (getline(myfile, line))
				{
					if (line.find(search) != line.npos)
					{
						cout << line << endl;
					}
				}
			}

			else if (option == 4)
			{
				cout << "Please insert the department you want ";
				getline(cin, search);
				cout << "Name\t\tStudentID\tDept\t\tBirth Year\tTel" << endl;

				/* explore all the contents of the file and output matching values */
				while (getline(myfile, line))
				{
					if (line.find(search) != line.npos)
					{
						cout << line << endl;
					}
				}
			}
			else if (option == 5)
			{
				cout << "Name\t\tStudentID\tDept\t\tBirth Year\tTel" << endl;

				/* just print all contents in the file1.txt */
				while (getline(myfile, line))
				{
					cout << line << endl;
				}
			}
			myfile.close();
		}

		else if (sel == 3)
		{
			int option;	// variable used when we select searching option
			int nol = 0;	// variable that stores number of lines 

			vector <Student> st;	// vector struct that stores the information from file1.txtand it is used when sorting

			cout << "- Sorting Option -" << endl;
			cout << "1. Sort by Name" << endl;
			cout << "2. Sort by Student ID (10 numbers)" << endl;
			cout << "3. Sort by Admission year (4 numbers)" << endl;
			cout << "4. Sort by Department name" << endl;

			myfile.open("file1.txt", ios::in);
			
			/* count count number of lines */
			while (getline(myfile, line))
			{
				nol++;
			}

			myfile.close();

			myfile.open("file1.txt", ios::in);
			
			Student get;	// struct temporarily storing the informations

			/* store all informations word by word from file.txt to get struct */
			for (int i = 0; i < nol; i++)
			{
				getline(myfile, word, '\t');
				get.Name = word;

				getline(myfile, word, '\t');
				get.SID = word;

				getline(myfile, word, '\t');
				get.Dept = word;

				getline(myfile, word, '\t');
				get.Byear = word;

				getline(myfile, word, '\n');
				get.Tel = word;

				st.push_back(get);	// push informations to vector
			}
			myfile.close();

			cin >> option;	// insert option
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			
			if (option == 1)
			{
				sort(st.begin(), st.end(), SortByName);	// sort the informations in vector by name

				myfile.open("file1.txt", ios::out);
				
				/* store sorted informations of all members to file1.txt from vector */
				for (int i = 0; i < nol; i++)
				{
					myfile << st[i].Name << '\t';
					myfile << st[i].SID << '\t';
					myfile << st[i].Dept << '\t';
					myfile << st[i].Byear << '\t';
					myfile << st[i].Tel << '\n';
				}
				myfile.close();
			}
			else if (option == 2)
			{
				sort(st.begin(), st.end(), SortBySID);

				myfile.open("file1.txt", ios::out);

				/* store sorted informations of all members to file1.txt from vector */
				for (int i = 0; i < nol; i++)
				{
					myfile << st[i].Name << '\t';
					myfile << st[i].SID << '\t';
					myfile << st[i].Dept << '\t';
					myfile << st[i].Byear << '\t';
					myfile << st[i].Tel << '\n';
				}
				myfile.close();
			}
			else if (option == 3)
			{
				sort(st.begin(), st.end(), SortByAdmissionyear);

				myfile.open("file1.txt", ios::out);
				
				/* store sorted informations of all members to file1.txt from vector */
				for (int i = 0; i < nol; i++)
				{
					myfile << st[i].Name << '\t';
					myfile << st[i].SID << '\t';
					myfile << st[i].Dept << '\t';
					myfile << st[i].Byear << '\t';
					myfile << st[i].Tel << '\n';
				}
				myfile.close();
			}
			else if (option == 4)
			{
				sort(st.begin(), st.end(), SortByDept);

				myfile.open("file1.txt", ios::out);
				
				/* store sorted informations of all members to file1.txt from vector */
				for (int i = 0; i < nol; i++)
				{
					myfile << st[i].Name << '\t';
					myfile << st[i].SID << '\t';
					myfile << st[i].Dept << '\t';
					myfile << st[i].Byear << '\t';
					myfile << st[i].Tel << '\n';
				}
				myfile.close();
			}
		}
		else if (sel == 4)
		{
			int nol = 0;	// variable that stores number of lines 
			vector <Student> st;	// vector struct that stores the information from file1.txt and it is used when sorting

			myfile.open("file1.txt", ios::in);

			/* count number of lines */
			while (getline(myfile, line))
			{
				nol++;
			}
			myfile.close();

			myfile.open("file1.txt", ios::in);

			Student get;	// struct temporarily storing the informations

			/* store the informations of all members from file1.txt to struct get */
			for (int i = 0; i < nol; i++)
			{
				getline(myfile, word, '\t');
				get.Name = word;

				getline(myfile, word, '\t');
				get.SID = word;

				getline(myfile, word, '\t');
				get.Dept = word;

				getline(myfile, word, '\t');
				get.Byear = word;

				getline(myfile, word, '\n');
				get.Tel = word;

				st.push_back(get);	// push informations to vector
			}
			myfile.close();

			sort(st.begin(), st.end(), SortByName);	// sort by name is default option

			myfile.open("file1.txt", ios::out);

			/* store sorted informations of all members to file1.txt from vector */
			for (int i = 0; i < nol; i++)
			{
				myfile << st[i].Name << '\t';
				myfile << st[i].SID << '\t';
				myfile << st[i].Dept << '\t';
				myfile << st[i].Byear << '\t';
				myfile << st[i].Tel << '\n';
			}
			myfile.close();
			return 0;
		}
	}
}