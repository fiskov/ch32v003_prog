# ch32v003

simple prog for education purpose

## Hardware

- WCH-LinkE-R0 - programmer
- CH32V003F4P6-EVT-R0 - demoboard

## Software

- MounRiver Studio_Community v1.70

## Task

- **test TMR and Standby** - Want to implement blink LED for several seconds and deepsleep (standby) with Auto-Wakeup for several seconds

### AWU

- is clocked by LSI 128 kHz (is need to check time-error ??)
- connected internal to EXTI9
- PWR_ AWUPSC register has AWUPSC with maximum 1111b (mask 0xF) == div by 61440
- PWR_AWUWR register has param AWUUWR with mask 0x3F, total `AWU max time = 128000/61440*32 == 15.36 s`

### Freq Error

(VCC=3.3V, t=20°C, SYSCLK_FREQ_24MHZ_HSI) HSI error ~0.8% < 30 seconds per hour

## Result

Current consumption in deepsleep-mode about 0.1 mA (without power LED)

