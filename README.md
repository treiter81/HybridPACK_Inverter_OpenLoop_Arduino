# HybridPACK_Inverter_OpenLoop_Arduino
Open loop inverter testing. Send Modulation Idx, Speed, fsw, PWM Deadtime,... to Arduino. The Arduino will reset gate drivers and start with softstart ramp the open loop inverter operation. Supported Space Vector Modulation (SVPWM), Sine SPWM, Third Harmonic Injection (ThiSPWM), several kind of DPWMs. PWM resolution 10ns (PWM module is overclocked to 100MHz, spec 96MHz).

Illustrative Setup (AI generated with support of some real pictures from Lab Setup)
![Overview Inverter OpenLoop Testing](/Pictures/Overview_AIgen.jpg)

# Motivation and why Arduino:
Attention: No support by Infineon Technologies. Motivation of this platform was to support open research activities. <br>
Open Loop Inverter Testing is crucial for Testing Inverter Hardware and Power Module/Gate Driver Performance. The Arduino plattform is establed for open-source and toolchains are widely available. Wheras it is recommended to use Infineon Aurix Microcontroller for serial applications and closed loop control, the Arduino Nano IoT 33 plattform with WIFI module is convenient tool for lab testing with focus on the power module and gate driver only. Due to the WIFI communication, minimum wiring is required in the Lab. With the Arduino the frequency control is worse than 0.2% and therefore the export control does not fall under the 3A225 category and can be shared open-source. 

# Get Started (fast track without MATLAB/Simulink):
* install the precompiled GUI (only Windows) from folder "CompiledWIN_noMATLAB"
* start the GUI
* connect Arduino NANO IoT 33 via USB and click the "FlashTool Arduino" button in GUI. Just follow the instructions.
* disonnect USB cable after flash process!
* establish a WIFI hotspot with SSID: FUSION; PW: ffffffff
* connect the PC (running the GUI) to the WIFI. Click reset button on Arduino.
* after about 5..10s both PC and Arduino should be in WIFI. You can now click Connect button in GUI and start
* Control the Inverter open loop. Use a 12V/25W power supply for seamless operation
  

# Get Started (MATLAB/Simulink):
* open "Inverter_Arduino.slx" Simulink model
* check your WIFI entries under Hardware Setting: default SSID: FUSION; PW: ffffffff
* connect Arduino NANO IoT 33 via USB
* under Hardware/Deployment => click Build, Beloy & Start
* Arduino will be flashed and started
* disonnect USB cable after flash process!
* open GUI. Ensure that PC is in same WIFI than Arduino. You can now click Connect button in GUI and start
* Control the Inverter open loop. Use a 12V/25W power supply for seamless operation <br>


# Needed Toolchain SW:
Can be used without MATLAB/Simulink => use precomiled GUI. This GUI includes also precompiled bin file, which can be flashed. <br>
But recommended with MATLAB/Simulink:
  
* Matlab/Simulink R2022b (tested version) <br>

requires installed <br>
* MATLAB Support Package for Arduino Hardware <br> 
* Simulink Support Package for Arduino Hardware <br>

# Toolchain HW:
* Arduino Nano IoT 33 <br>
* WIFI Network <br>
* Infineon Arduino Interface PCB: EV PCB INT; SP006038346 <br>

Compatible Gate Driver Boards (just some examples): <br>
* <a href="https://www.infineon.com/evaluation-board/EV-GB-HPD2-FU" target="_blank">EV GB HPD2 FU; SP006070794 (for Si/SiC Fusion Power Modules)</a>  <br>
* <a href="https://www.infineon.com/evaluation-board/EV-GB-HPD2-SIC" target="_blank">EV GB HPD2 SIC;	SP006056143 (for SiC Power Modules)</a>  <br>
* <a href="https://www.infineon.com/evaluation-board/EV-GB-HPD2-SI-08" target="_blank">EV GB HPD2 SI 08; SP006038350 (for 750V IGBT Power Modules)</a>  <br>
* <a href="https://www.infineon.com/evaluation-board/EV-GB-HPD2-SI-12" target="_blank">EV GB HPD2 SI 12; SP006038348 (for 1200V IGBT Power Modules)</a>  <br> 



Compatible Power Modules (just some examples): <br>
* FS980R08A7F32B (Si/SiC Fusion Power Module 780V/980A) <br>
* <a href="https://www.infineon.com/part/FS01MR08A8MA2LBC" target="_blank">FS01MR08A8MA2LBC (SiC MOSFET Power Module 750V/1mOhm)</a>  <br>
* <a href="https://www.infineon.com/part/FS02MR12A8MA2B" target="_blank">FS02MR12A8MA2B (SiC MOSFET Power Module 1200V/2mOhm)</a>  <br>
* <a href="https://www.infineon.com/part/FS1150R08A8P3LBC" target="_blank">FS1150R08A8P3B (IGBT Power Module 750V/1150A) </a>  <br> 
* <a href="https://www.infineon.com/part/FS520R12A8P1LB" target="_blank">FS520R12A8P1LB (IGBT Power Module 1200V/520A)</a>  <br>
* many more

Information to the Gate Drivers: <br>
*  <a href="https://www.infineon.com/part/1EDI3035AS" target="_blank">SiC MOSFET and Si/SiC Fusion optimized Gate Driver 1EDI3035AS</a>  <br> 
*  <a href="https://www.infineon.com/part/1EDI3025AS" target="_blank">IGBT optimized Gate Driver 1EDI3025AS</a>  <br>




