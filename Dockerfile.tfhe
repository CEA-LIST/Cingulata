#######################################
# Cingulata with TFHE library
#
# docker build -t cingulata:tfhe -f Dockerfile.tfhe --build-arg uid=$(id -u) .
# docker run -it --rm -v $(pwd):/cingu cingulata:tfhe
#######################################

FROM ubuntu:18.04

# install dependencies
RUN apt-get update -qq \
 && apt-get install --no-install-recommends -y \
    ca-certificates \
    cmake \
    g++ \
    git \
    libboost-program-options-dev \
    make

# clone and compile TFHE library
RUN git clone https://github.com/tfhe/tfhe /tfhe \
 && cd tfhe \
 && make

# add user
ARG uid=1000
ARG uname=cingu
RUN useradd -u $uid $uname

USER $uname

ENV build_dir=build_tfhe

# compilation command
CMD mkdir -p /cingu/$build_dir \
 && cd /cingu/$build_dir \
 && cmake -DTFHE_PATH=/tfhe .. \
 && make

WORKDIR /cingu/
