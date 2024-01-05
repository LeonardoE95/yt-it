#!/usr/bin/env python3

from bs4 import BeautifulSoup  # -- parsing HTML (web scraping)
import requests                # -- HTTP requests
import os                      # -- operating system stuff

URL = "http://www.informatica.uniroma2.it/pages/trien/orario/orario.htm"
INPUT_FILE = "orario.html"
OUTPUT_FILE = "orario.csv"

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

# Part 1 - Get the data
# ---------------------

if not os.path.exists(INPUT_FILE):
    print("About to download from server...")
    # -- download data from server
    r = requests.get(URL)
    if r.status_code != 200:
        print("Could not download page!")
        exit()

    # -- get text from response
    text = r.text
        
    # -- status code is OK, we can save the data.
    f = open(INPUT_FILE, "w+")
    f.write(text)
    f.close()
else:
    print("Reading from file...")

    f = open(INPUT_FILE, "r")
    text = f.read()
    f.close()

# -- from now on, text contains the html data of the page we're
# -- interested in.

# -- Part 2 - Parse data with beautiful soup

# -- prettify text
soup = BeautifulSoup(text, 'html.parser')

# -- print(soup.prettify())

table = soup.find("table")
rows = table.find_all("tr")

current_year = 0

with open(OUTPUT_FILE, "w+") as out:
    # -- write metadatai
    out.write("anno,ora,lunedì,martedì,mercoledì,giovedì,venerdì\n")
    
    for row in rows:
        if row.find("h2"):
            current_year = current_year + 1
            continue
        elif row.find("th"):
            continue

        # -- get juciy data
        columns = row.find_all("td")
        hour = columns[0].decode_contents().strip()

        schedule = []
        for col in columns[1:]:
            if col.find("a"):
                course = col.a.decode_contents().strip()
                schedule.append(COURSE_CODES[course])
            else:
                schedule.append("X")

        schedule_str = ",".join(schedule)
        out.write(f"{current_year},{hour},{schedule_str}\n")
