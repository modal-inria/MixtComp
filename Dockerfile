# set base image (host OS)
FROM python:3.9-slim-bullseye

# set the working directory in the container
WORKDIR /MixtComp

# Checkout MixtComp
COPY . .

# Allow sudo
RUN apt-get update && \
    apt-get -y install sudo
RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo

# Install MixtComp dependencies
RUN sudo apt-get -y install cmake libboost-all-dev libeigen3-dev libxml2-dev lcov

# Install c++ Compiler
RUN sudo apt-get -y install build-essential
RUN sudo apt-get -y install cmake g++

# Install git
RUN sudo apt-get -y install git


# Install Tests dependencies
RUN pip install pytest flake8

# Install Other Python Dependencies
RUN pip install -r pyMixtComp/python/requirements.txt

# Install Jupyter
RUN pip install jupyterlab

WORKDIR /MixtComp/pyMixtComp
# Compile PyMixtCompBridge
RUN ./build.sh
# Test PyMixtCompBridge
RUN ./utest.sh

WORKDIR /MixtComp/pyMixtComp/python
# Copy PyMixtCompBridge.so
RUN cp ../build/lib/pyMixtCompBridge.so pyMixtComp/pyMixtCompBridge.so

# Install PyMixtComp
RUN pip install -e .

# Test if installation is ok
WORKDIR /MixtComp
RUN python -c "from pyMixtComp import MixtComp"

# Test with pytest 
WORKDIR /MixtComp/pyMixtComp/python
RUN pytest ./tests

# command to run on container start
CMD ["jupyter", "lab", "--ip", "0.0.0.0", "--allow-root"]
