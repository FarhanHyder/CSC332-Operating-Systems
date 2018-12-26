'''[Extra Credit-1]
	author: Farhan Hyder
	CSC 332 - Assignment 3
	Date: 12/16/2018
'''

import matplotlib.pyplot as plt
import random

def random_color():
		r = random.uniform(0,0.7)
		g = random.uniform(0,0.7)
		b = random.uniform(0,0.7)
		return (r, g, b)

metrics = {}
metrics["FCFS"] = [2767.93, 2378.45, 4762, 5481, 14]
metrics["SJN"] = [2408.93, 2019.43, 4712, 5481, 14]
metrics["Priority"] = [2619.36, 2229.86, 5114, 5481, 14]
metrics["RR FCFS 10"] = [5334.14, 4944.64, 5784, 6549, 548]
metrics["RR FCFS 100"] = [4559.07, 4169.57, 4839, 5581, 64]
metrics["RR FCFS 1000"] = [2767.93, 2378.45, 4762, 5481, 14]
metrics["SRTN 50"] = [2504.5, 2115, 4918, 5687, 117]
metrics["Priority 50"] = [3010.36, 2620.86, 5320, 5687, 117]
choices = ["Average turnaround time", "Average wait time", "Longest wait time", "Total run time", "Total number of context switches"]
choice_range = [(i+1) for i in range(len(choices))]
keys = ["FCFS", "SJN", "Priority", "RR FCFS 10", "RR FCFS 100", "RR FCFS 1000", "SRTN 50", "Priority 50"]

print ("---Select metric to plot---")
for i in range(len(choices)):
	print ("{}) {}".format(i+1, choices[i]))

choice = int(input("Enter: "))
while (choice not in choice_range):
	choice = int(input("Enter a valid choice: "))

choice -= 1

for i in range(len(keys)):
	plt.bar([0.2*(i+1)], metrics[keys[i]][choice], color=random_color(), width=0.2, label=keys[i], align="center")

fig = plt.gca()
plt.title(choices[choice] + " plot")
fig.axes.get_xaxis().set_visible(False)
plt.ylabel(choices[choice])
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 0.02), fancybox=True, shadow=True, ncol=3)
plt.show()

