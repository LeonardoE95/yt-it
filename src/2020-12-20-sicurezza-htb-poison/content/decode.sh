#!/usr/bin/bash

END=12
FILE="pwdbackup.txt."

for i in $(seq 0 $END); do
    (cat $FILE$i | base64 -d) > $FILE$((i+1))
done
