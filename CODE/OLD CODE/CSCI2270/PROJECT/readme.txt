CSCI 2270 PROJECT DESCRIPTION:

PHASE 1:

To compile the program enter the command: g++ -std=c++17 driver.cpp miniGit.cpp -o minigit
To run the program enter the command: ./minigit

Make sure all files are in the same folder you are running the program in.

Options and Functionality:
1. init
    Initializes the repository by creating the .minigit folder and creating a doublyNode.
2. Add files to the current directory
    Enter a file to be added to the repository within the singlyNode.
3. Remove files from the current commit
    Enter a file to be removed from the repository within the singlyNode.
4. Commit
    Commits files added to the singlyNode list and pushes them to the .minigit repository. A deepcopy of the singlyNode will be created as well as a new doublyNode to represent the new commit.
5. Check out
    Enter a commit number to revert changes to listed files within that commit. To allow for add/remove/commit functionality, checkout to the most recent commit.
6. Exit
    Exits the program and deletes the .minigit repository and all changes made within it. 

