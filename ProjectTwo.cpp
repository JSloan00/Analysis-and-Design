#include <iostream>
#include <fstream>
#include <string>

#include "CSVparser.hpp"

using namespace std;

//forward declarations
double strToDouble(string str, char ch);

//define structure to hold course info
struct Course {
	string courseName;
	string courseNum;
	vector<string>preReqN;

	Course() {}

};

//internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//initialize with course
	Node(Course aCourse) :
		Node() {
		this->course = aCourse;
	}
};

//define class containing data members and methods for BST
class BinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void Insert(Course course);
	void InOrder();
	int NumPreReqs(Course course);
	Course PrintCourseInfo(string courseNum);

};

//default constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

//destructor
BinarySearchTree::~BinarySearchTree() {
	//FIXME: recurse from root deleting every node
}

//insert course info
void BinarySearchTree::Insert(Course course) {
	Node* node = new Node(course);

	//if tree is empty create new root
	if (root == nullptr) {
		root = new Node(course);
		node->left = nullptr;
		node->right = nullptr;
	}
	//else if tree has data
	else {
		Node* cur = root;

		while (cur != nullptr) {
			if (node->course.courseNum < cur->course.courseNum) {
				if (cur->left == nullptr) {
					cur->left = node;
					cur = nullptr;
				}
				else {
					cur = cur->left;
				}
			}
			// if right child of current node is empty, insert bid to right child
			else {
				if (cur->right == nullptr) {
					cur->right = node;
					cur = nullptr;
				}
				else {
					cur = cur->right;
				}
			}
		}
	}
	//new node's child nodes will be empty
	node->left = nullptr;
	node->right = nullptr;
}

int BinarySearchTree::NumPreReqs(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.preReqN.size(); i++) {
		if (course.preReqN.at(i).length() > 0) {
			count++;
		}
		return count;
	}
}

void BinarySearchTree::InOrder() {
	if (root == nullptr) {
		return;
	}
	inOrder(root->left);
	cout << root;
	inOrder(root->right);
}

Course BinarySearchTree::PrintCourseInfo(string courseNum) {
	//set current node equal to root
	Node* current = root;

	//keep looping downwards until bottom reached or courseNum found
	while (current != nullptr) {
		//if match found, return course info
		if (current->course.courseNum.compare(courseNum) == 0) {
			return current->course;
		}
		//if course num is smaller than current node, traverse left
		if (courseNum.compare(current->course.courseNum) < 0) {
			current = current->left;
		}
		//else larger so traverse right
		else {
			current = current->right;
		}
	}
	Course course;
	return course;
}

void BinarySearchTree::addNode(Node* node, Course course) {
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}

void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;
		inOrder(node->right);
	}
	return;
}

void displayCourse(Course course) {
	cout << course.courseNum << ", " << course.courseName << endl;
	return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */

void loadCourses(string csvPath, BinarySearchTree* bst) {
	cout << "Loading file " << csvPath << endl;

	//initialize the CSV parser using given path
	csv::Parser file = csv::Parser(csvPath);
	
	try {
		//loop to read rows of CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			//create data structure and add to collection of courses
			Course course;
			course.courseNum = file[i][0];
			course.courseName = file[i][1];
			course.preReqN.push_back(file[i][2]);
			course.preReqN.push_back(file[i][3]);

			bst->Insert(course);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}

double strToDouble(string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
}


int main(int argc, char* argv[]) {

	//process command line arguments
	string csvPath, courseKey;
	switch (argc) {
	case 2: 
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseKey = argv[2];
		break;
	default:
		csvPath = "courseInfo.csv";
		break;
	}

	//define bst to hold course info
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	cout << "Welcome to the course planner." << endl;


	int choice = 0;
	while (choice != 4) {
		cout << "Menu:" << endl;
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  4. Exit" << endl;
		cout << "What would you like to do? ";
		cin >> choice;

		switch (choice) {
		case 1:
			loadCourses(csvPath, bst);
			break;
		case 2:
			cout << "Here is a sample schedule:" << endl;
			bst->InOrder();
			cout << endl;
			break;
		case 3:			
			if (courseKey.length() == 0) {
				cout << "What course do you want to know about? ";
				cin >> courseKey;
				for (auto& choice : courseKey) {
					choice = toupper(choice);
				}
				bst->PrintCourseInfo(courseKey);

				if (!course.courseNum.empty()) {
					displayCourse(course);
				}
				else {
					cout << "Course " << courseKey << " not found" << endl;
				}
			}
		}
	}
	cout << "Thank you for using the course planner!" << endl;

	return 0;
}