```markdown
# ECSNetwork

## Build

Create the build directory:
```
mkdir build
```

Configure and generate the build files:
```
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cd ..
```

Compile the project and move the executables to the root:
```
cmake --build ./build ; \
  mv build/pong/client/pong_client . ; \
  mv build/pong/server/pong_server . ; \
  mv build/power4/network/client/power4_client . ; \
  mv build/power4/network/server/power4_server . ; \
  mv build/power4/local/power4_local .
```

## Run

### For Pong
Start the server:
```
./pong_server
```
Then start the client(s):
```
./pong_client <ip> <username>
```

### For Power4
For the network version:
- Start the server:
  ```
  ./power4_server
  ```
- Start the client:
  ```
  ./power4_client <ip> <username>
  ```

For the local version:
```
./power4_local
```
```