#ifndef Project_H
#define Project_H
#include <iostream>
#include <string>

using namespace std;

struct singlyNode;

struct doublyNode{
    int commitNumber;
    singlyNode* head;
    doublyNode* previous;
    doublyNode* next;
};
struct singlyNode{
    string fileName;
    string fileVersion; // Name of file in .minigit folder
    int fileVersionN = 0;//File version of the .minigit file
    singlyNode* next;
};
class MiniGit{
    public:
        MiniGit();//Creates .minigit folder
        ~MiniGit();//Deletes .minigit folder
        void init();//Runs constructor and creates commitHead
        void add(string fileName);//Add a file to the singly linked list
        void rm(string fileName);//Remove a file from the singly linked list
        void commit();//Commit files and insert into .minigit folder
        void checkout(int cNumber);//Checkout a certain version of the commit 
        bool cNumberHelper();//A helper function to lockout add/rm/commit functionality
    private:
        doublyNode* commitHead;//The head of the doubly linked list
        int commitNumberHelper;//Used in cNumberHelper
};

#endif