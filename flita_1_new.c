#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void put(char* str);

typedef struct node {
    char* data;
    struct node* next;
} Node;

typedef struct sets {
    char* name;
    Node* set;
    struct sets* next;
} Sets;

char* get_str() {
    char symb;
    char* string = (char*) calloc(0, sizeof(char));
    do {            
        for (uint64_t j = 0; (symb = getchar()) != '\n'; ++j) {
            if (!(j % 10)) {
                string = (char*) realloc(string, (j + 11) * sizeof(char));
            }
            string[j] = symb;
            string[j + 1] = '\0';
        }
        if (!*string) {
            puts("ERROR: NULL VALUE. Please, repeat the input");
        }
    } while (!*string);
    return string;
}

void put(char* str) {
    while (*str) {
        putchar(*str++);
    }
}

bool equivalent(char* first, char* second) {
    while (*first++ == *second++) { //если попадаются разные символы или если первая или вторая строка короче другой, то цикл завершается
        if (*(first - 1) == '\0') { //если строки одинаковой длины и все предыдущие символы одиноковы, то возвращает true из функции
            return true;
        }
    } 
    return false;
}

void print_set_elements(Node* head) {
    if (head == NULL) {
        puts("empty");
    } 
    else {
        while (head != NULL) {
            printf("%s", head -> data);
            head = head -> next;
            if (head != NULL) {
                put(", ");
            }
        }
        puts(".");
    }
}

Node* add_value(Node* head, char* value) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node -> data = value;
    new_node -> next = NULL;
    if (head == NULL) {
        head = new_node;
    } 
    else if (equivalent(head -> data, value)) {
        puts("ERROR: THIS ELEMENT IS ALREADY IN THIS SET");
        free(new_node);
    }
    else {
        Node* current = head;
        while (current -> next != NULL) {
            current = current -> next;
            if (equivalent(current -> data, value)) {
                puts("ERROR: THIS ELEMENT IS ALREADY IN THIS SET");
                free(new_node);
                return head;
            }
        }
        current -> next = new_node;
    }
    return head;
}

Node* remove_value(Node *head, char* value) {
    if (head == NULL) {
        return head;
    }
    Node* current = head;
    Node* prev = NULL;
    while (current != NULL) {
        if (equivalent(current -> data, value)) {
            if (prev != NULL) {
                prev -> next = current -> next;
            }
            else {
                head = current -> next;
            } 
            free(current);
            printf("Element %s deleted from this set\n", value);
            return head;
        }
        prev = current;
        current = current -> next;
    }
    puts("This element not in set");
    return head;
}

uint64_t count_input(void) {
    char number;
    bool lever;
    uint64_t count;
    uint64_t limit = 10000000000000000000ULL; // = 10 ^ 20
    do {
        count = 0;
        lever = true;
        while ((number = getchar()) != '\n') {
            count = (count * 10) + (number - '0');
            if (number < '0' || number > '9') {
                if (lever) {
                    put("ERROR: INCORRECT VALUE\nInput count again: ");
                }
                lever = false;
            }
        }
        if (count >= limit) {
            put("ERROR: INCORRECT VALUE\nInput count again: ");
            lever = false;
        }
    } while (!lever);
    return count;
}

bool check_name(Sets* sets, char* name) {
    if (equivalent(name, "exit")) {
        return true;
    }
	while (sets != NULL) {
		if (equivalent(sets -> name, name)) {
			return true;
		}
		sets = sets -> next;
	}
	return false;
}

char* get_name(Sets* sets) {
    puts("Input name of set");
    char* name = get_str();
    while (!check_name(sets, name)) {
    	puts("ERROR: Unknown name");
    	free(name);
    	name = get_str();
    }
    return name;
}

Node* void_set(Node* set_elem) {
    while (set_elem -> next != NULL) {
        Node* set_elem_next = set_elem -> next;
        free(set_elem);
        set_elem = set_elem_next; 
    }
    free(set_elem);
    return NULL;
}

Sets* get_copy(Sets* sets) {
    char* name = get_name(sets);
    if (equivalent(name, "exit")) {
        free(name);
        return NULL;
    }
    Sets* copy = sets;
    while (!equivalent(copy -> name, name)) {
    	copy = copy -> next;
    }
    free(name);
    return copy;
}

Sets* remove_elements(Sets* sets) {
    if (sets == NULL) {
	    puts("First create set");
		return sets;
	}
    char* escape = {"exit"};
    Sets* copy = get_copy(sets);
    if (copy == NULL) {
        return sets;
    }
    if (copy -> set == NULL) {
        puts("This set is void");
        return sets;
    }
    put("Input count of deleting elements: ");
    uint64_t count = count_input();
    for (uint64_t i = 0; i < count; ++i) {       
        puts("Input your element:");       
        char* value = get_str();
        if (equivalent(value, escape)) {
            puts("Cycle was breaked");
            break;
        }
        copy -> set = remove_value(copy -> set, value);
        if (copy -> set == NULL) {
            break;
        }    
        free(value);
    }
    return sets;
}

