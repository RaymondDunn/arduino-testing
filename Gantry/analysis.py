import csv
import os

# get data
p1_filename = "measurelist_2019_05_22_13_29_34.csv"

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

		# split line into the actual tuples of data
		data = line.split('\"')

		# ugh this is so gross
		for d in data:
			if len(d) > 1:

				# formatting
				chars_to_replace = ['[', ']', '(', ')', '\'']
				formattedline = d
				for char in chars_to_replace:
					formattedline = formattedline.replace(char, '')


				# append formatted list
				splitline = formattedline.split(',')		# split on commas
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
