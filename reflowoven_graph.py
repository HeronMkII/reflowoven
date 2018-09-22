# Python script to pull serial data

import serial
import datetime
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

SERIAL_PORT = '/dev/cu.wchusbserial1440'
BAUD_RATE = 9600
TEST_NAME = 'Reflowoven Temperature'
LOG_DIR = './logs'
TIMESTAMP_FORMAT = '%Y%m%d-%H%M%S'

global temperatures
temperatures = []
#pres_coeff = []

# Initializes the output file for the temp test, returns file object for
# the output csv
#
def init_script():
    # Check if logging directory exists
    logPath = os.path.abspath(LOG_DIR)
    if not os.path.exists(logPath):
        os.mkdir(logPath)
    # Format a timestamp for the log
    timestamp = datetime.datetime.now().strftime(TIMESTAMP_FORMAT)
    # Open up the output file
    filename = os.path.join(logPath, TEST_NAME + '_' + timestamp + '.csv')
    #outFile = open(filename, 'w')
    return filename

# Perform a handshake with the arduino -- look stuff up on google drive
def handshake(s):
    handshake = False
    t0 = time.clock()
    while not handshake and ((time.clock() - t0) < 15):
        data = s.readline()
        print(data)
        if data == b'1\r\n':
            s.write(b'2')
            print('Handshake recieved on python')
            handshake = True
        time.sleep(1);
    if ((time.clock() - t0 >= 15)):
        print('Timeout occurred')
        raise SystemExit
    t0 = time.clock()
    while data != b'3\r\n' and ((time.clock() - t0) < 15):
        print(data)
        data = s.readline();
    if ((time.clock() - t0 >= 15)):
        print('Timeout occurred')
        raise SystemExit
    print('Completed handshake')

#def get_coeff(s):
#    global pres_coeff
#    print('Reading conversion coefficients')
#    for i in range(0, 8):
#        data = int(s.readline())
#        print('Coefficient ' + str(i) +': ' + str(data))
#        pres_coeff.append(data)
#    print('Finished reading the conversion coefficients')



# def convert_pressure(rawPres, rawTemp):
#     # Returns - D1 is pressure --- D2 is temperature
#     # Returns - raw pressure - need to treat as signed int32_t and divide by 1000 to get the value in kPa
#     # raw_temperature - return value (value of TEMP)
#     # gives 24 bit result
#     # from p.13 of datasheet
#     #uint32_t pres_convert_raw_uncompensated_data_to_raw_pressure(uint32_t D1, uint32_t D2, uint32_t *raw_temperature){
#     global pres_coeff
#
#     dT = rawTemp - (pres_coeff[5] * 2**8)  # difference between actual and reference temperature
#     TEMP = 2000 + (dT * pres_coeff[6] / 2**23)  # actual temperature
#
#     OFF = (pres_coeff[2] * 2**16) + (pres_coeff[4] * dT / 2**7)  # offset at actual temperature
#     SENS = (pres_coeff[1] * 2**15) + (pres_coeff[3] * dT / 2**8)  # sensetivity at actual temperature
#     '''
#     if(TEMP < 2000): # low temperature
#         T2 = (dT * dT) / 2147483648
#         T2 = T2
#         OFF2 = 3 * ((TEMP - 2000) * (TEMP - 2000))
    #     SENS2 = 7 * ((TEMP - 2000) * (TEMP - 2000)) / 8
    #
    #     if(TEMP < -1500):  # very low temperature
    #         SENS2 += 2 * ((TEMP + 1500) * (TEMP + 1500))
    # else:  # high temperature
    #     T2 = 0
    #     OFF2 = 0
    #     SENS2 = 0
    #
    #     if(TEMP >= 4500):  # very high temperature
    #         SENS2 = SENS2 - (((TEMP - 4500) * (TEMP - 4500)) / 8)
    # TEMP = TEMP - T2
    # OFF = OFF - OFF2
    # SENS = SENS - SENS2
    # '''
    # pressure = ((rawPres * SENS / 2**21) - OFF) / 2**15
    #
    # return pressure / 100


# Generator function to read the serial port
#
#read_temp
#try convert to float
#except if it's an error -- name of valid exception (value error or type error)
#to read values
#try:
#except ((TypeError or)) ValueError #figure out which one it is -- python terminal float(helloworld)
#    return last value returned -- error handling or try reading again
#        while haven't read a valid value, try reading another value
#    yield

def read_temp(s, filename):
    while True:
        readvalue = False
        while not readvalue:
                line = str(s.readline())
                line = line.replace("b'", "")
                line = line.replace("\\r\\n'", "")
                try:
                    temp = float(line)
                    readvalue = True
                except ValueError:
                    print("SERIAL NOT OPEN, EXCEPTION")
                    readvalue = False
        print(line)
#        pressure = convert_pressure(nums[0], nums[1])
#        print("Pressure: " + str(pressure))
        write_output(temp, filename)
        yield temp

# Function exists in case I want to do any fancy formatting or processing before
# writing the output
#
def write_output(value, filename):
    time = datetime.datetime.now().strftime('%H:%M:%S')
    date = datetime.datetime.now().strftime('%Y%m%d')
    file = open(filename, 'a')
    file.write(date + ',' + time + ',' + str(value) + ',\n')
    file.close()

#def test_animation(mu):
#    while True:
#        p = 101.3 + mu * np.random.randn()
#        yield p


def animate(i, x, read_next):
    global temperatures

    temperatures.append(next(read_next))
    numReadings = len(temperatures)

    if numReadings > 100:
        xvals = x
        temperatures = temperatures[-100:]
        yvals = temperatures

    else:
        xvals = x[0:numReadings]
        yvals = temperatures
    line.set_xdata(xvals)
    line.set_ydata(yvals)
    return line,

if __name__ == "__main__":
    try:
        s = serial.Serial(SERIAL_PORT, BAUD_RATE)
        outputFilename = init_script()
        #handshake(s)
  #      get_coeff(s)
        print('Setting up the generator')
        read_next = read_temp(s, outputFilename)
        time.sleep(1);
        print('Starting main loop')
        fig, ax = plt.subplots()

        x = np.arange(0, 100, 1)
        line, = ax.plot([],[])
        ax.set_xlim(0, 100)
        ax.set_ylim(10, 300)
        ax.set_ylabel('Temperature (C)')

        print('Starting animation')
        ani = animation.FuncAnimation(fig, animate, fargs=(x, read_next), blit=True)
        plt.show()
    except:
        print('Closing the serial port')
        s.close()
