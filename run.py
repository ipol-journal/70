#!/usr/bin/env python3

import argparse
import os
import shutil
from PIL import Image
import subprocess
import sys

# parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("--thresholdtype", type=int)
ap.add_argument("--tmin", type=int)
ap.add_argument("--tmax", type=int)
ap.add_argument("--m", type=int)
ap.add_argument("--e", type=float)
ap.add_argument("--w", type=str)
args = ap.parse_args()

#string to bool
w = args.w.lower() == 'true'
list_commands = ""

def runCommand(command, stdOut=None, stdErr=None, comp=None):
    """
    Run command and update the attribute list_commands
    """
    global list_commands
    p = subprocess.run(command, stderr=stdErr, stdout=stdOut)
    index = 0
    # transform convert.sh in it classic prog command (equivalent)
    for arg in command:
        if arg == "convert.sh" :
            command[index] = "convert"
        index = index + 1
    command_to_save = ' '.join(['"' + arg + '"' if ' ' in arg else arg
                for arg in command ])
    if comp is not None:
        command_to_save += comp
    list_commands +=  command_to_save + '\n'
    return command_to_save


##  -------
## process 1: transform input file
## ---------
command_args = ['convert.sh', 'input_0.png', 'inputNG.pgm' ]
runCommand(command_args)

##  -------
## process 2: extract contour files
## ---------
with open("inputPolygon.txt", "w") as f, open("algoLog.txt", "w") as fInfo:
        command_args = ['pgm2freeman']+ ['-min_size', str(args.m), '-image', 'inputNG.pgm']+ ['-outputSDPAll' ]

        if not args.thresholdtype:  #autothreshold means thresholdtype=1
            command_args += ['-maxThreshold', str(args.tmax)]+ \
                            ['-minThreshold', str(args.tmin)]

        cmd = runCommand(command_args, f, fInfo, comp = ' > inputPolygon.txt')

        if os.path.getsize("inputPolygon.txt") == 0:
            with open('demo_failure.txt', 'w') as file:
                file.write("The parameters given produce no contours, please change them.")
                sys.exit(0)

with open("algoLog.txt", "r") as fInfo:
    #Recover otsu max value from output
    if args.thresholdtype:
        lines = fInfo.readlines()
        line_cases = lines[0].replace(")", " ").split()
        args.tmax = int(line_cases[17])

with open("tmp.dat", "w") as contoursList, open("inputPolygon.txt", "r") as f:
    contoursList.write("# Polygon contour obtained from the pgm2freeman"+\
                    " program with the following options: \n"+\
                    "# "+ cmd + "\n"+\
                    "# Each line corresponds to an resulting polygon. "+\
                    "All vertices (xi yi) are given in the same line: "+\
                    " x0 y0 x1 y1 ... xn yn \n")
    index = 0
    for contour in f:
        contoursList.write("# contour number: "+ str(index) + "\n")
        contoursList.write(contour+"\n")
        index = index + 1
    shutil.copy('tmp.dat','inputPolygon.txt')


##  -------
## process 3: apply algorithm
## ---------
inputWidth = Image.open('input_0.png').size[0]
inputHeight = Image.open('input_0.png').size[1]
command_args = ['frechetSimplification'] + \
                ['-imageSize', str(inputWidth), str(inputHeight)] + \
                ['-error', str(args.e), '-sdp',
                'inputPolygon.txt' ]+\
                ['-allContours']

with open("algoLog.txt", "a") as f:
    if w:
        command_args += ['-w']

    cmd = runCommand(command_args)
    
with open("tmp.dat", "w") as contoursList, open("output.txt", "r") as f:
    contoursList.write("# Set of resulting polygons obtained from the " +\
                        "frechetSimplification algorithm. \n"+\
                        "# Each line corresponds to an resulting polygon."+\
                        " All vertices (xi yi) are given in the same line:"+
                        "  x0 y0 x1 y1 ... xn yn \n"+\
                        "# Command to reproduce the result of the "+\
                        "algorithm:\n")
    contoursList.write("#"+ cmd+'\n')

    index = 0
    for line in f:
        contoursList.write("# contour number: "+ str(index) + "\n")
        contoursList.write(line+"\n")
        index = index +1
    
shutil.copy('tmp.dat','outputPolygon.txt')


## ---------
## process 4: converting to output result
## ---------
command_args = ['gs', '-q', '-dNOCACHE', '-dEPSCrop', '-dNOPAUSE', '-dBATCH', '-dSAFER', '-sDEVICE=png16m', '-r120',  '-sOutputFile=output.png', 'output.eps']
runCommand(command_args)


## ----
## Final step: save command line
## ----
with open("commands.txt", "w") as f:
    f.write(list_commands)
