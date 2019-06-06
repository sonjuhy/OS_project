#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hamberger {
	int material[7];
	int made_time;
}Hamb;
typedef struct process {
	int num;//process number
	int time;//order time
	int hamberger_madetime;
	int order_time;//arrive time
	int BrustTime;
	int remaind;
	int save_point;
	int wait;
	int TAT;//Turnaroundtime
	int stat;//0 : wait, 1 : runnung, 2 : finish, 3: stop
}Order;
typedef struct warehouse {
	int a;
};

int FileIO_fun(Order *order);
int TimeIO_fun(char *time_char);

int main() {
	Order o[50] = { 0, };
	int quntom = 0;
	quntom = FileIO_fun(o);
}
int FileIO_fun(Order *order) {
	FILE *file = 0;
	int buf_size = 0, count = 0, quntom = 0;
	char *inp = 0, *tmp = 0, *time = 0, *type = 0;
	file = fopen("C:\\Users\\SonJunHyeok\\Desktop\\Universty\\os.txt", "r");
	if (file == 0) {
		return 0;
	}

	fseek(file, 0, SEEK_END);
	buf_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	inp = (char*)malloc(sizeof(char)*(buf_size + 1));

	while (fgets(inp, buf_size, file)) {
		if (count == 0) {
			quntom = atoi(&inp[2]);
		}
		else {
			order[count-1].num = atoi(&inp[0]);
			tmp = &inp[2];
			time = strtok(tmp, "\t");
			type = strtok(NULL, "\t");
			if (strcmp("전화", type) == 0) {
				order[count-1].time = 3;
			}
			else {
				order[count-1].time = 5;
			}
			type = strtok(NULL, "\t");
			if (strcmp("새우", type) == 0) {
				order[count-1].hamberger_madetime = 110;
			}
			else {
				order[count-1].hamberger_madetime = 100;
			}
			order[count-1].order_time = TimeIO_fun(time);
		}
		count++;
	}
	return quntom;
}
int TimeIO_fun(char *time_char) {
	int time = 0;
	time = atoi(strtok(time_char, ":"))*100;
	time += atoi(strtok(NULL, " "));

	return time;
}
int RoundRobin_fun(Order *order, int qun) {
	int time = 0,i = 0, j = 0;
	while (1) {
		if (order[i].order_time <= time) {

		}
	}
}