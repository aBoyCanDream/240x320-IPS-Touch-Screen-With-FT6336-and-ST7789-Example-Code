Example Code for my test and review of the IPS Touch Screen found here: https://s.click.aliexpress.com/e/_DkVIfkD and on my youtube video found here: 

You can get the Adafruit libraries on the Arduino IDE. The touch library is found here: https://github.com/aselectroworks/Arduino-FT6336U/issues/6

Also, don't forget to connect the backlight led, marked "LED" on the PCB to your +3.3V rail!

And it looks like this board does not have pull-up resistors on the I2C lines (mine didn't), so add 10k resistors between SDA and +3.3V and between SCK and +3.3V. You don't want to leave these floating.
