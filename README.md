# multi_io-thing #

a simple thing with analog and digital I/O for the "Modular Things" project

This "thin" allows to control 4 digital input, 4 digital output, 2 analog input and 2 analog output pins in addition to the on-board RGB LED.

![Multi-IO](Multi-IO.jpg)

> Nota bene: in contrast to the "things" provided by the "Modular Things" project itself, this one has been developed for a [Pimoroni Tiny2040](https://shop.pimoroni.com/products/tiny-2040) board (simply because I just had one on my desk). Since it uses the built-in RGB LEDs of that board, you may have to adjust the LED output commands in the Arduino "Firmware" shown below in order to make a thing for a different board.

## Wiring ##

![Schematic](./multi_io/circuit/images/schematic.png)


## Application Example ##

```javascript
const BlinkDelay = 800 // LED toggles every BlinkDelay milliseconds

let Timestamp = Date.now(), Value = 0
loop(async () => {
  let now = Date.now()
  if (Timestamp + BlinkDelay < now) {
    Value = (Value === 0 ? 0.1 : 0)
    await MultiIO.setRGB(0,0,Value)
    
    Timestamp = now
  }

  let AnalogIn = await MultiIO.getAnalog(1)
  await MultiIO.setAnalog(0,AnalogIn)
  
  let DigitalIn = await MultiIO.getDigital(3)
  await MultiIO.setDigital(3,DigitalIn)
  
  let HTML = `<div style="padding:4px">
    <b>Value:</b> &nbsp; &nbsp; &nbsp; ${Value}<br>
    <b>AnalogIn:</b> ${AnalogIn.toFixed(3)}<br>
    <b>DigitalIn:</b>&nbsp; ${DigitalIn}
  </div>`
  let Fragment = document.createRange().createContextualFragment(HTML)
  render(Fragment.firstChild)
}, 10)
```
