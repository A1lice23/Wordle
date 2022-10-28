#include  "wordle_st.h"
#include  "wordle_ta.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

struct wordList{
    char words[30001][6];
    int length;
};

typedef struct wordList wL;

wL problemList;

void getProblemList(const char *file_path);

int main(void) {
    int rounds = 2;
    int error_code;
    char* file_path = "wordList.txt";
    char* test_word[] = { "cigar"};
    getProblemList(file_path);

    problemList.length = 1;
    for(int i = 0; i < problemList.length; i++){
        strcpy(problemList.words[i], test_word[i]);
    }

    rounds = problemList.length;
    //variables for the time checking
    time_t start, end;
    result_t test_result[rounds];

    init_game_ta(file_path);
    error_code = init_game(file_path);

    if(error_code){
        printf("Initializing Failed: %d\n", error_code);
    }

    for(int i = 0; i < rounds; i++){
        init_round();
        set_question(problemList.words[i]);

        start = clock();
        start_game(RENDER_ON);
        end = clock();

        store_result(&test_result[i], (float)(end-start)/CLOCKS_PER_SEC);
        if(test_result[i].try_cnt == 99){
            printf("%s\n", problemList.words[i]);
        }
    }

    //print result or store result(maybe file I/O)

    return 0;
}

void getProblemList(const char *file_path){
    int wordNum = 0;

    printf("fileopen\n");
    FILE * f = fopen(file_path, "r");
    fscanf(f, "%d", &wordNum);
    problemList.length = wordNum;

    //printf("%d\n", baseWordList.length);

    for(int i = 0; i < wordNum; i++){
        fscanf(f, "%s", problemList.words[i]);
        for(int j = 0; j < 5; j++)
            problemList.words[i][j] = (char) tolower(problemList.words[i][j]);
        problemList.words[i][5] = '\0';
    }
    fclose(f);
}