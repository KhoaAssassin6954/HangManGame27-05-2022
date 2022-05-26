#include<iostream>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<thread>
#include<chrono>
#include<map>
using namespace std;
int highscore();
char clues(string,string);
int mainload();
string chooseword(int);
void maingame(string,int&,int&,float&);
void newhighscore(int);
int tutorial();
void lose(string,int&,float&);
void draw(int);

int score;
int wrong;
float t;
main()
{
    system("cls");
    menu: // menu ban dau voi cac lua chon
    cout<<"Please choose your option: "<<endl<<endl;
    cout<<"1-Play the game"<<endl;
    cout<<"2-View the High Score"<<endl;
    cout<<"3-How to play"<<endl;
    cout<<"4-Exit"<<endl<<endl;
    cout<<"Your choice: ";
    string choice;
    int option;
    cin>>choice;
    if(choice=="1") option=mainload();
    else if(choice=="2") option=highscore();
    else if(choice=="3") option=tutorial();
    else if(choice=="4"){
        system("cls");
        return 0;
    }else{
        system("cls");
        cout<<"There is no such option :<...yet :v"<<endl;
        goto menu;
    }
    if(option==1){
        system("cls");
        goto menu;
    }
}
int highscore() // luu ket qua vao file highscore
{
    system("cls");
    fstream infile;
    infile.open("highscores.txt",fstream::in|fstream::out);
    if( infile.peek() == ifstream::traits_type::eof() )
    {
        cout<<"There aren't any highscore yet so go make some :3";
        this_thread::sleep_for(chrono::seconds(2));
        return 1;
    }
    int point,places=1;
    string name,temp;
    map <int,string> mymap;
    while(!infile.eof())
    {
        infile>>point;
        infile>>name;
        mymap.insert({point,name});
    }
    cout<<"Rank Name Score"<<endl;
    for(auto i=mymap.rbegin();i!=mymap.rend();i++)
    {
        if(places>10){break;}
        cout<<places++<<"."<<i->second<<' '<<i->first<<endl;
    }
    infile.close();
    cout<<endl<<"Go back to menu?"<<endl;
    cin>>temp;
    if(temp=="Happy")
    {
        system("cls");
        infile.open("highscores.txt",fstream::out|fstream::trunc);
        cout<<"Successfuly clear the highscore! Returning to the main menu..."<<endl;
        this_thread::sleep_for(chrono::seconds(2));
    }
    return 1;
}
char clues(string game,string mask) //ham lay ra goi i sau khi doan sai voi constructive tu
{
    again:
    int random=rand()%game.length();
    char letter;
    if(mask[random]=='-') letter=game[random];
    else goto again;
    return letter;
}
int mainload() //main cua game
{
    begin:
    system("cls");
    srand(time(NULL));
    int diff,choice;
    string game;char YN;
    cout<<"Choose the difficulty of the word: "<<endl<<endl; //chon do kho
    cout<<"1-Very Easy"<<endl<<"2-Easy"<<endl<<"3-Normal"<<endl<<"4-Hard"<<endl<<"5-Very Hard"<<endl<<"6-Insane"<<endl<<endl;
    cout<<"Your choice?: ";
    cin>>diff;
    system("cls");
    game=chooseword(diff);// lay tu trong file
    //game = "dogie";
    maingame(game,score,wrong,t);// chay game
    system("cls");
    //xu li ket qua va in ra man hinh tuong xung
    if(time <0){
        cout<<"TIME OUT"<<endl<<"YOU LOST";
    }
    if (wrong >= 6|| t <= 0) lose(game,score,t);
    else if(score<0) lose(game,score,t);
    else{
        cout<<"You Win! The word is: "<<game<<endl;
        cout<<"And your final score is: "<<score<<endl;
    }
    // sau khi xong 1 game hoi co muon quay xe khong hay save highscore
    option:
    if(score!=0){
        cout<<"Would you like to save your score? (Y/N):";cin>>YN;
        if(YN=='y'||YN=='Y'){newhighscore(score);system("cls");}
        system("cls");
    }
    cout<<"Would you like to try again? or go to the main menu?"<<endl;
    cout<<"1-Retry"<<endl;
    cout<<"2-Main Menu"<<endl;
    cin>>choice;
    if(choice==1) goto begin;
    else if(choice==2) return 1;
    else{
        system("cls");
        cout<<"There is no such option you dummy"<<endl;
        goto option;
    }
}
string chooseword(int x) //chon tu trong file data
{
    ifstream infile;
    infile.open("data.txt");
    vector<string>y;
    string z;
    while(!infile.eof()){
        infile>>z;
        int a=z.length();
        if(a==x+2&&x!=6) y.push_back(z);
        else if(x==6&&a>=x)y.push_back(z);
        else continue;
    }
    return y[rand()%y.size()];
}
void maingame(string game,int& score,int& wrong,float& t ) //src main cua game
{
    bool stop=false,used_clue=false;
    string mask,usedword,bigplay;
    char guess, help,YN;
    int consecutive=0, right_consecutive=0;
    score = 0, wrong = 0;
    float limit = game.length() * 10.0;
    for(unsigned int i=0;i<game.length();i++) mask+='-'; // tao mot string tuong trung cho day ---- de dien tu
    // hoi xem nguoi choi co muon doan' mot phat' an luon khong
    cout<<mask<<endl;
    cout<<"Would you like to guess the whole word? Success will grant you 100 points per word (Y/N): ";
    cin>>YN;
    cout<<endl;
    if(YN=='y'||YN=='Y')
    {
        cout<<"The word is?: ";
        cin>>bigplay;
        if(bigplay==game){
            score = game.length()*100;
            wrong = 0;
            return;
        }else{
            score =  -1;
            wrong = 7;
            return;
        }
    }

    //luu moc thoi gian
    float start = clock()/CLOCKS_PER_SEC;
    float finish = clock()/CLOCKS_PER_SEC;

    //xu li game
    do
    {
        finish = clock()/CLOCKS_PER_SEC;
        t = limit - finish + start;
        if(wrong==7||mask==game){stop=true;}
        system("cls");
        draw(wrong);
        cout<<"You have "<<limit - (finish - start)<<" second left"<<endl;
        cout<<"Your current score: "<<score<<endl;
        cout<<"Guessed word: "<<usedword<<endl;
        if(consecutive==3)
        {
            cout<<"Would you like a clue? Note that using clue won't give you points (Y/N): ";
            cin>>YN;
            if(YN=='y'||YN=='Y'){help=clues(game,mask);used_clue=true;score-=10;}
        }
        if(used_clue){cout<<"Your clue was that there is a letter: "<<help<<" in the word"<<endl;}
        cout<<mask<<endl<<"Your guess?: ";
        cin>>guess;
        bool correct = false;
        for(unsigned int i=0;i<game.length();i++)
        {
            if(game[i]==guess||game[i]==guess+32||game[i]==guess-32)
            {
                if(mask[i]==game[i]){correct=true;break;}
                mask[i]=game[i];
                correct=true;
                consecutive=0;
                score+=10;
                score+=right_consecutive;
                right_consecutive++;
            }
        }
        if(!correct){usedword+=guess;usedword+=' ';wrong++;consecutive++;right_consecutive=0;}
        finish = clock()/CLOCKS_PER_SEC;
        t = limit - finish + start;
        if(wrong==7||mask==game){stop=true;}
    }while(!stop && finish - start < limit);
    // gan dieu kien thua
    if(t <=0) {
        wrong =7;
        t=-1;
    }
    if(wrong == 7) score *=-1;
}
void newhighscore(int score) // in ra high score
{
    system("cls");
    string name;
    cout<<"Please enter your name: ";
    cin>>name;
    fstream outfile;
    outfile.open("highscores.txt",fstream::in|fstream::out|fstream::app);
    outfile<<score<<' '<<name<<endl;
}
int tutorial()
{
    system("cls");
    string temp;
    cout<<"Here's how you play the game:"<<endl<<endl;
    cout<<"1.You have one chance to guess the whole word but if you are wrong you will instantly lose but"<<endl;
    cout<<"if you are correct you gain 100 points for every single letters in the words"<<endl<<endl;
    cout<<"2.Every times you guesses wrong 3 times consecutively you can use help which reveal a single letters in the word"<<endl<<endl;
    cout<<"3.You can only guess wrong for 7 times total"<<endl<<endl;
    cout<<"4.If you guess correct consecutively you also gain a bonus 5 points"<<endl<<endl;
    cout<<"5.The amout of correct letters in the word also count as a consecutive correct"<<endl<<endl;
    cout<<"6.In the mode number 4 the player choose a word length by entering - according to the length of the word"<<endl<<endl;
    cout<<"Return to the menu?"<<endl;
    cin>>temp;
    return 1;
}
void lose(string game,int& score,float& t) // ham in ra ket qua thua tuy thuoc vao thoi gian hoac loi
{
    int i=0,j=0;
    if(score==-1){score=0;}
    score*=-1;
    while(j<=50)
    {
        system("cls");
        if(t>=0){
            cout<<"You lose! The Word is: "<<game<<endl;
            cout<<"Your final score is: "<<score<<endl;
            if(i==0){
                cout<<"   ------------" <<endl;
                cout << "   |         / " << endl;
                cout << "   |        O  " << endl;
                cout << "   |       /|\\" << endl;
                cout << "   |       / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }else if(i==1){
                cout << "   ------------" << endl;
                cout << "   |          |" << endl;
                cout << "   |          O" << endl;
                cout << "   |         /|\\" << endl;
                cout << "   |         / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }else if(i==2){
                cout << "   ------------" << endl;
                cout << "   |           \\" << endl;
                cout << "   |            O" << endl;
                cout << "   |           /|\\" << endl;
                cout << "   |           / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }else if(i==3){
                cout << "   ------------" << endl;
                cout << "   |          |" << endl;
                cout << "   |          O" << endl;
                cout << "   |         /|\\" << endl;
                cout << "   |         / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }
        }
        else{
            cout<<"TIME OUT"<<endl<<"YOU LOST"<<endl;
            if(i==0){
                cout<<"   ------------" <<endl;
                cout << "   |         / " << endl;
                cout << "   |        O  " << endl;
                cout << "   |       /|\\" << endl;
                cout << "   |       / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }else if(i==1){
                cout << "   ------------" << endl;
                cout << "   |          |" << endl;
                cout << "   |          O" << endl;
                cout << "   |         /|\\" << endl;
                cout << "   |         / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }else if(i==2){
                cout << "   ------------" << endl;
                cout << "   |           \\" << endl;
                cout << "   |            O" << endl;
                cout << "   |           /|\\" << endl;
                cout << "   |           / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }else if(i==3){
                cout << "   ------------" << endl;
                cout << "   |          |" << endl;
                cout << "   |          O" << endl;
                cout << "   |         /|\\" << endl;
                cout << "   |         / \\" << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "   |           " << endl;
                cout << "-------        " << endl;
            }
        }
        i++;j++;
        this_thread::sleep_for(chrono::milliseconds(25));
        if(i==4) i=0;
    }
}
void draw(int wrong) // ham ve khung hangman khi doan tu
{
    if(wrong==0){
        cout << "   ------------" << endl;
        cout << "   |          |" << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "-------        " << endl;
        cout<<endl;
        }
    else if(wrong==1){
        cout << "   ------------" << endl;
        cout << "   |          |" << endl;
        cout << "   |          O" << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "-------        " << endl;
        cout<<endl;
        }
    else if(wrong==2){
        cout << "   ------------" << endl;
        cout << "   |          |" << endl;
        cout << "   |          O" << endl;
        cout << "   |          |" << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "-------        " << endl;
        cout<<endl;
        }
    else if(wrong==3){
        cout << "   ------------" << endl;
        cout << "   |          |" << endl;
        cout << "   |          O" << endl;
        cout << "   |         /|" << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "-------        " << endl;
        cout<<endl;
        }
    else if(wrong==4){
        cout << "   ------------" << endl;
        cout << "   |          |" << endl;
        cout << "   |          O" << endl;
        cout << "   |         /|\\" << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "-------        " << endl;
        cout<<endl;
        }
    else if(wrong==5){
        cout << "   ------------" << endl;
        cout << "   |          |" << endl;
        cout << "   |          O" << endl;
        cout << "   |         /|\\" << endl;
        cout << "   |         / " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "-------        " << endl;
        cout<<endl;
        }
    else if(wrong==6){
        cout << "   ------------" << endl;
        cout << "   |          |" << endl;
        cout << "   |          O" << endl;
        cout << "   |         /|\\" << endl;
        cout << "   |         / \\" << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "   |           " << endl;
        cout << "-------        " << endl;
        cout<<endl;}
}
