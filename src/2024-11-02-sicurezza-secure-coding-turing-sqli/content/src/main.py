#!/usr/bin/env python3

from flask import Flask, request

import sqlite3
import os
import json

app = Flask(__name__)
db_path = "./test.db"

# curl http://127.0.0.1:1337/search?term=%27%20OR%201%3D1%20--%20
@app.route("/search")
def hello_world():
    global db_path
    con = sqlite3.connect(db_path)
    
    term = request.args.get('term')
    sql = f"SELECT item_id, label, price FROM items WHERE label='{term}'"
    sql_result = con.execute(sql).fetchall()

    result = []
    for entry in sql_result:
        result.append({
            "item_id": entry[0],
            "label": entry[1],
            "price": entry[2]
        })

    return json.dumps(result, indent=2)

# ----------------------------------------

if __name__ == "__main__":
    app.run(host="127.0.0.1", port=1337)
    
    
