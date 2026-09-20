import sympy as sp

x = sp.symbols('x')
f = (2*x + 1) / (x - 3)

# Vertical asymptote
print("Limit as x -> 3+:", sp.limit(f, x, 3, '+'))
print("Limit as x -> 3-:", sp.limit(f, x, 3, '-'))

# Horizontal asymptote
print("Limit as x -> +oo:", sp.limit(f, x, sp.oo))
print("Limit as x -> -oo:", sp.limit(f, x, -sp.oo))

# Oblique asymptote check
print("Degree numerator:", sp.degree(2*x+1, x))
print("Degree denominator:", sp.degree(x-3, x))