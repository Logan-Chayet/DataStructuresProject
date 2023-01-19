#include "miniGit.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <thread>

using namespace std;
using namespace std::chrono_literals;//Used for delaying cout statements so menu flows nicely

namespace fs = std::filesystem;

MiniGit::MiniGit(){
    fs::remove_all(".minigit");
    fs::create_directory(".minigit"); //Creat minigit directory
    commitNumberHelper = 0; //Commit number tracker for checkout function
}
MiniGit::~MiniGit(){
    fs::remove_all(".minigit");
}

void MiniGit::init(){
    commitHead = new doublyNode; //create new doublynode to prepare for adding files
    commitHead->commitNumber = 0;
    commitHead->next = NULL; //Setting all pointers to null to prepare for commit function
    commitHead->previous = NULL;
    commitHead->head = NULL;
}
void MiniGit::add(string fileName){
    if(commitHead == NULL){//Making sure minigit is initialized
        cerr << "MiniGit had not been initialized, please select option 1 to initialize .minigit directory" << endl;
    }
    else{
        doublyNode* curr = commitHead;
        while(curr != NULL){ //This loop is here to get to the last doublyNode when adding a file
            if(curr->next == NULL){
                singlyNode* temp = curr->head;
                while(temp != NULL){//Making sure filename isn't the same name
                    if(temp->fileName == fileName){
                        cout << "File already exists in the current commit, please try again" << endl;
                        return;
                    }
                    temp = temp->next;
                }
                singlyNode* newFile = new singlyNode(); //Create new singlyNode
                newFile->fileName = fileName;
                newFile->fileVersionN = 0;
                newFile->fileVersion = fileName + "_0" + to_string(newFile->fileVersionN);//Adding a file version name starting with _00
                newFile->next = curr->head;
                curr->head = newFile;
                cout << "Adding File..." << endl;
                std::this_thread::sleep_for(0.5s);//Cool implementation to make the menu flow better (delayed cout statements)
                cout << "File added!" << endl;
                std::this_thread::sleep_for(0.5s);
                return;
            }
            curr = curr->next;
        }

    }
}
void MiniGit::rm(string fileName){
    if(commitHead == NULL){//Making sure minigit is initialized
        cerr << "MiniGit had not been initialized, please select option 1 to initialize .minigit directory" << endl;
    }
    else{
        doublyNode* curr = commitHead;
        while(curr != NULL){//This loop is here to get to the last doublyNode when removing a file
            if(curr->next == NULL){
                singlyNode* temp = curr->head;
                singlyNode* prev = NULL;
                while(temp != NULL){//Iterate through singlyNode list
                    if(temp->fileName == fileName){
                        if(prev != NULL){//Middle remove node test case
                            prev->next = temp->next;
                            delete temp;
                            cout << "Removing File..." << endl;
                            std::this_thread::sleep_for(0.5s);
                            cout << "File removed!" << endl;
                            std::this_thread::sleep_for(0.5s);
                            return;
                        }
                        else{
                            curr->head = temp->next;//Remove last test case
                            delete temp;
                            cout << "Removing File..." << endl;
                            std::this_thread::sleep_for(0.5s);
                            cout << "File removed!" << endl;
                            std::this_thread::sleep_for(0.5s);
                            return;
                        }
                    }
                    prev = temp;
                    temp = temp->next;
                }
                cout << "File was not found or does not exist in the current commit." << endl;
                return;
            }
            curr = curr->next;
        }
    }
}
void file_copy(string file1, string file2){//File copy function to copy one input file to an output file
    ifstream f1(file1);
    ofstream f2(file2);
    char c1;
    char c2;
    bool check = true;
    if(!f1.is_open()){
        cout << "Input file not found." << endl; //Making sure the file exists
        return;
    }
    if(!f2.is_open()){
        cout << "Output file not found." << endl;//Making sure the file exists
        return;
    }
    while(1){
        c1 = f1.get();
        if(c1 == EOF){
            break;
        }
        f2 << c1; //Copy words char for char
    }
    f1.close();
    f2.close();
}
bool is_equal(string file1, string file2){//Is equal function to check if a new version should be committed into the repository
    ifstream f1(file1);
    ifstream f2(file2);
    char c1;
    char c2;
    if(!f1.is_open() && !f2.is_open()){ //Making sure both files exist and are empty
        return true;
    }
    if(!f1.is_open() || !f2.is_open()){ //Making sure both files can open
        return false;
    }
    while(1){
        c1 = f1.get();
        c2 = f2.get();
        if(c1 != c2){
            return false;
        }
        if(c1 == EOF || c2 == EOF){
            break;
        }
    }
    return true;

}
void MiniGit::commit(){
    doublyNode* temp = commitHead;
    doublyNode* newDoubly = new doublyNode;
    while(temp != NULL){//Function here to iterate to last doublyNode
        if(temp->next == NULL){
            singlyNode* curr = temp->head;
            while(curr != NULL){
                string filenameCHECK = ".minigit/" + curr->fileVersion;//Move file into the .minigit folder
                if(fs::exists(filenameCHECK) == false){//If the file committed doesn't exist in the .minigit, create it
                    string filename = ".minigit/" + curr->fileVersion;
                    file_copy(curr->fileName, filename);
                }
                else{
                    string filenameEQ = ".minigit/" + curr->fileVersion;
                    if(is_equal(curr->fileName, filenameEQ) == true){//If the file is equal, don't do anything
                    }
                    else{//If it is not equal, than create a new version and insert into the .minigit folder
                        curr->fileVersionN = curr->fileVersionN + 1;
                        curr->fileVersion = curr->fileName + "_0" + to_string(curr->fileVersionN);
                        string filename = ".minigit/" + curr->fileVersion;
                        file_copy(curr->fileName, filename);
                    }
                }
                curr = curr->next;
            }
            temp->next = newDoubly;//Creating new doublyNode
            newDoubly->next = NULL;
            newDoubly->previous = temp;//Setting next to NULL and previous to the previous doublyNode
            newDoubly->commitNumber = newDoubly->previous->commitNumber + 1;//Incrementing the commit number
            commitNumberHelper = commitNumberHelper + 1;//This is incremented for the commitnumberhelper function that keep track of the commit for checkouts
            // cout << newDoubly->commitNumber << endl;
            break;
        }
        temp = temp->next;
    }
    
    singlyNode* singlytemp = newDoubly->previous->head;
    singlyNode* headtemp = new singlyNode();
    newDoubly->head = headtemp;//Seting the current node of the doubly linked list with following attributes
    headtemp->fileName = singlytemp->fileName;
    headtemp->fileVersion = singlytemp->fileVersion;
    headtemp->fileVersionN = singlytemp->fileVersionN;
    headtemp->next = nullptr;
    singlyNode* current = headtemp;
    singlytemp = singlytemp->next;
    while(singlytemp != nullptr){//This loop makes a deep copy of the previous singly linked list
        current->next = new singlyNode();
        current = current->next;
        current->fileName = singlytemp->fileName;
        current->fileVersion = singlytemp->fileVersion;
        current->fileVersionN = singlytemp->fileVersionN;
        current->next = nullptr;
        singlytemp = singlytemp->next;
    }
}
void MiniGit::checkout(int cNumber){
    doublyNode* temp = commitHead;
    commitNumberHelper = cNumber;
    while(temp != NULL){//Iterate through each doublyNode
        if(temp->commitNumber == cNumber){//If commit number matches entered commit number enter this statement
            singlyNode* curr = temp->head;
            while(curr != NULL){
                string filename = ".minigit/" + curr->fileVersion;//Get the file from the .minigit and copy the contents to the OG file
                file_copy(filename, curr->fileName);
                // cout << curr->fileVersion << endl;
                curr = curr->next;
            }
        }
        temp = temp->next;
    }
    cout << "Checking out version..." << endl;//Nice cout statements for menu flow
    std::this_thread::sleep_for(0.5s);
    cout << "Completed!" << endl;
    std::this_thread::sleep_for(0.5s);
}
bool MiniGit::cNumberHelper(){//The purpose of this function is lockout add/rm/commit from the user UNTIL they checkout the most recent commit number
    doublyNode* temp = commitHead;
    while(temp != NULL){
        if(temp->next == NULL){
            if((temp->commitNumber) == commitNumberHelper){
                return true;
            }
        }
        temp = temp->next;
    }
    return false;
}