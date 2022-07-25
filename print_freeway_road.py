import matplotlib.pyplot as plt
import numpy as np
matrix = np.zeros((64,64), dtype=int)


for j in range(2, 60, 4):
    for i in range(7, 58, 4):
        matrix[i,j:j+3] = np.array([1]*3)
''''
plt.imshow(matrix, vmin=0, vmax=1)
plt.show()
'''

rows = []
row = ''
for r in range(matrix.shape[0]):
    row = "{"
    for c in range(matrix.shape[1]):
        row += str(int(matrix[r][c])) + ',' 
    rows.append(row[:-1] + '},')
rows[-1] = rows[-1][:-1] + '};'

with open('free_way_road.txt', 'w') as roadFile:
    roadFile.write(f'uint8_t road[64][64] = {{\n')
    roadFile.write("\n".join(row for row in rows))
