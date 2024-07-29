//Explenation: This is the simplest real hash map. It calculates an index based on the first three letters in the city name.
//The resulting speed is 0.000018 for one hundred additions on one and 10 million rows. This is roughly speaking a third to a fourth of the time of doing a real look-up.
//Unclear how to improve.

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
		index += pow(27,count)*((tolower(*line)-61)%27);
		count --;	
		line = line+1;
	}
	return index;
}

void map_add(map * m, char * line, double val) {
	item * current;
	int index = hash(line);

	item * location = &(m->s[index]);

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
