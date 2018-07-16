/* @Author
* Student Name:Melih Pekmez
* Student ID : 040140306
* Date: 31.10.2017
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#define nameLength 30
#define surnameLength 30
#define emailLength 75

using namespace std;

struct  studentInfo
{
	char name[nameLength];
	char surname[surnameLength];
	char email[emailLength];
	studentInfo* next;
	studentInfo* pre;
};
struct surnameMap
{
	char surname[surnameLength];
	studentInfo* info = NULL;
	surnameMap* next;
	surnameMap* pre;
	int nodeCount = 0;
};

struct List {
	surnameMap* head;
	surnameMap* last;
	surnameMap* createNode(char*);   // Yeni soyadý için "node" oluþturuyorum
	studentInfo* createStudent(char*, char*);  // Yeni öðrenci için "node" oluþturuyorum
	int recordCount;
	void createList();
	void readFile(); // Dosyayý okuyup, içinde insertAll çaðýrýyorum
	void insertAll(char*, char*);
	void insertNewRecord(); 
	void deleteStudent();
	void deleteSurnameNode(); 
	void updateList();
	void writeToFile();
	string generateMail(studentInfo*); // Otomatik mail oluþturucu
	bool checkMap(char*); // Ayný soyadýndan var mý kontrol ediyor
};
void List::createList()
{
	head = NULL;
	recordCount = 0;
}
void List::insertNewRecord()
{
	char newName[nameLength];
	char newSurname[nameLength];
	cout << endl;
	cout << endl;
	cout << "Please enter the name of new record" << endl;
	cin >> newName;
	cout << "Please enter the surname of new record" << endl;
	cin >> newSurname;
	insertAll(newName, newSurname);
}
void List::readFile()
{
	ifstream inFile("newStudentsList.txt");
	if (inFile.is_open())
	{
		cout << "File opened succesfully..." << endl;
	}

	char name[nameLength] = "";
	char surname[surnameLength] = "";

	while (!inFile.eof())
	{
		inFile >> name;
		inFile >> surname;

		insertAll(name, surname);
	}

	inFile.close();
}
surnameMap* List::createNode(char* surname)
{
	surnameMap *temp = new surnameMap;
	strcpy(temp->surname, surname);
	temp->next = NULL;
	temp->pre = NULL;
	return temp;
}
string List::generateMail(studentInfo* temp)
{
	surnameMap* traverse = head;
	string domain1 = "@itu.edu.tr";
	char *domain = &domain1[0u];
	string email ="";
	char* adress = strcpy(temp->email, temp->surname);
	adress = strcat(temp->email, domain);
	for (size_t i = 0; i <= strlen(adress); i++)
	{
		if (adress[i] >= 65 && adress[i] <= 92)
		{
			adress[i] = adress[i] + 32;
		}
	}
	int z = 1;
	if (checkMap(temp->surname))
	{
		strcpy(temp->email, temp->surname);
		email = strcat(temp->email, domain);
	}
	else
	{
		for (int i = 1; i <= recordCount; i++)
		{
			if (strcmp(temp->surname, traverse->surname) == 0)
			{
				break;
			}
			traverse = traverse->next;
		}
		studentInfo* tail = traverse->info;
		for (int i = 1, k=1; i <= traverse->nodeCount;i++)
		{
			if (strcmp(adress, tail->email) == 0)
			{
				adress = strcpy(temp->email, temp->surname);
				adress = strncat(temp->email, temp->name,k);
				adress = strcat(temp->email, domain);
				for (int i = 0; i <= strlen(adress); i++)
				{
					if (adress[i] >= 65 && adress[i] <= 92)
					{
						adress[i] = adress[i] + 32;
					}
				}
				k++;
				i = 1;
				tail = traverse->info;
			}
			tail = tail->next;
		}
		email = adress;
	} 
	for (size_t i = 0; i <= email.length(); i++)
	{
		if (email[i] >= 65 && email[i] <= 92)
		{
			email[i] = email[i] + 32;
		}
	}
	return email;
}
studentInfo* List::createStudent(char* name, char* surname)
{
	surnameMap *traverse = head;
	studentInfo *cTemp = new studentInfo;
	strcpy(cTemp->name, name);
	strcpy(cTemp->surname, surname);
	cTemp->pre = NULL;
	cTemp->next = NULL;
	return cTemp;
}
bool List::checkMap(char* surname)
{
	surnameMap* traverse = head;
	for (int i = 1; i <= recordCount; i++)
	{
		if (strcmp(surname, traverse->surname) == 0)
		{
			return false;	
		}
		traverse = traverse->next;
	}
	return true;
}
void List::insertAll(char* newName, char* newSurname) // alfabetik sýralamada burada yapýlýyor
{
	surnameMap *traverse = head;
	surnameMap *tail = NULL;
	surnameMap *temp = createNode(newSurname);
	studentInfo *ctemp = createStudent(newName, newSurname);
	string email = generateMail(ctemp);
	char *cstr = &email[0u];
	strcpy(ctemp->email, cstr);
	
	if (checkMap(temp->surname)) // eðer soyadýndan yoksa
	{
		if (head == NULL) 
		{
			head = last = temp;
			head->next = last->next = NULL;
			head->pre = last->pre = NULL;
			head->info = ctemp;
			head->nodeCount++;
			recordCount++;
		}
		else if(strcmp(temp->surname,head->surname)<0) 
		{
			temp->next = head;
			head->pre = temp;
			head = temp;
			head->pre = last;
			last->next = head;
			head->info = ctemp;
			head->nodeCount++;
			recordCount++;
		}
		else
		{
			for (int i = 1; i <= recordCount+1; i++)
			{
				surnameMap* s = traverse;
				traverse = traverse->next;
				if (strcmp(temp->surname, traverse->surname ) < 0)
				{
					s->next = temp;
					temp->pre = s;
					temp->next = traverse;
					traverse->pre = temp;
					temp->info = ctemp;
					temp->nodeCount++;
					recordCount++;
					break;
				}
				else if (traverse == last)
				{
					last->next = temp;
					temp->pre = last;
					last = temp;
					head->pre = last;
					last->next = head;
					last->info = ctemp;
					last->nodeCount++;
					recordCount++;
					break;
				}
			}
		}
	}
	else // eðer soyadý varsa
	{
		for (int i = 1; i <= recordCount; i++)
		{
			if (strcmp(temp->surname, traverse->surname) == 0)
			{
				break;
			}				
			traverse = traverse->next;
		}
		studentInfo* p = traverse->info;
		studentInfo* last = traverse->info;
		for (int i = 1; i <= traverse->nodeCount-1; i++)
		{
			last = last->next;
		}
		if (strcmp(ctemp->email, p->email) < 0)
		{
			p->pre = ctemp;
			ctemp->next = p;
			traverse->info = ctemp;
			traverse->nodeCount++;
		}
		else if(strcmp(ctemp->email,last->email) >0)
		{
			ctemp->pre = last;
			last->next = ctemp;
			traverse->nodeCount++;
		}
		else
		{
			for (int i = 1; i <= traverse->nodeCount; i++)
			{
				if (strcmp(ctemp->email, p->email) < 0)
				{
					break;
				}
				p = p->next;
			}
			ctemp->pre = p->pre;
			(p->pre)->next = ctemp;
			p->pre = ctemp;
			ctemp->next =p;
			traverse->nodeCount++;
		}
	}
}
void List::deleteSurnameNode()
{
	char surname[surnameLength];
	surnameMap* traverse = head;
	surnameMap* p = NULL;
	int index;
	int lastnumber = recordCount;
	cout << endl;
	cout << endl;
	cout << "Which surname do you want delete?" << endl;
	cout << endl;
	cout << "Please enter the surname " << endl;
	cin >> surname;
	for (index = 1; index <= recordCount; index++)
	{
		if (strcmp(surname, traverse->surname) == 0)
		{
			break;
		}
		traverse = traverse->next;
	}
	if (index == 1)
	{
		surnameMap* temp = head;
		studentInfo* remove;
		studentInfo* p = temp->info;
		for (int i = 1; i <= temp->nodeCount; i++)
		{
			remove = p;
			p = p->next;
			delete remove;
		}
		head = head->next;
		head->pre = last;
		last->next = head;
		delete temp;
		recordCount--;
	}
	else if (index == lastnumber)
	{
		surnameMap* temp = last;
		studentInfo* remove;
		studentInfo* p = temp->info;
		for (int i = 1; i <= temp->nodeCount; i++)
		{
			remove = p;
			p = p->next;
			delete remove;
		}
		head->pre = last->pre;
		(last->pre)->next = head;
		last = last->pre;
		delete temp;
		recordCount--;
	}
	else if (index > recordCount)
	{
		cout << "Index is not in finite" << endl;
	}
	else
	{
		surnameMap* temp = head;
		for (int i = 1; i <= index-1; i++)
		{
			temp = temp->next;
		}
		studentInfo* remove;
		studentInfo* p = temp->info;
		for (int i = 1; i <= temp->nodeCount; i++)
		{
			remove = p;
			p = p->next;
			delete remove;
		}
		(temp->next)->pre = temp->pre;
		temp->pre->next = temp->next;
		delete temp;
		recordCount--;
	}
}
void List::deleteStudent()
{
	char student[nameLength];
	cout << endl;
	cout << endl;
	cout <<" Please name of the student "<< endl;
	cin >> student;

	surnameMap* traverse = head;
	studentInfo* t = NULL;
	cout << "These students are found by this given name" << endl;
	cout << "----------------------------------------------" << endl;
	int flag = 0;
	for (int i = 1; i <= recordCount; i++)
	{
		t = traverse->info;
		for (int k = 1; k <= traverse->nodeCount; k++)
		{
			if (strcmp(student, t->name) == 0)
			{
				cout << i << "." << k << ". " << t->name << "  " << t->surname << endl;
				flag++ ;

			}	
			t = t->next;
		}
		traverse = traverse->next;
	}
	if (flag != 0)
	{
		cout << "----------------------------------------------" << endl;
		cout << "Please enter these indexes which you want to delete student" << endl;
		cout << "Please, enter the first index:" << endl;
		int fIndex;
		cin >> fIndex;
		while (fIndex > recordCount)
		{
			cout << "This index is not in finite" << endl;
			cout << "Please enter, again:" << endl;
			cin >> fIndex;
		}
		cout << "Please, enter the second index:" << endl;
		int sIndex;
		cin >> sIndex;



		traverse = head;
		for (int i = 1; i <= fIndex - 1; i++)
		{
			traverse = traverse->next;
		}

		while (sIndex >traverse->nodeCount)
		{
			cout << "This index is not in finite" << endl;
			cout << "Please enter, again:" << endl;
			cin >> sIndex;
		}
		studentInfo* p = traverse->info;

		for (int i = 1; i <= sIndex - 1; i++)
		{
			p = p->next;
		}

		if (sIndex == traverse->nodeCount)
		{
			delete p;
			traverse->nodeCount--;
			cout << "Student is deleted" << endl;
		}
		else if (sIndex == 1)
		{
			studentInfo* temp = p;
			p = p->next;
			traverse->info = p;
			delete temp;
			traverse->nodeCount--;
			cout << "Student is deleted" << endl;
		}
		else
		{
			studentInfo* temp = p;
			(p->pre)->next = p->next;
			(p->next)->pre = p->pre;
			delete temp;
			traverse->nodeCount--;
			cout << "Student is deleted" << endl;
		}
	}
	else
	{
		cout << "Record is not found" << endl;
	}
}
void List::updateList()
{
	char name[nameLength];
	char surname[surnameLength];
	cout << "Please, enter the name of student who you want to update" << endl;
	cin >> name;
	cout << "Please, enter the surname of student who you want to update" << endl;
	cin >> surname;

	surnameMap* traverse = head;
	for (int i = 1; i <= recordCount; i++)
	{
		if (strcmp(surname, traverse->surname) == 0)
		{
			break;
		}
		traverse = traverse->next;
	}

	studentInfo* p = traverse->info;

	cout << endl;
	cout << endl;
	cout << "Please, choose this student by index" << endl;
	for (int i = 1; i <= traverse->nodeCount; i++)
	{
		if (strcmp(p->name, name) == 0)
		{
			cout << i << ". " << p->name << " " << p->surname << " " << p->email << endl;
		}
		p = p->next;
	}
	
	int index;
	cin >> index;
	while (index > traverse->nodeCount)
	{
		cout << "This index is not in finite" << endl;
		cout << "Please enter, again:" << endl;
		cin >> index;
	}
	p = traverse->info;

	for (int i = 1; i <= index-1; i++)
	{
		p = p->next;
	}

	cout << endl;
	cout << endl;

	cout << "Please, enter the new name of student who you want to update" << endl;
	cin >> name;
	cout << "Please, enter the  new surname of student who you want to update" << endl;
	cin >> surname;

	char oldemail[emailLength];
	strcpy(oldemail, p->email);

	if (strcmp(surname, traverse->surname) != 0)
	{
		if (index == traverse->nodeCount)
		{
			delete p;
			traverse->nodeCount--;
			cout << "Student is deleted" << endl;
		}
		else if (index == 1)
		{
			studentInfo* temp = p;
			p = p->next;
			traverse->info = p;
			delete temp;
			traverse->nodeCount--;
			cout << "Student is deleted" << endl;
		}
		else
		{
			studentInfo* temp = p;
			(p->pre)->next = p->next;
			(p->next)->pre = p->pre;
			delete temp;
			traverse->nodeCount--;
			cout << "Student is deleted" << endl;
		}
		traverse = head;
		surnameMap * temp = createNode(surname);
		studentInfo* ctemp = createStudent(name, surname);
		strcpy(ctemp->email, oldemail);
		if (checkMap(temp->surname))
		{
			if (head == NULL)
			{
				head = last = temp;
				head->next = last->next = NULL;
				head->pre = last->pre = NULL;
				head->info = ctemp;
				head->nodeCount++;
				recordCount++;
			}
			else if (strcmp(temp->surname, head->surname)<0)
			{
				temp->next = head;
				head->pre = temp;
				head = temp;
				head->pre = last;
				last->next = head;
				head->info = ctemp;
				head->nodeCount++;
				recordCount++;
			}
			else
			{
				for (int i = 1; i <= recordCount + 1; i++)
				{
					surnameMap* s = traverse;
					traverse = traverse->next;
					if (strcmp(temp->surname, traverse->surname) < 0)
					{
						s->next = temp;
						temp->pre = s;
						temp->next = traverse;
						traverse->pre = temp;
						temp->info = ctemp;
						temp->nodeCount++;
						recordCount++;
						break;
					}
					else if (traverse == last)
					{
						last->next = temp;
						temp->pre = last;
						last = temp;
						head->pre = last;
						last->next = head;
						last->info = ctemp;
						last->nodeCount++;
						recordCount++;
						break;
					}
				}
			}
		}
		else
		{
			for (int i = 1; i <= recordCount; i++)
			{
				if (strcmp(temp->surname, traverse->surname) == 0)
				{
					break;
				}
				traverse = traverse->next;
			}
			studentInfo* p = traverse->info;
			studentInfo* last = traverse->info;
			for (int i = 1; i <= traverse->nodeCount - 1; i++)
			{
				last = last->next;
			}
			if (strcmp(ctemp->email, p->email) < 0)
			{
				p->pre = ctemp;
				ctemp->next = p;
				traverse->info = ctemp;
				traverse->nodeCount++;
			}
			else if (strcmp(ctemp->email, last->email) >0)
			{
				ctemp->pre = last;
				last->next = ctemp;
				traverse->nodeCount++;
			}
			else
			{
				for (int i = 1; i <= traverse->nodeCount; i++)
				{
					if (strcmp(ctemp->email, p->email) < 0)
					{
						break;
					}
					p = p->next;
				}
				ctemp->pre = p->pre;
				(p->pre)->next = ctemp;
				p->pre = ctemp;
				ctemp->next = p;
				traverse->nodeCount++;
			}
		}
	}
	else
	{
		strcpy(p->name, name);
		cout << "Student is updated..." << endl;
	}
}
void List::writeToFile()
{
	ofstream outputFile("emailList.txt");
	surnameMap* traverse = head;
	studentInfo* p = NULL;

	for (int i = 1; i <= recordCount; i++)
	{
		p = traverse->info;
		outputFile << traverse->surname <<endl;
		for (int j = 1; j <= traverse->nodeCount; j++)
		{
			outputFile <<"         " << p->email << "   " << p->name << "   " << p->surname << endl;
			p = p->next;
		}
		traverse = traverse->next;
	}

	outputFile.close();
}
void printMenu()
{
	cout << endl;
	cout << endl;
	cout << endl;
	cout << "e-mail Address Program" << endl;
	cout << "1)   Insert All" << endl;
	cout << "2)   Insert New Record" << endl;
	cout << "3)   Delete Student" << endl;
	cout << "4)   Delete Surname Node" << endl;
	cout << "5)   Update" << endl;
	cout << "6)   Write to File" << endl;
	cout << endl << endl << "Enter a choice (1,2,3,4,5,6) " << endl;
}
int main()
{
	int ch;
	List data;
	data.createList();
	while (1)
	{
		printMenu();
		cin >> ch;
		if (ch == 1)
		{
			data.readFile();
		}
		else if (ch == 2)
		{
			data.insertNewRecord();
		}
		else if (ch == 3)
		{
			data.deleteStudent();
		}
		else if (ch == 4)
		{
			data.deleteSurnameNode();
		}
		else if (ch == 5)
		{
			data.updateList();
		}
		else if (ch == 6)
		{
			data.writeToFile();
		}
		else
		{
			cout << "Wrong choice" << endl;
		}
	}
	getchar();
	getchar();
}
