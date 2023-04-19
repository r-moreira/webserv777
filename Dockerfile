# Build and run:
#   docker build -t clion/ubuntu/cpp-env:1.0 -f Dockerfile.cpp-env-ubuntu .

FROM arm64v8/ubuntu:latest

RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
  && apt-get install -y build-essential \
      gcc \
      g++ \
      gdb \
      clang \
      make \
      ninja-build \
      cmake \
      autoconf \
      automake \
      libtool \
      valgrind \
      locales-all \
      rsync \
      tar \
      python-is-python3 \
      python3 \
  && apt-get clean

COPY ./public/ /tmp/webserv777/cmake-build-debug-docker/
COPY ./configuration/ /tmp/webserv777/cmake-build-debug-docker/

EXPOSE 8080
EXPOSE 8081
EXPOSE 8082