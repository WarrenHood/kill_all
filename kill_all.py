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
params = "/s /p /f"
user_params = input("Enter shutdown params(default: /s /p /f): ").strip()
if len(user_params) > 0:
    params = user_params
def pad_if_1(n):
    if len(str(n)) == 1:
        return "0"+str(n)
    else:
        return str(n)
def shutdown(ws,params):
    os.system("shutdown "+params+" /m \\\\"+ws)
for base in BASE_NAMES:
    for ws_id in range(MIN_NUM,MAX_NUM+1,1):
        ws_name = base+pad_if_1(ws_id)
        if OWN_BASE > -1 and OWN_WS > -1 and BASE_NAMES[OWN_BASE] == base and ws_id == OWN_WS:
            print("\nSkipping",ws_name)
            continue
        print("\nTarget:",ws_name)
        print("Executing shutdown with params:",params)
        threading.Thread(target=shutdown,args=(ws_name,params)).start()
    
