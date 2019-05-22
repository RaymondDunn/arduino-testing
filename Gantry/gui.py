import matplotlib.pyplot as plt
import serial
from matplotlib.widgets import Button, TextBox
import time


# widgetlist to protect from garbage collection
widgetlist = []

# measurement list
measurelist = []

# create figure
fig = plt.figure('Gantry Controller', figsize=(9, 7))

# add main window and adjust subplots
sp_ax = fig.add_subplot(111)
plt.subplots_adjust(left=0.4, top=0.8, bottom=0.2)

# connect to serial interface
com = "COM4"
print('Establishing serial connection on port {}'.format(com))
ser = serial.Serial(com, 9600, timeout=1)

# helper function to write message to serial
def writeMessage(msg):

	# add newline cause apparently you need this in windows machines
	msg += ',\n'

	# convert string to bytes
	ser.write(msg.encode())

	# print output
	#resp = ser.readline()
	#print('Arduino: {}'.format(resp))

## move gantry button
# distance option
move_gantry_pos_ax = plt.axes([0.05, 0.9, 0.08, 0.08])
move_gantry_pos_tb = TextBox(move_gantry_pos_ax, 'dist:', initial='-500')
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

take_measurement_ax = plt.axes([0.05, 0.1, 0.15, 0.08])
take_measurement_button = Button(take_measurement_ax, "Measure!")
take_measurement_button.on_clicked(submit_measure)

# get current position
def submit_get_position_x(event):
	writeMessage('getGantryPosition,x')

def submit_get_position_y(event):
	writeMessage('getGantryPosition,y')

get_gantry_position_x_ax = plt.axes([0.05, 0.2, 0.08, 0.08])
get_gantry_position_x_button = Button(get_gantry_position_x_ax, "Get x")
get_gantry_position_x_button.on_clicked(submit_get_position_x)
get_gantry_position_y_ax = plt.axes([0.15, 0.2, 0.08, 0.08])
get_gantry_position_y_button = Button(get_gantry_position_y_ax, "Get y")
get_gantry_position_y_button.on_clicked(submit_get_position_y)

# do raster
raster_xy_ax = plt.axes([0.05, 0.65, 0.05, 0.08])
raster_xy_tb = TextBox(raster_xy_ax, 'x/y:', initial='y')
raster_steps_ax = plt.axes([0.17, 0.65, 0.05, 0.08])
raster_steps_tb = TextBox(raster_steps_ax, '#steps', initial='10')
raster_stepsize_ax = plt.axes([0.29, 0.65, 0.05, 0.08])
raster_stepsize_tb = TextBox(raster_stepsize_ax, 'stepSize', initial='-50')

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

# show figure
plt.show(block=False)

while True:

	plt.pause(1)
	resp = ser.readline()
	if len(resp) > 0:
		print('Arduino: {}'.format(resp))
