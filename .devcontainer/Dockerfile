FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    python3 \
    vim \
    emacs \
    git \
    wget
    
RUN apt-get install -y \
    unzip \
    zip

WORKDIR /assignments