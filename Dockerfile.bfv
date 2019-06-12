#######################################
# Cingulata with in-house B/FV SHE implementation
#
# docker build -t cingulata:bfv -f Dockerfile.bfv --build-arg uid=$(id -u) .
# docker run -it --rm -v $(pwd):/cingu cingulata:bfv
#######################################

FROM ubuntu:18.04

# install dependencies
RUN apt-get update -qq \
 && apt-get install --no-install-recommends -y \
    ca-certificates \
    cmake \
    g++ \
    git \
    libboost-graph-dev \
    libboost-program-options-dev \
    libflint-dev \
    libpugixml-dev \
    make \
    python3 \
    python3-networkx \
    python3-numpy \
    xxd

# add user
ARG uid=1000
ARG uname=cingu
RUN useradd -u $uid $uname

USER $uname

ENV build_dir=build_bfv

# compilation command
CMD mkdir -p /cingu/$build_dir \
 && cd /cingu/$build_dir \
 && cmake -DUSE_BFV=ON .. \
 && make

WORKDIR /cingu/

