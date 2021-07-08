#!/usr/bin/python

import os
import shutil
import sys
import subprocess
import glob
from pathlib import Path
from shutil import which


def showHelp():
    print('USAGE:')
    if len(sys.argv) > 0:
        print('    ' + sys.argv[0] + ' <COMMAND> [parameters...]')
    else:
        print('    ./project.py <COMMAND> [parameters...]')
    print('')
    print('COMMANDS:')
    print('    init -- Initialize CMake build tool, MUST use this after git clone')
    print('    build -- Compile the source code, with multithread if possible')
    print('    test [all|unit|e2e] -- Run tests, MUST use this before commit')
    print('    clean -- Deletes ".cmake/" and "out/" directories')
    print('    rebuild -- clean => init => build')
    print('    deploy -- Creates Arduino library without "*.test.cpp"')
    print('')


def runInit() -> bool:
    Path('.cmake').mkdir(exist_ok=True)
    Path('out').mkdir(exist_ok=True)
    os.chdir('.cmake')
    isSuccess = (subprocess.call(['cmake', '..']) == 0)
    os.chdir('..')
    return isSuccess


def runBuild() -> bool:
    # Clean last temp files to prevent common problems
    cmakeCachePath = os.path.join('.cmake', 'CMakeFiles')
    if os.path.exists(cmakeCachePath) and os.path.isdir(cmakeCachePath):
        shutil.rmtree(cmakeCachePath)

    # Compile with build tools configured by CMake
    os.chdir('.cmake')
    isSuccess = (subprocess.call(
        ['cmake', '--build', '.', '--parallel ' + str(os.cpu_count())]) == 0)
    os.chdir('..')
    return isSuccess


def runTestUnit() -> bool:
    # Resolve paths
    pathCommon = os.path.join('.cmake', 'm2tp-common-test-unit')
    pathMember = os.path.join('.cmake', 'm2tp-member-test-unit')
    pathLeader = os.path.join('.cmake', 'm2tp-leader-test-unit')

    # Add .exe for windows
    if os.name == 'nt':
        pathCommon += '.exe'
        pathMember += '.exe'
        pathLeader += '.exe'

    print('')
    print('===== Unit Test: Common =====')
    resultCommon = (subprocess.call([pathCommon]) == 0)

    print('')
    print('===== Unit Test: Member =====')
    resultMember = (subprocess.call([pathMember]) == 0)

    print('')
    print('===== Unit Test: Leader =====')
    resultLeader = (subprocess.call([pathLeader]) == 0)

    # Return overall result
    return resultCommon and resultMember and resultLeader


def runTestE2E() -> bool:
    return True


def runTest(arg: str) -> bool:
    if arg == 'all':
        return runTestUnit() and runTestE2E()
    elif arg == 'unit':
        return runTestUnit()
    elif arg == 'e2e':
        return runTestE2E()
    else:
        showHelp()
        return False


def runClean() -> bool:
    if os.path.exists('.cmake') and os.path.isdir('.cmake'):
        shutil.rmtree('.cmake')
    if os.path.exists('out') and os.path.isdir('out'):
        shutil.rmtree('out')
    return True


def runRebuild() -> bool:
    if runClean():
        if runInit():
            return runBuild()
    return False


def runDeploy() -> bool:
    # Resolve target
    targetPath = os.path.join('out', 'm2tp')

    # Delete previous deploy if exist
    if os.path.exists(targetPath) and os.path.isdir(targetPath):
        shutil.rmtree(targetPath)

    # Re-create the directory
    Path(targetPath).mkdir()
    Path(os.path.join(targetPath, 'src')).mkdir()

    # Copy entire "include directory"
    shutil.copytree('include', os.path.join(targetPath, 'include'))

    # Copy *.h and *.c files
    pattern = os.path.join('src', '**', '*.[hc]')
    srcFiles = glob.glob(pattern, recursive=True)
    for eachFile in srcFiles:
        destination = os.path.join(targetPath, eachFile)
        os.makedirs(os.path.dirname(destination), exist_ok=True)
        shutil.copy(eachFile, destination)

    # Copy library.properties
    shutil.copy('library.properties', os.path.join(
        targetPath, 'library.properties'))

    # Generate readme.md
    readmePath = os.path.join(targetPath, 'README.md')
    readmeFile = open(readmePath, 'w')
    readmeFile.write('# Minified Library of M2TP\r\n\r\n')
    readmeFile.write(
        'See [full library source code](https://github.com/Thor-x86/m2tp)\r\n')
    readmeFile.close()

    # Report success
    print('')
    print('Done! Now put "./out/m2tp" directory inside your "<Arduino Path>/libraries/"')
    print('')
    return True


# Making sure you have CMake installed
if which("cmake") is None:
    print('Please install cmake first')
    print('')
    exit(1)

# Main script logic here below:
if len(sys.argv) > 1:
    isSuccess = False

    if sys.argv[1] == 'init':
        isSuccess = runInit()
    elif sys.argv[1] == 'build':
        isSuccess = runBuild()
    elif sys.argv[1] == 'test':
        if len(sys.argv) > 2:
            isSuccess = runTest(sys.argv[2])
        else:
            isSuccess = runTest('all')
    elif sys.argv[1] == 'clean':
        isSuccess = runClean()
    elif sys.argv[1] == 'rebuild':
        isSuccess = runRebuild()
    elif sys.argv[1] == 'deploy':
        isSuccess = runDeploy()
    else:
        showHelp()

    if isSuccess:
        exit(0)
    else:
        exit(1)
else:
    showHelp()
