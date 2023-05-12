# Displite

### Under development

Display information on small screen display modules using Raspberry Pi Pico. 

Supported display controllers/Modules:

- ILI9486 (Tested on MPI3501 display)

### Build Steps

1. Clone Project

    ```bash
    git clone https://github.com/Displite/displite.git
    cd displite
    ```
2. Install build toolchain

    ```bash
    sudo apt update
    sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential git libstdc++-arm-none-eabi-newlib
    pip install -r requirements.txt
    ```

3. Configure project

    ```bash
    chmod +x ./configure.py
    ./configure.py init
    ```

4. Build

    ```bash
    mkdir build && cd build
    cmake ..
    make -j $(nproc)
    ```

5. Upload `displite_demo.uf2` or `displite_demo.elf` to rp2040 board.