from distutils.core import setup, Extension
 
def main():
	setup(name = "pycblas",
    	version = "1.0.0",
        description = "Wrapper for CBLAS in Python",
        author = "Stefan Arseneau",
        author_email = "sarsene1@jhu.edu",
        ext_modules = [Extension("pycblas", ["matrixop.c"])])

if __name__ == "__main__":
	main()
