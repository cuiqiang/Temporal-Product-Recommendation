#! /usr/bin/python

import sys
import time
import string

if __name__ == '__main__':
    input_file_name = sys.argv[1]
    #output_file_name = input_file_name + "-for-graph"
    output_file_name = input_file_name + "-notself-for-graph"
    input_file = open(input_file_name,"r")
    output_file = open(output_file_name,"w")
    num = 0;
    for line in input_file:
        num += 1
        print num
        temp_line = line.strip().split(" ")
        if(temp_line[0]!=temp_line[1]):
           continue 
        if(string.atof(temp_line[4])<=20):
            times = temp_line[2].split(",")
            if(len(times)>=20):
                output_file.write(line)
