//Explenation: This version of the simplest map removes a pointer to the target string and replaces it with an array. In thoeory this could increase cache locality.
//Runs at 0.000073 per 100 inserts for 1 to 10 million rows.
//This is a slight perfromance reduction surprisingly.

typedef struct item {
	char string[100];
	double val;
} item;

typedef struct map {
	item * s;
	int len;
	int current_len;
} map;

map * create_map(int len) {
	map * m = malloc(sizeof(map));
	m->len = len;
	m->current_len= 0;
	m->s = calloc(len,sizeof(item));
	return m;
}

void map_add (map * m, char * line, double val) {
	item * current;

	for (int i = 0; i < m->current_len; i++) {
		current = &(m->s[i]);	

		if (strcmp(line, current->string) == 0) {
			current->val += val;
			return;
		}

	}
	
	current = &(m->s[m->current_len]);	
	int line_length = strlen(line);
	strcpy(current->string, line);
	current->string[line_length] = '\0';
	current->val = val;
	m->current_len ++;
}

void map_print(map * m) {
	FILE * out = fopen("./Output_2", "w");
	
	item * current;

	for (int i = 0; i < m->current_len; i++) {
		current = &(m->s[i]);	
		fprintf(out,"%s: %f\n", current->string, current->val);
	}

	fclose(out);
}
