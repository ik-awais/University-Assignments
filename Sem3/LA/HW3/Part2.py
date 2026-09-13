import sympy as sp

s1 = sp.Matrix([1, 2, 1, 1])
s2 = sp.Matrix([2, 4, 0, -2])
s3 = sp.Matrix([4, 8, 2, 0])
s4 = sp.Matrix([0, 0, 2, 4])

S = sp.Matrix.hstack(s1, s2, s3, s4)

# Augmented matrix [S | 0] for the homogeneous system S x = 0
zero_col = sp.zeros(4, 1)
aug = sp.Matrix.hstack(S, zero_col)

rref_matrix, pivot_columns = aug.rref()

print("RREF of [S | 0]:")
sp.pprint(rref_matrix)
print("Pivot columns:", pivot_columns)

num_vars = S.shape[1]            # number of signals/columns = 4
num_pivots = len(pivot_columns)  # should be 2 (s1, s2)

if num_pivots < num_vars:
    print("The signals are linearly dependent because the system has free "
          "variables, meaning the homogeneous equation S x = 0 has "
          "non-trivial solutions.")
else:
    print("The signals are linearly independent "
          "(S x = 0 has only the trivial solution).")