ARG parent_image
FROM $parent_image

RUN apt-get clean
RUN apt-get update --fix-missing
RUN apt-get -y install wget git build-essential software-properties-common apt-transport-https --fix-missing

# The llvm we are looking for
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && echo $(gcc -v)
RUN apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main" -y
RUN apt-get update
RUN apt-get install -y gcc-7 g++-7 clang llvm

# Clone and build the repository for Waffle
RUN git clone -b waffle --single-branch https://github.com/behnamarbab/memlock-waffle.git /source_files
RUN cd /source_files/waffle && \
    unset CFLAGS CXXFLAGS && \
    AFL_NO_X86=1 make && \
    cd llvm_mode && make

#  Install the driver for communicating with FuzzBench
RUN wget https://raw.githubusercontent.com/llvm/llvm-project/5feb80e748924606531ba28c97fe65145c65372e/compiler-rt/lib/fuzzer/afl/afl_driver.cpp -O /source_files/afl_driver.cpp  && \
    cd /source_files/waffle && unset CFLAGS CXXFLAGS && \
    clang -Wno-pointer-sign -c /source_files/waffle/llvm_mode/afl-llvm-rt.o.c -I/source_files/waffle/ && \
    clang++ -stdlib=libc++ -std=c++11 -O1 -c /source_files/afl_driver.cpp  && \
    ar r /libAFL.a *.o
