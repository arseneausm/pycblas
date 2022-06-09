import pycblas

a = [1,2,3,4,
	1,1,1,1,
	3,4,5,6,
	5,6,7,8]
m = 4
n = 4
lda = 4

x = [1,2,1,1]
y = [1,1,1,1]

alpha = 2
beta = 0

print(pycblas.dgemv(a, m, n, lda, x, y, alpha, beta, 1, 1))
print(pycblas.sdot(len(x), x, y))
