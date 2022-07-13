# PseudoRandom Binary Sequence (PRBS) with Arduino
In this code, I applied 8-bit PRBS which declared as `unsigned char data[8] = {1,1,1,1,1,1,1,1}, prbs[8];`. As we know, the PRBS with 8-bit has the position tap of feedback at bit 2, 3, 4, and 8 as shown in the following figure.
![Drawing11](https://user-images.githubusercontent.com/80089377/178849636-4c5a8c08-80ab-484d-86a5-f47ba2ff3a29.png)

It can be seen in `temp = data[1]^data[2]^data[3]^data[7];`.

## Setting for Timer
For example, Arduino uno has 3 timer such as Timer0, Timer1, and Timer2. Here, I utilized Timer1 in Arduino which set it up in `void setup()` as

```
noInterrupts();       // disable all interrupts
TCCR1A  = 0;          // Reset entire TCCR1A register to 0
TCCR1B  = 0;          // Reset entire TCCR1B register to 0
TCNT1   = 64753;      // Set preload timer
TCCR1B  = 0b00000101; // Set the prescaler to 1024
TIMSK1  = 0x05;       // enable timer overflow interrupt
interrupts();         // enable all interrupts
```
As we know, Arduito has CPU frequency as big as 16MHz. 

Besides, Timer 1 in Arduino is 16-bit which it can count from 0 - 65535. Hence, the maximum preload Timer1 is 65535. On the other hand, Timer0 and Timer2 are 8-bit which can count from 0 - 255.

In the above code, we set the prescaler as big as 1024. You also can set the prescalar to 1 (no prescaling), 8, 64, 256, 1024. For example, you can change the above code to enable prescaler 256 with `TCCR1B  = 0b00000100;`. For more detail, I suggested to see the Arduino datasheet.

Now, to get `TCNT1`, you need to know the timer output you want it. For the above code, I set the output of Timer1 as big as 20Hz which mean 20 data for a second. First, we divided the CPU frequency with prescaler as `16000000/1024 = 15625`. After that, we divided again with desired Timer1 output such as `15625/20 = 781.25` or we rounded to be `782`. Due to Timer1 is 16-bit with maximum preload 65535, then `TCNT1` can be computed as `65535-782 = 64753`.
