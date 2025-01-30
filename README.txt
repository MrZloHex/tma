███╗   ███╗ ██████╗ ███╗   ██╗ ██████╗ ██╗     ██╗████████╗██╗  ██╗
████╗ ████║██╔═══██╗████╗  ██║██╔═══██╗██║     ██║╚══██╔══╝██║  ██║
██╔████╔██║██║   ██║██╔██╗ ██║██║   ██║██║     ██║   ██║   ███████║
██║╚██╔╝██║██║   ██║██║╚██╗██║██║   ██║██║     ██║   ██║   ██╔══██║
██║ ╚═╝ ██║╚██████╔╝██║ ╚████║╚██████╔╝███████╗██║   ██║   ██║  ██║
╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚═╝  ╚═╝
  
  ░▒▓█ _TMA_ █▓▒░  
  The monolithic gateway of **MONOLITH**—a WebSocket server  
  that bridges _obelisk_ to the outside world.  

  ───────────────────────────────────────────────────────────────  
  ▓ OVERVIEW  
  **_TMA_** is the network hub of MONOLITH, running on **HAL9000**  
  (an old but reliable machine).  
  It is a **pure C WebSocket server**, allowing **_obelisk_** (via ESP32)  
  to fetch real-time data:  
  ▪ RTC (Network Time)  
  ▪ Weather Conditions  
  ▪ External API Data  
  ▪ Internal MONOLITH System State  

  The ESP32 inside _obelisk_ connects to _TMA_ over WebSockets  
  to retrieve data, send commands, and communicate with the network.  

  ───────────────────────────────────────────────────────────────  
  ▓ HARDWARE / ENVIRONMENT  
  ▪ **HOST**: HAL9000 (Old Laptop)  
  ▪ **LANGUAGE**: Pure C  
  ▪ **SERVER**: Custom WebSocket Server  
  ▪ **CLIENTS**: ESP32 (_obelisk_), Future MONOLITH Nodes  
  ▪ **NETWORK**: Local & Internet  

  ───────────────────────────────────────────────────────────────  
  ▓ FEATURES  
  ▪ **Lightweight WebSocket Server in C**  
  ▪ **Low Latency Communication with _obelisk_**  
  ▪ **NTP Time Sync for Accurate RTC**  
  ▪ **Weather Data Fetching & Caching**  
  ▪ **High-Performance Non-Blocking I/O**  

  ───────────────────────────────────────────────────────────────  
  ▓ BUILD & RUN  

  ```sh
  make &&
  ./bin/tma -p <PORT> -f
  ```

  ───────────────────────────────────────────────────────────────  
  ▓ FINAL WORDS  
  This is not just a server.  
  This is **_TMA_**—the gateway of MONOLITH.  

