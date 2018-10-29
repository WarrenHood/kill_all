import os
import threading
BASE_NAMES = input("Enter a space seperated list of base names: ").strip().split()
MIN_NUM = int(input("Min Workstation number: "))
MAX_NUM = int(input("Max Workstation number: "))
print( "Choose your base name:\n\tBase Names:\n"+"0) None\n"+"\n".join([str(i+1)+") " + BASE_NAMES[i] for i in range(len(BASE_NAMES))]))
OWN_BASE = int(input().strip())-1
OWN_WS = -1
if OWN_BASE > -1:
    OWN_WS = int(input("Enter your Workstation number: "))
params = "/r /f /t 0"
TKILL = input("Kill remote processes instead of shutdown?(y/n)").lower()[0] == "y"
TO_KILL = ["chrome","firefox","iexplore",]
if TKILL:
    NEW_TO_KILL = input("Enter proccess names to kill seperated by a space\nDefault:\n"+"\n".join("\t"+TO_KILL[proc_ind] + ("\n" if proc_ind == len(TO_KILL)-1 else "") for proc_ind in range(len(TO_KILL)))).strip().split()
    if len(NEW_TO_KILL) > 0:
        TO_KILL = NEW_TO_KILL
        print("Targeting:\n" + "\n".join("\t"+proc for proc in TO_KILL))
else:
    user_params = input("Enter shutdown params(default: /r /f /t 0): ").strip()
    if len(user_params) > 0:
        params = user_params
LOOP_FOREVER = input("Loop forever?(y/n)").strip().lower()[0] == "y"
def pad_if_1(n):
    if len(str(n)) == 1:
        return "0"+str(n)
    else:
        return str(n)
def shutdown(ws,params,loop=False):
    os.system("@echo off & shutdown "+params+" /m \\\\"+ws)
    while loop:
        os.system("@echo off & shutdown "+params+" /m \\\\"+ws)
def task_kill(ws,loop=False):
    for proc in TO_KILL:
        os.system("@echo off & taskkill /S "+ws+" /IM " + proc + "* /F")
    while loop:
        for proc in TO_KILL:
            os.system("@echo off & taskkill /S "+ws+" /IM " + proc + "* /F")
for base in BASE_NAMES:
    for ws_id in range(MIN_NUM,MAX_NUM+1,1):
        ws_name = base+pad_if_1(ws_id)
        if OWN_BASE > -1 and OWN_WS > -1 and BASE_NAMES[OWN_BASE] == base and ws_id == OWN_WS:
            print("\nSkipping",ws_name)
            continue
        print("\nTarget:",ws_name)
        if not TKILL:
            print("Executing shutdown with params:",params)
        else:
            print("Killing specified processes")
        if LOOP_FOREVER:
            print("Looping...")
        if not TKILL:
            threading.Thread(target=shutdown,args=(ws_name,params,LOOP_FOREVER)).start()
        else:
            threading.Thread(target=task_kill,args=(ws_name,LOOP_FOREVER)).start()
