# ledCube_8_16
When cube will be ready install https://github.com/LuisMiCa/IRsmallDecoder in Arduino IDE.
From Example>IRsmallDecoder open HelloNEC example.
In the "IRsmallDecoder irDecoder(2);"  put your port number (in our case 10)
In "Serial.begin(250000);" put your terminal speed (Arduino Serial Monitor default value is 9600)
Burn sketch to cube.
Get your IR Pult and press all button one by one and remember it cmd.
Open LEDCube8_16.ino  and in loop void ander switch statement put thous cmd numbers. 
For example : "case 0x13:" where 0x13 - is yor cmd = button.
Note that not all pult works with this library. If no cmd shown - try another pult.
