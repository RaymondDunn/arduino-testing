import csv

# get data
p1_filename = "local data filename here"

# get path info
cwd = os.getcwd()
p1_path = os.path.join(cwd, p1_filename)

# fill holder variables with lines from file
measurements = []
gains = []
xs = []
ys = []
with open(p1_path) as f:
	
	# read all lines
	fcontent = f.readlines()

	for line in fcontent:

		# append formatted list
		formattedline = line[1:-1]		# remove brackeds
		splitline = line.split(',')		# split on commas
		val = int(splitline[0])
		gain = int(splitline[1])
		x = int(splitline[2])
		y = int(splitline[3])

		# store vals
		measurements.append(val)
		gains.append(gain)
		xs.append(x)
		ys.append(y)

# plot however we want
