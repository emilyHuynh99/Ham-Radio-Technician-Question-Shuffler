/*
given the pool of questions for the technician's ham radio exam, 
we parse the file and separate the questions, questionID, answer, 
and the answer choices. 

in this program we are able to decide if we want to the questions 
in order like in the original file or mixed up, we are given the option 
to write the questions to output files (1 file for questions, 1 for answers) 
or run the program and go through the entire question pool in runtime, the
program will prompt for an answer and compare that answer to the correct answer, 

tested to be fully functional: runtime terminal practice of pool questions and 
    mixing of the multiple choice answers

not completely working: writing to output file questions and answers

under development/planned: is the ability to mix the answer choices around
*/
#include <fstream>
#include <iostream>
#include <string> 
#include <cstring>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

struct questions
{
    string questionID; 
    string question;
    string answer;
    string choiceA;
    string choiceB;
    string choiceC;
    string choiceD;
    bool printed;
};

void printInOrder(questions pool[600], int questionTotal, ofstream& outq, bool write);
void printOutofOrder(questions pool[600], int questionTotal, ofstream& outq, bool write);

void NoOrderPrint(questions pool[600], int questionTotal, ofstream& out, ofstream& outa);
void OrderedPrint(questions pool[600], int questionTotal, ofstream& out, ofstream& outa);

void printMC1(questions pool[600], int index, ofstream& outq, bool write);
void printMC2(questions pool[600], int index, ofstream& outq, bool write);
void printMC3(questions pool[600], int index, ofstream& outq, bool write);
void printOG(questions pool[600], int index, ofstream& outq, bool write);

void showAnswer(questions pool[600], int index);
string getQuestionNum(ifstream& fileacccess); //needs to be passby reference
string getAnswer(ifstream& fileacc);
string getQuestion(ifstream& fileacc);
string getChoiceA(ifstream& fileacc);
string getChoiceB(ifstream& fileacc);
string getChoiceC(ifstream& fileacc);
string getChoiceD(ifstream& fileacc);

string filename;
bool mix; 
int correctCount, incorrectCount = 0; 
bool write; 

int main(int argc, char *argv[]){
    ifstream in;
    string nomix = "orderN";
    string mix = "orderY";
    string separateOutfile = "oFileY";
    string nseparateOutfile = "oFileN";

    if(argc < 4){ //if only a.out is commanded
        cout << "usage (4 arguments): ./a.out questionFileName.txt orderN(no question order)/orderY(questions in order) oFileN(answers & questions wont be separated)/oFileY\n";
        return 0;
    }
    else{ //now we will have the filename and whether or not print the questions mixed or in order 
        filename = argv[1];
        in.open(filename); //file was opened here 
        if(in.is_open() == false){ //checking for bad file 
            cout << "error: file failed to open\n";
            return 0; 
        }
        if(argv[2] != nomix && argv[2] != mix){ //checking for good argument
            cout << "error: bad third argument\n";
            return 0; 
        }  
        if(argv[3] != separateOutfile && argv[3] != nseparateOutfile){
            cout << "error: bad fourth argument\n";
            return 0;
        }
    }

    cout << "PREPARING INPUT FILE DATA..\n\n";

    //next step is to read in the questions and store them 
    questions pool[600]; 
    string temp;
    int questionCount = 0; 
    while(true){ //reading/storing questions 
        if(in.eof())
            break;
        //parse the entire question using parse functions
        /*if(((getQuestionNum(in)) == "") && (questionCount != 0)){
            cout << "broke\n";
            break;
        }*/
            
        pool[questionCount].questionID = getQuestionNum(in); 
        pool[questionCount].answer = getAnswer(in);
        pool[questionCount].question = getQuestion(in);
        pool[questionCount].choiceA = getChoiceA(in);
        pool[questionCount].choiceB = getChoiceB(in);
        pool[questionCount].choiceC = getChoiceC(in);
        pool[questionCount].choiceD = getChoiceD(in);
        pool[questionCount].printed = false;
        questionCount++;
        getline(in, temp); //to grab the ~~ at the end of the question
    }

    cout << "THERE ARE " << questionCount << " QUESTIONS IN THIS FILE\n\n";
        ofstream outq;
        ofstream outa;
    if(argv[3] == separateOutfile){
        write = true;

        outq.open("18-22_Technician_Pool_Questions.txt");
        outa.open("18-22_Technician_Pool_Answers.txt");

        if(argv[2] == nomix){
            NoOrderPrint(pool, questionCount, outq, outa);
        }
        else
        {
            OrderedPrint(pool, questionCount, outq, outa);
        }
    }
    else{
        write = false;
        if(argv[2] == nomix){
            printOutofOrder(pool, questionCount, outq, write);
        }
        else
        {
            printInOrder(pool, questionCount, outq, write);
        }
    }
    
}
void OrderedPrint(questions pool[600], int questionTotal, ofstream& outq, ofstream& outa){
    string answerChoice;
    for(int i = 0; i < questionTotal; i++){
        outq << pool[i].question << endl;
        outq << pool[i].choiceA << endl;
        outq << pool[i].choiceB << endl;
        outq << pool[i].choiceC << endl;
        outq << pool[i].choiceD << endl;

        outa << pool[i].answer << endl;
    }
    
}
void NoOrderPrint(questions pool[600], int questionTotal, ofstream& outq, ofstream& outa){
    string answerChoice;
    int printed = 0; 
    int question = 0;
    srand (time(NULL)); //randomized based on time, needed for random question

    while(printed < questionTotal){
        outq << "questions printed: " << printed << endl;
        question = rand() % questionTotal;

        if(pool[question].printed == false){ //then ask user the question
            pool[question].printed = true; //set it as visited
            outq << pool[question].question << endl;
            outq << pool[question].choiceA << endl; 
            outq << pool[question].choiceB << endl;
            outq << pool[question].choiceC << endl;
            outq << pool[question].choiceD << endl;

            outa << pool[question].answer << endl;
            
            printed++;
        }
    }
}
void printOutofOrder(questions pool[600], int questionTotal, ofstream& outq, bool write){
    string answerChoice;
    int printed = 0; 
    int question = 0;
    int choices = 0;
    srand (time(NULL)); //randomized based on time, needed for random question

    while(printed < questionTotal){
        cout << "Q: " << printed+1 << endl;
        
        //need more than 1 question for this to work lol
        question = rand() % questionTotal; 
        choices = rand() % 4;

        if(pool[question].printed == false){ //then ask user the question

            pool[question].printed = true; //set it as visited

            cout << pool[question].question << endl;
            if(choices == 0){
                printOG(pool, question, outq, write);
            }
            else if(choices == 1){
                printMC1(pool, question, outq, write);
            }
            else if(choices == 2){
                printMC2(pool, question, outq, write);
            }
            else if(choices == 3){
                printMC3(pool, question, outq, write);
            }

            cout << "ANSWER CHOICE: ";
            cin >> answerChoice; //capitalized letters only

            if(answerChoice != pool[question].answer){
                cout << "------------------INCORRECT, ";
                showAnswer(pool, question);
                cout << "\n";
            }
            else{
                cout << "******************CORRECT ANSWER\n";
                correctCount++;
            }

            cout << "$$ total correct: " << correctCount << endl << endl;
            printed++;
        }
    }
}

