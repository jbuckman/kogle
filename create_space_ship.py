import numpy as np 
from scipy.ndimage.interpolation import rotate

matrices = []
matrix1 = np.array([
    [0,0,1,0,0],
    [0,0,1,0,0],
    [0,1,1,1,0],
    [1,1,1,1,1],
    [1,1,1,1,1]
], dtype=np.int)
matrix2 = np.array([
    [0,0,0,1,1],
    [0,0,1,1,1],
    [0,1,1,1,1],
    [1,1,1,1,1],
    [0,0,1,1,1]
], dtype=np.int)

matrix3 = np.array([
    [0,0,0,0,1],
    [0,0,1,1,0],
    [0,1,1,1,0],
    [0,1,1,0,0],
    [1,0,1,0,0]
], dtype=np.int)
matrices.extend([matrix1, matrix2, matrix3])

for i in range(3, 0, -1):
   matrices.extend([np.rot90(matrix1, i), np.rot90(matrix2, i), np.rot90(matrix3, i)])
   

with open('ship.txt', 'w') as file:
    file.write(f'int sprites[12][5][5]  = {{')
    for matrix in matrices:
        file.write(f'\n  {{')
        for row in matrix:
            file.write(f'\n    {{')
            for idx, value in enumerate(row):
                if idx == len(row)-1:
                    file.write(f'{value}}},')
                
                else: 
                    file.write(f'{value},')

        file.write(f'\n  }},')
    file.write(f'\n}};')