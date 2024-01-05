from datetime import datetime, timedelta
import time
import subprocess
from hashlib import sha256

#shared secret token for OTP calculation
sharedSecret = 0

def TimeSet(country, hours, mins, seconds):
    now = datetime.now() + timedelta(hours=hours, minutes=mins)
    #time units: day, hour, minutes
    CurrentTime = int(now.strftime("%d%H%M"))
    print(country+' =')
    print((now.strftime("Time: %H:%M:%S")))
   
    OTP = (int(CurrentTime)) 
    
    # hash OTP
    hash = (sha256(repr(OTP).encode('utf-8')).hexdigest())
    truncatedOTP = hash[22:44]
    # truncate OTP
    print('OTP: ' + truncatedOTP)

while True:
    print('---------------------------------')
    print('Virtual Time Simulator Alpha 1.5 ')
    print('---------------------------------')
    print('     Updates every minute:       ')
    print('---------------------------------')
    TimeSet('Ukraine', 4, 43, 0)
    print('\n')

    TimeSet('Germany', 13, 55, 0)
    print('\n')

    TimeSet('England', 9, 19, 0)
    print('\n')
    
    TimeSet('Nigeria', 1, 6, 0)
    print('\n')
    
    TimeSet('Denmark', -5, 18, 0)
    
    # keep checking every second - for each passing minute, change OTP code
    time.sleep(1)
    subprocess.call("clear")



