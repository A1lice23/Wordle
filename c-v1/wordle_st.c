#include "wordle_st.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define MAX_IDX 40
#define TRUE 1
#define FALSE 0
#define RENDER_OFF 0
#define RENDER_ON 1
#define ERROR -1
#define WORD_LEN_MAX 5
#define TRY_MAX 6

struct wordList{
    char words[30001][6];
    int length;
};

struct indexList{
    int words[30001];
    int length;
};

struct guessMap{
    int guessResult[30001][30001];
};

struct wordStatus{
    char answer[WORD_LEN_MAX + 1];
    char result[WORD_LEN_MAX + 1];
};

typedef struct wordList wL;
typedef struct indexList iL;
typedef struct guessMap gM;

wL baseWordList;
iL baseIdxList;
gM resultMap;
struct wordStatus results[TRY_MAX];
char* kindOfResult[243] = {"bbbbb", "bbbby", "bbbbg", "bbbyb", "bbbyy", "bbbyg", "bbbgb", "bbbgy", "bbbgg", "bbybb", "bbyby", "bbybg", "bbyyb", "bbyyy", "bbyyg", "bbygb", "bbygy", "bbygg", "bbgbb", "bbgby", "bbgbg", "bbgyb", "bbgyy", "bbgyg", "bbggb", "bbggy", "bbggg", "bybbb", "bybby", "bybbg", "bybyb", "bybyy", "bybyg", "bybgb", "bybgy", "bybgg", "byybb", "byyby", "byybg", "byyyb", "byyyy", "byyyg", "byygb", "byygy", "byygg", "bygbb", "bygby", "bygbg", "bygyb", "bygyy", "bygyg", "byggb", "byggy", "byggg", "bgbbb", "bgbby", "bgbbg", "bgbyb", "bgbyy", "bgbyg", "bgbgb", "bgbgy", "bgbgg", "bgybb", "bgyby", "bgybg", "bgyyb", "bgyyy", "bgyyg", "bgygb", "bgygy", "bgygg", "bggbb", "bggby", "bggbg", "bggyb", "bggyy", "bggyg", "bgggb", "bgggy", "bgggg", "ybbbb", "ybbby", "ybbbg", "ybbyb", "ybbyy", "ybbyg", "ybbgb", "ybbgy", "ybbgg", "ybybb", "ybyby", "ybybg", "ybyyb", "ybyyy", "ybyyg", "ybygb", "ybygy", "ybygg", "ybgbb", "ybgby", "ybgbg", "ybgyb", "ybgyy", "ybgyg", "ybggb", "ybggy", "ybggg", "yybbb", "yybby", "yybbg", "yybyb", "yybyy", "yybyg", "yybgb", "yybgy", "yybgg", "yyybb", "yyyby", "yyybg", "yyyyb", "yyyyy", "yyyyg", "yyygb", "yyygy", "yyygg", "yygbb", "yygby", "yygbg", "yygyb", "yygyy", "yygyg", "yyggb", "yyggy", "yyggg", "ygbbb", "ygbby", "ygbbg", "ygbyb", "ygbyy", "ygbyg", "ygbgb", "ygbgy", "ygbgg", "ygybb", "ygyby", "ygybg", "ygyyb", "ygyyy", "ygyyg", "ygygb", "ygygy", "ygygg", "yggbb", "yggby", "yggbg", "yggyb", "yggyy", "yggyg", "ygggb", "ygggy", "ygggg", "gbbbb", "gbbby", "gbbbg", "gbbyb", "gbbyy", "gbbyg", "gbbgb", "gbbgy", "gbbgg", "gbybb", "gbyby", "gbybg", "gbyyb", "gbyyy", "gbyyg", "gbygb", "gbygy", "gbygg", "gbgbb", "gbgby", "gbgbg", "gbgyb", "gbgyy", "gbgyg", "gbggb", "gbggy", "gbggg", "gybbb", "gybby", "gybbg", "gybyb", "gybyy", "gybyg", "gybgb", "gybgy", "gybgg", "gyybb", "gyyby", "gyybg", "gyyyb", "gyyyy", "gyyyg", "gyygb", "gyygy", "gyygg", "gygbb", "gygby", "gygbg", "gygyb", "gygyy", "gygyg", "gyggb", "gyggy", "gyggg", "ggbbb", "ggbby", "ggbbg", "ggbyb", "ggbyy", "ggbyg", "ggbgb", "ggbgy", "ggbgg", "ggybb", "ggyby", "ggybg", "ggyyb", "ggyyy", "ggyyg", "ggygb", "ggygy", "ggygg", "gggbb", "gggby", "gggbg", "gggyb", "gggyy", "gggyg", "ggggb", "ggggy", "ggggg"};

