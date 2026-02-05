Project Overview
- This is the firmware for the GLOW eindhoven 2024 "Echoes of light" installation. The codename for this installation is called ROLF (Responsive Oracular Light Fixture).
- The installation consists of a network of interconnected pipes with horns at the ends of some pipes. When a user speaks into a horn, the sound is detected by a microphone in the horn, and a light effect is propagated through the network of pipes. Once this effect has reached another horn in this network, that audio is played back through a speaker in that horn.
- This repository contains two related PlatformIO/Arduino firmware projects that implement a modular, distributed LED-and-peripheral control system for an installation: `ROLF - Master` (central coordinator, network/comms, session and effect dispatch) and `ROLF - Module` (per-module ESP32 firmware driving LED panels and local peripherals). The project exists to prototype a scalable, self-discovering lighting network with synchronized effects and OTA updates for a GLOW installation.

Project Metadata
- Type: Firmware (embedded, PlatformIO / Arduino-style)
- Client: GLOW: Eindhoven Light Art Festival, as part of the Fontys Engineering beCreative minor.
- Status: Mixed — `ROLF - Module`: implemented and runnable on ESP32; `ROLF - Master`: prototype coordinator present but network-level validation incomplete
- Product: ROLF distributed module firmware (master + module images)

Technical Stack
- Programming language(s): C++ (Arduino-style, PlatformIO project)
- Hardware / platform: ESP32 (development boards / custom PCBs expected)
- Operating system / RTOS (if applicable): Arduino framework on ESP32 with FreeRTOS tasks used in `ROLF - Module`
- Tools & build system: PlatformIO (VSCode extension or CLI)
- Communication protocols / interfaces (if applicable): Wi‑Fi (used for development and OTA), planned UART passthrough (one-wire, half-duplex) for production, ArduinoOTA for firmware updates, GPIO, analog inputs, and WS28xx LED strips via the Trinity LED abstractions (TrinityLED library by me)

System Description
- High-level architecture: Two-tier system. A central Master builds a 3D map of modules and performs pathfinding to route light effects; individual Modules (ESP32) manage local LED panels, inputs and peripheral IO and receive effect instructions from the Master.
- Main components, tasks, or modules:
  - `ROLF - Master`:
    - `main.cpp`: session management, importing new clients, horn triggers, effect dispatch and module management.
    - `Comms/`, `moduleManager/`: communication helpers and topology/pathfinding logic.
  - `ROLF - Module`:
    - `main.cpp`: creates two pinned FreeRTOS tasks (`task_main` on core 0, `task_leds` on core 1); handles Wi‑Fi, OTA, horn sensing, and shared LED update buffer.
    - `ConnectorManager/`: module-side communications and buffering.
    - `Trinity` / `Panel` / LED stack: per-panel and per-diode effect scheduling and rendering.
  - Shared ideas: effect scheduling (VFXData), message ID / sender checks to avoid duplicates, and a hardware `LedSync` pin concept to synchronise playback across modules.

Current State
- What is working:
  - `ROLF - Module` contains a functional ESP32 firmware skeleton: Wi‑Fi connect, `ArduinoOTA`, FreeRTOS tasks, `Trinity` LED setup and per-panel effect scheduling.
  - `ROLF - Master` implements session ID management, accepts module reports, and can compute and transmit LED update payloads.
  - Basic message flow and per-module effect application are implemented in code paths.
- What is partially working:
  - Network orchestration (end-to-end multi-node discovery/pathing) has code but has not been validated at scale.
  - Prototype pathfinding and demo flows exist but include TODOs and comments referencing unfinished features.
- What is not implemented or known limitations:
  - The production communication plan (one-wire UART passthrough) was not used for development; Wi‑Fi was used instead and must be replaced for production.
  - Self-discovery mapping was observed to be non-functional in final tests; root causes may include wiring, SoftwareSerial reliability, or mapping logic.

Key Technical Challenges
- SoftwareSerial / one-wire UART reliability: Software-based serial on GPIO showed missed bytes and flipped bits under test. SoftwareSerial is required to minimise pin usage (6 pins are required for all the xyz directions a pipe can be in). 
- Synchronized playback across many modules: network and transport jitter can cause desynchronisation. Existing approach: buffer incoming LED instructions locally and use a `LedSync` pulldown pin propagated through the chain to trigger simultaneous start. Verify signal propagation and timing margins in hardware tests.
- 3D self-discovery and map placement: rotating and placing modules in 3D is more complex than 2D. Approach used: Master requests connection data and performs placement, with a planned fallback of predefined placements by MAC if discovery fails. (Though not documented, there has been the idea to skip creating a 3d map. The creation of such a map is complex and error-prone. Needing a human to be able to read this map may not be required.)
- Practical multicore design on ESP32: pinning FreeRTOS tasks to cores to isolate network/OTA from timing-sensitive (and also processor heavy) LED rendering.


Future Improvements
- Replace Wi‑Fi development transport with the planned UART passthrough and rework connector logic.
- Fix SoftwareSerial issues, add CRC and reliable ACK/retry mechanisms.
- Complete and stabilise self-discovery (3D), add removal and dynamic map resizing; or add a predefined-placement fallback mode.
- Document wiring, PCB pin labels, and create example hardware diagrams and a deployment checklist.
- Replacing modulare components with libraries (e.g., The light codebase being turned into TrinityLED).

Disclaimer
- This Readme file has been generated by an AI language model (ChatGPT) based on the provided code context.
- I have only quickly reviewed and edited the content for accuracy.
