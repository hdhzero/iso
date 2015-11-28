TOOLCHAIN_SRC="$PWD"/src
NASM_DIR="$PWD"/nasm
BOCHS_DIR="$PWD"/bochs
SDL_DIR="$PWD"/sdl
CDRTOOLS_DIR="$PWD"
PREFIX="$PWD"
TARGET=i686-elf
PATH="$PREFIX/bin:$PATH"
 
mkdir -p src
cd src
echo "Downloading source code"
sleep 5
wget -c http://mirrors.concertpass.com/gcc/releases/gcc-4.9.1/gcc-4.9.1.tar.gz
wget -c ftp://sourceware.org/pub/binutils/releases/binutils-2.24.tar.gz
wget -c http://www.nasm.us/pub/nasm/releasebuilds/2.11.08/nasm-2.11.08.tar.gz
wget -c -O bochs-2.6.8.tar.gz http://sourceforge.net/projects/bochs/files/bochs/2.6.8/bochs-2.6.8.tar.gz/download
wget -c -O cdrtools-3.00.tar.gz http://sourceforge.net/projects/cdrtools/files/cdrtools-3.00.tar.gz/download
wget -c https://www.libsdl.org/release/SDL-1.2.15.tar.gz
 
echo "Decompressing files"
sleep 5
tar xf binutils-2.24.tar.gz
tar xf gcc-4.9.1.tar.gz
tar xf nasm-2.11.08.tar.gz
tar xf bochs-2.6.8.tar.gz
tar xf cdrtools-3.00.tar.gz
tar xf SDL-1.2.15.tar.gz
 
echo "Compiling and installing SDL..."
sleep 5
cd SDL-1.2.15
./configure --prefix=$SDL_DIR
make
make install
cd ..
echo "SDL installed"
 
 
echo "Compiling and installing NASM..."
sleep 5
cd nasm-2.11.08
chmod +x configure
./configure --prefix=$NASM_DIR
make
make install
cd ..
echo "NASM installed"
 
echo "Compiling and installing Bochs..."
sleep 5
export CFLAGS="`$SDL_DIR/bin/sdl-config --cflags`"
export CXXFLAGS="`$SDL_DIR/bin/sdl-config --cflags`"
export LDFLAGS="`$SDL_DIR/bin/sdl-config --static-libs`"
export LIBS="-lSDL -lpthread -lm -ldl -lpthread"
cd bochs-2.6.8
./configure --enable-smp \
              --enable-cpu-level=6 \
              --enable-all-optimizations \
              --enable-x86-64 \
              --enable-pci \
              --enable-vmx \
              --enable-debugger \
              --enable-disasm \
              --enable-debugger-gui \
              --enable-logging \
              --enable-fpu \
              --enable-3dnow \
              --enable-sb16=dummy \
              --enable-cdrom \
              --enable-x86-debugger \
              --enable-iodebug \
              --disable-plugins \
              --disable-docbook \
              --with-sdl --prefix=$BOCHS_DIR
make
make install
cd ..
echo "Bochs installed"
 
 
echo "Compiling and installing mkisofs..."
sleep 5
 
cd cdrtools-3.00
make
cd mkisofs/OBJ
cd `ls`
cp mkisofs $CDRTOOLS_DIR
cd $PREFIX
echo "mkisofs installed"
 
echo "Compiling and installing binutils..."
sleep 5
cd src
mkdir build-binutils
cd build-binutils
../binutils-2.24/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
echo "Binutils installed"
 
echo "Compiling and installing gcc..."
cd ..
cd gcc-4.9.1
./contrib/download_prerequisites
cd ..
mkdir build-gcc
cd build-gcc
../gcc-4.9.1/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
 
echo "Gcc installed"
echo "Exiting..."
