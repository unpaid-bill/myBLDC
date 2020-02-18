# myBLDC
Firmware for the myBLDC motor driver board. 


## TODO
- [x] SPI driver refactoring needs testing - UPDATE: it works!
- [x] replace ADC_SAMPLES[] with a struct and check memory addressing
- [x] write PID library
- [ ] figure out if/where I need to use floats or Q_types 
    * currently using floats everywhere
- [ ] implement fault codes for DRV
    - [x] sort of!
- [ ] think of fault modes that aren't reported by DRV. _eg: can't read from DRV_
- [ ] SWO printing of ADC data on a separate channel, for eventual visualisation (research cortex-debug vis)
- [ ] Calculate back EMF
- [ ] test current sensing by logging and plotting current
- [ ] angle observer (this is kind of a big thing...)
    * hinges on back EMF sensing
- [ ] Blind startup (ramp from unknown state until angle can be properly observed)
- [ ] CANbus comms
- [ ] logging to file, perhaps over CAN?