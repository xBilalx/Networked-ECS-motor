# Pong and Power4
## Build

First intall conan :
```
pip install conan
```
Then configure conan :
```
conan profile detect
```
Then configure the build :
```
mkdir build
conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
```
Then configure some more and compile
```
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cd ..
cmake --build ./build
```
Now launch !
```
# start the server of the game of your choice :
./pong_server
or
./power4_server

# start the client of the game of your choice :
./pong_client
or
./power4_client
```
