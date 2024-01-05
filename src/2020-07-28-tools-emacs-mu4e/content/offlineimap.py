#!/usr/bin/env python3

import os
import subprocess

# NOTE: change this
email_to_index = {
    "mu4e.example@gmail.com": 0,
    "mu4e.example@outlook.it": 1,
}

def mailpasswd(acct):
    acct = os.path.basename(acct)
    path = "/home/leo/emails.gpg" # NOTE: change this
    args = ["gpg", "--use-agent", "--quiet", "--batch", "-d", path]
    try:
        return subprocess.check_output(args).strip().decode('ascii').split()[email_to_index[acct]]
    except subprocess.CalledProcessError:
        return ""


if __name__ == "__main__":
    pass
