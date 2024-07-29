//Explenation: This is the simplest possible map implementation. It simply uses an array of pointers to structs.
//Surprisingly, this is now the slowest method availiable. Times for 100 hits at 1 to 10 million rows are about 0.000068.

typedef struct item {
	char * string;
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
	m->s = calloc(len,sizeof(char *));
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
	current->string = malloc(sizeof(char) * (1+line_length));
	strcpy(current->string, line);
	current->string[line_length] = '\0';
	current->val = val;
	m->current_len ++;
}

