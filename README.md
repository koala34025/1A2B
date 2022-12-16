# 1A2B
An Arduino 1A2B game
![](https://imgur.com/Rt9aeGp.jpg)
## Features
- Finite state machine
![](https://imgur.com/viqQYOf.jpg)
- Included libraries
    - SevSeg.h
    - LiquidCrystal_I2C.h
- Self-made delay
    - Since you cannot use SevSeg and delay simultaneously
- Answer randomizer
    - Randomize the answer if the player did not set up one (0000).
- Check validness
    - Check the validness of answer and guess before judging
## Arduino Component and Modules
![](https://imgur.com/ShWuC1Y.jpg)
- 4 bits 7 segment display
    - Display number
- LCD
    - Display prompt message
- 4 LEDs
    - Showing the bit of number the player is adjusting
- 3 buttons
    - UP
        - Increase the selected bit by 1
        - Back to 0 the the bit was 9
    - RIGHT
        - Right shift changing what bit the player is adjusting
    - OK
- Arduino Uno Board
    - 11 pins for 7 segment display
    - 2 pins for LCD
    - 4 pins for 4 LEDs
    - 3 pins for 3 buttons
- Laser cutting wooden plank package
## Demo video
https://www.youtube.com/watch?v=wgPNuXfr_Ew