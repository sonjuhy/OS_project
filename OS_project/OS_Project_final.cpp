#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hamberger {
	int material[7];
	int made_time;
}Hamb;
typedef struct process {
	int num;//process number
	int time_hour;//order time(hour)
	int time_min;//order time(min)
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
int LRU_wareA(int *ware_a, int *ware_a_queue);
int row_count = 0;

int main() {
	Order o[50] = { 0, };
	int ware_a[7] = { 0, };//bread, cheese, pickle, chicken, shrimp, vage, tomato
	int ware_b[7] = { 0, };//same ware_a, display header place
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
				order[count-1].order_time = 3;
			}
			else {
				order[count-1].order_time = 5;
			}
			type = strtok(NULL, "\t");
			if (strcmp("새우", type) == 0) {
				order[count-1].hamberger_madetime = 110;
			}
			else {
				order[count-1].hamberger_madetime = 100;
			}
			order[count - 1].time_hour = atoi(strtok(time, ":"));
			order[count - 1].time_min = atoi(strtok(NULL, " "));
			order[count - 1].remaind = order[count - 1].hamberger_madetime + order[count - 1].order_time;
		}
		count++;
	}
	row_count = count;
	return quntom;
}
int RoundRobin_fun(Order *order, int qun) {
	int time = 0, time_hour = 9, time_min = 0, i = 0, j = 0, finish_count = row_count;
	while (1) {
		if ((order[i].time_hour + order[i].time_min) <= time) {
			for (j = 0; j < qun; j++) {
				order[i].remaind--;
				time_min++;
				if (time_min >= 60) {
					time_hour++;
					time_min = 0;
				}
				time = time_hour * 100 + time_min;
				if (order[i].remaind == 0) {
					order[i].stat = 2;//finish
					order[i].TAT = (time_hour - order[i].time_hour) * 100 + time_min - order[i].time_min;
					finish_count--;
					break;
				}
			}
			if (order[i].stat != 2) {
				order[i].save_point = time;
				order[i].stat = 3;//stop
			}
			i++;
			if (i >= row_count) {
				i = 0;
			}
			if (finish_count == 0) {
				break;
			}
		}
		time_min++;
		if (time_min >= 60) {
			time_hour++;
			time_min = 0;
		}
		time = time_hour * 100 + time_min;
	}
}
int LRU_fun(Order *order, int *ware_a, int ware_b) {
	int ware_a_queue[7] = { 0, }, chieken[5] = { 1,2,3,4,6 }, shrimp[5] = {1,3,5,6,7};
	int disk_time = 0, time  = 0, ware_tmp = 0, ware_sum = 0;
	if (order->hamberger_madetime == 100) {//chicken
		for (int i = 0; i < 5; i++) {
			if (ware_a[chieken[i]] == 0) {
				//diks_time = disk_fun
				LRU_wareA(ware_a, ware_a_queue);
			}
			else {
				ware_a[chieken[i]]--;
				for (int j = 0; i < 6; j++) {
					ware_tmp = ware_a_queue[j + 1];
					ware_a_queue[j+1] = ware_a_queue[0];
					ware_a_queue[0] = ware_tmp;
					if (ware_a_queue[j] == chieken[i]) {
						ware_a_queue[0] = chieken[i];
						break;
					}
				}
			}
		}
	}
	else {//shrimp
		for (int i = 0; i < 5; i++) {
			if (ware_a[shrimp[i]] == 0) {
				//disk_fun
				LRU_wareA(ware_a, ware_a_queue);
			}
			else {
				ware_a[shrimp[i]]--;
				for (int j = 0; i < 6; j++) {
					ware_tmp = ware_a_queue[j + 1];
					ware_a_queue[j + 1] = ware_a_queue[0];
					ware_a_queue[0] = ware_tmp;
					if (ware_a_queue[j] == shrimp[i]) {
						ware_a_queue[0] = shrimp[i];
						break;
					}
				}
			}
		}
	}
	return time;
}
int LRU_wareA(int *ware_a, int *ware_a_queue) {
	int ware_sum = 0;
	for (int j = 0; j < 7; j++) {
		ware_sum += ware_a[j];
	}
	if (ware_sum >= 50) {
		for (int j = 6; j >= 0; j--) {
			if (ware_a[ware_a_queue[j]] != 0) {
				ware_a[ware_a_queue[j]]--;//반납
				break;
			}
		}
	}
}
int Disk_fun(int meterial_num, int *ware_b) {

}