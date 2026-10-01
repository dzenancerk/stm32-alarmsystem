# STM32 Alarm System

Motion-triggered alarm for the **STM32F407 Discovery** board, written in bare-metal C using direct register access (no HAL / StdPeriph drivers).

## How it works

The alarm has three states:

| State | Indicator | Description |
|-------|-----------|-------------|
| `OFF` | all LEDs off | Alarm disarmed, audio muted |
| `ON` | blue LED blinks slowly | Armed, monitoring the accelerometer |
| `TRIGGERED` | all LEDs blink fast + alarm tone | Motion exceeded threshold |

**Controls**
- **User button (PA0)**: toggles OFF → ON, and ON/TRIGGERED → OFF (with software debounce).
- **Double tap** on the board (LIS3DSH state machine, interrupt on PE1): same toggle behaviour.

**Motion detection:** the accelerometer is polled every 100 ms. If any axis deviates from its rest value by more than `THRESHOLD`, the alarm is triggered.

**Audio:** a sine-wave tone is generated into a buffer at startup and played in a loop via DMA1 Stream5 → SPI3 (I2S) → CS43L22 → headphone jack.

## Project structure

| File | Purpose |
|------|---------|
| `main.c` | State machine, button handling, motion check, LEDs, tone generation |
| `lis3dsh.c/h` | LIS3DSH accelerometer driver (SPI), double-tap state machine, EXTI |
| `spi.c/h` | SPI1 driver |
| `cs43l22.c/h` | CS43L22 audio DAC init, I2S3 + DMA setup |
| `i2c.c/h` | I2C1 driver (CS43L22 control interface) |
| `delay.c/h` | Blocking delays (TIM12) and free-running system timer (TIM2) |
| `system_stm32f4xx.c` | ST system/clock initialization |
| `stm32_flash.ld` | Linker script |
| `Makefile` | Build and flash |

## Hardware

- STM32F407 Discovery (STM32F4DISCOVERY)
- Headphones plugged into the on-board 3.5 mm jack

## Build and flash

Requires `arm-none-eabi-gcc`, the STM32F4 CMSIS library, and [stlink](https://github.com/stlink-org/stlink). Edit the paths at the top of the `Makefile` (`STLINK`, `STM_COMMON`) to match your setup, then:

```bash
make
make upload
```
