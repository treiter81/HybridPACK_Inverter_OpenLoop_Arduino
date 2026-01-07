# HybridPACK_Inverter_OpenLoop_Arduino
Project still in preparation phase.

# Motivation and why Arduino:
Motivation of this plattform and datasharing was to support open research activities. <br>
Open Loop Inverter Testing is crucial for Testing Inverter Hardware and Power Module/Gate Driver Performance. The Arduino plattform is establed for open-source and toolchains are widely available. Wheras it is recommended to use Infineon Aurix Microcontroller for serial applications and closed loop control, the Arduino Nano IoT 33 plattform with WIFI module is convenient tool for lab testing with focus on the power module and gate driver only. Due to the WIFI communication, minimum wiring is required in the Lab. With the Arduino the frequency control is worse than 0.2% and therefore the export control does not fall under the 3A225 category and can be shared open-source. 

# Needed Toolchain SW:
  Matlab/Simulink R2022b (tested version) <br>
    requires installed <br>
    MATLAB Support Package for Arduino Hardware <br> 
    Simulink Support Package for Arduino Hardware <br>

# Toolchain HW:
  Arduino Nano IoT 33 <br>
  Infineon Arduino Interface PCB: EV PCB INT; SP006038346 <br>
  <br>
  Compatible Gate Driver Boards (just some examples): <br>
    EV GB HPD2 FU; SP006070794 (for Si/SiC Fusion Power Modules) <br>
    EV GB HPD2 SIC;	SP006056143 (for SiC Power Modules) <br>
    EV GB HPD2 SI 08; SP006038350 (for 750V IGBT Power Modules) <br>
    EV GB HPD2 SI 12; SP006038348 (for 1200V IGBT Power Modules) <br>
  <br>
  Compatible Power Modules (just some examples): <br>
    FS980R08A7F32B (Si/SiC Fusion Power Module 780V/980A) <br>
    FS01MR08A8MA2LBC (SiC MOSFET Power Module 750V/1mOhm) <br>
    FS02MR12A8MA2B (SiC MOSFET Power Module 1200V/2mOhm) <br>
    FS1150R08A8P3B (IGBT Power Module 750V/1150A) <br>
    FS520R12A8P1LB (IGBT Power Module 1200V/520A) <br>
    
  
    
