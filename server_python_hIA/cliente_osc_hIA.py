'''
Neviim: cliente_osc_hIA.py 

Cliente de osc 
'''

import threading
import OSC

class Client:
    def __init__(self):
        HOST, PORT = "localhost", 12000
        self.client = OSC.OSCClient()
        self.client.connect((HOST,PORT))

    def send(self, data):
        strdata = data.split(' ')
        print strdata
        address = strdata[0]
        typeddata = [] 
        for i in strdata[1:]:
            if i.isdigit():
                typeddata.append(int(i))
                continue
            try:
                f = float(i)
                typeddata.append(f)
                continue
            except:
                pass
            typeddata.append(i)

        m = OSC.OSCMessage(address)
        [m.append(d) for d in typeddata]
        self.client.send(m)

    def close(self):
        self.client.close()


def main():

    client = Client()
    data = "/hello world de neviim@ghost.net"

    while data != "quit":
        client.send(data)
        data = raw_input()

    #import pprint
    #import codecs
    #codecs.open('reaper_osc.txt','w','utf-8').write(pprint.pformat(server.reaper, indent=2))


if __name__=='__main__':
   main() 