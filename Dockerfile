FROM voidlinux/voidlinux:latest

WORKDIR /opt/yippy

RUN xbps-install -S gcc make cmake -y

COPY . /opt/yippy/

RUN cmake . && make 

RUN ./yippy -v

CMD ["./yippy"]
