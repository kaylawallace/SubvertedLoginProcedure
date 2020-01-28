//Delta10z
#include "authlib.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <ctime>
#include <openssl/sha.h>

using namespace std;

string u,p; //initialise username and password strings

//sha256 function taken from https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
string sha256(const string p)
{
    unsigned char hash[SHA256_DIGEST_LENGTH]; 
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, p.c_str(), p.size());
    SHA256_Final(hash, &sha256);
    stringstream ourStream;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ourStream << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ourStream.str();
}


//Method to read in details from the text file
vector<string> readFromFile(){
	vector<string> savedDetails; //vector storing usernames & passwords
	fstream readFile;
	readFile.open("pwdb.txt", ios::in); //start reading the text file "pwdb"
	string currentLine;
	if(readFile.is_open()){
		while (getline(readFile, currentLine)){ //While there is still data to read in the file
			savedDetails.push_back(currentLine); //Add the current line to our vector savedDetails
		}
		readFile.close(); //Close the reader
	}
	else {
		cout << "Error - unable to open file" << endl; //Display error message
	}
	return savedDetails; //Return the vector containing each line in the text file
}


//Method to compare the user input with saved password
void compareLogin(string hashedP, vector<string> savedDetails)
{
	vector<string>::iterator it; //Create an iterator to traverse through our vector
	string enteredLogin = u + ":" + hashedP; //Put user input into same format as text file
	it = find (savedDetails.begin(), savedDetails.end(), enteredLogin); //search vector "savedDetails" for user entry

	if(it != savedDetails.end()){ //If not at the end of the vector
		authenticated(u); //Authenticate user
		exit(0);
	}
	else {
		rejected(u); //Reject user
	}
}


//Method to get the user to enter their username and password   
void getLogin(){
	cout << "Please enter your username :" << endl; //Get username
	cin >> u;

	cout << "Please enter your password :" << endl; //Get password
	cin >> p;

	string hashedPassword = sha256(p); //hash password entered by the user
	vector<string> savedDetails = readFromFile(); //read in saved usernames and passwords from text file
	compareLogin(hashedPassword, savedDetails);//Compare the saved details and the user entry
}


int main() {
	int counter = 0;
	while(counter < 3){ //While user has not had 3 attempts at logging in
		getLogin();
		counter++;
	}

	if (counter == 3){ //If user has entered details incorrectly 3 times display error message and close program
		cout <<"Sorry - maximum amount of tries." << endl;
		exit(0);
	}
}

