STM32 QEI Library using EXTI based on mbed-QEI for STM32CubeIDE users.
Below is an example of GPIO Pin configuration for ioc file:
Encoder Pin | X4 Encoding | X2 Encoding
--- | --- | ---
**CH_A** | `EXTI (Rising-Falling) with Pull-Up` | `EXTI (Rising-Falling) with Pull-Up`
**CH_B** | `EXTI (Rising-Falling) with Pull-Up` | `Input Mode / Polling with Pull-Up`
