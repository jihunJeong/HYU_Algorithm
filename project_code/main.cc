#include "project.h"

using namespace std;

int main(int argc, char *argv[])
{
	string db_name = File_Name(argv[1]);
	string query_name = File_Name(argv[2]);

	string output_name = db_name + "_" + query_name;
	string path1 = argv[3];
	string output = path1 + "/" + output_name + ".txt";
	const char *path = output.c_str();

	Local_Align(argv[1], argv[2], path);

	return 0;
}