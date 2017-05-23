#include "dic.h"
#include "debug.h"
#include "mispelling.h"

bool symlink_exists(const char* path)
{
    struct stat buf;
    int result;

    result = lstat(path, &buf);

    return (result == 0);
}

bool test(const char* path)
{
    bool exists = symlink_exists(path);

    return exists;
    // if(exists){
    //     printf("1. %s does%s exist.\n", path, exists ? "" : " not");
    // }else{
    //     printf("2. %s does%s exist.\n", path, exists ? "" : " not");
    // }

    //printf("%s does%s exist.\n", path, exists ? "" : " not");
}


void processDictionary(FILE* f){
    //printf("[ processDictionary function ]\n");
    dict->num_words = 0;

    while(!feof(f))
    {
        //initialize the current word.

        struct dict_word* currWord;
        if((currWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
        {
            printf("OUT OF MEMORY.\n");
            return;
        }
        currWord->num_misspellings = 0;
        currWord->misspelled_count = 0;

        //variables
        char word[MAX_SIZE];
        char* wdPtr = word;
        char line[MAX_SIZE];
        char* character = line;
        //char word_list[MAX_MISSPELLED_WORDS+1][MAX_SIZE];
        int counter = 0;
        int firstWord = 1;

        *character = 0x00;
        character = line;




        void* newLine = fgets(line, MAX_SIZE+1, f);

        if(newLine == NULL){
            free(currWord);
            break;
        }

        //printf("EMPTY LINE. *character (%d),  *(character+1) (%d)\n",*(character+1),*(character));
        if(*(character+1) == 0 && (*character == '\n' || *character == ' ')){
            free(currWord);
            break;
        }

        //if there isn't a space at the end of the line, put one there
        if((line[strlen(line)-2] != ' ' && line[strlen(line)-1] == '\n') || (line[strlen(line)-1] != ' ' && line[strlen(line)-1] != '\n'))
            strcat(line, " ");

        while(*character != 0x00) // error NULL, comparison between pointer and integer
        {
            if(counter >= MAX_MISSPELLED_WORDS+1)
                break;
            //if the character is a space, add the word in word_list and make word NULL.
            if(*character == ' ')
            {
                *wdPtr = 0X00; // error NULL, assignment makes integer from pointer without a cast
                wdPtr = word;


                if(firstWord)
                {
                    addWord(currWord, wdPtr);
                    dict->num_words++;

                    firstWord = 0;
                }
                else
                {
                    struct misspelled_word* currMisspelling;
                    if((currMisspelling = malloc(sizeof(struct misspelled_word))) == NULL)
                    {
                        printf("ERROR: OUT OF MEMORY.");
                        return;
                    }

                    currWord = addMisspelledWord(currMisspelling, currWord, wdPtr);
                    //free(currMisspelling);
                }
            }
            //if the character isn't a space or a new line, add the character to word.
            else if(*character != '\n')
                *(wdPtr++) = *character;
            character++;
        }

    }


}

void addWord(struct dict_word* dWord, char* word){

    //dWord = malloc(sizeof(dWord));
    //dWord = (struct dict_word*) malloc(sizeof(struct dict_word));
    //setting up dWord fields
    strcpy(dWord->word, word);
    dWord->misspelled_count = 0;
    dWord->num_misspellings = 0;
    dWord->misspelled[MAX_MISSPELLED_WORDS] = NULL;
    dWord->next = dict->word_list;

    dict->word_list = dWord;

    // char word[WORDLENGTH];
    // int misspelled_count; // initialize to 0
    // int num_misspellings; // initialize to 0
    // struct misspelled_word* misspelled[MAX_MISSPELLED_WORDS];
    // struct dict_word* next;
}


void freeWords(struct dict_word* currWord){
    if(currWord != NULL)
    {
        freeWords(currWord);

        // int i;
        //free word
        printf("FREED %s\n", currWord->word);
        free(currWord);
    }
}

void printWords(struct dict_word* currWord, FILE* f){
    if(currWord != NULL)
    {
        struct dict_word* nextWord = currWord->next;
        //printf("currWord: %s,  nextWord->word : %s\n", currWord->word, nextWord->word);
        //printf("currWord: %s\n", currWord->word);

        if(nextWord != NULL ){
            printWords(currWord->next, f);
        }else{
            return;
        }

        char line[MAX_SIZE];
        //int i;

        sprintf(line, "%s\n", currWord->word);
        fwrite(line, strlen(line)+1, 1, f);

        //sprintf(line, "\tNUMBER OF TIMES WORD IS MISSPELLED: %d\n", currWord->misspelled_count); // puts string into buffer
        //fwrite(line, strlen(line)+1, 1, f);

        //sprintf(line, "\tNUMBER OF MISSPELLINGS: %d\n", currWord->num_misspellings);
        //fwrite(line, strlen(line)+1, 1, f);

        // for(i = 0; i<currWord->num_misspellings; i++)
        // {
        //     if((currWord->misspelled)[i] != NULL){
        //         sprintf(line, "\tMISPELLED WORD #%d: %s\n", i,((currWord->misspelled)[i])->word);
        //         fwrite(line, strlen(line)+1, 1, f);

        //         sprintf(line,"\t\tMISPELLED?: %d\n", ((currWord->misspelled)[i])->misspelled);
        //         fwrite(line, strlen(line)+1, 1, f);

        //         if(((currWord->misspelled)[i])->correct_word != NULL){
        //             sprintf(line, "\t\tACTUAL WORD %s\n", ((currWord->misspelled)[i])->correct_word->word);
        //             fwrite(line, strlen(line)+1, 1, f);
        //         }

        //         if(((currWord->misspelled)[i])->next->word != NULL)
        //         {
        //             sprintf(line, "\t\tNEXT MISPELLED WORD: %s\n", ((currWord->misspelled)[i])->next->word);
        //             fwrite(line, strlen(line)+1, 1, f);
        //         }
        //     }
        // }

        // if(currWord->next != NULL){
        //     if((currWord->next)->word != NULL)
        //     {
        //         sprintf(line,"\tNEXT WORD: %s\n", (currWord->next)->word);
        //         fwrite(line, strlen(line)+1, 1, f);
        //     }
        // }

    }
}
//void processWord(char* inputWord, int n, char* filename){
FILE* processWord(char* inputWord, int n, char* filename){
    FILE* newdFile;

    if(foundMisspelledMatch(inputWord)){
        // add ' '
        return NULL;
    }
    if(foundDictMatch(inputWord)){
        // add ' '
        return NULL;
    }
    else
    {
        if(n == 0){

            // just print out

            return NULL;
        }

        // make new dic file and add it automatically
        //printf("\"%s\" was not found in the dictionary. Create new dictionary file.\n", inputWord);
        //printf("Original File name: %s\n", filename);

        char * fptr = calloc(256,sizeof(char));
        strcpy(fptr,filename);
        char * leading = fptr;
        bool foundslash = false;

        // more than 8 bytes - error
        char * newLabel = calloc(256,sizeof(char));
        strcpy(newLabel,"new_");

        do{
            if(*leading == '/'){
                leading++;
                foundslash = true;
                break;
            }else{
                leading++;
            }
        }while(*leading!=0x00);

        //printf("    leading: %s\n\n", leading);

        char * ending = calloc(256,sizeof(char));
        strcpy(ending,leading);

        if(foundslash){
            strcpy(leading,newLabel);
            strcat(leading,ending);
        }


        //printf("    |- Renamed File name: %s\n\n", fptr);


        newdFile = fopen(fptr, "w");

        free(fptr);
        free(ending);
        free(newLabel);

        // while(!feof(iFile))
        // {
        //     fwrite(line, strlen(line)+1, 1, newdFile);
        // }

        // do
        // {
        //     printf("\"%s\" was not found in the dictionary. Do you want to add it (Y/N)? ", inputWord);
        //     scanf("%c", &conf);
        //     while ((ch = getchar()) != '\n' && ch != EOF);
        // }while(conf!='Y' && conf!='N');

        // if(conf == 'Y')
        // {
            struct dict_word* newWord;
            //int counter = 0;

            if((newWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
            {
                printf("ERROR: OUT OF MEMORY.\n");
                return NULL;
            }

            addWord(newWord, inputWord);
            dict->word_list = newWord;

            //[ given code ] ///////////////////////////////////////
            //printf("Added \"%s\" to Dictionary. Add misspellings (Y/N)? ", inputWord);

            //printf("Added \"%s\" to Dictionary.\n", inputWord);

            int numMisspellings = n;

            while(numMisspellings > 0)
            {

                struct misspelled_word* newMWord;

                if((newMWord = (struct misspelled_word*) malloc(sizeof(struct misspelled_word))) == NULL)
                {
                    printf("ERROR: OUT OF MEMORY.");
                    return NULL;
                }

                //char** getNewMword = 'd';

                char** getNewMword = gentypos(numMisspellings, inputWord);
                char* getNewMwordW = *getNewMword;
                addMisspelledWord(newMWord, newWord, getNewMwordW);

                //printf("numMisspellings : %d, %p, %p\n", numMisspellings, (void*)newMWord,(void*)m_list);
                numMisspellings--;

                //free(newMWord); // make error
                //free(getNewMwordW);

            }
    }

    return newdFile;
}

bool foundMisspelledMatch(char* inputWord){
    struct misspelled_word* listPtr = m_list;
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
        {
            strcpy(inputWord, listPtr->correct_word->word);
            listPtr->misspelled = 1;
            listPtr->correct_word->misspelled_count++;
            return true;
        }
        listPtr = listPtr->next; // error listPtr-- ->next;
    }
    return false;
}

bool foundDictMatch(char* inputWord){
    //printf("[foundDictMatch function] inputWord: %s\n\n",inputWord);
    struct dict_word* listPtr = dict->word_list;
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
            return true;
        listPtr = listPtr->next;
    }
    return false;
}

void debugPrintWords(){

    struct dict_word *ptr = dict->word_list;
    printf("\n debugPrintWords [ ");

    while(ptr != NULL){
        printf("  ( %s )  ",ptr->word);
        ptr = ptr->next;
    }

    printf(" ]\n\n");
}

void debugPrintWordsMis(){

    struct dict_word *ptr = dict->word_list;
    printf("\n debugPrintWordsMis [ ");

    while(ptr != NULL){
        printf("\n  [ %s ]  ",ptr->word);//, ptr->misspelled[0]->word);
        //struct misspelled_word* ptrMis = ptr->misspelled;
        printf(" -< ");
        int c = 0;
        while(c<(ptr->num_misspellings)){
            printf("  ( %s )  ",(ptr->misspelled)[c]->word);
            //printf("  ( %s )  ",m_list->next->next->next->next->word);
            c++;
            //ptrMis++;
        }
        printf(">- ");
        ptr = ptr->next;
    }

    printf(" ]\n\n");
}

void newDictPrintWordsMis(FILE* file){
    struct dict_word *ptr = dict->word_list;
    char line[MAX_SIZE];

    while(ptr != NULL){
        sprintf(line, "%s ", ptr->word);
        fwrite(line, strlen(line)+1, 1, file);

        int c = 0;
        while(c<(ptr->num_misspellings)){
            sprintf(line, "%s ", (ptr->misspelled)[c]->word);
            fwrite(line, strlen(line)+1, 1, file);
            c++;
        }
        ptr = ptr->next;
        sprintf(line, "\n");
        fwrite(line, strlen(line)+1, 1, file);
    }
}



//addMisspelledWord(currMisspelling, currWord, wdPtr);
struct dict_word* addMisspelledWord(struct misspelled_word* misspelledWord, struct dict_word* correctWord, char* word){
    //setting up misspelledWord fields
    //misspelledWord = (struct misspelled_word*) malloc(sizeof(struct misspelled_word));

    strcpy(misspelledWord->word, word);


    misspelledWord->misspelled = 0;
    misspelledWord->correct_word = correctWord;
    misspelledWord->next = m_list;

    int c = correctWord->num_misspellings;
    (correctWord->misspelled)[c] = misspelledWord;
    //printf("addMisspelledWord1:  %d( %s )  \n",c,(correctWord->misspelled)[c]->word);

    c ++;
    correctWord->num_misspellings = c;

    //printf("addMisspelledWord2:  %d( %d )  \n",c, correctWord->num_misspellings );
    m_list = misspelledWord;
    nMlist++;
    //printf("------- %p\n",(void *)correctWord);
    return correctWord;
    // char word[WORDLENGTH];
    // int misspelled; // initialize to 0
    // struct dict_word* correct_word;
    // struct misspelled_word* next;
}

void debugMlist(){
    struct misspelled_word *ptr = m_list;
    printf("\nM_list [ ");

    while(ptr != NULL){
        printf("( %s ) ",ptr->word);
        ptr = ptr->next;
    }

    printf(" ]\n\n");
}


void printOutput(){
    fprintf(stderr, "Total number of words in dictionary: %d\n", countWordsInDict());
    int sizeDic = sizeof(struct dictionary) + (sizeof(struct dict_word) * (dict->num_words) );
    fprintf(stderr, "Size of dictionary (in bytes): %d\n", sizeDic);
    int sizeMlist = sizeof(struct misspelled_word) * (nMlist) ;
    fprintf(stderr, "Size of misspelled word list (in bytes): %d\n", sizeMlist);
    fprintf(stderr, "Total number of misspelled words: %d\n", countWordsInMListVer2());
    fprintf(stderr, "Top 3 misspelled words:\n");

    getHighestWord();
}

int countWordsInDict(){

    struct dict_word *ptr = dict->word_list;
    int count = 0;
    while(ptr != NULL){
        count++;
        ptr = ptr->next;
    }

    return count;
}

int countWordsInMList(){

    struct misspelled_word *ptr = m_list;
    int count = 0;
    while(ptr != NULL){
        count++;
        ptr = ptr->next;
    }

    return count;
}

int countWordsInMListVer2(){

    int count = 0;

    struct dict_word *ptr = dict->word_list;

    while(ptr != NULL){

        //fprintf(stderr, "countWordsInDict: %d : \n", countWordsInDict());
            int missNumData =  ptr->misspelled_count;
            if( missNumData > 0){
                count++;
            }
        ptr = ptr->next;
    }

    return count;
}

void getHighestWord(){//FILE* f

    //f = stderr;

    struct dict_word *ptr = dict->word_list;
    int numberOfPrinted = 0;
    //int threeRank[3] = {0};

    // find the hightest number
    int hightest = getTheHightestNumber();
    //threeRank[0] = hightest;

    //char line[MAX_SIZE];
    while(ptr != NULL){

        //fprintf(stderr, "countWordsInDict: %d : \n", countWordsInDict());

        if(hightest == (ptr->misspelled_count) && numberOfPrinted < 3 && numberOfPrinted <= countWordsInDict()){
            numberOfPrinted++;



            fprintf(stderr, "%s (%d times): ", ptr->word, ptr->misspelled_count);
            //sprintf(line, "%s (%d times): ", ptr->word, ptr->misspelled_count);
            //fwrite(line, strlen(line)+1, 1, f);

            int c = 0;
            while(c<(ptr->num_misspellings)){

                if(c != (ptr->num_misspellings)-1){
                    fprintf(stderr, "%s, ", (ptr->misspelled)[c]->word);
                    // sprintf(line, "%s, ", (ptr->misspelled)[c]->word);
                    // fwrite(line, strlen(line)+1, 1, f);
                }else{
                    fprintf(stderr, "%s ", (ptr->misspelled)[c]->word);
                    // sprintf(line, "%s ", (ptr->misspelled)[c]->word);
                    // fwrite(line, strlen(line)+1, 1, f);
                }
                c++;
            }
            fprintf(stderr, "\n");
            // sprintf(line, "\n");
            // fwrite(line, strlen(line)+1, 1, f);
        }
        ptr = ptr->next;
    }

    if(numberOfPrinted<3){

        ptr = dict->word_list;

        // if there is no 2nd and 3rd. Just print
        //bool printed = false;
        // find top 2 hightest numbers
        for(int i=1; i<3; i++){
            hightest = getHightestNumbers(hightest);
            while(ptr != NULL){
                if(hightest == (ptr->misspelled_count) && numberOfPrinted < 4 && numberOfPrinted < countWordsInDict()){
                    numberOfPrinted ++;
                    //printed = true;
                    fprintf(stderr, "%s (%d times): ", ptr->word, ptr->misspelled_count);
                    // sprintf(line, "%s (%d times): ", ptr->word, ptr->misspelled_count);
                    // fwrite(line, strlen(line)+1, 1, f);

                    int c = 0;
                    while(c<(ptr->num_misspellings)){

                        if(c != (ptr->num_misspellings)-1){
                            fprintf(stderr, "%s, ", (ptr->misspelled)[c]->word);
                            // sprintf(line, "%s, ", (ptr->misspelled)[c]->word);
                            // fwrite(line, strlen(line)+1, 1, f);
                        }else{
                            fprintf(stderr, "%s ", (ptr->misspelled)[c]->word);
                            // sprintf(line, "%s ", (ptr->misspelled)[c]->word);
                            // fwrite(line, strlen(line)+1, 1, f);
                        }
                        c++;
                    }
                    fprintf(stderr, "\n");
                    // sprintf(line, "\n");
                    // fwrite(line, strlen(line)+1, 1, f);
                }
                ptr = ptr->next;
            }
        }
    } // end if(numberOfPrinted<3)

}

int getTheHightestNumber(){
    int hightest = 0;

    struct dict_word *ptr = dict->word_list;
    while(ptr != NULL){
        if(hightest < (ptr->misspelled_count)){
            hightest = ptr->misspelled_count;

        }
        ptr = ptr->next;
    }
    //printf("1st hightest: %d\n", hightest);
    return hightest;
}

int getHightestNumbers(int currentHigh){
    int hightest = 0;

    struct dict_word *ptr = dict->word_list;
    while(ptr != NULL){
        if(hightest < (ptr->misspelled_count) && currentHigh > (ptr->misspelled_count))
            hightest = (ptr->misspelled_count);
        ptr = ptr->next;
    }
    //printf("2nd hightest: %d\n", hightest);
    return hightest;
}

char* getSubString(char *someString, int n)
{
    if(n<=0){
        return someString;
    }
    char getWord[n]; // n+1
    getWord[n] = 0;

    char* getPtr = getWord;
    strncpy(getPtr, someString, n);

    //memset(getWord,'\0',n);
    return getPtr;
}