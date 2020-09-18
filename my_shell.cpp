#include <bits/stdc++.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

using namespace std;

void ls(string& cmd, stringstream& ss)
{	
    string constraint1,constraint2;
    ss>>constraint1;
    ss>>constraint2;
    if(constraint1=="\0")
    {  
        DIR *dr = opendir("."); 
        struct dirent *de;
	//total 5 values are in diren 
	//file name max of 256
        if (dr == NULL)
        { 
            cout<<"Failed to open current directory"<<endl;
            return; 
        }
	//listing all the file one by one
        while ( (de = readdir(dr)) != NULL )
            if(de->d_name[0]!='.')
                cout<<de->d_name<<"\n";
        cout<<endl;
        closedir(dr);
    }
   else if(constraint1!="-a") //listing file of directory provided in command
   {
        if(constraint1[0]=='-')
           {cout<<"Unavailable command"<<endl;return ;}

        DIR *dr = opendir(constraint1.c_str()); 
        struct dirent *de;

        if (dr == NULL)
        { 
            cout<<"No such directory"<<endl;
            return; 
        }

        while ( (de = readdir(dr)) != NULL )
	    if(de->d_name[0]!='.')
                cout<<de->d_name<<"\n";
        cout<<endl;
        closedir(dr);
   }
   else //when -a attribute is used with ls
   {
        DIR *dr;
        if(constraint2!="\0")
            dr = opendir(constraint2.c_str()); 
        else
            dr = opendir(".");
        struct dirent *de;

        if (dr == NULL)
        { 
            cout<<"No such directory"<<endl;
            return; 
        }

        while ((de = readdir(dr)) != NULL) 
            cout<<de->d_name<<"\n";
        cout<<endl;
        closedir(dr);
   }
}

//function for change directory
void cd(string& cmd, stringstream& ss)
{
    string constraint1,constraint2;
    ss>>constraint1;
    ss>>constraint2;	
    if(constraint2!="\0")
    {
	cout<<"bash: cd: too many arguments"<<endl;
    }
    else if(chdir(constraint1.c_str())!=0) //change if path is correct
    {
	cout<<"bash: cd: No such file or directory"<<endl;
    }
}

//function for cat
void cat(string& cmd, stringstream& ss)
{
    string constraint1,constraint2;
    ss>>constraint1;
    ss>>constraint2;

    if(constraint1!="-n")
    {
	if(constraint1[0]=='-')
        {
            cout<<"Unavailable command"<<endl;return ;
        }
        ifstream file;
	file.open(constraint1);
	if(file.is_open())
	{
            string line;
            while(file)
            {
                getline(file,line);
		if(!file.eof())
                cout<<line<<endl;
            }
            file.close();
	}
	else
            cout<<"No Such File"<<endl;
    }
    else if(constraint1=="-n") //when -n parameter is used
    {
	ifstream file;
	file.open(constraint2);
	if(file.is_open())
	{
            string line;
            int line_no=1; //keep track of line no
            while(file)
            {
                getline(file,line);
		if(!file.eof())
                cout<<"  "<<line_no++<<"  "<<line<<endl;
            }
            file.close();
	}
	else
            cout<<"No Such File"<<endl;
    }
}

//function for make directory
void mkdir(string& cmd, stringstream& ss)
{
    string constraint1;
    ss>>constraint1;
    if(constraint1!="-m")  //when no mode is used
    {
        if(constraint1[0]=='-')
        {
            cout<<"Unavailable command"<<endl;return ;
        }
        int loc=umask(0);
        if (mkdir(constraint1.c_str(),0755) == -1) ;
        cerr<< strerror(errno) << endl;
    }
    else  //case to deal with the mode of the directory
    {
        string constraint3,constraint4;
        ss>>constraint3; //mode provided by user
        ss>>constraint4;
        if(constraint4=="\0")
            cout<<"mkdir: missing operand"<<endl;
        else
        {       
            char input[4];
            input[0]='0';
            if(constraint3.length()>3)
            {cout<<"Invalid Mode"<<endl; return ;}
            else
            {       
                int i;
                for(i=0;i<constraint3.length();i++)
                {
                    if(constraint3[i]>='0'&&constraint3[i]<='7')//mode validation
                    input[i+1]=constraint3[i];
                    else
                    {
                        cout<<"Invalid Mode"<<endl; return ;
                    }
                }
                for(int j=i+1;j<=3;j++)
                    input[j]='0';	
            }
            int temp;
            temp= strtol(input, 0, 8);

            int loc=umask(0); //setting umask to 0 initially
            if (mkdir(constraint4.c_str(),temp) == -1)
                cerr<< strerror(errno) << endl; 
        }
    }
}


