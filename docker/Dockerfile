FROM frolvlad/alpine-gcc
RUN mkdir -p /home/build
COPY src/main.c /home/build
WORKDIR /home/build
RUN gcc --static -o memleak main.c

FROM alpine:3.14
RUN mkdir -p /opt/memleak
WORKDIR /opt/memleak
COPY --from=0 /home/build/memleak /opt/memleak/
CMD ["/opt/memleak/memleak"]
