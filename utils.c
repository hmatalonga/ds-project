#include "utils.h"

void toUpperCase(char *value) {
    while (*value != '\0') {
        if (*value >= 'a' && *value <= 'z')
            *value -= 32;
        value++;
    }
}

void clearScreen() {
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void doPause() {
    printf("%s", PAUSE_MESSAGE);
    getchar();
}

int checkQueryFormat(char *query) {
    int isValid = 0, i = 0, prev = 0;
    while (query[i] != '\0' && query[i] != '\n') {
        if (query[i] == 'C') {
            i++;
            prev = i;
            while (query[i] >= '0' && query[i] <= '9')
                i++;
            if (prev == i) {
                isValid = 0;
                break;
            }
            if (!(query[i] == '+' || query[i] == '-')) {
                isValid = 0;
                break;
            }
            i++;
            prev = i;
            while (query[i] >= '0' && query[i] <= '9')
                i++;
            // Check if grade has less than 2 digits
            if (prev == i || (i-prev) > 2) {
                isValid = 0;
                break;
            }
            isValid = 1;
        }
        else {
            isValid = 0;
            break;
        }
    }
    if ((strcmp(query, "HELP\n") == 0) || (strcmp(query, "?\n") == 0))
        isValid = 2;
    else if (strcmp(query, "EXIT\n") == 0)
        isValid = 3;
    return isValid;
}

int peformQuery(CNODE *tree, char *query, int verbose) {
    unsigned short int course;
    unsigned char grade;
    char operator, *pch = strtok(query, "C");
    unsigned long int totalRec = 0;
    RESLIST *output = NULL, *ret = NULL;
    BLIST *tempGrade = NULL;
    CNODE *courseTree = NULL;

    /**
     * Ler query de pesquisa
     * Aceder a primeira query
     * Loop:
     *   Percorrer Lista e encontrar equals da segunda lista
     *   inserir elementos iguais numa nova lista
     * :endLoop
     * contar elementos da lista de resultados
     * sort lista de resultados
     * output de valores
     */
    
    while (pch != NULL) {
        sscanf(pch, QUERY_FORMAT, &course, &operator, &grade);
        
        courseTree = CNodeLookUp(tree, course);
        
        if (operator == '+') {
            while (grade <= 20) {
                tempGrade = courseTree->hash->table[grade];
                while (tempGrade != NULL) {
                    ret = ResAppend(ret, tempGrade);
                    tempGrade = tempGrade->next;
                }                
                if (verbose)
                    printf("\tUpdating output list...");
                output = ResCopy(output, ret);
                if (verbose)
                    printf(" -> DONE\n");            
                if (verbose)
                    printf("\tClearing memory of auxiliar variables...");          
                ret = ResClearAll(ret); // ret = NULL
                if (verbose)
                    printf(" -> DONE\n");                                 
                grade++;
            }                
        }
        else if(operator == '-') {
            while (grade >= 0) {
                totalRec += courseTree->hash->count[grade];
                if (grade == 0)
                    break;
                grade--;
            }            
            //printf("C%hu < %hu\n", course, grade);
        }
        
        // search for results
        // if course = 0 show all courses
        
        pch = strtok(NULL, "C");
    }
    
    if (verbose)
        printf("DONE\nSorting results...\n");
    output = quickSort(output, getTail(output));
    if (verbose)
        printf("DONE\nRemoving duplicates...\n");
    output = removeDuplicates(output);
    if (verbose)
        printf("DONE\n");
    long int c = ResCount(output);
    printf("%ld\n", c);
    ResPrint(output);
    if (verbose)
        printf("Results count -> %ld\n", c);    
    
    return 0;
}