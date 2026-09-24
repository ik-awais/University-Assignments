import sympy as sp

# ---------------------------------------------------------------
# 1. Define A
# ---------------------------------------------------------------
A = sp.Matrix([
    [ 1, -1,  2,  0,  3],
    [-2,  2, -4,  1, -5],
    [ 0,  0,  0,  2,  2],
    [ 3, -3,  6, -1,  8]
])

print("Matrix A:")
sp.pprint(A)
print()

# ---------------------------------------------------------------
# 2. RREF and pivot columns  (part i)
# ---------------------------------------------------------------
rref_matrix, pivot_columns = A.rref()
print("RREF of A:")
sp.pprint(rref_matrix)
print("Pivot columns (0-indexed):", pivot_columns)

rank_A = A.rank()
print(f"rank(A) = {rank_A}")
print()

# ---------------------------------------------------------------
# 3. Basis for Col(A)  (part ii)
#    IMPORTANT: sympy's .columnspace() correctly pulls pivot columns
#    from the ORIGINAL matrix, not the RREF -- exactly as required.
# ---------------------------------------------------------------
col_space = A.columnspace()
print("Basis for Col(A):")
for vec in col_space:
    sp.pprint(vec.T)
print()

# ---------------------------------------------------------------
# 4. Basis for Row(A)  (part iii)
# ---------------------------------------------------------------
row_space = A.rowspace()
print("Basis for Row(A):")
for vec in row_space:
    sp.pprint(vec)
print()

# ---------------------------------------------------------------
# 5. Basis for Nul(A)  (part iv)
# ---------------------------------------------------------------
null_space = A.nullspace()
print("Basis for Nul(A):")
for vec in null_space:
    sp.pprint(vec.T)
print()

# ---------------------------------------------------------------
# 6. Basis for Nul(A^T), i.e. the left null space  (part v)
# ---------------------------------------------------------------
left_null_space = A.T.nullspace()
print("Basis for Nul(A^T)  (left null space):")
for vec in left_null_space:
    sp.pprint(vec.T)
print()

# ---------------------------------------------------------------
# 7. Verify the Fundamental Theorem of Linear Algebra  (part vi)
# ---------------------------------------------------------------
m, n = A.shape
r = rank_A
dim_col   = len(col_space)
dim_row   = len(row_space)
dim_null  = len(null_space)
dim_lnull = len(left_null_space)

print("--- Fundamental Theorem check ---")
print(f"n (columns of A) = {n},  m (rows of A) = {m},  rank(A) = {r}")
print(f"dim(Col A) + dim(Nul A)   = {dim_col} + {dim_null} = {dim_col + dim_null}  (should equal n = {n})")
print(f"dim(Row A) + dim(Nul A^T) = {dim_row} + {dim_lnull} = {dim_row + dim_lnull}  (should equal m = {m})")

assert dim_col + dim_null == n, "Rank-Nullity theorem failed for Col/Nul(A)!"
assert dim_row + dim_lnull == m, "Rank-Nullity theorem failed for Row/Nul(A^T)!"
print("Both identities verified. ✓")

# ---------------------------------------------------------------
# 8. Sanity check: every null-space / left-null-space vector
#    actually satisfies its defining equation.
# ---------------------------------------------------------------
print()
print("--- Verification ---")
for v in null_space:
    assert A * v == sp.zeros(m, 1), "A*v != 0 for a claimed null space vector!"
print("Confirmed: A @ v = 0 for every basis vector of Nul(A).")

for y in left_null_space:
    assert A.T * y == sp.zeros(n, 1), "A^T*y != 0 for a claimed left-null vector!"
print("Confirmed: A.T @ y = 0 for every basis vector of Nul(A^T).")