FROM voidlinux/voidlinux:latest

WORKDIR /opt/yippy

RUN xbps-install -S bash python3 gcc make -y

COPY . /opt/yippy/

RUN make

RUN ./yippy -v

CMD ["./yippy"]

RUN echo "Performing tests." && cd tests/ && YIPPY_DIR=../ ./tests
