import os

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

with open("test.vm","wb") as f:
    code = op["XOR"] + p8(0)
    code += op["LOCK"]
    code += op["LD"] + p64(0x1337000)
    code += op["ADD"] + p8(0) + p8(1)
    code += op["MOVI"] + p8(0x1) + p64(0x1)
    code += op["ADD"] + p8(0) + p8(1)
    code += op["ST"] + p64(0x1337000)
    code += op["UNL"]
    code *= 0x10
    code += op["HLT"]
    f.write(code)

with open("pro.vm","wb") as f:

    MAIN_LOOP = -61
    UNL = 15

    code = ""
    code += op["MOVI"] + p8(2) + p64(0x10) # loop times
    code += op["MOVI"] + p8(3) + p64(0x5)  # max producation
    code += op["XOR"] + p8(4) # cnt
    code += op["XOR"] + p8(5)
    code += op["MOVI"] + p8(6) + p64(1) # one
    l1 = len(code)
    code += op["NOP"] # MAIN_LOOP
    code += op["LOCK"]
    code += op["LD"] + p64(0x1337000)
    code += op["BC"] + p8(3) + p8(1) + p64(UNL)
    l2 = len(code)
    code += op["MOV"] + p8(0x0) + p8(0x1)
    code += op["ADD"] + p8(0) + p8(6)
    code += op["ST"] + p64(0x1337000)
    l3 = len(code)
    code += op["UNL"] #UNL
    code += op["ADD"] + p8(4) + p8(6)
    code += op["BC"] + p8(2) + p8(4) + p64(9)
    code += op["B"] + p64(MAIN_LOOP)
    l4 = len(code)
    code += op["HLT"]
    code += op["HLT"]
    print(l4-l1)
    print(l3-l2)
    

    f.write(code)

with open("con.vm","wb") as f:

    MAIN_LOOP = -61
    UNL = 15

    code = ""
    code += op["MOVI"] + p8(2) + p64(0x10) # loop times
    code += op["MOVI"] + p8(3) + p64(0x0)  # max producation
    code += op["XOR"] + p8(4) # cnt
    code += op["XOR"] + p8(5)
    code += op["MOVI"] + p8(6) + p64(1) # one
    l1 = len(code)
    code += op["NOP"] # MAIN_LOOP
    code += op["LOCK"]
    code += op["LD"] + p64(0x1337000)
    code += op["BC"] + p8(3) + p8(1) + p64(UNL)
    l2 = len(code)
    code += op["MOV"] + p8(0x0) + p8(0x1)
    code += op["SUB"] + p8(0) + p8(6)
    code += op["ST"] + p64(0x1337000)
    l3 = len(code)
    code += op["UNL"] #UNL
    code += op["ADD"] + p8(4) + p8(6)
    code += op["BC"] + p8(2) + p8(4) + p64(9)
    code += op["B"] + p64(MAIN_LOOP)
    l4 = len(code)
    code += op["HLT"]
    code += op["HLT"]
    print(l4-l1)
    print(l3-l2)
    

    f.write(code)

