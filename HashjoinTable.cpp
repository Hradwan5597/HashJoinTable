#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

using db_table = vector<vector<int>>;
static int MIN_SIZE = 1;
static int MAX_BUCKETS = 5; //N in f(k)= k mod N

void printTable(db_table table) 
{
	for (int i = 0; i < table.size(); i++) 
	{
		for (int j = 0; j < table[i].size(); j++) 
		{
			cout << table[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

db_table getTableFromFile(string fileName) 
{
	//Populate table from file 
	db_table table;
	int cNum;
	string cLine;

	ifstream ifs;
	ifs.open(fileName);
	int i = 0;
	while (getline(ifs, cLine)) 
	{
		istringstream stringstrm(cLine);
		table.push_back({});

		while (stringstrm >> cNum) 
		{
			table[i].push_back(cNum);
		}
		i++;
	}

	return table;
}

db_table hashJoin(db_table tableA, db_table tableB, int a1, int a2) 
{
	// result table
	db_table result = tableA;

	//For each attribute in tableA at a1 hash into an array
	multimap<int, int> m;
	pair<int, int> p;

	int hashVal;
	int selectivity = 0; //increment for each tuple satisfying F

	//Creating the hash table for table 2 to search for elements in table 1
	for (int i = 0; i < tableA.size(); i++) 
	{
		hashVal = tableA[i][a1] % MAX_BUCKETS; // Hash function f(k)= k mod N
		p = { hashVal, i };
		m.insert(p); // insert value into m
	}

	for (int i = 0; i < tableB.size(); i++) 
	{
		hashVal = tableB[i][a2] % MAX_BUCKETS;
		//checks if empty
		if (m.count(hashVal) > 0) 
		{
			auto range = m.equal_range(hashVal);
			//iterate through range and add this row to result row
			for (auto r = range.first; r != range.second; r++) 
			{
				int currRow = (*r).second;
				result[currRow].insert(result[currRow].end(), tableB[i].begin(), tableB[i].end());
				selectivity++; //increment for each satisfaction of f
			}
		}
	}

	cout << "Selectivity = " << selectivity << "/" << (tableA.size() * tableB.size()) << endl;

	return result;
}


int main() {

	int atrbA, atrbB;// joining attributes provided by user input

	string fileNameA, fileNameB;
	db_table tableA, tableB;
	db_table result;

	//User filename inputs
	cout << "Enter the first file for data. (example fileA.txt)" << endl;
	cin >> fileNameA;
	cout << "Enter the second file for data. (example fileA.txt)" << endl;
	cin >> fileNameB;

	//Getting tables from files
	tableA = getTableFromFile(fileNameA);
	tableB = getTableFromFile(fileNameB);

	cout << "Enter the first attribute *as an int* from table 1 to join with table 2:(ex: 1)" << endl;
	printf("(min: %d, max: %d)\n", MIN_SIZE, tableA.size());
	cin >> atrbA;

	cout << "Enter the first attribute *as an int* from table 2 to join with table 1:(ex. 4)" << endl;
	printf("(min: %d, max: %d)\n", MIN_SIZE, tableB.size());
	cin >> atrbB;

	cout << endl << "Joining R1 at attribute " << atrbA << endl;
	printTable(tableA);
	cout << "with R2 at attribute " << atrbB << endl;
	printTable(tableB);


	result = hashJoin(tableA, tableB, atrbA - 1, atrbB - 1);

	printTable(result);

	system("pause");
	return 0;
}