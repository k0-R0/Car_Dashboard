# PIC18 Automotive CAN Node Simulator

This project is an automotive network simulator built using PIC18 microcontrollers. It simulates a distributed vehicle system where sensor data (speed, RPM, gears, indicators) is read by transmitter nodes and shared over a Controller Area Network (CAN) bus to a central display node (ECU3).

## Table of Contents
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Required](#hardware-required)
- [Getting Started](#getting-started)

## Features
- **Multi-Node Network:** Independent PIC18 nodes communicating over a shared CAN bus.
- **Hardware CAN:** Uses the PIC18 microcontroller's internal CAN module paired with external CAN transceivers.
- **Sensor Acquisition:** Simulates driving metrics using analog inputs (potentiometers for speed/RPM) and digital inputs (switches for gears and turn indicators).

## System Architecture

The following diagram shows the hardware configuration and data flow across all three nodes on the CAN network, where ECU1 and ECU2 transmit sensor readings, and ECU3 receives and displays them on the CLCD and output:

```mermaid
flowchart TD
    %% Styling
    classDef input fill:#2e3440,stroke:#81a1c1,stroke-width:2px,color:#d8dee9;
    classDef inner fill:#434c5e,stroke:#4c566a,color:#e5e9f0;
    classDef bus fill:#8fbcbb,stroke:#81a1c1,stroke-width:2px,color:#2e3440,font-weight:bold;
    classDef output fill:#2e3440,stroke:#a3be8c,stroke-width:2px,color:#d8dee9;

    %% External Inputs
    Pot1(Potentiometer: Speed):::input
    Key1(Digital Keypad: Gear):::input
    Pot2(Potentiometer: RPM):::input
    Key2(Digital Keypad: Indicator):::input

    %% ECU 1 Nodes
    subgraph ECU1 [ECU 1: Speed & Gear Transmitter]
        ADC1(ADC):::inner
        CPU1(MCU Core / Processing):::inner
        Ctrl1(CAN Controller):::inner
        Trans1(CAN Transceiver):::inner
        
        ADC1 --> CPU1
        CPU1 --> Ctrl1
        Ctrl1 --> Trans1
    end

    %% ECU 2 Nodes
    subgraph ECU2 [ECU 2: RPM & Indicator Transmitter]
        ADC2(ADC):::inner
        CPU2(MCU Core / Processing):::inner
        Ctrl2(CAN Controller):::inner
        Trans2(CAN Transceiver):::inner
        
        ADC2 --> CPU2
        CPU2 --> Ctrl2
        Ctrl2 --> Trans2
    end

    %% Shared Network
    Bus(Shared CAN Bus):::bus

    %% ECU 3 Nodes
    subgraph ECU3 [ECU 3: Dashboard Receiver]
        Trans3(CAN Transceiver):::inner
        Ctrl3(CAN Controller):::inner
        CPU3(MCU Core / Processing):::inner
        
        Trans3 --> Ctrl3
        Ctrl3 --> CPU3
    end

    %% External Outputs
    CLCD(CLCD Display: Dashboard):::output

    %% Connections to ECU 1
    Pot1 -- Analog Signal --> ADC1
    Key1 -- Digital Input --> CPU1

    %% Connections to ECU 2
    Pot2 -- Analog Signal --> ADC2
    Key2 -- Digital Input --> CPU2

    %% Network Connections
    Trans1 <==> Bus
    Trans2 <==> Bus
    Bus <==> Trans3

    %% Output Connections
    CPU3 -- LCD Output --> CLCD
```

## Hardware Required
- 3x Rhydolabz PIC18 Development Boards (with onboard CAN transceivers)
- 2x 10kΩ Potentiometers (for Speed and RPM simulation)
- 2x Digital Keypads/Switches (for Gear and Indicator selection)
- Twisted-pair wiring (for CAN_H / CAN_L connections)
- 120Ω termination resistors (or termination jumpers enabled on development boards)

## Getting Started

### 1. Clone the Repository
```bash
git clone https://github.com/k0-R0/Car_Dashboard.git
```

### 2. Environment Setup
- Open MPLAB X IDE.
- Ensure the XC8 compiler is installed and configured.

### 3. Flashing the ECUs
Open and flash the following projects to the respective boards:
- Open `ECU1.X`, compile, and flash to Board 1.
- Open `ECU2.X`, compile, and flash to Board 2.
- Open `ECU3.X`, compile, and flash to Board 3.

### 4. Wiring and Bus Termination
- Connect the CAN High (`CAN_H`) and CAN Low (`CAN_L`) terminals across all three boards using twisted-pair wiring.
- Ensure exactly two 120Ω termination resistors (or termination jumpers) are enabled at the physical ends of the CAN bus.
