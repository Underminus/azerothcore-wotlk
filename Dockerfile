FROM ubuntu:20.04 as base

ARG USER_ID=1000
ARG GROUP_ID=1000
ARG USER=wobgob

ENV TZ=Australia/Sydney
ENV DEBIAN_FRONTEND=noninteractive

RUN sed --in-place --regexp-extended "s/(\/\/)(archive\.ubuntu)/\1au.\2/" /etc/apt/sources.list && \
	apt-get update && apt-get upgrade --yes

RUN apt-get update && apt-get install -y \
  gdb gdbserver git dos2unix lsb-core sudo curl unzip make cmake clang \
  libmysqlclient-dev libboost-system1.7*-dev libboost-filesystem1.7*-dev \
  libboost-program-options1.7*-dev libboost-iostreams1.7*-dev build-essential \
  libtool cmake-data openssl libgoogle-perftools-dev google-perftools \
  libssl-dev libmysql++-dev libreadline6-dev zlib1g-dev libbz2-dev mysql-client \
  libncurses5-dev ccache \
  && rm -rf /var/lib/apt/lists/*

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && dpkg-reconfigure --frontend noninteractive tzdata

RUN addgroup --gid $GROUP_ID wobgob && \
    adduser --disabled-password --gecos '' --uid $USER_ID --gid $GROUP_ID wobgob && \
    passwd -d wobgob && \
    echo 'wobgob ALL=(ALL:ALL) NOPASSWD: ALL' >> /etc/sudoers

RUN mkdir /core
RUN chown -R $USER:$USER /core

COPY --chown=$USER:$USER apps /core/apps
COPY --chown=$USER:$USER bin /core/bin
COPY --chown=$USER:$USER conf /core/conf
COPY --chown=$USER:$USER data /core/data
COPY --chown=$USER:$USER deps /core/deps
COPY --chown=$USER:$USER etc /core/etc
COPY --chown=$USER:$USER modules /core/modules
COPY --chown=$USER:$USER src /core/src
COPY --chown=$USER:$USER CMakeLists.txt /core/CMakeLists.txt

WORKDIR /core

RUN chown -R $USER:$USER /home/wobgob
RUN chown -R $USER:$USER /run
RUN chown -R $USER:$USER /opt

RUN mkdir build
RUN cd build && \
  cmake ../ -DCMAKE_INSTALL_PREFIX=/core/ -DCMAKE_C_COMPILER=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DWITH_WARNINGS=1 -DTOOLS=0 \
    -DSCRIPTS=static -DMODULES=static && \
  make -j $(nproc) && \
  make install

FROM ubuntu:20.04 as server

ARG USER_ID=1000
ARG GROUP_ID=1000
ARG USER=wobgob

ENV TZ=Australia/Sydney
ENV DEBIAN_FRONTEND=noninteractive

RUN addgroup --gid $GROUP_ID wobgob && \
    adduser --disabled-password --gecos '' --uid $USER_ID --gid $GROUP_ID wobgob && \
    passwd -d wobgob && \
    echo 'wobgob ALL=(ALL:ALL) NOPASSWD: ALL' >> /etc/sudoers

RUN sed --in-place --regexp-extended "s/(\/\/)(archive\.ubuntu)/\1au.\2/" /etc/apt/sources.list && \
	apt-get update && apt-get upgrade --yes

RUN apt-get update && apt-get install -y \
  dos2unix gdb gdbserver google-perftools libgoogle-perftools-dev net-tools \
  libboost-system1.7*-dev libboost-filesystem1.7*-dev \
  libboost-program-options1.7*-dev libboost-iostreams1.7*-dev tzdata \
  libmysqlclient-dev mysql-client curl unzip \
  && rm -rf /var/lib/apt/lists/*

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && dpkg-reconfigure --frontend noninteractive tzdata

RUN chown -R $USER:$USER /home/wobgob
RUN chown -R $USER:$USER /run
RUN chown -R $USER:$USER /opt

USER $USER

COPY --chown=$USER:$USER --from=base /core/data /core/data
COPY --chown=$USER:$USER --from=base /core/etc /core/etc
COPY --chown=$USER:$USER --from=base /core/modules /core/modules
COPY --chown=$USER:$USER --from=base /core/bin/authserver /core/bin/authserver
COPY --chown=$USER:$USER --from=base /core/bin/worldserver /core/bin/worldserver
COPY --chown=$USER:$USER entrypoint.sh /

EXPOSE 3724
EXPOSE 8085

ENTRYPOINT ["/entrypoint.sh"]
