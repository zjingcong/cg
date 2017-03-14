import numpy as np

spec_name = "spec.txt"
# test: example spectrum: R,G,B = 0.840782, 0.219115, 0.413645
# spec_name = 'spec_example.txt'

x_white = 0.31271
y_white = 0.32902
x_r = 0.690
y_r = 0.300
x_g = 0.205
y_g = 0.715
x_b = 0.150
y_b = 0.045

s_list = []
fx_list = []
fy_list = []
fz_list = []


def load_s():
    with open(spec_name) as spec_file:
        for line in spec_file:
            lamb = float(line.split('      ')[0])
            r = float(line.split('      ')[1])
            s_list.append(r)


def load_f(file_name, f_list):
    with open(file_name) as f_file:
        for line in f_file:
            lamb = line.split(', ')[0]
            r = float(line.split(', ')[1].split(',')[0])
            f_list.append(r)


def get_ab(i, f_list):
    a = (f_list[i + 1] - f_list[i]) / 10
    b = f_list[i] - (f_list[i + 1] - f_list[i]) / 10 * (380 + i * 10)

    return a, b


def get_N():
    h = 0
    for i in xrange(39):
        lamb_1 = 380 + i * 10
        lamb_2 = 380 + (i + 1) * 10
        a, b = get_ab(i, fy_list)
        h += (0.5 * a * pow(lamb_2, 2) + b * lamb_2) - (0.5 * a * pow(lamb_1, 2) + b * lamb_1)

    return h


def integrate_func(a, b, c, d, lamb):
    xx = ((a * c) / 3) * pow(lamb, 3.0)
    yy = ((b * c + a * d) / 2) * pow(lamb, 2.0)
    zz = b * d * lamb

    return xx + yy +zz


def get_XYZ(f_list, N):
    h = 0
    for i in xrange(39):
        lamb_1 = 380 + i * 10
        lamb_2 = 380 + (i + 1) * 10
        a, b = get_ab(i, s_list)
        c, d = get_ab(i, f_list)
        h += (integrate_func(a, b, c, d, lamb_2) - integrate_func(a, b, c, d, lamb_1))

    return h / N


def get_z(x, y):
    return float(1 - x - y)


def white_recover():
    X_w = float(x_white) / float(y_white)
    Y_w = 1.0
    Z_w = float(1 - x_white - y_white) / float(y_white)

    return X_w, Y_w, Z_w


def get_C(X_w, Y_w, Z_w):
    z_r = get_z(x_r, y_r)
    z_g = get_z(x_g, y_g)
    z_b = get_z(x_b, y_b)
    matrix_1 = np.mat([[x_r, x_g, x_b],
                       [y_r, y_g, y_b],
                       [z_r, z_g, z_b]])
    matrix_xyz = np.mat([[X_w], [Y_w], [Z_w]])
    matrix1_prime = matrix_1.I
    matrix_c = matrix1_prime * matrix_xyz

    Cr = matrix_c.tolist()[0][0]
    Cg = matrix_c.tolist()[1][0]
    Cb = matrix_c.tolist()[2][0]

    return Cr, Cg, Cb


def get_rgb(Cr, Cg, Cb, X, Y, Z):
    z_r = get_z(x_r, y_r)
    z_g = get_z(x_g, y_g)
    z_b = get_z(x_b, y_b)
    matrix_2 = np.mat([[x_r * Cr, x_g * Cg, x_b * Cb],
                       [y_r * Cr, y_g * Cg, y_b * Cb],
                       [z_r * Cr, z_g * Cg, z_b * Cb]])
    matrix_XYZ = np.mat([[X], [Y], [Z]])
    matrix2_prime = matrix_2.I
    matrix_RGB = matrix2_prime * matrix_XYZ

    R = matrix_RGB.tolist()[0][0]
    G = matrix_RGB.tolist()[1][0]
    B = matrix_RGB.tolist()[2][0]

    return R, G, B


def main():
    load_s()
    load_f('fx.txt', fx_list)
    load_f('fy.txt', fy_list)
    load_f('fz.txt', fz_list)
    # print "s: ", s_list
    # print "fx: ", fx_list
    # print "fy: ", fy_list
    # print "fz: ", fz_list

    N = get_N()
    X = get_XYZ(fx_list, N)
    Y = get_XYZ(fy_list, N)
    Z = get_XYZ(fz_list, N)
    print "N: ", N
    print "X: ", X
    print "Y: ", Y
    print "Z: ", Z

    X_w, Y_w, Z_w = white_recover()
    print "white | (X_w, Y_w, Z_w): ", (X_w, Y_w, Z_w)

    Cr, Cg, Cb = get_C(X_w, Y_w, Z_w)
    print "(Cr, Cg, Cb): ", (Cr, Cg, Cb)

    R, G, B = get_rgb(Cr, Cg, Cb, X, Y, Z)
    print "(R, G, B): ", (R, G, B)

if __name__ == '__main__':
    main()
