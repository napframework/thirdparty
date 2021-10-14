ARG inst_dir="/tmp/out"
ARG image_name
FROM nap-${image_name} AS builder
ARG inst_dir

ENV src_dir="/input"
COPY source ${src_dir}

RUN \
  cd ${src_dir} && \
  cmake -DCMAKE_INSTALL_PREFIX=${inst_dir} -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC CMakeLists.txt && \
  make -j `nproc` && \
  make -j `nproc` install && \
  rm -rf build ${src_dir}

FROM scratch
ARG inst_dir
COPY --from=builder ${inst_dir} /
