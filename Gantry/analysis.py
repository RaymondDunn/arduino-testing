import csv
import os
import matplotlib.pyplot as plt


# get data
p1_filenames = [
"measurelist_2019_05_31_13_21_29.csv",
"measurelist_2019_05_31_13_24_28.csv",
"measurelist_2019_05_31_13_26_55.csv",
"measurelist_2019_05_31_13_29_20.csv",
"measurelist_2019_05_31_13_33_23.csv",
"measurelist_2019_05_31_13_35_39.csv",
"measurelist_2019_05_31_13_45_45.csv",
"measurelist_2019_05_31_13_48_07.csv",
"measurelist_2019_05_31_14_09_12.csv",
"measurelist_2019_05_31_14_11_50.csv"
]

# get path info
cwd = os.getcwd()
#p1_path = os.path.join(cwd, p1_filename)

# fill holder variables with lines from file
measurements_all = []
gains_all = []
xs_all = []
ys_all = []

for fname in p1_filenames:

	# recording vars
	measurements = []
	gains = []
	xs = []
	ys = []

	# get path
	p1_path = os.path.join(cwd, fname)

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

	# append inner holding variables to list
	measurements_all.append(measurements)
	gains_all.append(gains)
	xs_all.append(xs)
	ys_all.append(ys)


# plot first recording in list
plt.plot(measurements_all[0])
plt.show()

##############################

# code to process multiple measurelists
# this will be of shape recs x measurements
meas = np.array(measurements_all)

##############################