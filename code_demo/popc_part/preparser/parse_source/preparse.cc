#include <iostream>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

void fokpid(char *argument[3],string *tmp, int tmpSize);


int main (int argc, char *argv[])
{
	cout<<"argument size : "<<argc<<endl;
	int size = argc-1;
	string tmparg[size];
	for (int k=1;k<argc;++k)
	{
		tmparg[k-1]=argv[k];
	}

	//1step begin
	char* argument[3];
	argument[0] = "./1step";
	argument[1] =  const_cast<char*> (tmparg[0].c_str());
	argument[2] = (char *)0;

	fokpid(argument,tmparg,size);

	//2step begin
	string renameSec[size];
	for (int k=1;k<argc;++k)
	{
		string tmp;
		int ind;
		tmp = tmparg[k-1];
		ind = tmp.find('.',0);
		string front = tmp.substr(0,ind);
		string rear = tmp.substr(ind,tmp.size());

		char num='1';
		string newname=front+"_"+num+rear;
	
		cout<<"newName is "<<newname<<endl;
		//const char *name = newname.c_str();
		//renameSec[k-1] = const_cast<char*> (name);
		renameSec[k-1] = newname;
		cout<<"Char* is "<<renameSec[k-1]<<endl;
	}

	char* argumentS[3];
	argumentS[0] = "./2step";
	argumentS[1] =  const_cast<char*> (renameSec[0].c_str());
	argumentS[2] = (char *)0;
	
	cout<<"Start second Parser !"<<endl;
	cout<<"argument 1 : "<<renameSec[0]<<endl;
	fokpid(argumentS,renameSec,size);


	//3step begin
	
	string renameThd[size];
	for (int k=1;k<argc;++k)
	{
		string tmp;
		int ind;
		tmp = tmparg[k-1];
		ind = tmp.find('.',0);
		string front = tmp.substr(0,ind);
		string rear = tmp.substr(ind,tmp.size());

		char num='2';
		string newname=front+"_"+num+rear;
	
		cout<<"newName is "<<newname<<endl;
		//const char *name = newname.c_str();
		//renameSec[k-1] = const_cast<char*> (name);
		renameThd[k-1] = newname;
		cout<<"Char* is "<<renameThd[k-1]<<endl;
	}

	char* argumentT[3];
	argumentT[0] = "./3step";
	argumentT[1] =  const_cast<char*> (renameThd[0].c_str());
	argumentT[2] = (char *)0;
	
	cout<<"Start Third Parser !"<<endl;
	cout<<"argument 1 : "<<renameThd[0]<<endl;
	fokpid(argumentT,renameThd,size);

	//system("rm -rf *_0.* *_1.* *_2.* ");
	//system("rm -rf filename*.* *.json ");
	return 0;
}

void fokpid(char *argument[3], string *tmp, int tmpSize)
{
	int status;
	int index = 0;
	pid_t PID = 0;
	PID = fork();
	if (PID==0)
	{
		cout<<"argument : "<<argument[1]<<endl;
		execvp (argument[0],argument);
	}	
	else
	{
		waitpid(PID, &status, 0);
		std::cout << "Process returned " << WEXITSTATUS(status) << ".\n";
		do
		{
			index++;
			PID = fork();
		
			if (PID==0)
			{
				argument[1] =  const_cast<char*> (tmp[index].c_str());

				cout<<"argument : "<<argument[1]<<endl;
				execvp (argument[0],argument);
			}
			else
			{
				waitpid(PID, &status, 0);
				std::cout << "Another Process returned " << WEXITSTATUS(status) << ".\n";
			}	
		}while(index!=(tmpSize-1));
	
	}


}

