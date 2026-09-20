import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-9, 0, 1000)
g = 2 * np.sqrt(-x)

plt.figure(figsize=(7, 6))
plt.plot(x, g, 'b', lw=2, label=r'$g(x)=2\sqrt{-x}$')
plt.axhline(0, color='k', lw=0.5)
plt.axvline(0, color='k', lw=0.5)
plt.grid(True, alpha=0.3)
plt.legend()
plt.title('Reflected and vertically stretched square root')
plt.xlabel('x')
plt.ylabel('g(x)')
plt.show()