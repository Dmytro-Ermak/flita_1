#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct moves {
    char* move;
    char* num;
} Moves;

Moves* scan_not_f(Moves* action) {
    char symb;    
    char* move = (char*) malloc(sizeof(char));
    char* num = (char*) malloc(sizeof(char));
    for (uint64_t i = 0; (symb = getchar()) != ' '; ++i) {
        if (!(i % 10)) {
            move = (char*) realloc(move, (i + 11) * sizeof(char));
        }
        (move)[i] = symb;
        (move)[i + 1] = '\0';
        if (symb == '\n') {
            action -> move = move;
            action -> num = "f\0";
            free(move);
            return action;
        }
    }
    for (uint64_t i = 0; (symb = getchar()) != '\n'; ++i) {
        num = (char*) realloc(num, (i + 2) * sizeof(char));
        (num)[i] = symb;
        (num)[i + 1] = '\0';
    }
    action -> move = move;
    action -> num = num;
    free(move);
    free(num);
    return action;
}

typedef struct node {
    char* data;
    struct node* next;
} Node;

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

void print_set(Node* head) {
    put("Current set: ");
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

bool check_value(Node* head, char* value) {     //не используется, может быть использована для проверки наличия элемента в списке
    if (head == NULL) {
        return false;
    }
    while (head != NULL) {
        if (equivalent(head -> data, value)) {
            return true;
        }
        head = head -> next;
    }
    return false;
}

Node* add_value(Node* head, char* value) {
    if (*value == '\0'){
        puts("ERROR: VOID ELEMENT");
        return head;
    }
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node -> data = value;
    new_node -> next = NULL;
    if (head == NULL) {
        head = new_node;
    } 
    else if (equivalent(head -> data, value)) {
        puts("ERROR: THIS ELEMENT IS ALREADY IN THIS SET");
        free(new_node);
        return head;
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

Node* input_output(Node* set, bool output) {
    char symb;
    char* escape = {"exit, please!\0"};
    uint64_t count = count_input();
    for (uint64_t i = 0; i < count; ++i) { 
        char* value = (char*) calloc(0, sizeof(char));      
        puts("Input your element:");       
        for (uint64_t j = 0; (symb = getchar()) != '\n'; ++j) {
            if (!(j % 10)) {
                value = (char*) realloc(value, (j + 12) * sizeof(char));
            }
            value[j] = symb;
            value[j + 1] = '\0';
        }
        if (equivalent(value, escape)) {
            puts("Cycle was breaked");
            break;
        }
        if (output) {
            set = remove_value(set, value);
            if (set == NULL) {
                break;
            }  
        }
        else {         
            set = add_value(set, value);
        }
        value = NULL;
    }
    print_set(set);
    return set;
}

int main(void) {
    char* value;
    Node* set_1 = NULL;
    Node* set_2 = NULL;
    char* exit = {"exit"};
    char* del = {"delete"};
    char* print = {"print"};
    char* input = {"input"};
    char* num2 = {"2"};
    char* num1 = {"1"};
    Moves* action = (Moves*) malloc(sizeof(Moves));
    put("Enter count of 1st set elements no more than 10^20-1: ");   
    set_1 = input_output(set_1, false);
    put("Enter count of 2st set elements no more than 10^20-1: ");   
    set_2 = input_output(set_2, false);
    do {
        puts("Enter option with set(delete, input, print) and number of set (1 or 2) after space or \"exit exit\" to finish the program");
        action = scan_not_f(action);
        char* move = action -> move;
        char* number = action -> num;
        if (equivalent(move, input)) {
            if (equivalent(number, num1)) {
                put("Enter count of elements that will be inputed no more than 10^10-1: ");
                set_1 = input_output(set_1, false);
            }
            else if (equivalent(number, num2)) {
                put("Enter count of elements that will be inputed no more than 10^10-1: ");
                set_2 = input_output(set_2, false);        
            }
            else{
               puts("ERROR: INCORRECT ACTION"); 
            }
        }
        else if (equivalent(move, print)) {
            if (equivalent(number, num1)) {
                print_set(set_1);
            }
            else if (equivalent(number, num2)) {
                print_set(set_2);        
            }
            else{
               puts("ERROR: INCORRECT ACTION"); 
            }
        }
        else if (equivalent(move, del)) {
            if (equivalent(number, num1)) {
                if (set_1 == NULL) {
                    puts("This set is empty");
                    continue;
                }
                put("Enter count of elements no more than 10^10-1 that will be deleted: ");
                set_1 = input_output(set_1, true);
            }
            else if (equivalent(number, num2)) {
                if (set_2 == NULL) {
                    puts("This set is empty");
                    continue;
                }
                put("Enter count of elements no more than 10^10-1 that will be deleted: ");
                set_2 = input_output(set_2, true);        
            }
            else{
               puts("ERROR: INCORRECT ACTION"); 
            }
        }
        else if (!(equivalent(move, exit) && equivalent(number, exit))) {
            puts("ERROR: INCORRECT ACTION");
        }
    } while (!(equivalent(action -> move, exit) && equivalent(action -> num, exit)));
    return 0;
}