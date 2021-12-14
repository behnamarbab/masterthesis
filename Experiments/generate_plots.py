import os
import json
import matplotlib.pyplot as plt
import numpy as np
from numpy.core.defchararray import find

def load_data(json_file='parsed_data_nobucket.json'):
    data = None
    with open(json_file) as f:
        data = json.load(f)

    return data

def find_histogram(lst, mn, mx, title):
    # arr = np.array(lst)
    plt.clf()
    lst=[l//1000 for l in lst]
    mx //= 1000
    mn //= 1000
    plt.hist(lst, range=[mn, mx], bins=150)
    plt.xlabel('Execution times (ms)')
    plt.ylabel('Frequency')
    plt.yscale('log')
    plt.title(title)
    plt.savefig(os.path.join('execs', title+'.png'))

def find_max(data):
    maxes = {}
    for d in data:
        mx = 0
        for f in data[d]:
            mx = max(mx, max(data[d][f]['exec_times']))
        maxes[d] = mx
    return maxes

if __name__=='__main__':
    data = load_data()
    maxes = find_max(data)
    mn = 0
    for d in data:
        for f in data[d]:
            find_histogram(data[d][f]['exec_times'], 0, maxes[d], d+'-'+f)
