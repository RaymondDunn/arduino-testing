import matplotlib.pyplot as plt
import serial
from matplotlib.widgets import Button, TextBox
import time
import datetime
import csv

# master variable to open the gui even if arduino isn't connected
DEMO_MODE = False

# widgetlist to protect from garbage collection
widgetlist = []

# measurement list
measurelist = []

# create figure
fig = plt.figure('Gantry Controller', figsize=(9, 7))
plt.subplots_adjust(left=0.4, top=0.8, bottom=0.2)

# decorate plot... how do we want to structure data?
# plot for xy representation
sp1_ax = fig.add_subplot(211)
sp2_ax = fig.add_subplot(212)

def decorate_plots():
    sp1_ax.set_title('Luminometer measurements')
    sp1_ax.set_xlabel('Position X')
    sp1_ax.set_ylabel('Position Y')

    # second figure for raw plotting
    sp2_ax.set_xlabel('Measurement #')
    sp2_ax.set_ylabel('Value')

# make plots
decorate_plots()

# for coloring
max_gain = 127
sensor_range = 1000
val_range = sensor_range * max_gain
colors = plt.get_cmap('viridis', val_range).colors

# connect to serial interface
com = "COM4"
if DEMO_MODE:
    print('Running in DEMO_MODE')
    ser = []
else:
    print('Establishing serial connection on port {}'.format(com))
    ser = serial.Serial(com, 9600, timeout=1)


# helper function to write message to serial
def writeMessage(msg):

    # add newline cause apparently you need this in windows machines
    msg += ',\n'

    # convert string to bytes
    ser.write(msg.encode())


## move gantry button
# distance option
move_gantry_pos_ax = plt.axes([0.05, 0.9, 0.08, 0.08])
move_gantry_pos_tb = TextBox(move_gantry_pos_ax, 'dist:', initial='-513')
widgetlist.append(move_gantry_pos_tb)

# xy option
move_gantry_xy_ax = plt.axes([0.2, 0.9, 0.05, 0.08])
move_gantry_xy_tb = TextBox(move_gantry_xy_ax, 'x/y:', initial='y')

# submit move gantry option
def submit_move_gantry(event):
    pos = move_gantry_pos_tb.text
    xy = move_gantry_xy_tb.text
    print('Moving gantry {} by {}'.format(pos, xy))
    writeMessage("moveGantry," + xy + ',' + pos)

# make button that clicks to move gantry
move_gantry_button_ax = plt.axes([0.05, 0.8, 0.2, 0.08])
move_gantry_button = Button(move_gantry_button_ax, 'Move gantry')
move_gantry_button.on_clicked(submit_move_gantry)

# go to zero
def submit_zero_position(event):
    writeMessage("setZeroPosition")

zero_pos_button_ax = plt.axes([0.8, 0.9, 0.15, 0.08])
zero_pos_button = Button(zero_pos_button_ax, 'Home (0,0)')
zero_pos_button.on_clicked(submit_zero_position)

# measure
def submit_measure(event):
    writeMessage('takeMeasurement')

take_measurement_ax = plt.axes([0.05, 0.05, 0.15, 0.08])
take_measurement_button = Button(take_measurement_ax, "Measure!")
take_measurement_button.on_clicked(submit_measure)

# get current position
def submit_get_position_x(event):
    writeMessage('getGantryPosition,x')

def submit_get_position_y(event):
    writeMessage('getGantryPosition,y')

get_gantry_position_x_ax = plt.axes([0.05, 0.15, 0.08, 0.08])
get_gantry_position_x_button = Button(get_gantry_position_x_ax, "Get x")
get_gantry_position_x_button.on_clicked(submit_get_position_x)
get_gantry_position_y_ax = plt.axes([0.15, 0.15, 0.08, 0.08])
get_gantry_position_y_button = Button(get_gantry_position_y_ax, "Get y")
get_gantry_position_y_button.on_clicked(submit_get_position_y)

# change acceleration
gantry_acceleration_tb_ax = plt.axes([0.4, 0.9, 0.08, 0.08])
gantry_acceleration_tb = TextBox(gantry_acceleration_tb_ax, 'accel.', initial='3000')

def submit_set_gantry_acceleration(event):
    accel = gantry_acceleration_tb.text
    writeMessage('setGantryAcceleration,{}'.format(accel))

gantry_acceleration_button_ax = plt.axes([0.5, 0.9, 0.1, 0.08])
gantry_acceleration_button = Button(gantry_acceleration_button_ax, 'Set accel.')
gantry_acceleration_button.on_clicked(submit_set_gantry_acceleration)

# change photoluminometer gain
pl_gain_tb_ax = plt.axes([0.4, 0.05, 0.08, 0.08])
pl_gain_tb = TextBox(pl_gain_tb_ax, 'gain', initial='1')

def submit_set_pl_gain(event):
    gain = pl_gain_tb.text
    writeMessage('setPhotoluminometerGain,{}'.format(gain))

pl_gain_button_ax = plt.axes([0.5, 0.05, 0.1, 0.08])
pl_gain_button = Button(pl_gain_button_ax, 'Set gain')
pl_gain_button.on_clicked(submit_set_pl_gain)

