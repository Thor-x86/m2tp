#!/usr/bin/python3

# This is main script for End-to-End testing

import os
import subprocess
import socket
import time
import sys
import signal

print('')
print('Make sure you did run these before:')
if os.name == 'nt':
    print('.\\project.py init')
    print('.\\project.py build')
    print('.\\project.py install (must under Adminsitrator privilege)')
else:
    print('./project.py init')
    print('./project.py build')
    print('./project.py install')
print('')
print('If not, press CTRL and C together, right now!')
print('')
print('Starting in 5 seconds...')
time.sleep(5)
print('')

# Get vacant UDP port
udpPort = int(0)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(('', 0))
    udpPort = int(s.getsockname()[1])

# Start the virtual network
virtualNetwork = subprocess.Popen(
    ['python3', 'virtual_network.py', str(udpPort)],
    stderr=subprocess.STDOUT
)

# Wait virtual network ready for a while
time.sleep(1)

# Start first virtual device as leader
leaderPath: str
if os.name == 'nt':
    leaderPath = os.path.join('leader', 'out', 'm2tp-e2e-leader.exe')
else:
    leaderPath = os.path.join('leader', 'out', 'm2tp-e2e-leader')
leader = subprocess.Popen(
    [leaderPath, str(udpPort)],
    stderr=subprocess.STDOUT
)

# Start second virtual device as member
member1Path: str
if os.name == 'nt':
    member1Path = os.path.join('member_1', 'out', 'm2tp-e2e-member_1.exe')
else:
    member1Path = os.path.join('member_1', 'out', 'm2tp-e2e-member_1')
member1 = subprocess.Popen(
    [member1Path, str(udpPort)],
    stderr=subprocess.STDOUT
)

while True:
    if virtualNetwork.poll() is not None:
        print(
            '\x1b[1;31mFATAL: Virtual Network is dead!\x1b[0m',
            file=sys.stderr
        )
        leader.terminate()
        member1.terminate()
        exit(1)
    elif leader.poll() is not None and member1.poll() is not None:
        break

# Cleanups
virtualNetwork.terminate()

print('')


def resolveErrorCode(code: int) -> str:
    if code == 1:
        return 'Aborted \x1b[38;5;246m(because other device failed)\x1b[0m'
    elif code > 1:
        return os.strerror(code)
    elif code < 0:
        return signal.strsignal(code * -1)
    else:
        return 'Failed successfully \x1b[38;5;246m(probably runner.py is broken)\x1b[0m'


# Which one is failed?
success = True
if leader.returncode != 0:
    success = False
    print(
        '\x1b[1;31mLeader:\x1b[0m',
        resolveErrorCode(leader.returncode),
        file=sys.stderr
    )
if member1.returncode != 0:
    success = False
    print(
        '\x1b[1;31mMember #1:\x1b[0m',
        resolveErrorCode(member1.returncode),
        file=sys.stderr
    )

if success:
    print('\x1b[1;32mEnd-to-End Test Success!\x1b[0m')
    exit(0)
else:
    print('', file=sys.stderr)
    print('\x1b[1;31mEnd-to-End Test Failed!\x1b[0m', file=sys.stderr)
    exit(1)