// TA_function
//int guess_fix(const char *answer, char *result);

double log2(double x);
void render_start_scene();
void render_progress_scene(int try_cnt);
void render_end_scene();
void getWordList(const char *file_path);
int getResultIdx(char* resultWord);
void getResultMap(gM* guessResultMap);
iL* filterWord(int guessWord, int result, iL* possibleWords);
double getFilterWordVolume(int guessWord, int result, iL* possibleWords);
int wordScore(iL* possibleWords, int guessTime);
void solveWordle(iL* possibleWords, int render_option);

// return log2(x) value
double log2(double x){
    // if x is negative number log function can occur error
    if(x > 0){
        // correct case : return log2(x)
        return log(x)/log(2);
    }
    else{
        // error case : return 0
        return 0;
    }
}

void render_start_scene(){
    //rendering
    printf("-----------------\n");
    printf("Wordle game!!\n");
    sleep(1);//delay
}

void render_progress_scene(int try_cnt){
    //rendering
    system("clear");
    printf("┏━━━┳━━━┳━━━┳━━━┳━━━┓\n");
    for(int i = 0 ; i < TRY_MAX; i++){
        printf("┃");
        for(int j = 0; j < WORD_LEN_MAX; j++){
            if(i <= try_cnt){
                printf(" %c ┃", results[i].result[j]);
            }
            else{
                printf("   ┃");
            }
        }
        if(i <= try_cnt) {
            printf(" %s", results[i].answer);
        }
        printf("\n");
        if(i < TRY_MAX-1)
            printf("┣━━━╋━━━╋━━━╋━━━╋━━━┫\n");
    }
    printf("┗━━━┻━━━┻━━━┻━━━┻━━━┛\n");
    fflush(stdout);
    sleep(1);//delay
}

void render_end_scene(){
    //rendering
    sleep(1);//delay
}

// update the baseWordList with lower letter from the file_path
void getWordList(const char *file_path){
    int wordNum = 0;
    // open file
    FILE * f = fopen(file_path, "r");
    fscanf(f, "%d", &wordNum);
    baseWordList.length = wordNum;
    for(int i = 0; i < wordNum; i++){
        fscanf(f, "%s", baseWordList.words[i]);
        // save the word with lower letter
        for(int j = 0; j < WORD_LEN_MAX; j++)
            baseWordList.words[i][j] = (char) tolower(baseWordList.words[i][j]);
        baseWordList.words[i][WORD_LEN_MAX] = '\0';
    }
    fclose(f);
}

// change the result string to result index
int getResultIdx(char* resultWord){
    int idx = 0, level = 1;
    for(int i = 0; i < WORD_LEN_MAX; i++){
        // idx is represented with base 3, b is 0, y is 1, g is 2
        if(resultWord[i] == 'g'){
            idx += level * 2;
        }
        else if(resultWord[i] == 'y'){
            idx += level;
        }
        level *= 3;
    }
    return idx;
}

// get the map of result index with 
void getResultMap(gM* guessResultMap){
    char test_result[5];

    for(int i = 0; i < baseWordList.length; i++){
        for(int j = 0; j < baseWordList.length; j++){
            int checked[5] = {0, };

            for(int m = 0; m < WORD_LEN_MAX; m++){
                test_result[m] = 'b';
            }
            for(int m = 0; m < WORD_LEN_MAX; m++){
                if(baseWordList.words[i][m] == baseWordList.words[j][m]){
                    test_result[m] = 'g';
                    checked[m] = 1;
                }
            }
            for(int m = 0; m < WORD_LEN_MAX; m++){
                if(test_result[m] == 'b'){
                    for(int n = 0; n < WORD_LEN_MAX; n++){
                        if(!checked[n] && baseWordList.words[i][m] == baseWordList.words[j][n]){
                            test_result[m] = 'y';
                            checked[n] = 1;
                            break;
                        }
                    }
                }
            }
            guessResultMap->guessResult[i][j] = getResultIdx(test_result);
        }
    }

}

// remove word that are not correct word
iL* filterWord(int guessWord, int result, iL* possibleWords){
    // check the result of the guess
    // filter word with not used letter
    iL* filteredWords = malloc(sizeof(iL));
    filteredWords->length = 0;

    for(int i = 0; i < possibleWords->length; i++){
        if(resultMap.guessResult[guessWord][possibleWords->words[i]] == result){
            filteredWords->words[filteredWords->length++] = possibleWords->words[i];
        }
    }
    return filteredWords;
}

double getFilterWordVolume(int guessWord, int result, iL* possibleWords){
    double filterWordVolume = 0;
    // check the result of the guess
    for(int i = 0; i < possibleWords->length; i++){
        if(resultMap.guessResult[guessWord][possibleWords->words[i]] == result){
            filterWordVolume++;
        }
    }

    return filterWordVolume;
}

