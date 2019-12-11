# myBLDC
Firmware for the myBLDC motor driver board. 


## TODO
- [ ] SPI driver refactoring needs testing
- [x] replace ADC_SAMPLES[] with a struct and check memory addressing
- [ ] figure out if/where I need to use floats or Q_types 
    * currently using floats everywhere
- [ ] implement fault codes for DRV
- [ ] think of fault modes that aren't reported by DRV. _eg: can't read from DRV_
- [ ] SWO printing of ADC data on a separate channel, for eventual visualisation (research cortex-debug vis)
- [ ] DRV driver (WIP)
- [ ] ADC driver (basic functionality implemented. Untested/Unproven)
    * need to implement alligned sampling correctly
    * move ADC and DRM driver into own source files
- [ ] Calculate back EMF
    * maybe a helper function for converting an ADC reading to voltage?
- [ ] test current sensing by logging and plotting current
- [ ] angle observer (this is kind of a big thing...)
    * hinges on back EMF sensing
- [ ] Blind startup (ramp from unknown state until angle can be properly observed)