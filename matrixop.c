CFLAGS="-lblas" python3 ./setup.py install
#include <stdbool.h>
#include <Python.h>

#include <stdio.h>
#include <stdlib.h>

/* alias cbgcc='gcc -I /home/arseneusm/github/cblas/CBLAS/include' */
#include "/home/arseneausm/Documents/github/cblas/CBLAS/include/cblas.h"

double *build_array(PyObject *py_obj, int len) {
	double *arr;
	arr = (double *) malloc(sizeof(double *)*len);

	for(int index = 0; index < len; index++) {
		PyObject *item;
		item = PyList_GetItem(py_obj, index);
		arr[index] = PyFloat_AsDouble(item);
	}

	return arr;
}

/*
static PyObject *sdot(PyObject *self, PyObject *args) {
	int n;

	PyObject *py_sx, *py_sy;	

	double *sx, *sy;
	int memx, memy;

	if(!PyArg_ParseTuple(args, "iOiOi", &n, &py_sx, &memx, &py_sy, &memy)) {
		return NULL;
	}

	sx = build_array(py_sx, n);
	sy = build_array(py_sy, n);

	// double res = cblas_sdot(n, sx, memx, sy, memy);

	free(sx);
	free(sy);
	
	double res = 0;
	return PyLong_FromLong(res);
}
*/

static PyObject *dgemv(PyObject *self, PyObject *args) {
	enum CBLAS_ORDER order;
	enum CBLAS_TRANSPOSE transa;

	PyObject *py_a, *py_x, *py_y;

	double *a, *x, *y;
	double alpha, beta;
	int  m, n, lda, incx, incy, i;

	/* specifies that the storage is column-major and to not use
		the transpose of the a matrix

		https://www.ibm.com/docs/en/xl-c-aix/13.1.2?topic=blas-function-syntax
		
		m 		 	Size of Column
		n		 	Size of Row
		lda		 	Leading dimension (e.g. for 5*4 matrix, lda = 5)
		incx		?
		incy		?
		alpha		coefficient 1
		beta		coefficient 2
	 */

	order = CblasColMajor;
	transa = CblasNoTrans;	

	/* parse the damn args */

	if(!PyArg_ParseTuple(args, "OiiiOOddii", &py_a, &m, &n, &lda, &py_x, &py_y, &alpha, &beta, &incx, &incy)) {
		return NULL;
	} 

	a = build_array(py_a, m*n);
	x = build_array(py_x, m);
	y = build_array(py_y, m);

	cblas_dgemv( order, transa, m, n, alpha, a, lda, x, incx, beta, y, incy );

	for( i = 0; i < n; i++) {
		printf(" y%d = %f\n", i, y[i]);
	}

	PyObject *ret = Py_BuildValue("[d,d,d,d]", y[0]/1.0, y[1]/1.0, y[2]/1.0, y[3]/1.0);	

	free(a);
	free(x);
	free(y);
	
	return ret;
}

static PyMethodDef PyCBLASMethods[] = {
	{"dgemv", dgemv, METH_VARARGS, "Matrix multiplication (4x4)"},
	//{"sdot", sdot, METH_VARARGS, "n-length dot product"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef pycblas = {
	PyModuleDef_HEAD_INIT,
	"pycblas",
	"Wrapper for CBLAS in Python",
	-1,
	PyCBLASMethods
};

PyMODINIT_FUNC PyInit_pycblas(void) {
	return PyModule_Create(&pycblas);
}



