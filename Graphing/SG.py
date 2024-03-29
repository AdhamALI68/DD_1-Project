import matplotlib.pyplot as plt
import numpy as np
import random as rndm

colors = ['r', 'b', 'g', 'y']

def my_lines(ax, pos, *args, **kwargs):
    if ax == 'x':
        for p in pos:
            plt.axvline(p, *args, **kwargs)
    else:
        for p in pos:
            plt.axhline(p, *args, **kwargs)

def Graph_one_line(ax, t, list_of_line, offset):
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

    ax.step(t, lst_of_binary, rndm.choice(colors), label=list_of_line[2], linewidth=3, where='post')
    ax.legend()

def graph_all_outs(t, two_d_list, offset):
    num_lines = len(two_d_list)
    fig, axes = plt.subplots(num_lines, 1, figsize=(10, 5*num_lines))
    
    for i, ax in enumerate(axes):
        Graph_one_line(ax, t, two_d_list[i], i * 3)
        axes[i].locator_params(axis = 'y', nbins= 2)
        ax.set_ylim([-0.1,1.1])

    fig.text(0.5, 0.01, 'Time (ps)', ha='center', va='center')
    fig.text(0.06, 0.5, 'Output', ha='center', va='center', rotation='vertical')
    plt.tight_layout(pad = 5/4 * offset)
    plt.show()