void printInOrder(questions pool[600], int questionTotal, ofstream& outq, bool write){
    string answerChoice;
    for(int i = 0; i < questionTotal; i++){
        cout << pool[i].question << endl;
        cout << pool[i].choiceA << endl;
        cout << pool[i].choiceB << endl;
        cout << pool[i].choiceC << endl;
        cout << pool[i].choiceD << endl;

        cout << "ANSWER CHOICE: ";
        cin >> answerChoice; //capitalized letters only

        if(answerChoice != pool[i].answer){
            cout << "------------------INCORRECT, RIGHT ANSWER: ";
            showAnswer(pool, i);
            cout << "\n";
        }
        else{
            cout << "******************CORRECT ANSWER\n";
            correctCount++;
        }
        cout << "$$ total correct: " << correctCount << endl << endl;
    }
}
void showAnswer(questions pool[600], int index){
    cout << "CORRECT ANSWER: " << pool[index].answer;
    return;
}

string getQuestionNum(ifstream& fileacc){ //ifstream fileacc
    string questionID;

    getline(fileacc, questionID , '(');

    if(fileacc.eof())
            return "";

    //cout << "questionID: "<< questionID << endl;
    return questionID;
}

string getAnswer(ifstream& fileacc){
    string temp;

    string answer;
    getline(fileacc, answer , ')');

    getline(fileacc, temp); //grab everything until newline 

    if(fileacc.eof())
            return "";

    //cout << "answer: "<< answer << endl;
    return answer;
}

string getQuestion(ifstream& fileacc){ //ifstream fileacc
    string question;

    getline(fileacc, question);

    if(fileacc.eof())
            return "";

    //cout << "question: "<< question << endl;
    return question;
}

string getChoiceA(ifstream& fileacc){
    string chA;
    getline(fileacc, chA);

    if(fileacc.eof())
            return "";

    //cout << chA << endl;
    return chA; 
}

string getChoiceB(ifstream& fileacc){
    string chB;
    getline(fileacc, chB);
    if(fileacc.eof())
        return "";
        
    //cout << chB << endl;
    return chB; 
}

string getChoiceC(ifstream& fileacc){
    string chC;
    getline(fileacc, chC);
    if(fileacc.eof())
        return "";
        
    //cout << chC << endl;
    return chC; 
}

string getChoiceD(ifstream& fileacc){
    string chD;
    getline(fileacc, chD);
    if(fileacc.eof())
        return "";
        
    //cout << chD << endl;
    return chD; 
}

