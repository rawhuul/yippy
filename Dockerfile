FROM alpine:latest

WORKDIR /opt/yippy

RUN apk --no-cache add gcc g++ make cmake

COPY . /opt/yippy/

RUN cmake . && make 

RUN ./yippy -h
