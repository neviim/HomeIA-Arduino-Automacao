#!/usr/bin/env python
#
# Cliente em python para mandar mensagem para um server OSC em Processing
# 
# Modo de uso: python cliente_processing_hIA.py
#
# Por: neviim - 01/07/2012
# ------------------------

from OSC import OSCServer, OSCClient, OSCMessage

# Variaveis definicao conecsao para arduino homeIA
netServerIP = "172.10.10.200"  # Servidor Arduino homeIA
netPortaServer = 10000         # Porta do servidor homeIA

# Conecta ao servidor escrito em homeIA
client = OSCClient()
client.connect( (netServerIP, netPortaServer) )

# Seta mensagem e parametro a ser inviado.
# msg = OSCMessage("/ping")
# msg.append(123)
# msg.append(456)
lampada = 8 # Numero da porta output do arduino UNO (0 a 9) homeIA

# msg = OSCMessage("/hia/luz")
# msg = OSCMessage("/1/2nav1")
msg = OSCMessage("/hia/luz")
msg.append(lampada) # Liga ou desliga a lampada "x"

# Envia mensagem para o server.
client.send( msg )

#DEGUG
print msg.items()


# --- Server
# netIPLocal = "localhost"  # Endereco desta aplicacao.
# netPortaLocal = 12001
# server = OSCServer( (netIPLocal, netPortaLocal) )
