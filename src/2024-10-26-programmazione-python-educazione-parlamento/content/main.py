#!/usr/bin/env python3

import requests
import json
from bs4 import NavigableString, BeautifulSoup

# ------------------------------------------------------------

def scrape_camera_education(user_id: str) -> str:
    url = f"https://www.camera.it/leg19/29?tipoAttivita=&tipoVisAtt=&tipoPersona=&shadow_deputato={user_id}&idLegislatura=19"
    r = requests.get(url)
    soup = BeautifulSoup(r.text, "html.parser")

    data = soup.find("div", { "class": "datibiografici" }).text
    education = data.split("\n")[-1]

    return education

def scrape_camera() -> [dict]:
    url = "https://www.camera.it/leg19/28"
    r = requests.get(url)

    # parse the HTML data into a soup object
    soup = BeautifulSoup(r.text, "html.parser")
    
    deputati_html = soup.find("select", { "id": "idPersona" })
    deputati_html = [x for x in list(deputati_html)[1:] if not isinstance(x, NavigableString)]

    deputati = []
    for x in deputati_html:
        x_id = x.attrs["value"]
        x_name = x.text.replace(" ", " ").strip()
        
        print(f"[INFO] - Downloading data of name={x_name}, id={x_id}")
        x_education = scrape_camera_education(x_id)
        print(f"[INFO]      {x_education}")

        deputato = {
            "name": x_name,
            "id": x_id,
            "education": x_education
        }

        deputati.append(deputato)

    return deputati

# ---------------------

def read_data(filename: str) -> [dict]:
    with open(f"./data/{filename}", "r") as f:
        return json.loads(f.read()) if ".json" in filename else f.read()

def write_data(filename: str, data):
    with open(f"./data/{filename}", "w") as f:
        f.write(json.dumps(data)) if ".json" in filename else f.write(data)
    
# ------------------------------------------------------------

def main():
    dati_camera = scrape_camera()
    write_data("dati-camera-2.json", dati_camera)
    
if __name__ == "__main__":
    main()
