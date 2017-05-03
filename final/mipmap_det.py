import numpy as np

# screen coordinates
x = 0.0
y = 1.0

size = 1024

A = 1/4.0
B = 1
C = 0
D = 0
E = 1/16.0
F = 0
G = 1
H = 0

ABC = A * x + B * y + C
DEF = D * x + E * y + F
bottom = G * x + H * y + 1
bottom_2 = pow(bottom, 2.0)

sx = (A * bottom - ABC * G) / float(bottom_2)
print "sx: ", sx
sy = (B * bottom - ABC * H) / float(bottom_2)
print "sy: ", sy
tx = (D * bottom - DEF * G) / float(bottom_2)
print "tx: ", tx
ty = (E * bottom - DEF * H) / float(bottom_2)
print "ty: ", ty

mat = np.mat([[sx, sy], [tx, ty]])
print "mat: "
print mat

det = np.linalg.det(mat)
print "determinant: ", det

s = size * size * det
level = np.log2(np.sqrt(s))
print "level: ", level

