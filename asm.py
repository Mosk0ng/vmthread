import os
import sys

def p8(x):
    return chr(x)

def p64(x):
    if(x<0):
        x = 0x10000000000000000 + x
    res = ""
    for i in range(8):
        res += chr((x&0xff))
        x = x >> 8
    return res

op = {
    "NOP":'\x90',
    "SUB":'\x60',
    "ADD":'\x61',
    "MOVI":'\x62',
    "MOV":'\x64',
    "PR":'\x20',
    "PS":'\x21',
    "HLT":'\xff',
    "LD":'\x40',
    'ST':'\x41',
    "POP":'\x80',
    "PUSH":'\x81',
    "XOR":'\x63',
    "LOCK":'\x10',
    "UNL":'\x11',
    "B":'\x30',
    "BC":'\x31'
}

regs = ["R0","R1","R2","R3","R4","R5","R6","R7"]

def asm(line):
    if line[-1] == "\n":
        line = line[:-1]
    res = ""
    keys = line.split(" ")
    for k in keys:
        if(k in op):
            res += op[k]
        elif(k in regs):
            res += chr(int(k[1:2]))
        else:
            res += p64(int(k,16))
    return res

out = open(sys.argv[2],"wb")
bin = ""

with open(sys.argv[1],"r") as f:
    while 1:
        line = f.readline()
        if(line == ""): break
        print line
        bin+=asm(line)
    

out.write(bin)
