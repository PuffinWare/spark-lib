# RHT03 Temp and Relative Humidity Library

A library for the spark-core to read the temp and RH
values from an RHT03 using interrupts rather than
delay-check-loop routines. Allows the spark to do other things.

## Usage

### RHT03
#### Code Example
```
RHT03 rht = RHT03(ioPin);

void loop() {
  bool update = rht.poll();

  ...

  if (update) {
    // these can be called anytime or when update is true
    int temp = rht.getTemp();
    int rh = rht.getRH();
    doStuff(temp, rh);
  }
}

```

#### Debugging
If you pass in a valid digial pin with an LED connected, you can visualze the comm
with the RHT03. This is helpful getting it working and ensuring that data is being processed.
```
RHT03 rht = RHT03(ioPin, ledPin);
```

### TMP3x
The TMP3x series temp sensors are a little flaky and jump around a little, even with a small cap
between the data pin and ground. The *numReadings* value will control how many readings, each
taken *delay* millis apart, will be collected to generate an average. Values will be returned
even before the full *numReadings* has been reached.

#### Code Example
```
TMP3x tmp = TMP3x(anlgPin, 10, 1000);

void loop() {
  bool update = tmp.poll();

  ...

  if (update) {
    int temp = rht.getTemp();
    doStuff(temp);
  }
}

```

### Values

Returns integer values for Temp and RH. Caller must devide by 10 to get actual value,
or just lop off the last digit if you only care about the integer value.

Temp: 351 = 35.1c

RH:   428 = 42.8rh
