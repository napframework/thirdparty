ARG inst_dir="/tmp/out"
ARG image_name
FROM nap-${image_name} AS builder
ARG image_name
ARG inst_dir

ENV package_version="2.0.9"
ENV package_dir="SDL-release-${package_version}"
ENV package_archive="release-${package_version}.tar.gz"

RUN \
  wget --progress=bar:force:noscroll https://github.com/SDL-mirror/SDL/archive/refs/tags/${package_archive} && \
  tar -xvf ${package_archive} && \
  cd ${package_dir} && \
  ./configure --prefix=${inst_dir} --enable-static=no --enable-sndio=no && \
  make -j `nproc` && \
  make -j `nproc` install && \
  rm -rf ../${package_archive} \
    ../${package_dir}

FROM scratch
ARG inst_dir
COPY --from=builder ${inst_dir} /
