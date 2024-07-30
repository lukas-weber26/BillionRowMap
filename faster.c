#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct item {
	int n_items;
	char string [200];
	double vals[6];
	int lengths[6]; //may be worth tracking lenghts and indixes
	struct item * overflow; 
} item;

typedef struct map {
	item * s;
} map;

map * create_map() {
	map * m = calloc(sizeof(map), 1);
	m->s = calloc(sizeof(item), 26*26*26);
	return m;
}

int hash(char * line, int line_length) {
	int hash_val = 26*26*((abs(line[0])%41)%26);

	switch (line_length) {
		case (1):
			return hash_val;
		case (2):
			hash_val += 26*((abs(line[1])%61)%26);
			return hash_val;
		default:
			hash_val += 26*((abs(line[1]%61))%26);
			hash_val += ((line[1]%61)%26);
			return hash_val;
	}
}

void map_add(map * m, char * line, int line_length, double val) {
	//unfortunately the new map add is a bit hard.
	int index = hash(line,line_length);	
	item * location = &m->s[index];
	
	//danger, this thing is less initialized than you think.

	int offset = 0;
	int i; 
	search:
		for (i = 0; i < location ->n_items; i++ ) {
			if (line_length == location->lengths[i]) {
				//potential match 
				if (strncmp(line,location->string+offset, location->lengths[i])) {
					//match
					location->vals[i] += val; 
					return;
				}
			}
			offset += location->lengths[i];
		}

		//add to the item or go to the next item.
		//note, this works because if the item you are searching for would fit into the current location, 
		//you have either seen it already or it would be here 
		if (location->n_items < 6 && offset + line_length < 200) {
			location->n_items ++;
			location->lengths[i] = line_length; //use of i here is suss
			location->vals[i] = val; //use of i here is suss
			strncpy(location->string+offset, line, line_length);
			return;
		}

		//if there is a next item, go there.
		if (location ->overflow) {
			location = location->overflow;
			offset = 0;
			i = 0;
			goto search;
		}
		
		//none of the items have what you are looking for. Need a new item.
		item * new = calloc(sizeof(item), 1);
		new ->n_items = 1;
		new ->overflow = NULL;
		new ->lengths[0] = line_length;
		new ->vals[0] = val;
		strncpy(new ->string, line, line_length);
		location -> overflow = new;
		return;
}


int main() {
	map * m = create_map();
	FILE * fd = fopen("./million_rows.txt", "r"); 
	char * line = NULL;
	unsigned long size;
	double total_time;
	int total_rows;
	double start_time;
	double end_time; 
	int measurements = 0;

	while (getline(&line, &size, fd) != -1) {

		//so ideally this speeds things up.... will see.
		int string_length = 1; //skip first because there is definitely a letter there...
		char * end_pointer = line + string_length;
		while (*end_pointer != ';') {
			string_length++;
			end_pointer = line + string_length;
		}
		double string_val = strtod(end_pointer+1,NULL);

		////only for testing!	
		//char print_string[100];
		//strncpy(print_string, line, string_length);
		//print_string[string_length] = '\0';
		//printf("%s: %f\n", print_string, string_val);

		map_add(m,line, string_length, string_val);
		
	}

	//map_print(m);

	printf("Average time per 100 hits: %f\n",total_time/measurements);

	free(line);
	fclose(fd);

}