void printMC1(questions pool[600], int index, ofstream& outq, bool write){
    int position = 0; 

    if(write == false){
        position = (pool[index].choiceC).find(".");
        cout << "A" << (pool[index].choiceC).substr(position) << endl;

        position = (pool[index].choiceD).find(".");
        cout << "B" << (pool[index].choiceD).substr(position) << endl;

        position = (pool[index].choiceB).find(".");
        cout << "C" << (pool[index].choiceB).substr(position) << endl;

        position = (pool[index].choiceA).find(".");
        cout << "D" << (pool[index].choiceA).substr(position) << endl;
    }
    /*else{
        position = (pool[index].choiceC).find(".");
        outq << "A" << (pool[index].choiceC).substr(position) << endl;

        position = (pool[index].choiceD).find(".");
        outq << "B" << (pool[index].choiceD).substr(position) << endl;

        position = (pool[index].choiceB).find(".");
        outq << "C" << (pool[index].choiceB).substr(position) << endl;

        position = (pool[index].choiceA).find(".");
        outq << "D" << (pool[index].choiceA).substr(position) << endl;
    }*/
    
    if(pool[index].answer == "A"){
        pool[index].answer = "D";
    }
    else if(pool[index].answer == "B"){
        pool[index].answer = "C";
    }
    else if(pool[index].answer == "C"){
        pool[index].answer = "A";
    }
    else if(pool[index].answer == "D"){
        pool[index].answer = "B";
    }
}

void printMC2(questions pool[600], int index, ofstream& outq, bool write){
    int position = 0; 

    if(write == false){
        position = (pool[index].choiceB).find(".");
        cout << "A" << (pool[index].choiceB).substr(position) << endl;

        position = (pool[index].choiceC).find(".");
        cout << "B" << (pool[index].choiceC).substr(position) << endl;

        position = (pool[index].choiceA).find(".");
        cout << "C" << (pool[index].choiceA).substr(position) << endl;

        position = (pool[index].choiceD).find(".");
        cout << "D" << (pool[index].choiceD).substr(position) << endl;
    }
    /*else{
        position = (pool[index].choiceC).find(".");
        outq << "A" << (pool[index].choiceC).substr(position) << endl;

        position = (pool[index].choiceD).find(".");
        outq << "B" << (pool[index].choiceD).substr(position) << endl;

        position = (pool[index].choiceB).find(".");
        outq << "C" << (pool[index].choiceB).substr(position) << endl;

        position = (pool[index].choiceA).find(".");
        outq << "D" << (pool[index].choiceA).substr(position) << endl;
    }*/
    
    if(pool[index].answer == "A"){
        pool[index].answer = "C";
    }
    else if(pool[index].answer == "B"){
        pool[index].answer = "A";
    }
    else if(pool[index].answer == "C"){
        pool[index].answer = "B";
    }
    else if(pool[index].answer == "D"){
        pool[index].answer = "D";
    }
}

void printMC3(questions pool[600], int index, ofstream& outq, bool write){
    int position = 0; 

    if(write == false){
        position = (pool[index].choiceB).find(".");
        cout << "A" << (pool[index].choiceB).substr(position) << endl;

        position = (pool[index].choiceD).find(".");
        cout << "B" << (pool[index].choiceD).substr(position) << endl;

        position = (pool[index].choiceA).find(".");
        cout << "C" << (pool[index].choiceA).substr(position) << endl;

        position = (pool[index].choiceC).find(".");
        cout << "D" << (pool[index].choiceC).substr(position) << endl;
    }
    /*else{
        position = (pool[index].choiceC).find(".");
        outq << "A" << (pool[index].choiceC).substr(position) << endl;

        position = (pool[index].choiceD).find(".");
        outq << "B" << (pool[index].choiceD).substr(position) << endl;

        position = (pool[index].choiceB).find(".");
        outq << "C" << (pool[index].choiceB).substr(position) << endl;

        position = (pool[index].choiceA).find(".");
        outq << "D" << (pool[index].choiceA).substr(position) << endl;
    }*/
    
    if(pool[index].answer == "A"){
        pool[index].answer = "C";
    }
    else if(pool[index].answer == "B"){
        pool[index].answer = "A";
    }
    else if(pool[index].answer == "C"){
        pool[index].answer = "D";
    }
    else if(pool[index].answer == "D"){
        pool[index].answer = "B";
    }
}

void printOG(questions pool[600], int index, ofstream& outq, bool write){
    int position = 0; 

    if(write == false){
        cout << pool[index].choiceA << endl;
        cout << pool[index].choiceB << endl;
        cout << pool[index].choiceC << endl;
        cout << pool[index].choiceD << endl;
    }
    /*else{
        outq << pool[question].question << endl;
            outq << pool[question].choiceA << endl; 
            outq << pool[question].choiceB << endl;
            outq << pool[question].choiceC << endl;
            outq << pool[question].choiceD << endl;
    }*/
}
