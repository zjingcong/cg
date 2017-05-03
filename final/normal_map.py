import numpy as np

V0 = np.array([0, 0, 2])
V1 = np.array([1, 0, 2])
V2 = np.array([1, -1, 2])

u0 = 0
v0 = 0
u1 = 0
v1 = 1
u2 = 1
v2 = 0

xyz = np.mat([[1 / pow(2.0, 0.5)], [0], [-1 / pow(2.0, 0.5)]])
print "xyz: "
print xyz

s1 = u1 - u0
s2 = u2 - u0
t1 = v1 - v0
t2 = v2 - v0

print "s1: ", s1
print "s2: ", s2
print "t1: ", t1
print "t2: ", t2

st = np.mat([[s1, t1], [s2, t2]])
print "st: "
print st
st_inverse = st.I
print "st inverse: "
print st_inverse

v = np.mat([V1 - V0, V2 - V0])
print "v: "
print v

tb = st_inverse * v
print "tb: "
print tb

T = tb[0].A1
B = tb[1].A1
N = np.cross(V1-V0, V2-V0)
N = N / np.linalg.norm(N)
print "T: ", T
print "B: ", B
print "N: ", N

tbn = np.transpose(np.mat([T, B, N]))
print "tbn: "
print tbn
normal = tbn * xyz
print "new normal: ", normal.A1



