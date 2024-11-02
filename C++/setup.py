"""

To build this run: python -m build

"""

from setuptools import setup, Extension

module = Extension('bitboard', 
                   sources=['bitboard.cpp'],  # Your C/C++ source file
                   libraries=['python310'],  # Adjust based on your Python version
                   include_dirs=[r'C:\Users\Jacob Bondy\anaconda3\envs\Chess\include'],  # Adjust the path
                   library_dirs=[r'C:\Users\Jacob Bondy\anaconda3\envs\Chess\libs\python310.lib'])  # Adjust the path

setup(name='bitboard',
      version='1.1',
      description='A Python wrapper for your C/C++ code',
      ext_modules=[module])