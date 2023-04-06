FROM ubuntu:22.04 as base

ENV TZ=Australia/Sydney
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && dpkg-reconfigure --frontend noninteractive tzdata

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install --assume-yes git cmake make clang \
    libmysqlclient-dev libssl-dev libbz2-dev libreadline-dev libncurses-dev \
    libboost-all-dev mysql-server
RUN rm -rf /var/lib/apt/lists/*

FROM base as builder

ENV TZ=Australia/Sydney
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && dpkg-reconfigure --frontend noninteractive tzdata

COPY .git /azerothcore/.git
COPY apps /azerothcore/apps
COPY bin /azerothcore/bin
COPY conf /azerothcore/conf
COPY data /azerothcore/data
COPY deps /azerothcore/deps
COPY etc /azerothcore/etc
COPY modules /azerothcore/modules
COPY src /azerothcore/src
COPY CMakeLists.txt /azerothcore/CMakeLists.txt

WORKDIR /azerothcore/build
RUN --mount=type=cache,target=/azerothcore/build \
  cmake ../ -DCMAKE_INSTALL_PREFIX=/azerothcore/ -DCMAKE_C_COMPILER=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DWITH_WARNINGS=1 -DTOOLS_BUILD=none \
    -DSCRIPTS=static -DMODULES=static && \
  make --jobs=$(nproc) && \
  make install

FROM base as server

ENV TZ=Australia/Sydney
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && dpkg-reconfigure --frontend noninteractive tzdata

COPY --from=wobgob/client-data:v16 /azerothcore/data /azerothcore/data
COPY --from=builder /azerothcore/data /azerothcore/data
COPY --from=builder /azerothcore/etc /azerothcore/etc
COPY --from=builder /azerothcore/modules /azerothcore/modules
COPY --from=builder /azerothcore/bin/authserver /azerothcore/bin/authserver
COPY --from=builder /azerothcore/bin/worldserver /azerothcore/bin/worldserver
COPY entrypoint.sh /entrypoint.sh

RUN mkdir /azerothcore/logs /azerothcore/temp

EXPOSE 3306
EXPOSE 3724
EXPOSE 8085

ENTRYPOINT ["/entrypoint.sh"]
