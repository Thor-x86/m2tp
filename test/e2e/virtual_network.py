#!/usr/bin/python3

# This script will simulate a M2TP network
# which is basically an UDP Forwarder.

import sys
import socket
from typing import List


# Get port number from runner.py
udpPort = 0
if len(sys.argv) > 1:
    udpPort = int(sys.argv[1])
else:
    print("USAGE: ./virtual_network.py <UDP Port>")
    print("")
    exit(1)

debugtag = '\x1b[1;33m[Virtual Network]\x1b[0m'

# This is how you print something here
print(debugtag, 'Ready! UDP port:', udpPort)

# List of connected port
connected: List[int] = []

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    # Prepare connection
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('127.0.0.1', udpPort))

    while True:
        # Block thread until receiving data
        data, addressPort = s.recvfrom(1024)
        port = addressPort[1]

        # Register if not yet
        if connected.count(port) == 0:
            connected.append(port)
            print(
                debugtag,
                'New connection from',
                port
            )

        # Broadcast the data
        for eachPort in connected:
            if eachPort != port:
                try:
                    s.sendto(data, ('127.0.0.1', eachPort))
                    # print(debugtag, port, '=>', eachPort, ':', data)
                except socket.error as e:
                    print(
                        debugtag, 'port', eachPort,
                        'disconnected with message:',
                        e.strerror
                    )
                    connected.remove(eachPort)
