#!/usr/bin/env python3

from bs4 import BeautifulSoup

# -- read file
f = open("./web_page_example.html", "r")
text = f.read()
f.close()

# -- generate DOM structure
soup = BeautifulSoup(text, 'html.parser')

# -- find all tags of the form <p> ... </p>
# paragraphs = soup.find_all("p")
# print(paragraphs[0].text.strip())

# # -- find all tags with a certain attribute
footer_div = soup.find("div", {"id": "footer"})

if footer_div:
    # -- find firsts <p>...</p> inside <div id="footer"> ... </div>
    author_p = footer_div.find("p")

# -- get all data (html and textual)
print(author_p.decode_contents())
# -- get only textual data
print(author_p.text)
