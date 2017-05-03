import math

i = 2
n = 4
x = 1 / 4.0

# factorial
n_f = math.factorial(n)
i_f = math.factorial(i)
n_i_f = math.factorial(n-i)
print "n factorial: ", n_f
print "i factorial: ", i_f
print "n-1 factorial: ", n_i_f

f = n_f / (i_f * n_i_f)
print "n, i: ", f

B = f * (x ** i) * ((1 - x) ** (n - i))
print "B: ", B

