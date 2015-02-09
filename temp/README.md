# RHT03 Temp and Relative Humidity Library

A library for the spark-core to read the temp and RH
values from an RHT03 using interrupts rather than
delay-check-loop routines. Allows the spark to do other things.

## Usage

```
RHT03 rht = RHT03(ioPin);

rht.update();  // call this periodically > 2sec
```
50ms or so later the first temp will be available.
```
int temp = rht.getTemp();
int rh = rht.getRH();

```

Once the first reading is set, you can call the get methods anytime and update() on a regular schedule.

## Values

Returns integer values for Temp and RH. Caller must devide by 10 to get actual value,
or just lop off the last digit if you only care about the integer value.

Temp: 351 = 35.1c
RH:   428 = 42.8rh