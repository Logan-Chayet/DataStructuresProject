#include "miniGit.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <thread>

using namespace std;
using namespace std::chrono_literals;//Used for delaying cout statements so menu flows nicely

int main(){
    MiniGit* MG;//Calling in the MiniGit functions
    doublyNode* temp;

    bool quit = false;
    bool initialized = false;
    string choice;
    int option = 0;

    while(!quit){//Loop menu until user wants to quit on option 6
        cout << "======Main Menu======" << endl;
        cout << "1. init" << endl;
		cout << "2. Add files to the current directory" << endl;
		cout << "3. Remove files from the current commit" << endl;
        cout << "4. Commit" << endl;
		cout << "5. Check out" << endl;
        cout << "6. Exit" << endl;
        cout << "+-------------------------+" << endl;
        cout << "#> ";
    
        getline(cin, choice);
        option = stoi(choice);

        switch(option){
            case 1:{
                if(!initialized){//Making sure .minigit is initialized so user can commit changes
                    initialized = true;//When init called, allows for the rest of the menu to work
                    MG = new MiniGit();//Calls constructor and gets ready for init() function
                    MG->init();
                }
                break;
            }
            case 2:{
                if(initialized){//Making sure .minigit is initialized so user can commit changes
                    if(MG->cNumberHelper() == true){
                        string fileName;
                        cout << "Please enter a file name: ";
                        getline(cin, fileName);
                        MG->add(fileName);//Call add function
                    }
                    else{
                        cout << "Option locked because of checkout version!" << endl;//Locking user out because not checked out in current commit
                        cout << "To return to full functionality please return to most recent checkout" << endl;
                        std::this_thread::sleep_for(1s);
                    }
                }
                else{
                    cout << "MiniGit not intialized! Kindly press option 1." << endl;
                    std::this_thread::sleep_for(1s);
                }
                break;
            }
            case 3:{
                if(initialized){//Making sure .minigit is initialized so user can commit changes
                    if(MG->cNumberHelper() == true){
                        string fileName;
                        cout << "Please enter a file name: ";
                        getline(cin, fileName);
                        MG->rm(fileName);
                    }
                    else{
                        cout << "Option locked because of checkout version!" << endl;//Locking user out because not checked out in current commit
                        cout << "To return to full functionality please return to most recent checkout" << endl;
                        std::this_thread::sleep_for(1s);
                    }
                }
                else{
                    cout << "MiniGit not intialized! Kindly press option 1." << endl;
                    std::this_thread::sleep_for(1s);
                }
                break;
            }
            case 4:{
                if(initialized){//Making sure .minigit is initialized so user can commit changes
                    if(MG->cNumberHelper() == true){
                        MG->commit();
                        cout << "Commiting..." << endl;
                        std::this_thread::sleep_for(0.5s);
                        cout << "Files committed!" << endl;
                        std::this_thread::sleep_for(0.5s);
                    }
                    else{
                        cout << "Option locked because of checkout version!" << endl;//Locking user out because not checked out in current commit
                        cout << "To return to full functionality please return to most recent checkout" << endl;
                        std::this_thread::sleep_for(1s);
                    }
                }
                else{
                    cout << "MiniGit not intialized! Kindly press option 1." << endl;
                    std::this_thread::sleep_for(1s);
                }
                break;
            }
            case 5:{
                if(initialized){//Making sure .minigit is initialized so user can commit changes
                    string number = "";
                    cout << "Enter a commit number: ";
                    getline(cin, number);
                    MG->checkout(stoi(number));
                }
                else{
                    cout << "MiniGit not intialized! Kindly press option 1." << endl;
                    std::this_thread::sleep_for(1s);
                }   
                break;
            }
            case 6:{
                quit = true;
                MG->~MiniGit();//Delete the .minigit folder and exit program
            }
        }
    }
    cout << "Goodbye!" << endl;
    return 0;
}