ARG inst_dir="/tmp/out"
ARG image_name
FROM nap-${image_name} AS builder
ARG inst_dir

ENV src_dir="/input"
COPY linux/source ${src_dir}

RUN \
  cd ${src_dir}/src/driver/libetherdream && \
  make && \
  mkdir ${inst_dir} && \
  cp libetherdream.so ${inst_dir} && \
  patchelf --set-soname libetherdream.so libetherdream.so && \
  rm -rf build ${src_dir}

FROM scratch
ARG inst_dir
COPY --from=builder ${inst_dir} /
