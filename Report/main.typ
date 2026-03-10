#import "@preview/academic-alt:0.1.0": *
#show: university-assignment.with(
  title: "Práctica 1: Parser descendiente recursivo",
  subtitle: "Procesadores del lenguaje",
  authors: ("Alejandro Quirante", "Carlos Martín Gallardo"),
  date: "10 de Marzo de 2026"
)

= Introducción

En esta primera práctica se lleva a cabo el desarrollo de un traductor de notación prefija a infija en operaciones básicas de una calculadora. Para ello empleamos un Analizador Descendente Recursivo implementado en C en el archivo drLL.c a partir de una gramática cuyo funcionamiento y desarrollo también se abarcan en esta práctica

= ¿Qué elementos léxicos proporciona la función rd_lex()?

Primeramente analizamos el fichero drLL.c proporcionado, con el analizador rd_lex() ya programado y sin necesidad de realizar cambios y observamos que:
+ En esta parte de la función: 
    ```c
    do {
        c = getchar () ;
        if (c == '\n') 
            line_counter++ ;	// info for rd_syntax_error()
    } while (c == '\t' || c == ' ' || c == '\r') ;
    ```
    Vemos que se ignoran los caracteres tab ("\\t"), espacio (" ") y el caracter de retorno ("\\r") ya que si el programa encuentra cualquiera de ellos sigue extrayendo caracteres del flujo de entrada.
+ 

= Methodology

== Hardware Setup

The following components were used:
- Raspberry Pi 4 Model B
- Red LED (2.1V forward voltage, 20mA forward current)
- 220Ω current-limiting resistor
- Breadboard for prototyping
- Jumper wires for connections

The LED was connected between GPIO pin 18 and ground, with the current-limiting resistor in series.

== Software Implementation

Two implementations were developed:



Both programs implement the same functionality: blinking an LED at 1Hz (500ms on, 500ms off).

== Code Examples

The Python implementation:

```python
import RPi.GPIO as GPIO
import time

# Set up GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.OUT)

try:
    while True:
        GPIO.output(18, GPIO.HIGH)  # Turn LED on
        time.sleep(0.5)
        GPIO.output(18, GPIO.LOW)   # Turn LED off
        time.sleep(0.5)
except KeyboardInterrupt:
    GPIO.cleanup()
```

The C implementation:

```c
#include <wiringPi.h>
#include <stdio.h>

#define LED_PIN 18

int main(void) {
    wiringPiSetupGpio();
    pinMode(LED_PIN, OUTPUT);
    
    while (1) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
    
    return 0;
}
```

= Results

Both implementations successfully controlled the LED with the following observations:

- The LED blinked consistently at 1Hz
- Visual timing appeared identical between implementations
- The C implementation showed slightly more precise timing
- Python implementation was easier to develop and debug

== Performance Analysis

Timing measurements were conducted using a logic analyzer:

The C implementation demonstrated more consistent timing, likely due to reduced overhead compared to Python's interpreted execution.

= Discussion

The lab successfully demonstrated basic GPIO control on the Raspberry Pi. Key findings include:

- GPIO configuration is straightforward with both RPi.GPIO and WiringPi libraries
- Hardware setup requires attention to current-limiting resistors
- C implementations offer better timing precision for real-time applications
- Python provides faster development cycles for prototyping

= Conclusion

This lab provided hands-on experience with GPIO control on embedded systems. The successful implementation of LED blinking in both Python and C demonstrates the versatility of the Raspberry Pi platform for embedded programming education.

Future work could explore:
- PWM control for LED brightness modulation
- Interrupt-driven input handling
- Multi-threaded applications
- Real-time operating system integration


