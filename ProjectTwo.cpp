//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Zachary LaFlair
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Course {
    string courseId; // unique identifier
    string title;
    string preq1;
    string preq2;
    //string fund;
    //double amount;
    Course() {
        preq1 = ""; // initializes prereqs to blanks they're filled in later if they have a val in the csv
        preq2 = "";
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class CourseList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Course course;
        Node* next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Course course) {
            course = course;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    CourseList();
    virtual ~CourseList();
    void Append(Course course);
    void Prepend(Course course);
    void PrintList();
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
};

/**
 * Default constructor
 */
CourseList::CourseList() {
    // FIXME (1): Initialize housekeeping variables - DONE
    head = nullptr; // initialize a head as nothing and points to nothing
    tail = nullptr; // initialize a tail as nothing and points to nothing
    size = 0;
}

/**
 * Destructor
 */
CourseList::~CourseList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void CourseList::Append(Course course) {
    // FIXME (2): Implement append logic
    //Create new node - call it newNode
    Node* newNode = new Node;  // define a new node to be used locally for the append
    newNode->course = course;  // newnodes course is the course past in
    newNode->next = nullptr;  // it points at nothing
    if (head == nullptr) {  // if the head is nothing the new node is both the head and the tail
        head = newNode;
        tail = newNode;
        ++size;
    }
    else {
        tail->next = newNode;  // otherwise the tail's next node points at the newNode
        tail = newNode;  // then the newNode becomes the new tail
        ++size;
    }
}

/**
 * Prepend a new bid to the start of the list
 */
void CourseList::Prepend(Course course) {
    // FIXME (3): Implement prepend logic
    Node* newNode = new Node;

    if (head != NULL) {
        newNode->next = head; // new node points to current head as its next node

    }

    head = newNode;  // regardless the newNode is now head of the list and only points to the old head if something was there (addressed in if statement above)
    ++size;

}

/**
 * Simple output of all bids in the list
 */
void CourseList::PrintList() {
    // FIXME (4): Implement print logic
    Node* currNode = head;  // start at the head node

    // while loop over each node looking for a match
        //output current bidID, title, amount and fund
        //set current equal to next
    while (currNode != nullptr) {  // while the node doesn't point at nothing HINT: The last node will point at nothing
        cout << currNode->course.courseId << ", ";  // output the details
        cout << currNode->course.title << endl;
        //cout << currNode->bid.amount << " | ";
        //cout << currNode->bid.fund << endl;
        currNode = currNode->next;  // move to the next node
    }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void CourseList::Remove(string courseId) {
    // FIXME (5): Implement remove logic
    
    Node* currNode = head;
    if (currNode == nullptr) { // special case if the head node is the matching case
        delete currNode;
        return ;
    }
    else {
        while (currNode->course.courseId != courseId) { // as long as the current nodes bid Id does not match the bidId we passed in
            currNode = currNode->next;
        }
        delete currNode;
        --size;
    }

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Course CourseList::Search(string courseId) {
    // FIXME (6): Implement search logic
    Node* currNode = head;
    Node* placeHolder = new Node;
    placeHolder->course.courseId = "";
    while (currNode != nullptr) { // while the current node does not equal null
        cout << currNode->course.courseId << endl;  // cout bidId of the currentNode
        if (currNode->course.courseId == courseId) {  // if current node's bidId equals the bidId we passed in
            return currNode->course;  // return the bid at the current node
        }
        currNode = currNode->next;  // current node is now the next node in the list
    }
    return placeHolder->course;  
}

/**
 * Returns the current size (number of elements) in the list
 */
int CourseList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayCourse(Course course) {  // display the course passed in, handle the inputs in getCourse()
    cout << course.courseId << ", " << course.title << endl;
    cout << "Prerequisites: " << course.preq1 << ", " << course.preq2 << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Course getCourse() {
    Course course;

    cout << "What course do you want to know about? ";
    cin.ignore();
    getline(cin, course.courseId);  // gets in some text converts it to a courseId

    //cout << "Enter title: ";
    //getline(cin, course.title);

    //cout << "Enter fund: ";
    //cin >> bid.fund;

    //cout << "Enter amount: ";
    //cin.ignore();
    //string strAmount;
    //getline(cin, strAmount);
    //bid.amount = strToDouble(strAmount, '$');

    return course;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadCourses(string csvPath, CourseList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Course course;
            course.courseId = file[i][0];
            course.title = file[i][1];

            // Handle if the row has 2, 3, or 4 columns...
            try {
                if (file[i][2] != "") {
                    course.preq1 = file[i][2];
                }
            }
            catch (const std::exception&)
            {
            }
            try {
                if (file[i][3] != "") {
                    course.preq2 = file[i][3];
                }
            }
            catch (const std::exception&)
            {
            }


            //bid.fund = file[i][8];
            //bid.amount = strToDouble(file[i][4], '$');

            cout << course.courseId << ": " << course.title << " Pre-requisites: " << course.preq1 << "; " << course.preq2 << endl;  // output so we know we're reading files correctly

            // add this bid to the end
            list->Append(course);  // append the course to the list we passed in
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        //courseKey = "98110";
        break;
    case 3:
        csvPath = argv[1];
        //bidKey = argv[2];
        break;
    default:
        csvPath = "ABCU_Advising_Program_Input.csv";
       // bidKey = "98110";
    }

    clock_t ticks;

    CourseList courseList;

    Course course;

    int choice = 0;
    while (choice != 9) {
        // print the menu
        cout << "Welcome to the course planner." << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
        case 1:  // load data structure
            loadCourses(csvPath, &courseList);
            cout << courseList.Size() << " courses read" << endl;
            break;

        case 2:  // print course list
            courseList.PrintList();
            break;

        case 3:  // lookup a course
        
            string input;

            cout << "What course do you want to know about? ";
            cin.ignore();
            getline(cin, input);

            //course = courseList.Search(input);
            displayCourse(courseList.Search(input));  // display the course searched with our input
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;  // exit statement

    return 0;
}
