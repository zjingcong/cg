#!/usr/bin/env python

import numpy as np

# XYZ = trans_mat * RGB
# RGB = trans_mat_invers * XYZ
trans_mat = np.mat([[0.606864, 0.173507, 0.200335], [0.298903, 0.586620, 0.114477], [0, 0.066098, 1.116151]])
RGB = np.mat([[1], [1], [0]])
# get inverse matrix: matrix.I

XYZ = trans_mat * RGB

print "translation matirx: "
print trans_mat
print '-' * 10
print "RGB: "
print RGB
print '-' * 10
print "XYZ: "
print XYZ

