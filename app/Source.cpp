#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
using namespace std;

int main()
{
    //get all csv file from folder
    //this might only work for C++ above 17 version
    //to change the C++ version in Visual Studio go to - under project > Properties > C/C++ > Language > C++ Language Standard
    //change it to the C++17 and apply change.
    string filePath;
    vector<string> allTicketFiles;
    //path to the folder where all csv tickets files are stored(make sure you only have csv files)
    string path = "C:\\Users\\jinyc\\Downloads\\project224\\powerline_db\\EB_tickets";
    for (const auto& entry : filesystem::directory_iterator(path)) {
        filePath = entry.path().string();
        cout << filePath << std::endl;
        allTicketFiles.push_back(filePath);//store all files in this vector
    }
    //loop through all the files and extarct the content of each file
    for (int i = 0; i < int(allTicketFiles.size()); i++) {
        ifstream inFile;
        string filePath = allTicketFiles.at(i);
        //need to handle first 3 lines differently 
        int header;//to indentify 1st line 
        int secondLine;//to indentify 2nd line 
        int thirdLine;//to identify 3rd line
        string line;
        string time;
        string remark;
        string address;
        string address1;
        string fullAddress;//to store full address
        string status;//to store 3rd line
        string word;

        vector<string> Line;
        vector<string> Time;
        vector<string> Remark;
        inFile.open(filePath);
        if (inFile.is_open()) {
            cout << endl << endl << "File Number: " << i << endl;
            cout << "Reading: " << filePath<< " File" << endl;
        }
        else {
            cout << "Failed to open: " << filePath << endl;
        }

        header = 1;
        secondLine = 1;
        thirdLine = 1;
        while (!inFile.eof()) {
            if (header == 1) {
                getline(inFile, line, ',');
                getline(inFile, time, ',');
                getline(inFile, remark, '\n');
                header = 0;
                cout << line + " " + time + " " + remark << endl;
            }
            else if (secondLine == 1) {
                secondLine = 0;
                //get the content of 2nd line
                getline(inFile, line, ',');
                getline(inFile, time, ',');
                getline(inFile, remark, ',');
                getline(inFile, address, ',');
                getline(inFile, address1, '\n');
                fullAddress = address + address1;
                cout << "2nd line is: " << time + " " + remark + " " + fullAddress << endl;
            }
            else if (thirdLine == 1) {
                getline(inFile, status);//you can store this status value in database
                cout << "3rd line is: " << status<< endl;
                thirdLine = 0;
            }
            else {//other remaining lines
                while (getline(inFile, word)) {//this loop will continue till end of the line
                    stringstream str(word);
                    getline(str, line, ',');
                    getline(str, time, ',');
                    getline(str, remark, '\n');
                    Line.push_back(line);//store line number
                    Time.push_back(time);//store time
                    Remark.push_back(remark);//store comment
                }
                cout << "Total comments are : " << line << endl;
            }
        }
        for (int i = 0; i < int(Line.size()); i++) {
            cout << "Line: " << Line.at(i) << ' ';
            cout << "Time: " << Time.at(i) << ' ';
            cout << "Remark: " << Remark.at(i) << endl;
        }
        inFile.close();
        //system("pause");
    }
    return 0;
}