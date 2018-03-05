FROM ubuntu:16.04

# Setup ubuntu base image and Cingulata dependencies
RUN \
  apt-get update && \
  apt-get -y upgrade && \
  apt-get install -y \
    build-essential \
    git \
    libflint-dev \
    libmpfr-dev \
    libxml2-dev \
    libpugixml-dev \
    libreadline-dev \
    cmake \
    mercurial \
    libboost-program-options-dev \
    libboost-graph-dev \
    python3-networkx \
    python3-mpmath  \
    software-properties-common && \
  rm -rf /var/lib/apt/lists/*

# a number (e.g. 4, 8, etc.) corresponding to the number of cores on which the compilation takes place
ARG CINGULATA_CORES=4

# Build Cingulata
RUN \
  git clone https://github.com/CEA-LIST/Cingulata.git && \
  cd Cingulata && \
  mkdir build && \
  cd build/  && \
  cmake ..  && \
  make -j ${CINGULATA_CORES}
