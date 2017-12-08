#include "project.h"

#define match 5
#define mismatch -4
#define gap -10

matrix arr;

//Use this function in getValue() function
int parseLine(char *line) {
	int i = strlen(line);

	while (*line < '0' || *line > '9')
		line++;

	line[i - 3] = '\0';
	i = atoi(line);
	return i;
}

/*
* To get virtual memory in process
* using parseline function to get memory size
*/
int getValue() {
	ifstream file("/proc/self/status");
	int result = -1;
	char line[128];

	while (file.getline(line, 128) != NULL) {
		if (strncmp(line, "VmSize:", 6) == 0) {
			cout << line << endl;
			result = parseLine(line);
			break;
		}
	}

	file.close();
	return result;
}

/*
* Get File name Function
* using substr so get name in path
*/
string File_Name(string path) {
	int found = path.find_last_of("/");
	int last = path.find_last_of(".");
	string name = path.substr(found + 1, last - found - 1);
	return name;
}

//Initialize vector two dimension with x,y size
matrix Make_Matrix(int x_size, int y_size) {
	for (int i = 0; i < y_size; i++) {
		vector<int> v;
		v.resize(x_size);
		arr.push_back(v);
	}

	for (int i = 0; i < y_size; i++) {
		for (int j = 0; j < x_size; j++) {
			arr[i][j] = 0;
		}
	}
	return arr;
}

//Find Max value in matrix[i][j] with local alignmet algorithm
int Max(int y, int x, bool same) {
	int max_score = 0;

	max_score = max(max_score, arr[y][x - 1] + gap);
	max_score = max(max_score, arr[y - 1][x] + gap);

	if (same) 
		max_score = max(max_score, arr[y - 1][x - 1] + match);
	else 
		max_score = max(max_score, arr[y - 1][x - 1] + mismatch);

	return max_score;
}

/*
* implement Local Alignmet Algorithm
* Using matrix so find max value in alignment
* In this function we get max value that means best sequence
*/
void Local_Align(char *d, char *q, const char *o) {
	int i, j, best_x_loc, best_y_loc, best_score = 0;
	bool same = false;

	ifstream if_db(d);
	vector<char> db; 

	//Read the first line which contain user information
	char cstr[1024];
	if_db.getline(cstr, 1024);

	//Use trash character for indexing clearly
	db.push_back('!');

	char c = if_db.get();

	while (if_db.good()) {
		if (c != '\n' && c != '\r') {
			db.push_back(c);
		}
		c = if_db.get();
	}

	if_db.close();

	
	ifstream if_query(q);
	vector<char> query; 

	//Read the first line which contain user information
	if_query.getline(cstr, 1024);

	//Use trash character for indexing clearly
	query.push_back('!');

	c = if_query.get();

	while (if_query.good()) {
		if (c != '\n' && c != '\r') {
			query.push_back(c);
		}
		c = if_query.get();
	}

	if_query.close();

	Make_Matrix(db.size(), query.size());

	//Start to find best trace with local alignment algorithm
	for (i = 1; i < query.size(); i++) {
		for (j = 1; j < db.size(); j++) {
			if (query.at(i) == db.at(j))
				same = true;

			arr[i][j] = Max(i, j, same);
			
			same = false;

			if (arr[i][j] >= best_score) {
				best_score = arr[i][j];
				best_x_loc = j;
				best_y_loc = i;
			}
		}
	}
	
	getValue();

	vector<char> local_db;
	vector<char> local_query;	
	
	//Start backtracking	
	while (best_score != 0) {
		if (db.at(best_x_loc) == query.at(best_y_loc) || best_score == arr[best_y_loc - 1][best_x_loc - 1] + mismatch) {
			//Match Case or Mismatch Case
			local_db.insert(local_db.begin(), db.at(best_x_loc));
			local_query.insert(local_query.begin(), query.at(best_y_loc));

			best_x_loc -= 1;
			best_y_loc -= 1;
			best_score = arr[best_y_loc][best_x_loc];
		} else if (best_score == arr[best_y_loc - 1][best_x_loc] + gap) {
			//DB Gap Case
			local_db.insert(local_db.begin(), '-');
			local_query.insert(local_query.begin(), query.at(best_y_loc));
			
			best_y_loc -= 1;
			best_score = arr[best_y_loc][best_x_loc];
		} else if (best_score == arr[best_y_loc][best_x_loc - 1] + gap) {
			//Query Gap Case
			local_db.insert(local_db.begin(), db.at(best_x_loc));
			local_query.insert(local_query.begin(), '-');
			
			best_x_loc -= 1;	
			best_score = arr[best_y_loc][best_x_loc];
		}
	}

	//Store the local alignment value in output file
	ofstream of_output(o);

	string db_name = File_Name(d);
	of_output << "DB : " << db_name << ".fa" << endl;

	string query_name = File_Name(q);
	of_output << "Query : " << query_name << ".fa" << endl;
	of_output << endl;

	of_output << "Alignmet" << endl;
	
	vector<char>::iterator it;
	for (it = local_db.begin(); it != local_db.end(); ++it) {
		of_output << *it;
	}

	of_output << "\n" << endl;

	for (it = local_query.begin(); it != local_query.end(); ++it) {
		of_output << *it;
	}
	of_output.close();
}