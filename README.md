##Disclaimer

Tested on Win11 x64, MSVC 14.29.30133, CMake 3.20.2

##Instructions

1. Run lab4-server.exe in `server-out/`
2. Run lab4-client.exe in `client-out/` with argument `0` (text processor) or `1` (calculator)
3. Every second `Idle` event is sent to server by each client
4. DB's are located in `server-out/*-db.txt` files