void cp(string& cmd, stringstream& ss)
{
    string option;
    string srcFile, destFile;
    bool isUset = false;
    ss>>option;
    //Check if option -u is set
    if(option == "-u")
    {
        ss>>srcFile;
        ss>>destFile;
        isUset = true;
    }
    else if(option[0] == '-')
    {
        cout<<"Unsupported command option"<<endl;
        return;
    }
    else
    {
        srcFile = option;
        ss>>destFile;
    }
    if(srcFile=="" && destFile=="")
    {
        //No src and dest file operands given
        cout<<"cp: missing file operand"<<endl;
    }
    else if(destFile=="")
    {
        cout<<"cp: missing destination file operand after \'"<<srcFile<<"\'"<<endl;
    }
    else
    {
        //Everything correct
        //cout<<"Option: "<<option<<" SrcFile: "<<srcFile<<" DestFile: "<<destFile<<endl;
        //Get source and dest file creation time
        struct stat srcF, destF;
        bool destDne = false;
        bool performCopy = false;
        
        if(stat(srcFile.c_str(), &srcF) < 0)
        {
            //Src file not found
            cout<<"cp: cannot stat \'"<<srcFile<<"\': No such file or directory"<<endl;
            return;
        }
        else if(stat(destFile.c_str(), &destF) < 0)
        {
            //Dest file does not exists
            //Useful when -u option is set
            //cout<<"Dest DNE time: "<<endl;
            destDne = true;
        }
        
        
        ifstream src(srcFile);
        if(isUset)
        {
            //If option -u is set, check for update timestamp 
            if(destDne || (destF.st_mtime < srcF.st_mtime))
                performCopy = true;
        }
        else
        {
            performCopy = true;
        }
        
        //Write code for actual copying
        if(performCopy)
        {
            //cout<<"Perform copy is true"<<endl;
            ofstream dest(destFile);
            dest << src.rdbuf();
            dest.close();
        }
        src.close();    
    }
}


void sort(string& cmd, stringstream& ss)
{
    vector<string> lines;
    vector<string>::iterator it;
    string option, fileName;
    bool reverse = false;
    ss>>option;
    if(option == "-r")
    {
        reverse = true;
        ss>>fileName;
    }
    else if(option[0] == '-')
    {
        cout<<"Unsupported command option"<<endl;
        return;
    }
    else
    {
        fileName = option;
    }
    ifstream input(fileName);
    if(!input.is_open())
    {
        //File does not exists
        cout<<"No such file: "<<fileName<<endl;
        return;
    }
    //Push line in a vector
    for(string line; getline( input, line ); )
    {
        lines.push_back(line);
    }
    //Perform desired sorting
    if(reverse)
    {
        sort(lines.begin(), lines.end(), greater<string>()); 
    }
    else
    {
        sort(lines.begin(), lines.end()); 
    }
    //Print lines
    for(it = lines.begin(); it != lines.end(); it++)
    {
        cout<<*it<<endl;
    }
}


