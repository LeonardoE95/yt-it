import hashlib

text = b"Hello World"
hash_text = hashlib.sha256(text).hexdigest()
print(hash_text)