// find the score of word and return the best word index
int wordScore(iL* possibleWords, int guessTime){

    int maxIdx = 0;
    double entropy, maxEntropy = -1000;
    double totalWordLen = possibleWords->length;
    iL* pickList;
    if(guessTime < 6) {
        pickList = &baseIdxList;
    }
    else{
        pickList = possibleWords;
    }
    //printf("round: %d\n", guessTime);
    for (int i = 0; i < pickList->length; i++) {
        entropy = 0;
        for (int j = 0; j < 243; j++) {
            double filterCount = getFilterWordVolume(pickList->words[i], j, possibleWords);
            entropy += filterCount * (-log2(filterCount / totalWordLen));
        }

        if (maxEntropy < entropy) {
            maxEntropy = entropy;
            maxIdx = pickList->words[i];
        }
    }
    return maxIdx;
}

// 돌릴 때 무조건 소문자로 변환하여 입력 받기
void solveWordle(iL* possibleWords, int render_option){

    int bestWordIdx = wordScore(possibleWords, 1);
    // print the first bestWord
    //printf("1\n");
    //printf("%s\n", possibleWords->words[bestWordIdx]);

    int guessWordIdx;
    char guessResult[5];

    // when it is not playing mode
    strcpy(results[0].answer, baseWordList.words[bestWordIdx]);

    // get the guess and result
    int resultState = guess(baseWordList.words[guessWordIdx], guessResult);
    int resultWordIdx = getResultIdx(guessResult);

    //printf("%s\n", resultWord);
    strcpy(results[0].result, kindOfResult[resultWordIdx]);

    if(resultState == ERROR){
        while(resultState == ERROR){
            /*
            scanf("%s", guessWord);
            resultState = guess_fix(guessWord, resultWord);
             */
        }
    }
    if(render_option == RENDER_ON)
        render_progress_scene(0);

    iL* filteredWords;

    int guessLeft = 5;
    while(resultWordIdx == 242 && guessLeft > 0){

        filteredWords = filterWord(bestWordIdx, resultWordIdx, possibleWords);
        free(possibleWords);
        possibleWords = filteredWords;
        /*
        for(int i = 0; i < possibleWords->length; i++)
            printf("%s ", possibleWords->words[i]);
        printf("\n");
        */
        //printf("left_word : %d\n", possibleWords->length);
        //printf("left_word : %d\n", filteredWords->length);
        if(possibleWords->length == 0)
            break;

        //frequencyOfLetter(possibleWords, frequencyDict);
        bestWordIdx = wordScore(possibleWords, TRY_MAX - guessLeft + 1);

        // print the second ~ sixth bestWord
        //printf("%s\n", possibleWords->words[bestWordIdx]);

        strcpy(results[TRY_MAX - guessLeft].answer, baseWordList.words[bestWordIdx]);

        // get the guess and result
        resultState = guess(baseWordList.words[bestWordIdx], guessResult);
        resultWordIdx = getResultIdx(guessResult);
        //printf("%s\n", resultWord);
        strcpy(results[TRY_MAX - guessLeft].result, kindOfResult[resultWordIdx]);

        if(resultState == ERROR){
            while(resultState == ERROR){
                /*
                scanf("%s", guessWord);
                resultState = guess_fix(guessWord, resultWord);
                 */
            }
        }
        if(render_option == RENDER_ON)
            render_progress_scene(TRY_MAX - guessLeft);

        guessLeft--;
    }
    if(possibleWords->length == 0){
        // error!! result of the guess was wrong in some input
        //printf("ERROR IN THE RESULT\n");
    }
    else if(resultWordIdx == 242 && guessLeft == 0){
        // failed the guess in 6 times
        //printf("RUN OUT OF TIME\n");
    }
    else{
        // solve the wordle
        //printf("SOLVE!!!\n");
    }
}

// 돌릴 때 무조건 소문자로 변환하여 입력 받기
int init_game(const char *file_path) {
    getWordList(file_path);
    getResultMap(&resultMap);
    for(int i = 0; i < baseWordList.length; i++){
        baseIdxList.words[i] = i;
    }
    return 0;
}

void start_game(int render_option){
    iL* possibleWords = malloc(sizeof(iL));

    // initialize the possibleWords List
    possibleWords->length = baseWordList.length;
    for(int i = 0; i < baseWordList.length; i++){
        possibleWords->words[i] = i;
        //strcpy(possibleWords->words[i], baseWordList.words[i]);
    }
    if(render_option == RENDER_ON){
        //sleep(3);//delay
        render_start_scene();
    }

    solveWordle(possibleWords, render_option);

    if(render_option == RENDER_ON)
        render_end_scene();
}