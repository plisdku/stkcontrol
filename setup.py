from setuptools import setup

setup(name="stkcontrol",
    version="0.1",
    description="STK WAV writer",
    url="https://github.com/plisdku/stkcontrol",
    author="Paul Hansen",
    author_email="paul.c.hansen@gmail.com",
    license="MIT",
    packages="stkcontrol",
    install_requires=[
        "numpy"
    ],
    zip_safe=False)