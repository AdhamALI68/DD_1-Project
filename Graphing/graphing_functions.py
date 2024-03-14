import matplotlib.pyplot as plt
import numpy as np
import random as rndm
import matplotx

colors = ['r' , 'b' , 'g' , 'y']
def set_y_lim(n):
    plt.ylim([-1,3*n])

def my_lines(ax, pos, *args, **kwargs):
    if ax == 'x':
        for p in pos:
            plt.axvline(p, *args, **kwargs)
    else:
        for p in pos:
            plt.axhline(p, *args, **kwargs)

my_lines('x', [0], color='.5', linewidth=1)
my_lines('y', [0,0], color='.5', linewidth=1)
plt.title("Waveform of the circuit")
plt.xlabel("Time")
plt.ylabel("Output")
plt.yticks(np.arange(0,1))

def Graph_one_line(t, list_of_line , offset):

    lst_of_binary = []
    for i in range(1500):
        if i < int(list_of_line[0]):
            if int(list_of_line[4]) == 1:
                lst_of_binary.append(0)
            else:
                lst_of_binary.append(1)
        else:
            if int(list_of_line[4]) == 1:
                lst_of_binary.append(1)
            else:
                lst_of_binary.append(0)

    lst_modified = [x+offset for x in lst_of_binary]
    plt.step(t, lst_modified, rndm.choice(colors),  label = list_of_line[2] ,linewidth = 3, where='post')
    matplotx.line_labels()


def graph_all_outs(t,two_d_list):
    for i in range(len(two_d_list)):
        Graph_one_line(t,two_d_list[i], i*3)
    plt.gca().axis('on')
    plt.show()
