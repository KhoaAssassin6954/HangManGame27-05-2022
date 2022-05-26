#include<iostream>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<thread>
#include<chrono>
#include<map>
using namespace std;
main()
{
    system("cls");
    srand(time(NULL));
    string game,temp,first={'E','A','R','I','O','T','N','S','L','C'};
    vector <string> word;
    ifstream infile;
    infile.open("data.txt");
    cout<<"Choose your word carefully player >:D : ";
    cin>>game;
    while(!infile.eof())
    {
        infile>>temp;
        if(temp.length()==game.length()){word.push_back(temp);}
    }
    if(word.empty()){system("cls");cout<<"I give up :< Your words is too hard >:("<<endl;}
    cout<<word[rand()%word.size()];
}