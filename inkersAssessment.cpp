#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <ctime>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include<fstream>


using namespace std;
using namespace cv;

//Function for adding gaussian filter.
void gaussianFilter(Mat mat) {
    GaussianBlur(mat, mat, Size(101, 101), 0);
}

bool login() {
    bool flag = false;
    ifstream userDetails;
    userDetails.open("userData.txt");
    string line;
    string name, password;
    cout << "\n\nEnter your name : ";
    cin >> name;
    cout << "\nEnter your password : ";
    cin >> password;
    string namePass = name + " " + password;
    int offset;
    if (userDetails.is_open()) {
        bool flag = false;
        while (!userDetails.eof()) {
            getline(userDetails, line);
            if ((offset = line.find(namePass, 0)) != string::npos) {
                flag = true;
                return true;
            }
        }
    }
    else {
        cout << "Some error occured!!!\n";
        return false;
    }

    if (flag == false) {
        cout << "Inavild Crentials!!!\n";
        cout << "Press 1 to try again and any other to exit : ";
        int op;
        cin >> op;
        if (op == 1) {
            login();
        }
        else {
            return false;
        }
    }

}


bool signUp() {
    ofstream userDetails;
    userDetails.open("userData.txt", ios_base::app);
    if (userDetails.is_open()) {
        string username;
        string password;
        cout << "\nEnter the user name : ";
        cin >> username;
        cout << "\nEnter the password : ";
        cin >> password;
        if (username == "" || password == "") {
            cout << "Invalid Input!!!! \n";
            cout << "Press 1 to try again and any other to exit : ";
            int op;
            cin >> op;
            if (op == 1) {
                login();
            }
            else {
                return false;
            }
        }
        string data = username + " " + password + "\n";
        userDetails << data;
        userDetails.close();
        cout << "Record stored successfully!!!\n";
        return true;

    }
    else {
        cout << "Some error occured!!!\n";
        return false;
    }
}

int main() {

    cout << "---------------VIDEO PROCESSING ASSESSMENT-----------------------\n\n";
    cout << "1         -          LOGIN\n";
    cout << "2         -          SIGNUP\n";
    int ch;
    cout << "Enter your Choice : ";
    cin >> ch;
    bool flag = false;
    if (ch == 1) {
        if (login() == true) {
            flag = true;
        }
        else {
            cout << "\n\n Terminating the program!!!!";
            return 0;
        }
    }
    else if (ch == 2) {
        if (signUp() == true) {
            flag = true;
        }
        else {
            cout << "\n\n Terminating the program!!!!";
        }
    }
    else {
        cout << "Invalid Choice\n";
        cout << "Program Teminating!!!!!!\n";
        return 0;
    }

    if (flag) {

        time_t delayStart;
        time_t delayEnd;
        time(&delayStart);

        while (1) {
            time(&delayEnd);
            double dif = difftime(delayEnd,delayStart);
            cout << "\n\n\n------------------------------------Redirecting to video Part---------------------------------\n";
            if (dif == 2) {
                break;
            }
        }

        time_t start, end;

        VideoCapture vcap(0);
        if (!vcap.isOpened()) {
            cout << "Error opening video stream or file" << endl;
            return -1;
        }

        int frame_width = vcap.get(cv::CAP_PROP_FRAME_WIDTH);
        int frame_height = vcap.get(cv::CAP_PROP_FRAME_HEIGHT);
        VideoWriter video("out.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height), true);

        time(&start);

        vector<Mat> vid;

        // Capturing video from Web Cam.
        while (1) {

            Mat frame;
            //Taking input of the frames
            vcap >> frame;
            //Storing frames in a vector
            vid.push_back(frame);
            imshow("Frame", frame);

            char c = (char)waitKey(33);
            if (c == 27)
                break;
            time(&end);
            double dif = difftime(end, start);
            printf("Capturing Video....\n");

            if (dif == 10) {
                std::cout << "Video Captured Successfully!!" << "\n";
                break;
            }

        }

        //Processing every frame of the video
        for (int i = 0; i < vid.size(); ++i) {

            cout << "Processing!!!!!\n";

            Mat frame = vid[i];
            Mat rgbChannels[3];

            //Spliting the frame into its rgb Channels
            cv::split(frame, rgbChannels);

            //Applying gaussian filter to the red channnel of the frame
            gaussianFilter(rgbChannels[2]);

            vector<Mat> channels = { rgbChannels[0],rgbChannels[1],rgbChannels[2] };

            //Merging the updated rgb channels to the frame.
            cv::merge(channels, frame);

            //Writing the frames to the video.
            video.write(frame);

        }
        cout << "Video Saved Successfully\n";
    }
    waitKey(0);
    return 0;
}
