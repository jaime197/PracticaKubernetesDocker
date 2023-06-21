FROM ubuntu:20.04

RUN apt-get -y update
RUN apt-get -y install cmake
RUN apt-get -y install g++ gdb gcc

COPY . /multMatrix
WORKDIR /multMatrix

RUN rm -r build
RUN mkdir build

WORKDIR /multMatrix/build
RUN cmake ..
RUN make

EXPOSE 32444

CMD ./server
