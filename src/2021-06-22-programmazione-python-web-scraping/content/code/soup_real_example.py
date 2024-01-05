#!/usr/bin/env python3

"""This scripts shwocases a possible usage of the python library
BeautifulSoup4 by extracing the lecture schedure data from the
official site for computer science @ Tor Vergata.

"""

from bs4 import BeautifulSoup
import requests
import os

URL = "http://www.informatica.uniroma2.it/pages/trien/orario/orario.htm"
INPUT_FILE = "orario.html"
OUTPUT_FILE = "orario.csv"
TEXT = ""

COURSE_CODES = {
    "Analisi Matematica": "AM",
    "Architettura dei sistemi di elaborazione": "AE",
    "Fisica": "FI",
    "Geometria ed algebra": "GA",
    "Logica e reti logiche": "LRL",
    "Matematica discreta": "MD",
    "Programmazione dei calcolatori con laboratorio": "PR",
    "Algoritmi e strutture dati": "ASD",
    "Basi di dati e di conoscenza": "BDC",
    "Calcolo delle probabilità e statistica": "CP",
    "Fondamenti di informatica": "FO",
    "Linguaggi e metodologie di programmazione": "LMP",
    "Ricerca operativa": "RO",
    "Sistemi operativi e reti": "SOR",
    "Algoritmi e strutture dati 2": "ASD2",
    "Calcolo numerico": "CN",
    "Ingegneria del software": "IS",
    "Intelligenza artificiale 1": "IA",
    "Lingua inglese": "LING",
    "Modelli e linguaggi di simulazione": "MLS",
    "Programmazione Java per dispositivi mobili": "PJDM",
    "Programmazione Web": "PW"
}

# -------------------------------
# Part 1 - Get the data
# -------------------------------

if not os.path.exists(INPUT_FILE):
    # -- if we don't have the file, download it and save it
    print("About to download...")
    r = requests.get(URL)
    if r.status_code != 200:
        print("Could not download page!")
        exit()

    # -- prettify text
    TEXT = r.text
    
    f = open(INPUT_FILE, "w+")
    f.write(TEXT)
    f.close()

else:
    print("Reading from file...")
    # -- otherwise simply read html from file
    f = open(INPUT_FILE, "r")
    TEXT = f.read()
    f.close()

# ---------------------------------
# Part 2 - Parse and write the data
# ---------------------------------

soup = BeautifulSoup(TEXT, 'html.parser')

title = soup.find("h1").decode_contents().strip()

# -- compute current sem
semester = ""
if "primo" in title:
    semester = "1"
elif "secondo" in title:
    semester = "2"

table = soup.find("table")
rows = table.find_all("tr")
current_year = 0
skip = False
default_room = ""

with open(OUTPUT_FILE, "w+") as out:
    # -- first row with metadata
    out.write("anno,ora,lunedì,martedì,mercoledì,giovedì,venerdì\n")

    for row in rows:
        if skip:            
            skip = False
            continue

        if row.find("h2"):
            current_year += 1
            # -- skip next row
            skip = True
        else:
            # -- get juicy data
            cols = row.find_all("td")
            hour = cols[0].decode_contents().strip()
            
            # -- schedule[i] := course of i-th day of the week for a
            # -- specified hour.
            schedule = []
            for col in cols[1:]:
                if col.find("a"):
                    course = col.a.decode_contents().strip()
                    schedule.append(COURSE_CODES[course])
                else:
                    schedule.append("X")

            schedule_str = ",".join(schedule)
                            
            out.write(f"{current_year},{hour},{schedule_str}\n")
