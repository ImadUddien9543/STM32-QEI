STM32 QEI Library using external interrupt and HAL library based on mbed-QEI.
This library only support X4 encoding
set 
```
External Interrupt Mode with Rising/Falling edge trigger detection
```
in ioc configuration.

or set manually in 
```
MX_GPIO_Init(void){
...
GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
...
}
```