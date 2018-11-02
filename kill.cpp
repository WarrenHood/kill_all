#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <thread>
#include <windows.h>
#include <limits>
#define GreenStatus 0x2F
#define RedStatus 0x4F
using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
vector<char*> targets;
vector<char*> target_procs;
vector<char*> whitelist;
vector<thread> threads;
int status_col = GreenStatus;
char status_message[256];
bool kill_running = true;
void set_status(char* msg,bool error){
	if(error){
	sprintf(status_message,"ERROR: %s\n",msg);
	status_col = RedStatus;
	}
	else {
		sprintf(status_message,"%s\n",msg);
		status_col = GreenStatus;
	}
}
void WaitEnter() {
	system("pause");
}
bool contains(const vector<char*>& v,char* name){
	for(char* x : v){
		if( strcmp(x,name) ==0 )return true;
	}
	return false;
}
void whitelist_range(char* basename,int min,int max){
	char* target;
	for(int i=min; i<= max; i++){
		target = (char*) malloc(sizeof(char)*512);
		sprintf(target,"%s%d",basename,i);
		if(contains(whitelist,target))continue;
		whitelist.push_back(target);
	}
}
void add_target(char* name){
	if(contains(targets,name))return;
	char *cname = (char*) malloc(sizeof(char)*512);
	sprintf(cname,"%s",name);
	targets.push_back(cname);
}
void add_range(char* basename,int min,int max){
	char* target;
	for(int i=min; i<= max; i++){
		target = (char*) malloc(sizeof(char)*512);
		sprintf(target,"%s%d",basename,i);
		if(contains(targets,target))continue;
		add_target(target);
	}
}
void add_proc(char* name){
	if(contains(target_procs,name))return;
	char *cname = (char*) malloc(sizeof(char)*512);
	sprintf(cname,"%s",name);
	target_procs.push_back(cname);
}
void whitelist_target(char* name){
	if(contains(whitelist,name))return;
	char *cname = (char*) malloc(sizeof(char)*512);
	sprintf(cname,"%s",name);
	whitelist.push_back(cname);
}
void list_targets(){
	system("cls");
	printf("Targets:\n");
	for(auto x : targets)cout << x << endl;
	char n;
	WaitEnter();
	set_status(" ",false);
	status_col = 0;
}
void list_procs(){
	system("cls");
	printf("Targets:\n");
	for(auto x : target_procs)cout << x << endl;
	WaitEnter();
	set_status(" ",false);
	status_col = 0;
}
void list_whitelist(){
	system("cls");
	printf("Targets:\n");
	for(auto x : whitelist)cout << x << endl;
	WaitEnter();
	set_status(" ",false);
	status_col = 0;
}
void restart_target(char* name){
	char command[300];
	sprintf(command,"@echo off & shutdown /r /f /t 0 /m \\\\%s >nul 2>nul",name);
	system(command);
}
void shutdown_target(char* name){
	char command[300];
	sprintf(command,"@echo off & shutdown /s /f /t 0 /m \\\\%s >nul 2>nul",name);
	system(command);
}
void tkill_target(char* name,char* proc){
	char command[300];
	sprintf(command,"@echo off & taskkill /S %s /IM %s* /F >nul 2>nul",name,proc);
	system(command);
}
void tkill_target_loop(char* name,char* proc){
	kill_running = false;
	char command[300];
	sprintf(command,"@echo off & taskkill /S %s /IM %s* /F >nul 2>nul",name,proc);
	while(!kill_running)system(command);
}
void charpVecClear(vector<char*>& v){
	for(char* cp : v)free(cp);
	v.clear();
}
void menu_show(){
	system("cls");
	int col = 2;
	SetConsoleTextAttribute(hConsole, col);
	printf("                              Project WinKill\n");
	col = 15;
	SetConsoleTextAttribute(hConsole,col);
	printf("0) Exit\n");
	printf("1) List Targets\n");
	printf("2) Add Target\n");
	printf("3) Add Targets(Range)\n");
	printf("4) Clear all targets\n");
	printf("5) List Whitelist\n");
	printf("6) Whitelist Target\n");
	printf("7) Whitelist Targets(Range)\n");
	printf("8) Clear whitelist\n");
	printf("9) List target processes\n");
	printf("10) Add processes to kill\n");
	printf("11) Clear processes list\n");
	printf("12) Kill processes on targets\n");
	printf("13) Kill processes on targets(loop)\n");
	printf("14) Stop all process killing loops\n");
	printf("15) Restart targets\n");
	printf("16) Shutdown Targets\n");
	col = 4 + 15*16;
	SetConsoleTextAttribute(hConsole, col);
	printf("                 Targets:%d                          Whitelist:%d                 \n",targets.size(),whitelist.size());
	SetConsoleTextAttribute(hConsole,status_col);
	printf("%s",status_message);
	col = 10;
	SetConsoleTextAttribute(hConsole, col);
	printf("WinKill>");
	int choice;
	char base[512];
	char target[512];
	int min;
	int max;
	int choices = 16;
	scanf("%d",&choice);
	if(choice < 0 || choice > choices)set_status("Invalid choice",true);
	else if(choice == 0)std::exit(0);
	else if(choice == 1)list_targets();
	else if(choice == 2){
		printf("Enter target name:");
		scanf("%s",target);
		printf("\n");
		add_target(target);
		set_status("Target has been added",false);
	}
	else if(choice == 3){
		printf("Enter base name:");
		scanf("%s",base);
		printf("\nEnter range min:");
		scanf("%d",&min);
		printf("\nEnter range max:");
		scanf("%d",&max);
		printf("\n");
		add_range(base,min,max);
		set_status("Targets have been added",false);
	}
	else if(choice == 4){
		charpVecClear(targets);
		set_status("Targets have been cleared",false);
		status_col = RedStatus;
	}
	else if(choice == 5)list_whitelist();
	else if(choice == 6){
		printf("Enter target name:");
		scanf("%s",target);
		printf("\n");
		whitelist_target(target);
		set_status("Target has been whitelisted",false);
	}
	else if(choice == 7){
		printf("Enter base name:");
		scanf("%s",base);
		printf("\nEnter range min:");
		scanf("%d",&min);
		printf("\nEnter range max:");
		scanf("%d",&max);
		printf("\n");
		whitelist_range(base,min,max);
		set_status("Targets have been whitelisted",false);
	}
	else if(choice == 8){
		charpVecClear(whitelist);
		set_status("Whitelist cleared",false);
		status_col = RedStatus;
	}
	else if(choice == 9)list_procs();
	else if(choice == 10){
		printf("Enter process name:");
		scanf("%s",target);
		printf("\n");
		add_proc(target);
		set_status("Target added",false);
	}
	else if(choice == 11){
		charpVecClear(target_procs);
		set_status("Process list has been cleared",false);
		status_col = RedStatus;
	}
	else if(choice == 12){
		for(char* targ : targets)if(!contains(whitelist,targ)){
			for(char* proc : target_procs){
				thread(tkill_target,targ,proc).detach();
			}
		}
		set_status("Processes have been killed on remote targets",false);
		status_col = RedStatus;
	}
	else if(choice == 13){
			for(char* targ : targets)if(!contains(whitelist,targ)){
				for(char* proc : target_procs)threads.emplace_back(tkill_target_loop,targ,proc);
			}
		set_status("Running process killer",false);
	}
	else if(choice == 14){
		kill_running = true;
		for (thread& t : threads)t.join();
		threads.clear();
		set_status("Stopped process killer",false);
		status_col = RedStatus;
	}
	else if(choice == 15){
		for(char* targ : targets)if(!contains(whitelist,targ)){
			thread(restart_target,targ).detach();
		}
		set_status("Targets have been restarted",false);
		status_col = RedStatus;
	}
	else if(choice == 16){
		for(char* targ : targets)if(!contains(whitelist,targ)){
			thread(shutdown_target,targ).detach();
		}
		set_status("Targets have been shut down",false);
		status_col = RedStatus;
	}
	col = 15;
	SetConsoleTextAttribute(hConsole, col);
}
int main(int argc, char** argv){
	while(1)menu_show();
}