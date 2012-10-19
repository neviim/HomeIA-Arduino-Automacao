'''
Neviim: server_osc_hIA.py

Server de osc 
'''

from pymongo import Connection
import threading
import OSC
import datetime 
import time
        

class Server(threading.Thread):
    def discovery_handler(self,path,container):
        #print 'just discovered', path, container
        print '.',
        print 

    def reaper_handler(self, addr, tags, data, source):
        # is an illegal character in osc address
        # - so with that in front we avoid name clashes
        #m: monitors
        #t: tags
        #d: data

        # collect all reaper data and celebrate each new discovery
        # with a call to discovery_handler
        slot = self.reaper
        addr_parts = addr.split('/')[1:]

        for p in addr_parts:
            if not p in slot:
                slot[p]={'#m':self.discovery_handler(addr_parts,slot)}
            slot = slot[p]
            
        slot['#d'] = data
        slot['#t'] = tags

        # defini data e hota atual.
        now  =  datetime.datetime.now()
        dia  = now.date()
        hora = now.time()

        # Trata os dados recebidos para armazenar no banco de dados.
        print "-------------------------------------------------------"
        #                       {'#t': 'ii', '#m': None, '#d': [7, 0]}
        print "Matriz dado....: "+str(slot)
        print "IP destino.....: "+str(source)
        print "Identidade.....: "+str(addr)
        print "Tipo parametro.: "+str(tags)
        print "Led............: "+str(data[0])
        print "Estado.........: "+str(data[1])
        print "Data...........: "+str(dia)
        print "Hora...........: "+str(hora)
        print
        print "Status geral...: "+str(data[2])
        print 

        if data[1] == 0:
            print "Luz apagada."
        else:
            print "Luz acessa."
        
        # grava no banco de dados mongodb tabela loghomeia.
        connection = Connection('localhost', 27017)
        db = connection.loghomeia
        collection = db.loghomeia

        postlog = {"ipdest": str(source[0]), 
                   "identidade": str(addr),
                   "tipo": str(tags), 
                   "portastatus": [str(data[0]), str(data[1]), str(data[2])], 
                   "data": str(dia),
                   "hora": str(hora)}

        # armazena os dados do log
        posts = db.posts
        posts.insert(postlog)


    def run(self):
        HOST, PORT = "172.10.10.249", 12000          # Server python no ubuntu: 172, 10, 10, 37
        self.reaper = {}
        self.selected_track = 0
        self.server = OSC.OSCServer((HOST, PORT))
        self.server.print_tracebacks = True
        self.server.addDefaultHandlers()
        self.server.addMsgHandler('default', self.reaper_handler)

        # run the server
        self.server.serve_forever()

        print "3"

    def close(self):

        print "4"
        self.server.close()


def main():
    server = Server()
    server.daemon = True
    server.start()

    print "Servidor ativo..."
    while 1:
        dig = raw_input()
        if dig == 'sai': break

# inicializa chamada.
if __name__=='__main__':
   main() 
