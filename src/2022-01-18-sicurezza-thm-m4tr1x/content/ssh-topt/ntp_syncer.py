from time import ctime
import ntplib

import time
import os

try:
    import ntplib
    client = ntplib.NTPClient()
    response = client.request('192.168.10.0') #IP of linux-bay server
    print(response)
    os.system('date ' + time.strftime('%m%d%H%M%Y.%S',time.localtime(response.tx_time)))
except:
    print('Could not sync with time server.')

print('Done.')
