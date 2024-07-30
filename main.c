#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct item {
	char * string;
	double val;
	struct item * next_item;
} item;

typedef struct map {
	item * s;
	int len;
} map;

map * create_map(int len) {
	map * m = malloc(sizeof(map));
	m->len = 27*27*27;
	m->s = calloc(27*27*27,sizeof(item));
	return m;
}

int hash(char * line) {
	int count = 2;
	int index = 0;
	while (*line != '\0' && count >= 0)  {
		index += pow(27,count)*((tolower(*line)%61)%27);
		count --;	
		line = line+1;
	}
	return index;
}

void map_add(map * m, char * line, double val) {
	item * current;
	int index = hash(line);

	item * location = &(m->s[index]);
	int dephth = 0;

	if (location->string != 0) {
		while (location) {
			if (strcmp(location->string, line) == 0) {
				location ->val += val;
				return;
			}

			if (location ->next_item == NULL) {
				break;
			}

			location = location -> next_item;	
			//dephth ++;
			//printf("%d\n", dephth);
		}

		location ->next_item = malloc(sizeof(item));
		location= location -> next_item;

		int string_size = strlen(line);
		location -> string = malloc(sizeof(char) * (1+string_size));
		strcpy(location -> string, line);
		location->string[string_size] = '\0';
		location ->next_item = NULL;
		location ->val = val;
		return;
		
	}

	int string_size = strlen(line);
	location -> string = malloc(sizeof(char) * (1+string_size));
	strcpy(location -> string, line);
	location->string[string_size] = '\0';
	location ->next_item = NULL;
	location ->val = val;

}


int main() {
	map * m = create_map(10000);
	FILE * fd = fopen("./million_rows.txt", "r"); 
	char * line = NULL;
	unsigned long size;
	double total_time;
	int total_rows;
	double start_time;
	double end_time; 
	int measurements = 0;

	while (getline(&line, &size, fd) != -1) {
		//parse the line 
		//if (total_rows % 100 == 0) { 
		//	if (total_rows !=0) {
		//		end_time = (double)clock()/CLOCKS_PER_SEC;
		//		total_time += end_time - start_time;
		//		measurements += 1;
		//		start_time = (double)clock()/CLOCKS_PER_SEC;
		//	} else {
		//		start_time = (double)clock()/CLOCKS_PER_SEC;
		//	}
		//}

		char * end = strchr(line,';');
		double string_val = strtod(end+1,NULL);
		int num_length = strlen(end);
		int string_length = strlen(line) - num_length; 
		line[string_length] = '\0';	
		
		//add to map and time
		//rules: Must allocate memory for the string

		map_add(m,line,string_val);

		//total_rows ++;
		
		//print logic: needed because the hash has to figure out its own string allocation 
		//printf("%s: %f\n", line, string_val);
		
	}

	//map_print(m);

	printf("Average time per 100 hits: %f\n",total_time/measurements);

	free(line);
	fclose(fd);

}
