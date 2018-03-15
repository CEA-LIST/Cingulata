FROM debian:stretch-slim
MAINTAINER Henri Binsztok, hbinsztok@wallix.com

# Build system
RUN apt-get update && apt-get install -y \
	ca-certificates \
	build-essential

# Cingulata Build deps
RUN apt-get install -y --no-install-recommends \
	libflint-dev libmpfr-dev \
	libxml2-dev libpugixml-dev libreadline-dev \
	libboost-program-options-dev libboost-graph-dev \
	cmake mercurial git

# Cingulata Runtime deps
RUN apt-get install -y --no-install-recommends \
	python3-networkx python3-mpmath python3-numpy

# Build
RUN git clone https://github.com/CEA-LIST/Cingulata \
	&& cd Cingulata \
	&& mkdir build \
	&& cd build \
	&& cmake .. \
	&& make -j$(nproc)

# Run Hello
WORKDIR "/Cingulata/build/tests/hello"
CMD ["bash", "run.sh"]
