import sympy as sp

# Part 1: Network Flow augmented matrix [A | b]
# Column order: x1, x2, x3, x4, x5 | RHS
A = sp.Matrix([
    [1,  1,  0,  0,  0,  500],
    [1,  0, -1, -1,  0, -200],
    [0,  1,  1,  0, -1,  400],
    [0,  0,  0,  1,  1,  300],
])

rref_matrix, pivot_columns = A.rref()

print("RREF of augmented matrix:")
sp.pprint(rref_matrix)
print("Pivot columns (0-indexed):", pivot_columns)

# --- How to read this against your hand solution ---
# pivot_columns should be (0, 1, 3) -> these are x1, x2, x4: the BASIC variables.
# Columns 2 and 4 (x3, x5) are NOT pivot columns -> these are the FREE variables,
# matching the s (=x3) and t (=x5) parameters used by hand.
# Reading the pivot rows off rref_matrix reproduces:
#   x1 - x3 + x5 = 100   ->  x1 = 100 + x3 - x5
#   x2 + x3 - x5 = 400   ->  x2 = 400 - x3 + x5
#   x4 + x5 = 300        ->  x4 = 300 - x5
# which is exactly x = p + s*u + t*v with
#   p = [100, 400, 0, 300, 0],  u = [1,-1,1,0,0],  v = [-1,1,0,-1,1]