//Grep helper function
//Finite Automata to input pattern as linux does it 
string getPattern(stringstream& ss, bool &isNoption)
{
    string pattern = "";
    char c;
    //Eleminate leading spaces
    while(1)
    {
        ss.get(c);
        if(ss.eof() || c!=' ')
            break;
    }
    if(c=='-')
    {
        char temp = ss.peek();
        if(temp == 'n')
        {
            ss.get(c);
            ss.get(c);
            ss.get(c);
            isNoption = true;
        }
        else
        {
            cout<<"Unsupported command option"<<endl;
            return"";
        }
    }
    while(1)
    {
        //cout<<"Got: "<<c<<endl;
        if(ss.eof())
            break;
        else if(c == '\\')
        {
            ss.get(c);
            pattern.push_back(c);
        }
        else if(c==' ')
            break;
        else if(c=='\'')
        {
            while(1)
            {
                ss.get(c);
                if(ss.eof())
                    break;
                else if(c == '\\')
                    ss>>c;
                else if(c=='\'')
                    break;
                pattern.push_back(c);
            }
        }
        else if(c=='\"')
        {
            while(1)
            {
                ss.get(c);
                if(ss.eof())
                    break;
                else if(c == '\\')
                    ss>>c;
                else if(c=='\"')
                    break;
                pattern.push_back(c);
            }
        }
        else
            pattern.push_back(c);
        ss.get(c);
    }
    return pattern;
}
void grep(string& cmd, stringstream& ss)
{
    bool isNoption = false;
    string pattern = getPattern(ss, isNoption);
    //cout<<"pattern: "<<pattern<<endl;
    if(pattern == "")
        return;
    while(1)
    {
        string fileName;
        int lineNumber = 0;
        ss>>fileName;
        if(fileName =="")
            break;
        //cout<<"File: "<<fileName<<endl;
        ifstream input(fileName);
        if(!input.is_open())
        {
            cout<<"grep: "<<fileName<<": No such file or directory"<<endl;
        }
        for(string line; getline( input, line ); )
        {
            lineNumber++;
            if(line.find(pattern) != string::npos)
            {
                if(isNoption)
                {
                    cout<<lineNumber<<": "<<line<<endl;
                }
                else
                {
                    cout<<line<<endl;
                }
            }
        }
        
    }
}
void executeCommands()
{
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    getlogin_r(username, LOGIN_NAME_MAX);
    
    
    string pwd = get_current_dir_name(), un = username, hn = hostname;
    string nextCmd;
    vector<string> history;
    while(1)
    {
	pwd = get_current_dir_name();
        //string prompt = un + "@" + hn + ":" + pwd + "$ ";
	char prompt[PATH_MAX];
	snprintf(prompt, PATH_MAX, "%s%s@%s%s:%s~%s%s$ ",GREEN,un.c_str(),hn.c_str(),RESET,BLUE,pwd.c_str(),RESET);
	//printf(GREEN);
	//cout<<prompt;
        char *abc = readline(prompt);
	//printf(RESET);
        add_history(abc);
        nextCmd = abc;
	stringstream ss(nextCmd);
        string cmd;
        ss>>cmd;
        if(cmd == "exit")
            break;
        else
        {
            history.push_back(nextCmd);

            if(cmd == "ls")
                ls(cmd, ss);
            else if(cmd == "cd")
                cd(cmd, ss);
            else if(cmd == "cat")
                cat(cmd, ss);
            else if(cmd == "mkdir")
                mkdir(cmd, ss);
            else if(cmd == "cp")
                cp(cmd, ss);
            else if(cmd == "sort")
                sort(cmd, ss);
            else if(cmd == "grep")
                grep(cmd, ss);
            else if(cmd == "clear");
                //clear-screen();
            else
            {
                //Display some error
                //Unsupported command
                cout<<"Unavailable command"<<endl;
            }
        }
    }
}

int main()
{
    string pwd = get_current_dir_name();
    system("clear");
    printf(YELLOW);
    cout<<"----------------Welcome to customized shell implemented in C++----------------"<<endl;
    printf(RESET);
    
    cout<<"Author: Ankit Agrawal(194101006) and Sonu Singh (194101049)\n\n\n"<<endl;
    executeCommands();
    return 0;
}