# do raster
raster_xy_ax = plt.axes([0.05, 0.65, 0.05, 0.08])
raster_xy_tb = TextBox(raster_xy_ax, 'x/y:', initial='y')
raster_steps_ax = plt.axes([0.17, 0.65, 0.05, 0.08])
raster_steps_tb = TextBox(raster_steps_ax, '#steps', initial='16')
raster_stepsize_ax = plt.axes([0.29, 0.65, 0.05, 0.08])
raster_stepsize_tb = TextBox(raster_stepsize_ax, 'stepSize', initial='-513')

def submit_raster_scan(event):
    xy = raster_xy_tb.text
    steps = raster_steps_tb.text
    stepsize = raster_stepsize_tb.text

    # write message
    print('Beginning raster scan at {} of {} by {}'.format(xy, steps, stepsize))
    writeMessage('rasterScan,{},{},{}'.format(xy, steps, stepsize))

raster_scan_button_ax = plt.axes([0.05, 0.55, 0.15, 0.08])
raster_scan_button = Button(raster_scan_button_ax, 'Raster scan')
raster_scan_button.on_clicked(submit_raster_scan)

# do full plate scan (xy raster)
plate_scan_numx_ax = plt.axes([0.05, 0.45, 0.05, 0.08])
plate_scan_numx_tb = TextBox(plate_scan_numx_ax, '#x:', initial='24')
plate_scan_numy_ax = plt.axes([0.17, 0.45, 0.05, 0.08])
plate_scan_numy_tb = TextBox(plate_scan_numy_ax, '#y', initial='16')
plate_scan_stepsize_ax = plt.axes([0.29, 0.45, 0.05, 0.08])
plate_scan_stepsize_tb = TextBox(plate_scan_stepsize_ax, 'stepSize', initial='-513')

def submit_plate_scan(event):
    numx = plate_scan_numx_tb.text
    numy = plate_scan_numy_tb.text
    stepsize = plate_scan_stepsize_tb.text

    # write message
    print('Beginning full plate scan of x {} by {} at {} steps'.format(numx, numy, stepsize))
    writeMessage('raster2DScan,{},{},{}'.format(numx, numy, stepsize))

full_plate_scan_button_ax = plt.axes([0.05, 0.35, 0.15, 0.08])
full_plate_scan_button = Button(full_plate_scan_button_ax, 'Full plate scan')
full_plate_scan_button.on_clicked(submit_plate_scan)

# add clear plot button
def submit_clear_plot(event):
    print('Clearing existing data...')
    measurelist = []

    # update display
    sp1_ax.cla()
    sp2_ax.cla()
    decorate_plots()
    fig.canvas.draw()

submit_clear_plot_ax = plt.axes([0.7, 0.05, 0.08, 0.08])
submit_clear_plot_button = Button(submit_clear_plot_ax, 'Clear plot')
submit_clear_plot_button.on_clicked(submit_clear_plot)

# add save measurelist plot
def submit_save_measurelist(event):
    print('Saving existing data')

    # write measurelist to csv
    now = datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    with open('measurelist_' +  now + '.csv', 'w') as myfile:
        writer = csv.writer(myfile)
        writer.writerow(measurelist)

submit_save_measurelist_ax = plt.axes([0.8, 0.05, 0.08, 0.08])
submit_save_measurelist_button = Button(submit_save_measurelist_ax, 'Save data')
submit_save_measurelist_button.on_clicked(submit_save_measurelist)

# helper function that sees if there's data embedded in the arduino response
def checkResponseForData(resp):

    # measurement data will be embedded in a delimiter
    delim = '$'
    resp_string = resp.decode("utf-8") 

    # if there's data add it to our measure list and update display plot
    # suggested data format: "$val,x,y,gain$" this covers the essentials
    resp_string_split = resp_string.split(delim) 

    #import pdb
    #pdb.set_trace()

    # if there's data embedded in the string grab it
    if len(resp_string_split) > 1:

        # grab data
        data = resp_string_split[1].split(',')
        measure = int(data[0])
        gain = int(data[1])
        x_pos = int(data[2])
        y_pos = int(data[3])

        # append as tuple to measure list
        print('Received data! {}; {}; {}; {}'.format(measure, gain, x_pos, y_pos))
        measurelist.append((measure, gain, x_pos, y_pos))
        update_display_plot()

# helper function to replot acccording to contents of measurelist
def update_display_plot():

    # plot
    # assuming we're only plotting the most recent point
    m = measurelist[-1]

    # get values
    adj_val = m[0]
    x = m[2]
    y = m[3]
    c = colors[adj_val]

    # plot
    sp1_ax.scatter(x,y,color=c)
    #sp2_ax.plot(len(measurelist), adj_val, color=c)
    sp2_ax.plot(len(measurelist), adj_val, linestyle='--', marker='o')

    # redraw canvas
    fig.canvas.draw()

# show figure
plt.show(block=False)

while True:

    plt.pause(1)

    # while there's data to read, keep reading it
    while True:

        if DEMO_MODE:
            resp = []
        else:
            resp = ser.readline()
        if len(resp) > 0:
            checkResponseForData(resp)
            print('Arduino: {}'.format(resp))
        else:
            # if there's no data to read break from inner loop
            break