Sets* print_names(Sets* sets) {
	if (sets == NULL) {
		puts("First create set");
		return sets;
	}
	Sets* copy = sets;
	while (copy != NULL) {
		put(copy -> name);
		if (copy -> next != NULL) {
			put(", ");
		}
		copy = copy -> next;
	}
	puts(".");
	return sets;
}

Sets* add_set (Sets* sets) {
    puts("Input name of a future set");
    char* name = get_str();
    while (check_name(sets, name)) {
        if (equivalent(name, "exit")) {
            free(name);
            return sets;
        }
        free(name);
        puts("This name is used. Input free name or 'exit' to leave");
        char* name = get_str();
    }
    Sets* new_set = (Sets*) malloc(sizeof(Sets));
    new_set -> name = name;
    if (sets == NULL) {
    	return new_set;
    }
    Sets* set = sets;
    while (set -> next != NULL) {
        set = set -> next;      
    }
    set -> next = new_set;
    return sets;
}

Sets* delete_set(Sets* sets) {
    if (sets == NULL) {
		puts("First create set");
		return sets;
	}
    char* name = get_name(sets);
    Sets* deleting_set = sets -> next;
    if (equivalent(sets -> name, name)) {
    	if (sets -> set != NULL) {
    		sets -> set = void_set(sets -> set);
    	}
        free(sets -> name);
    	free(sets);
        free(name);
    	return deleting_set;
    }
    Sets* prev_set = sets;
    while (!equivalent(deleting_set -> name, name)) {
        prev_set = deleting_set;
        deleting_set = deleting_set -> next;
    }
    if (sets -> set != NULL) {
    	deleting_set -> set = void_set(deleting_set -> set);
    }
    prev_set -> next = deleting_set -> next;
    free(deleting_set -> name);
    free(deleting_set);
    free(name);
    return sets;
}

Sets* print_all (Sets* sets) {
	if (sets == NULL) {
		puts("First create set");
		return sets;
	}
	Sets* copy = sets;
	while (copy != NULL) {
		put(copy -> name);
		put(": ");
		print_set_elements(copy -> set);
		copy = copy -> next;
	}
	return sets;
}

Sets* input_elements(Sets* sets) {
    if (sets == NULL) {
		puts("First create set");
		return sets;
	}
    Sets* copy = get_copy(sets);
    if (copy == NULL) {
        return sets;
    }
    char* escape = {"exit"};
    put("Input count of new elements: ");
    uint64_t count = count_input();
    for (uint64_t i = 0; i < count; ++i) {       
        puts("Input your element:");       
        char* value = get_str();
        if (equivalent(value, escape)) {
            puts("Cycle was breaked");
            free(value);
            break;
        }        
        copy -> set = add_value(copy -> set, value);
    }
    return sets;
}

Sets* free_set(Sets* sets) {
    if (sets == NULL) {
		puts("First create set");
		return sets;
	}
    Sets* copy = get_copy(sets);
    if (copy == NULL) {
        return sets;
    }
    copy -> set = void_set(copy -> set);
    return sets;
}

Sets* print_set(Sets* sets) {
    if (sets == NULL) {
		puts("First create set");
		return sets;
	}
    Sets* copy = get_copy(sets);
    if (copy == NULL) {
        return sets;
    }
    put(copy -> name);
    put(": ");
    print_set_elements(copy -> set);
    return sets;
}

typedef struct definitions {
	char* key;
	Sets* (*func)(Sets*);
} Definitions;

int def_count(Definitions pointer[]) {
    int i;
    for (i = 0; pointer[i].key != NULL; i++);
    return i; 
}

void help(Definitions def[], int count) {
	char* description[] = {
		"print all names of sets",
		"delete set with name",
		"create a set",
		"print all sets and they names",
		"add elements in set",
		"delete set without name",
		"remove elements from set",
        "print all elements of one set"
	};
	for (int i = 0; i < count; i++){
		printf("%s: %s\n", def[i].key, description[i]);
	}
    puts("exit: exin on every step");
}

bool stop() {
	char confirm[2] = {'\0'};
	char symb;
    puts("Y/n?");
	confirm[0] = getchar();
	while ((symb = getchar()) != '\n') {
		confirm[1] = symb;
	}
	if (equivalent(confirm, "Y")) {
		return true;
	}
	return false;
}

void handling(Sets* sets) {
	Definitions def[] = {
    		{"names", print_names},
	    	{"delete", delete_set},
    		{"create", add_set},
    		{"print all", print_all},
            {"add elem", input_elements},
            {"void set", free_set},
            {"remove", remove_elements},
            {"print set", print_set},
            {NULL, NULL}
    };
    int count = def_count(def);
	while (true) {
	    puts("Handling. Print 'help' to see commands");     
	    char* action = get_str();
    	for (uint8_t i = 0; i < count; i++) {
    		if (equivalent(def[i].key, action)) {
    			sets = def[i].func(sets);
	    	}
    	}
    	if (equivalent(action, "exit")) {
	    	if (stop()) {
	    		free(action);
	    		break;
	    	}
	    }
	    else if (equivalent("help", action)) {
	    	help(def, count);
	    }
	    free(action);
	}
	return;
}

int main(void) {
    Sets* sets = NULL;
    (handling(sets));
    return 0;
}