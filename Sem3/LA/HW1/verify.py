import numpy as np
A = np.array([
    [2,4,6],
    [1,3,4],
    [1,2,5]
], dtype=float) 
b = np.array([40,25,30], dtype=float)
solution = np.linalg.solve(A,b)
print(f"Solution:\nType A VMs = {solution[0]}\nType B VMs = {solution[1]}\nType C VMs = {solution[2]}\n")