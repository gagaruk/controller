# Universal Remote Controller -V1
## Remote controller that would be suitable for many RC hobby projects

--photo

## Schematic

<img width="3000" height="1912" alt="circuit_image" src="https://github.com/user-attachments/assets/75380b6d-558f-404a-adaf-365c677c918e" />


## Bill of materials

can be found in [BOM.csv](bom.csv)

## Code- how to approach

1.Use the classes in the lib to build your own functionalities based on the task
2. Tests are for testing the hardware
3. ControllerConfig.h hosts all the necessary constants and settings.
4. To change value recieved / sent change:
    -> ControllerConfig.h 
            - TelemetryPackage   (recieved)
            - PeripheralPackage  (sent)

## Building Your own (Tips and experience)

1. $\color{red}{\text{Start With Breadboard}}$ and test the connections

2. Make or look to use this schematic

3. Wire them on the breadboard $\color{red}{\text{one by one}}$
    *Changing hardware and making new connections get impossible later on
        -> I cant fix the broken IMU because its under the esp32

4. Remember $\color{red}{\text{schematic is reversed }}$ if you're soldering from the back of the board
    -Needed to solder everything once again

5. But extra of the little parts like capacitors -> easy to loose
6. After soldering check if the system works as a whole with code
7. Write the test while programming the rest.
8. If you want to use it for a variety of projects. Build a moduler structure with classes.

Tip: A break might help you shift perspectives sometimes.
