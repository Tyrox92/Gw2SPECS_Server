FROM alpine:latest

ENV PORT 1234
RUN apk add --no-cache qt5-qtbase git qt5-qtbase-dev g++

RUN apk add --no-cache make

RUN mkdir /root/gw2specs

WORKDIR /root/gw2specs
RUN git clone https://github.com/Tyrox92/Gw2SPECS_Server.git .

RUN rm -rf /var/cache/apk/*
RUN qmake-qt5 && make

COPY entry-point.sh .
RUN chmod u+x entry-point.sh

CMD ["sh","entry-point.sh"]
