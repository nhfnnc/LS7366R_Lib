
# LS7366R_Lib
A library for using the LS7366R quadrature encoder IC with Arduino.  This chip communicates over the SPI bus and impliments the standard Arduino SPI library.  Reference the LS7366R product datasheet [here](https://lsicsi.com/datasheets/LS7366R.pdf) for more information.  Function and parameter names are heavily corrilated with the datasheet.

## INSTALLATION
Download this repository as a ZIP file.  Then within the Arduino IDE import he library via 'Sketch > Include Lirary > Add .ZIP Library...' and select the downloaded ZIP file.

## Example Code
```C++
    #include "LS7366R_Lib.h"

    LS7366R_Lib encoder(9);
    uint32_t testInt = 1000;  // Value to start the counter at.

    void setup() {
    Serial.begin(115200);
    encoder.begin();
    encoder.set_MDR0(x4, free_run, disable_index, async, filter_1);
    encoder.set_MDR1(byte_4, count_ena, nop, nop, nop, nop);
    encoder.clear_CNTR();
    encoder.set_DTR(testInt);  
    encoder.load_CNTR();  // Sets the counter to start at 1,000.
    }

    void loop() {
    encoder.load_OTR();
    Serial.println(encoder.get_OTR());
    delay(1000);
    }
```
## Usage

### Initialize
Sets up an instance of the library for use.  It takes an integer argument to define the SPI Chip Select pin to be used.  One instance is needed for each encoder used.
```C++
    LS7366R_Lib encoder(9);  // Chip Select on pin 9.
```
### Set Speed *(Optional)*
Sets the speed of the SPI bus in *Hz*.  For stability on 16Mhz boards, the speed defaults to 4Mhz if this function is not used. Call before `begin()`.
```C++
    encoder.set_Speed(4000000);
```
### Begin
Starts communication with the LS7366R at default or user set speed.
```C++
    encoder.begin();
```
### Setting Register and Counter Values
There are three registers that you can write to on the LS7366R.

#### MRD0
MDR0 is an 8 bit settings register.  There are five setting in this register.

Quadrature mode setting the number of encoder counts per quadrature cycle. `non_quad`: non  quadrature mode, `x1`: one count, `x2`: two counts, and `x4`: four counts.

Count modes `free_run`, `single_cycle`, `range_limit`, and `modulo_n`.

Index trigger modes `disable_index`, `CNTR_load`, `CNTR_reset`, and `OTR_load`.

Index sync modes `async`, and `sync`.

Clock filter divisions `filter_1`, and `filter_2`.
```C++
    encoder.set_MDR0(x4, free_run, disable_index, async, filter_1);
```
#### MDR1
MDR1 is an 8 bit settings register.  There are six setting in this register.

A byte mode that sets the number of bytes in counter registers (CNTR/OTR/DTR).
`byte_4`, `byte_3`, `byte_2`, and `byte_1`.

Counting enabled or disabled. `count_ena`, and `count_dis`.

And then four flag options that are either enabled or disabled by setting as `nop`.
Flags are `flag_IDX`, `flag_CMP`, `flag_BW`, and `flag_CY`.
```C++
    encoder.set_MDR1(byte_4, count_ena, nop, nop, nop, nop);
```
#### DTR
The DTR is an 1, 2, 3, or 4 byte register (settable via the MDR1 'byte mode' option).  This allows the transfer of a value from the Arduino into the LS7366R to be set as the current counter value or other functions by the IC.  **Note:** While the register can be set as for different bitwidths, the function must be supplied a `uint32_t` formatted value.
```C++
    encoder.set_DTR((uint32_t)1000);  // Sets DTR register to 1,000.
```

### Getting Count and Register Values
There are five registers that values can be read from on the LS7366R.  


#### OTR
The OTR register acts as a buffer between the counter (CNTR) register and the arduino as to not interfere with the counting process. But does not contain the counter value constantly.  The current counter value must first be moved from the counter to the OTR first. **Note:** While the register can be set as for different bitwidths, the function will always return a `uint32_t` formatted value.
```C++
    encoder.load_OTR();
    Serial.println(encoder.get_OTR());  // Prints the current encoder count via OTR.
```
#### Counter
While not always recommended due to stability in fast systems, the counter value can be accessed directly.  **Note:** While the register can be set as for different bitwidths, the function will always return a `uint32_t` formatted value.
```C++
    Serial.println(encoder.get_CNTR());  // Prints the current encoder count via CNTR.
```
#### MDR0
Returns the current settings contents of MDR0. Returns a sigle byte `uint8_t`.
```C++
    Serial.println(encoder.get_MDR0(), BIN);  // Prints the 8 bit binary contents of MDR0.
```
#### MDR1
Returns the current setting values of MDR1.  Returns a sigle byte `uint8_t`.
```C++
    Serial.println(encoder.get_MDR1(), BIN);  // Prints the 8 bit binary contents of MDR1.
```
#### STR
Returns the current contents of the STR register.  This holds information about the count status.  *CY* carry latch, *BW* borrow latch, *CMP* compare latch, *CIDX* index latch, count enable status, power loss latch, up/down count direction indicator, and positive/negaitive sign bit.  See the LS7366R product datasheet [here](https://lsicsi.com/datasheets/LS7366R.pdf) for more information.  Returns a sigle byte `uint8_t`.
```C++
    Serial.println(encoder.get_STR(), BIN);  // Prints the 8 bit binary contents of STR.
```

### Clearing Registers
Some registers have commands to be cleared and set to 0's.
#### CNTR
```C++
    encoder.clear_CNTR();
```
#### STR
```C++
    encoder.clear_STR();
```
#### MDR0
```C++
    encoder.clear_MDR0();
```
#### MDR1
```C++
    encoder.clear_MDR1();
```

### Loading Values Between Registers
Values in the DTR and counter (CNTR) registers can be moved between the LS7366R's internal registers.

#### OTR
Transfers the value from the counter (CNTR) register into the OTR register.

This is useful to access the current counter value from the OTR without intererfing with the counting process during high speed operation.
```C++
    encoder.load_OTR();
```
**Example:** 
```C++
    encoder.load_OTR();     // Loads CNTR into OTR.
    Serial.println(encoder.get_OTR());      // Prints the value from OTR.
```
#### CNTR
Transfers the value from the DTR register into the CNTR register.
```C++
    encoder.load_CNTR();
```
