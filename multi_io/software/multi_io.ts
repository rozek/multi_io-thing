import Thing       from "../../../src/lib/thing"
import Serializers from "../../../src/lib/osapjs/utils/serializers"

const { readUint16,writeUint16 } = Serializers

export default class multi_io extends Thing {
  async setRGB (R:number, G:number, B:number):Promise<void> {
    const LED_R = Math.floor(65535 * Math.max(0,Math.min(R,1)))
    const LED_G = Math.floor(65535 * Math.max(0,Math.min(G,1)))
    const LED_B = Math.floor(65535 * Math.max(0,Math.min(B,1)))

    const Datagram = new Uint8Array([
      LED_R & 0xFF, (LED_R >> 8) & 0xFF,
      LED_G & 0xFF, (LED_G >> 8) & 0xFF,
      LED_B & 0xFF, (LED_B >> 8) & 0xFF,
    ])
    await this.send('setRGB',Datagram)
  }

/**** Digital Input ****/

  async getDigital (Port:number):Promise<boolean> {
    Port = Math.floor(Port)
    if ((Port < 0) || (Port > 3)) throw new Error(
      'multi-io thing: invalid digital input port ' + Port
    )

    const Data = await this.send('getDigital',new Uint8Array([Port]))
    return (Data[0] > 0)
  }

/**** Digital Output ****/

  async setDigital (Port:number, Value:boolean):Promise<void> {
    Port = Math.floor(Port)
    if ((Port < 0) || (Port > 3)) throw new Error(
      'multi-io thing: invalid digital output port ' + Port
    )

    const Datagram = new Uint8Array([Port,Value ? 255 : 0])
    await this.send('setDigital',Datagram)
  }

/**** Analog Input ****/

  async getAnalog (Port:number):Promise<number> {
    Port = Math.floor(Port)
    if ((Port < 0) || (Port > 1)) throw new Error(
      'multi-io thing: invalid analog input port ' + Port
    )

    const Data = await this.send('getAnalog',new Uint8Array([Port]))
    return (Data[0] + Data[1]*255) / 4096
  }

/**** Analog Output ****/

  async setAnalog (Port:number, Value:number):Promise<void> {
    Port = Math.floor(Port)
    if ((Port < 0) || (Port > 1)) throw new Error(
      'multi-io thing: invalid analog output port ' + Port
    )

    Value = Math.floor(4096 * Math.max(0,Math.min(Value,1)))
    await this.send('setAnalog',new Uint8Array([
      Port, Value & 0xFF, Value >> 8 & 0xFF
    ]))
  }

/**** API Specification ****/

  public api = [{
    name:  'setRGB',
    args:  [
      'R: 0 to 1',
      'G: 0 to 1',
      'B: 0 to 1'
    ]
  },{
    name:  'getDigital',
    args:  [ 'port: 0 to 3' ],
    return:'true or false'
  },{
    name:  'setDigital',
    args:  [ 'port: 0 to 3', 'value: true or false' ]
  },{
    name:  'getAnalog',
    args:  [ 'port: 0 to 1' ],
    return:'0 to 1'
  },{
    name:  'setAnalog',
    args:  [ 'port: 0 to 1', 'value: 0 to 1' ]
  }]
}
