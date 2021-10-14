ARG inst_dir="/tmp/out"
ARG image_name
FROM nap-$image_name AS builder
ARG image_name
ARG inst_dir

ENV src_dir="/input"
COPY source ${src_dir}

# TODO debug and re-enable tests
#   RUN apt-get update && apt-get --no-install-recommends install -y libgtest-dev && \
#     rm -rf /var/lib/apt/lists/*

RUN \
  cd ${src_dir} && \
  cmake \
    -DCMAKE_INSTALL_PREFIX=${inst_dir} \
    -DBUILD_SHARED_LIBS=ON \
    -DASSIMP_BUILD_TESTS=OFF \
    -DCMAKE_C_FLAGS=-fPIC \
    -DCMAKE_CXX_FLAGS=-fPIC \
    CMakeLists.txt \
    -G 'Unix Makefiles' && \
  make -j `nproc` && \
  make -j `nproc` install && \
  rm -rf build ${src_dir}

FROM scratch
ARG inst_dir
COPY --from=builder ${inst_dir} /
