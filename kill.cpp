#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <thread>
#include <windows.h>
#include <limits>
using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
vector<char*> targets;
vector<char*> target_procs;
vector<char*> whitelist;
vector<thread> threads;
bool kill_running = false;
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
}
void list_procs(){
	system("cls");
	printf("Targets:\n");
	for(auto x : target_procs)cout << x << endl;
	WaitEnter();
}
void list_whitelist(){
	system("cls");
	printf("Targets:\n");
	for(auto x : whitelist)cout << x << endl;
	WaitEnter();
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
	char command[300];
	sprintf(command,"@echo off & taskkill /S %s /IM %s* /F >nul 2>nul",name,proc);
	while(kill_running)system(command);
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
	if(choice < 0 || choice > choices)printf("Invalid choice:%d",choice);
	else if(choice == 0)std::exit(0);
	else if(choice == 1)list_targets();
	else if(choice == 2){
		printf("Enter target name:");
		scanf("%s",target);
		printf("\n");
		add_target(target);
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
	}
	else if(choice == 4)charpVecClear(targets);
	else if(choice == 5)list_whitelist();
	else if(choice == 6){
		printf("Enter target name:");
		scanf("%s",target);
		printf("\n");
		whitelist_target(target);
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
	}
	else if(choice == 8)charpVecClear(whitelist);
	else if(choice == 9)list_procs();
	else if(choice == 10){
		printf("Enter process name:");
		scanf("%s",target);
		printf("\n");
		add_proc(target);
	}
	else if(choice == 11){
		for(char* targ : targets)if(!contains(whitelist,targ)){
			for(char* proc : target_procs){
				thread(tkill_target,targ,proc).detach();
			}
		}
	}
	else if(choice == 12){
		kill_running = true;
			for(char* targ : targets)if(!contains(whitelist,targ)){
				for(char* proc : target_procs)threads.emplace_back(tkill_target_loop,targ,proc);
			}
	}
	else if(choice == 13){
		kill_running = false;
		for (thread& t : threads)t.join();
		threads.clear();
	}
	else if(choice == 14){
		for(char* targ : targets)if(!contains(whitelist,targ)){
			thread(restart_target,targ).detach();
		}
	}
	else if(choice == 15){
		for(char* targ : targets)if(!contains(whitelist,targ)){
			thread(shutdown_target,targ).detach();
		}
	}
	col = 15;
	SetConsoleTextAttribute(hConsole, col);
}
int main(int argc, char** argv){
	while(1)menu_show();
}