from pwn import *
from Crypto.Util.number import *
import hashlib
from Crypto.Cipher import AES

r = remote('group-projection.chal.uiuc.tf', 1337)

r.recvuntil(b'g = ')
g = int(r.recvline().strip())
r.recvuntil(b'p = ')
p = int(r.recvline().strip())
r.recvuntil(b'A = ')
A = int(r.recvline().strip())

assert (p-1) % 4 == 0
r.sendline(str((p-1)//4).encode())

r.recvuntil(b'c = ')
c = int(r.recvline().strip())
c = long_to_bytes(c)

S = 1
key = hashlib.md5(long_to_bytes(S)).digest()
cipher = AES.new(key, AES.MODE_ECB)
flag = cipher.decrypt(c)
print(flag)

r.interactive()
