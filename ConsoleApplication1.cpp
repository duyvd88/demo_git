#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
struct inputdata
{
	struct tm t;
	char cmd[10], brkey[10], data_hash[20], type[10], reqid[10], state[10];
};
struct outputdata
{
	struct tm t;
	char cmd[10], obj_type[10], obj_data[20], obj_exc_cmd[10], obj_exc_para[10], reqid[10];
};
int demSoDong(const char* file_path) {
	char buff[500];
	FILE* f = fopen(file_path, "r");

	if (f == NULL) {
		printf("Khong the mo file de doc.\n");
		return -1;
	}

	int row = 0;
	while (fgets(buff, 500, (FILE*)f)) {
		row++;
	}

	fclose(f);
	return row;
}
int divtime(struct tm t1, struct tm t2) {
	int div = 0;
	div = (t1.tm_hour - t2.tm_hour);
	return div;
}
void xoaphay(char* s, int length) {
	for (int i = 0; i < length; i++)
	{
		if (*(s + i) == '"') *(s + i) = ' ';
	}
}
void convertbool(char* c)
{
	
	int a = strlen(c);
	for (int i = a - 5; i < a; i++)
	{
		*(c + i) = NULL;
	}
}
void convertreqid(char* c)
{
	int a = strlen(c);
	*(c + a-1) = NULL;
}
void datetime(FILE* f, struct tm t)
{
	char day[10],time[12];
	fseek(f, 7, SEEK_SET);
	fread(day, 10, 1, f);
	sscanf(day, "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
	fread(time, 12, 1, f);
	sscanf(time, "%d:%d:%d", &t.tm_hour, &t.tm_min, &t.tm_sec);
}
void getdataout(FILE* f, char* cmd, char* obj_type, char* obj_data, char* obj_exc_cmd, char* obj_exc_para, char* reqid)
{
	char line[200];
	fscanf(f, "]{%s %s", line, reqid);
	xoaphay(line, strlen(line));
	sscanf(line, " cmd : %s , objects :[{ type : %s , data :[ %s ], execution :{ command : %s , params :{ on : %s }}}], raw : %s , reqid :", cmd, obj_type, obj_data, obj_exc_cmd, obj_exc_para);
	convertbool(obj_exc_para);
	convertreqid(reqid);
}
void getdatain(FILE* f, char* cmd, char* brkey, char* data_hash, char* data_type, char* reqid, char* state)
{
	char line[200];
	fscanf(f, "]{%s %s", line, reqid);
	xoaphay(line, strlen(line));
	convertreqid(reqid);
	sscanf(line, " cmd : %s ,  objects  : [{ bridge_key : %s ,  data  : [{ hash : %s ,  states  : { OnOff :{ on :%s}}}] ,  type  :  %s }] ", cmd, brkey, data_hash, state, data_type);
	convertbool(state);
}
int main()
{
	const char* file = "test.txt";
	FILE* f = fopen(file, "r");
	int sodong = demSoDong(file);
	printf("%d\n", sodong);
	inputdata* input = (inputdata*)malloc(sodong / 2 * sizeof(inputdata));
	outputdata* output = (outputdata*)malloc(sodong / 2 * sizeof(outputdata));
	for (int ii = 0; ii < sodong/2; ii++)
	{
		printf("%d\n", ii);
		datetime(f, output[ii].t);
		getdataout(f, output[ii].cmd, output[ii].obj_type, output[ii].obj_data, output[ii].obj_exc_cmd, output[ii].obj_exc_para, output[ii].reqid);
		datetime(f, input[ii].t);
		getdatain(f, input[ii].cmd, input[ii].brkey, input[ii].data_hash, input[ii].type, input[ii].reqid, input[ii].state);
		printf("%s %s", input[ii].reqid, output[ii].reqid);
	}
	//test
	
}