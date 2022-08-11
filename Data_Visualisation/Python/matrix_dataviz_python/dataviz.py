import glob
import matplotlib.pyplot as plt
import numpy as np
import os
import sys
import serial
import time
import csv 

from drawnow import *
from serial_thread import SerialThread
from typing import List

NUM_TX=4
NUM_RX=6

MIN=10
MAX=150
matrix=np.zeros((NUM_TX,NUM_RX))

baudrate=115200

def _toArray(data)->None:
    for tx in range(NUM_TX):
        for rx in range(NUM_RX):
            matrix[tx][rx]=int(data[tx*NUM_RX+rx])

def plotMatrix()->None:
    plt.imshow(matrix,interpolation='bilinear',cmap='inferno',origin='lower',vmin=MIN,vmax=MAX)
    plt.colorbar() 
    plt.show()

def serial_ports()->List[str]:
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

def select_port()->str:
    ports = serial_ports()
    if(len(ports)>0):
        for i in range(len(ports)):
            print(f"{i}: {ports[i]}")
        index=input("Select your port: ")
        os.system('cls' if os.name == 'nt' else 'clear')
        return ports[int(index)]
    print("No ports available")
    return None

def main()->None:
    port = select_port()
    if port is not None:
        serial_thread=SerialThread(port=port,baudrate=baudrate,buffer_size=2)
        serial_thread.start()
        plt.ion() #Tell matplotlib you want interactive mode to plot live data
        plt.figure(figsize=(NUM_RX*2,NUM_TX*2))
        file_path = 'record.csv'
        f = open(file_path,'w',encoding='UTF8',newline='')
        header = []
        for tx in range(NUM_TX):
            for rx in range(NUM_RX):
                header.append('x'+str(rx)+'y'+str(tx))
        writer = csv.writer(f)
        writer.writerow(header)
        while True:
            data=serial_thread.get_last_values()
            if data is None:
                time.sleep(0.0001)
                continue
            _toArray(data)
            writer.writerow(data)
            drawnow(plotMatrix)
            plt.pause(0.0001)
            if len(plt.get_fignums())==0:
                break
        plt.close()
        serial_thread.stop()
            
if __name__ == '__main__':
    main()