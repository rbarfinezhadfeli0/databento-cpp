"""
setup.py for databento-cpp Python bindings

Install with:
    pip install .

Or for development:
    pip install -e .
"""

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import sys
import os

__version__ = "1.0.0"

# Get the directory of this file
here = os.path.abspath(os.path.dirname(__file__))

ext_modules = [
    Pybind11Extension(
        "databento_cpp",
        ["python/databento_py.cpp", "src/parser.cpp"],
        include_dirs=[os.path.join(here, "include")],
        extra_compile_args=["-O3", "-march=native", "-std=c++20"],
        cxx_std=20,
    ),
]

# Read README for long description
with open(os.path.join(here, "README.md"), "r", encoding="utf-8") as f:
    long_description = f.read()

setup(
    name="databento-cpp-fast",
    version=__version__,
    author="databento-cpp-fast contributors",
    author_email="",
    description="Ultra-fast Databento DBN parser (200M+ records/sec) - Alternative to official databento-cpp",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/YOUR_USERNAME/databento-cpp",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=[
        "pybind11>=2.10.0",
    ],
    extras_require={
        "dev": ["pytest", "numpy", "pandas"],
    },
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Financial and Insurance Industry",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: Office/Business :: Financial",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: C++",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS",
    ],
    keywords="databento market-data high-frequency-trading hft parsing performance",
)
