# multi_io-thing #

a simple thing with analog and digital I/O for the "Modular THings" project

## Application Example ##

```javascript
const BlinkDelay = 800 // LED toggle every BlinkDelay millisecond

let Timestamp = Date.now(), Value = 0
loop(async () => {
  let now = Date.now()
  if (Timestamp + BlinkDelay < now) {
    Value = (Value === 0 ? 0.1 : 0)
    await MultiIO.setRGB(0,0,Value)
    
    Timestamp = now
  }

  let AnalogIn = await MultiIO.getAnalog1()
  await MultiIO.setAnalog0(AnalogIn)
  
  let DigitalIn = await MultiIO.getDigital3()
  await MultiIO.setDigital3(DigitalIn)
  
  let HTML = `<div style="padding:4px">
    <b>Value:</b> &nbsp; &nbsp; &nbsp; ${Value}<br>
    <b>AnalogIn:</b> ${AnalogIn.toFixed(3)}<br>
    <b>DigitalIn:</b>&nbsp; ${DigitalIn}
  </div>`
  let Fragment = document.createRange().createContextualFragment(HTML)
  render(Fragment.firstChild)
}, 10)
```
