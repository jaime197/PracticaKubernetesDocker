FROM ubuntu:20.04

RUN apt-get -y update
RUN apt-get -y install cmake
RUN apt-get -y install g++ gdb gcc

COPY . /fileManager
WORKDIR /fileManager

RUN rm -r build
RUN mkdir build

WORKDIR /fileManager/build
RUN cmake ..
RUN make

EXPOSE 32443

RUN mkdir pruebas
RUN cd pruebas
RUN echo "Texto de prueba 1" > example1.txt
RUN echo "Sample text 2" > example2.txt
RUN cd ..

COPY ./bash.sh .

CMD ./bash.sh
