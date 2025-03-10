// second iteration
// includes colors now!
// includes list of letters that have been used already

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <string.h>

using namespace std;

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

class word{
private:
    char * w;
public:
    const char * getWord(){return w;}
    void setWord(const char * n){
        int len = 0;
        while(n[len] != '\0'){len++;}
        w = new char[len+1];
        len = 0;
        while(n[len] != '\0'){
            w[len] = n[len];
            len++;
        }
        w[len] = '\0';
    }
    word(){w = nullptr;}
    ~word(){delete [] w;}
};

int strLen(const char *str){
    int len = 0;
    while(str[len] != '\0'){len++;}
    return len;
}

int readSize(const char * f){
    ifstream data(f);
    if(!data){
        return -1;
    }
    int i = 0;
    char buffer[1000];
    while(!data.eof()){
        data.getline(buffer, 1000);
        i++;
    }
    return i;
}

int readFile(const char * f, word *&w, int size){
    ifstream data(f);
    if(!data){
        return -2;
    }
    int i = 0;
    char * temp = new char[6];
    while(!data.eof()){
        data.ignore();
        data >> temp;
        w[i].setWord(temp);
        i++;
    }
    return i;
}

void lowerToUpper(char * guess){
    int len = 0;
    while(guess[len] != '\0'){len++;}
    for(int i = 0; i < len; i++){
        if(96 < guess[i] && guess[i] < 123){
            guess[i] = guess[i]-32;
        }
    }
}

void findLetters(const char * word, int let[]){
    int ind;
    for(int i = 0; i < 5; i++){
        ind = word[i]-65;
        let[ind]++;
    }
}

char * checkGuess(const char * guess, const char * answer, int letters[], char usedLetters[], int &usedSize){
    char * correct = new char[6];
    for(int i = 0; i < 5; i++){
        correct[i] = '-'; // everything is wrong by default
    }
    for(int i = 0; i < 5; i++){
        // checks to see if letter and position matches up
        if(guess[i] == answer[i]){
            correct[i] = '*';
            letters[guess[i]-65]--;
        }
    }
    for(int i = 0; i < 5; i++){
        // checks to see if letter is in at all. if it is, check to see if the letter is already correct. if not, letter is in wrong position
        if(letters[guess[i]-65] != 0 && correct[i] != '*'){
            correct[i] = '&';
            letters[guess[i]-65]--;
        }
    }
    correct[5] = '\0';
    bool inList = false;
    cout << correct << endl;
    for(int i = 0; i < 5; i++){
        inList = false;
        if(correct[i] == '-'){
            // check to see if already in the list
            for(int j = 0; j < usedSize; j++){
                if(usedLetters[j] == guess[i]){
                    inList = true;
                    break;
                }
            }
            if(!inList){
                usedLetters[usedSize] = guess[i];
                usedSize++;
            }
        }
    }
    cout << "usedSize = " << usedSize << endl;
    return correct;
}

bool sameWord(const char * str1, const char * str2){
    int len1 = 0;
    while(str1[len1] != '\0'){len1++;}
    int len2 = 0;
    while(str2[len2] != '\0'){len2++;}
    if(len1 != len2){return false;}
    for(int i = 0; i < len1; i++){
        if(str1[i] != str2[i]){return false;}
    }
    return true;
}

bool binarySearch(const char * w, word * list, int size){
    int bigOrSmall;
    int left = 0;
    int right = size-1;
    int mid;
    while(left <= right){
        mid = (left+right)/2;
        bigOrSmall = strcmp(w, list[mid].getWord());
        if(bigOrSmall == 0){return true;}
        else if(bigOrSmall < 0){right=mid-1;}
        else{left=mid+1;}
    }
    return false;
}

int main(){
    srand(time(0));
    const char * filename = "words.txt";
    int size = readSize(filename);
    word * list = new word[size];
    int read = readFile(filename, list, size);
    int letters[26] = {0};
    const char * answer = list[rand() % size].getWord();
    char * guess = new char[6];
    int guesses = 0;
    bool isCorrect = false;
    bool repeat = false;
    char * code;
    cout << WHITE << "Letter will be " << GREEN << "green" << WHITE << " if it is correct, " << YELLOW << "yellow" << WHITE << " if the letter is in the wrong position, and white if the letter is not in the word\n";
    char * usedLetters = new char[26];
    int usedLettersSize = 0;
    while(!isCorrect && guesses < 6){
        if(guesses != 0 && !repeat){
            cout << WHITE << "Guesses remaining: " << 6-guesses << endl;
            cout << "Used letters: ";
            for(int i = 0; i < usedLettersSize; i++){
                cout << usedLetters[i] << " ";
            }
            cout << endl;
        }
        repeat = false;
        cout << WHITE << "Enter guess: ";
        cin >> guess;
        lowerToUpper(guess);
        for(int i = 0; i < 26; i++){letters[i] = 0;}
        findLetters(answer, letters);
        int len = strLen(guess);
        if(len < 5){cout << "Not enough letters\n"; repeat = true;}
        else if(len > 5){cout << "Too many letters\n"; repeat = true;}
        else if(!binarySearch(guess, list, size)){cout << "Not in word list\n"; repeat = true;}
        else{
            code = checkGuess(guess, answer, letters, usedLetters, usedLettersSize);
            for(int i = 0; i < 5; i++){
                if(code[i] == '-'){cout << WHITE << guess[i];}
                else if(code[i] == '&'){cout << YELLOW << guess[i];}
                else{cout << GREEN << guess[i];}
            }
            cout << endl;
            if(sameWord(code, answer)){
                isCorrect = true;
            }
            guesses++;
        }
    }
    if(!isCorrect){
        cout << WHITE << "The word was " << answer << endl;
    }
    else{
        cout << WHITE << "Congrats! You guessed the word " << answer << " in " << guesses;
        if(guesses == 1){cout << " try\n";}
        else{cout << " tries\n";}
    }
    return 0;
}