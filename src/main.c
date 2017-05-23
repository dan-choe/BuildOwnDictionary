#include "dic.h"
#include "debug.h"
#include "mispelling.h"
#include <getopt.h>

int main(int argc, char *argv[]){
    //DEFAULT_DICT_FILE = "dictionary.txt";
    strcpy(DEFAULT_DICT_FILE, "rsrc/dictionary.txt");

    DEFAULT_INPUT = stdin;
    DEFAULT_OUTPUT = stdout;
    nMlist = 0;

    //create dictionary
    if((dict = (struct dictionary*) malloc(sizeof(struct dictionary))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }

    dict->num_words = 0;
    dict->word_list = NULL;

    // if((m_list = (struct misspelled_word*) malloc(sizeof(struct misspelled_word*))) == NULL)
    // {
    //     printf("ERROR: OUT OF MEMORY.\n");
    //     return EXIT_FAILURE;
    // }
    // free(m_list);
    // m_list = NULL;

    struct Args args;
    // Set struct default values
    args.d = false;
    args.i = false;
    args.o = false;
    strcpy(args.dictFile, DEFAULT_DICT_FILE); //  error
    args.n = 0;

    // Make a loop index
    //int i;

    //Declare Files
    FILE* dFile;
    FILE* iFile = DEFAULT_INPUT;
    FILE* oFile = DEFAULT_OUTPUT;

    char opt = '\0';

    while((opt = getopt(argc, argv, "hi:o:d:A:")) != -1 ){

        if(optarg == NULL){
            // printf("[ optarg == NULL EXIT_FAILURE ]\n");
            return EXIT_FAILURE;
        }

        if(opt == 'h')
        {
            USAGE(EXIT_SUCCESS);
            return EXIT_SUCCESS;
        }
        if(opt == 'i')
        {
            //char* currArg = optarg;
            // if(optarg == NULL){
            //     printf("[ input file: EXIT_FAILURE ]\n");
            //     return EXIT_FAILURE;
            // }
            // printf("[ input file: %s ]\n", currArg);
            strcpy(args.input, optarg);

            bool exists  = test(args.input);
            if(exists == false){
                USAGE();
                return EXIT_FAILURE;
                //printf("[ input file is broken: %s ]\n", optarg);
            }

            args.i = true;
            iFile = fopen(args.input, "r");
        }
        if(opt == 'o')
        {
            //char* currArg = optarg;
            // if(optarg == NULL){
            //     printf("[ output file: EXIT_FAILURE ]\n");
            //     return EXIT_FAILURE;
            // }
            //char* currArg = optarg;
            //printf("[ output file: %s ]\n", currArg);
            strcpy(args.output, optarg);

            bool exists  = test(args.output);
            if(exists == false){
                USAGE();
                return EXIT_FAILURE;
                //printf("[ output file is broken: %s ]\n", optarg);
            }

            args.o = true;
            oFile = fopen(args.output, "w");
        }
        if(opt == 'd')
        {
            //char* currArg = optarg;
            //printf("[ dictFile file: %s ]\n", currArg);
            strcpy(args.dictFile, optarg);

            bool exists  = test(args.dictFile);
            if(exists == false){
                USAGE();
                return EXIT_FAILURE;
                //printf("[ dictFile file is broken: %s ]\n", optarg);
            }

            args.d = true;
        }
        if(opt == 'A')
        {
            char* currArg = optarg;

            // Invaild if number is greater than 5 (2 digits)
            if(*currArg < '0' || *currArg > '5' || *(currArg+1) != 0x00){
                USAGE(EXIT_FAILURE);
                return EXIT_FAILURE;
            }else{
                args.n = *currArg - '0';
                //printf("[ A : %s ]\n", currArg);
            }
        }
        opt = '\0';

    }

    dFile = fopen(args.dictFile, "r");

    // int ch = getc(stdin);
    // if (ch == EOF || ch<=11){
    //     USAGE();
    //     return EXIT_FAILURE;
    // }

    //No Arguments

    //ch = fseek( stdin, 1, SEEK_SET );
    //printf(">>>>>>>> ch : %d\n", ch);

    //char w = 50;
    int iFileVaild = 0;

    if(args.o == false && args.i == false){
        //int ch = fseek( stdin, 1, SEEK_SET );
        //printf(">>>>>>>> ch 1 : \n");

        if(argc == 1){
            iFileVaild = -1;
        }
    }

    if(args.o == true && args.i == false){
        //ch = fseek( stdin, 1, SEEK_SET );
        //printf(">>>>>>>> ch 2 : %d\n", ch);
        //if(!feof(stdin)){
            char w = fgetc(stdin);


        //printf("        >>>>>>>> w : %d\n", w);

        if( w <32  ){
            //printf("        >>>>>>>> empty \n");
            iFileVaild = -1;
        }else{
            ungetc(w,stdin);
            //printf("        >>>>>>>> no empty \n");
        }
    }


    if(args.o == true && args.i == true){
        int cmp = strcmp(args.input, args.output);

        if(cmp == 0){
            return EXIT_FAILURE;
        }

    }


    if(iFile == NULL && args.i == true)
    {
        //printf("        >>>>>>>> error 1 \n");
        return EXIT_FAILURE;
    }

    if(iFile == NULL && args.o == true)
    {
        //printf("        >>>>>>>> error 2 \n");
        return EXIT_FAILURE;
    }

    if(dFile == NULL)
    {
        USAGE();
        return EXIT_FAILURE;
    }
    else
    {
        processDictionary(dFile);
    }

    //debugPrintWords();
    //debugPrintWordsMis();
    //debugMlist();

    //strcpy(line,"\n--------INPUT FILE WORDS--------\n");
    //fwrite(line, strlen(line)+1, 1, oFile);



    while(iFileVaild != -1 && !feof(iFile))
    {
        char line[MAX_SIZE];
        char word[MAX_SIZE] = {'\0'};
        char* wdPtr = word;

        char subWord[MAX_SIZE] = {'\0'};
        char *subPtr = subWord;

        // save leading/ending symbols
        char leadingSymbol[MAX_SIZE] = {'\0'};
        char *leadingSymbolPtr = leadingSymbol;
        char endingSymbol[MAX_SIZE] = {'\0'};
        char *endingSymbolPtr = endingSymbol;

        char* character = line;
        char* firstLetter = 0X00;
        int count = 0; // the number of time until read first character

        int lengthWord = 0;



        void* newLine = fgets(line, MAX_SIZE+1, iFile);
        if(newLine == NULL){
            break;
        }

        // if(*(character+1) == 0 && (*character == '\n' || *character == ' ')){
        //     //free(currWord);
        //     break;
        // }

        //int inputfileEnd = 0;
        //if there isn't a space or newline at the end of the line, put one there
        if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n')){
            strcat(line, " ");
           // inputfileEnd = -1;
        }
        //printf("\n-----------------------------------------------------line : %s\n", line);

        //replaces spaces within a line with new lines
        while(*character != 0x00)
        {
        // error NULL comparison between pointer and integer
        // {   printf("%c\n", *character);
        //     if(*character == '\n'){
        //         printf("%s\n", "found new line");
        //     }
            if(*character == ' ' || *character == '\n' || (strcmp(character,"\n")==0)){
                //printf("\n------------------------read a word : %s //\n", word);

                //printf("The length word: %d\n\n",lengthWord);
                    lengthWord--;

                //if(*word == 0x00){
                    //printf("word is empty.\n");
                    //strcat(character, "empty");
                    //fwrite(character, strlen(character)+1, 1, oFile);
                    //break;
                //}


                if(firstLetter != 0X00){
                    //printf("original word: %s, count: %d\n",word, count);
                    subPtr = word;
                    subPtr = subPtr + (count-1);

                    strncpy(leadingSymbolPtr, word, (count-1));
                    //printf("leadingSymbolPtr: %s, %d\n\n",leadingSymbolPtr, count);

                    //printf("subPtr: %s//\n\n",subPtr);
                }



                //////----------------------------------
                //void * testPtr = listPtr;
                char * test = calloc(256,sizeof(char));
                char * test2 = calloc(256,sizeof(char));
                char * endingpunct = test2;



                //strcpy(endingpunct, wdPtr-1);// contains pointing last char of word

                strcpy(endingpunct, subPtr);
                //printf("1 endingpunct: %s//\n", endingpunct);

                int totalNumwords = 0;
                int totalLetter = 0;
                while(*endingpunct !='\0'){
                    if(*endingpunct>='a' && *endingpunct<='z'){
                        totalLetter++;
                    }
                    totalNumwords++;
                    endingpunct++;
                }


                //printf("totalNumwords without leading symbols: %d, %c\n\n", totalNumwords, *endingpunct);

                int endPunct = 0;

                if(totalLetter!=0){

                    while(!((*endingpunct>='a' && *endingpunct<='z') || (*endingpunct>='A' && *endingpunct<='Z') ))
                    {
                        //printf("while endPunct: %d\n", endPunct);
                        endPunct++;
                        endingpunct--;
                    }
                    endingpunct++; // staring punctuation from the end of word
                    endPunct--;


                    //printf("endPunct: %d\n", endPunct);
                    if(endPunct <0){
                        endPunct = 0;
                    }

                    //printf(" (totalNumwords-endPunct): %d\n", (totalNumwords-endPunct));

                    if(endPunct!=0){
                        subPtr = getSubString(subPtr, (totalNumwords-endPunct)); //(endPunct-1)
                        strcpy(test,subPtr);
                    }else{

                        strcpy(test,subPtr);
                    }


                    //if(firstLetter!=0x00){
                        //printf("final subPtr: %s, firstLetter: %c\n\n",test,*firstLetter);
                    //}



                    /// find ending symbols
                    /////////////////////////////////////////////// error
                    strcpy(endingpunct, word);


                    //printf("1 endingpunct: %s//\n", endingpunct);

                    while(*endingpunct !='\0'){
                        endingpunct++;
                    }

                    int numSymbolEnding = 0;
                    while(!((*endingpunct>='a' && *endingpunct<='z') || (*endingpunct>='A' && *endingpunct<='Z')))
                    {
                        numSymbolEnding++;
                        endingpunct--;
                    }
                    endingpunct++;

                    int copyOnlySymbol = numSymbolEnding-1;

                    if(copyOnlySymbol > 1){
                        strncpy(endingSymbolPtr, endingpunct, copyOnlySymbol);
                    }else{
                        strncpy(endingSymbolPtr, endingpunct, 2);
                    }




                    //printf("endingSymbolPtr: %s, %d\n\n",endingSymbolPtr, copyOnlySymbol);

                    //printf("\n----------------input contains symbols----------------\n");

                    firstLetter=0x00;


                    //printf("test : %s\n",test);
                    FILE* newdFile = NULL;
                    newdFile = processWord(test, args.n, args.dictFile);

                    if(newdFile != NULL){
                        //printf("copy into new dict : \n");
                        newDictPrintWordsMis(newdFile);
                        fclose(newdFile);
                    }
                    // else{
                        // the input word is in correct word or m_list
                        //printf("the input word is in correct word or m_list : %p\n",(void*)oFile);



                        if(*leadingSymbolPtr !='\0'){
                            fwrite(leadingSymbolPtr, strlen(leadingSymbolPtr)+1, 1, oFile);

                            memset(leadingSymbolPtr,'\0',(count-1));
                        }

                        if(*endingSymbolPtr =='\0'){
                            if(*character == ' '){
                                strcat(test, " ");
                            }

                            fwrite(test, strlen(test)+1, 1, oFile);
                        }else{
                            fwrite(test, strlen(test)+1, 1, oFile);
                        }

                        if(*endingSymbolPtr !='\0'){
                            if(*character == ' '){
                                strcat(endingSymbolPtr, " ");
                            }
                            fwrite(endingSymbolPtr, strlen(endingSymbolPtr)+1, 1, oFile);

                            memset(endingSymbolPtr,'\0',(numSymbolEnding-1));
                        }

                        if(*character == '\n'){
                            //printf("end reading character.\n");
                            fwrite(character, strlen(character)+1, 1, oFile);
                        }

                    count = 0;

                }else{ //if(totalNumwords!=0)

                    // case : only contains symbols
                    firstLetter=0x00;
                    count = 0;


                    if(*character == ' '){
                                strcat(word, " ");
                            }

                    fwrite(word, strlen(word)+1, 1, oFile);

                    if(*character == '\n'){
                        //printf("end reading character.\n");
                        //strcat(character, "empty");
                        fwrite(character, strlen(character)+1, 1, oFile);
                        //break;
                    }

                }

                    // reset word array
                    *wdPtr = 0x00;
                    wdPtr = word;


                    memset(word,'\0',(lengthWord+1));
                    //printf("[[[[ reset word ]]]]]  %s, %d\n\n", word, lengthWord);
                    lengthWord = 0;

                    free(test2);
                    free(test);



            }
            else
            {

                if(firstLetter==0x00){
                    count++;
                    if((*character>='a' && *character<='z') || (*character>='A' && *character<='Z')){
                        firstLetter = character;
                    }
                }

                // change capital letter to lower case
                if(*character>='A' && *character<='Z'){
                    *character += 32;
                }

                *(wdPtr++) = *character;

            }
            lengthWord++;
            character++;
        }



        if(iFile == stdin){
            break;
        }
    }

    //printf("%s\n", "<--------------while ends---------------->");

    //strcpy(line, "\n--------DICTIONARY WORDS--------\n");
    //fwrite(line, strlen(line)+1, 1, oFile);

    //printWords(dict->word_list , oFile);
    printOutput();
    //printf("\n--------FREED WORDS--------\n");
//    freeWords(dict->word_list);

    fclose(dFile);
    fclose(iFile);
    fclose(oFile);


    struct misspelled_word *curr = m_list;
    struct misspelled_word *next = m_list;
    while(curr!=NULL){
        next = curr->next;
        if(curr!=NULL){
            free(curr);
        }
        curr = next;
    }

    struct dict_word *cur = dict->word_list;
    struct dict_word *curnext = dict->word_list;
    while(cur!=NULL){
        curnext = cur->next;
        if(cur!=NULL){
            free(cur);
        }
        cur = curnext;
    }

    free(dict);
    //free(newMWord);
    //free(m_list);

    return EXIT_SUCCESS;
}


