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
