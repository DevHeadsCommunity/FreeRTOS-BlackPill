# FreeRTOS-BlackPill

## It is Tested on STM32F411CEU6

## Objectives 
  - Showing How to Port FreeRTOS for STM32 MCU's
  - Writig own Driver code and implement that with FreeRTOS
  - How to Do some advance Task using own Written Basic Drivers
  - It also contains *printk* which is used to logged data via USART2 peripheral
## Plan 
1. Setting FreeRTOS First and Test it is working or not
2. Fixing The structure of the project
3. Adding Custom Driver
4. Testing those Driver with FreeRTOS
5. Once Driver's are complete adding some library to control certain things like Display, Motors and Sensors

## Progress 
1. FreeRTOS is already set
2. Porject Structure is also done
3. UART Driver is written (Blocking)
4. GPIO Driver is also under development

### Some gudiance for who is trying to port FreeRTOS on a different MCU

1. Sometimes you may see the MCU triggering Hard Fault on *vTaskStartScheduler();* one of the reason I figured out that was 
   Treating FreeRTOS as a library Can introduce issues regarding linking the ISR(Interrupt Handlers). It is due to how Linker works
   At first it looks for Strong Symbol -> Then it looks for weak Symbols -> Then it looks for Unresolved Symbol (Symbols which should exist but they aren't)
   now by default All of our ISR's comes with week definition so if there any stonger implementation they will be replaced but when we Treat FreeRTOS as a Library
   Then it is compiled first as a library after that when we compile our project while linking those week symbols are found by linker due to linking order for this reason 
   It doesn't look for Strongly Implemented ISRs inside FreeRTOS. for this reason the ISR's contains nothing when called and a HardFault occurs  
