import os
import matplotlib.pyplot as plt
import numpy as np

direct = "swap"
dir_path = "../" + direct
# dir_path = "../random"
# graph_path = "graphics"

plt.rcParams['figure.figsize'] = (10, 7)

for file_path in os.listdir(dir_path):
    if file_path.find("list") == -1:
        continue

    sort_name = file_path[0:file_path.find("_")]
    if sort_name in {"InsertionSort", "ShellSort", "ShellSort2", "BubbleSort", "ShakerSort"}:
        continue
    file_name = file_path[:-4]

    dots_x, dots_y = list(), list()
    with open(os.path.join(dir_path, file_path)) as f:
        for line in f.readlines():
            x, y = line.split()
            dots_x.append(float(x))
            dots_y.append(float(y))

    dots_x_new, dots_y_new = list(), list()

    dots_x_new.append(dots_x[0])
    dots_y_new.append(dots_y[0])

    for i in range(1, len(dots_y) - 1):
        if abs(dots_y[i] - (dots_y[i - 1] + dots_y[i + 1]) / 2) <= 10 * abs(dots_y[i - 1] - dots_y[i + 1]) / 2:
            dots_x_new.append(dots_x[i])
            dots_y_new.append(dots_y[i])

    dots_x_new.append(dots_x[-1])
    dots_y_new.append(dots_y[-1])

    dots_y_new = np.asarray(dots_y_new)
    for i in range(10):
        dots_y_new[1:-1] = 0.25 * (dots_y_new[:-2] + 2 * dots_y_new[1:-1] + dots_y_new[2:])

    plt.plot(dots_x_new, dots_y_new, label=file_name)

    plt.ylabel('Time, ms')
    plt.xlabel('Size')
    plt.title(f'Sorting algorithms comparison ({direct})')

    # plt.savefig(os.path.join(graph_path, file_name) + ".png")
    # plt.gcf().clear()
plt.legend()
# plt.show()
plt.grid()
plt.savefig(f"graphics_{direct}_list.png")
