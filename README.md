Telefon - Old board telephone operating within modern cellular networks. 

Prerequisites: old phone like this one below with microcontroler (Raaspberry Pi Pico), and some wires. 

![image](https://cdn.discordapp.com/attachments/744190749209395301/972916321853534268/unknown.png)

# How it works

By reading state of pin connected to spinning board (using an interrupt). Then it debounces it so there's no garbage and counts it to get a number. After 5s from last number entered it calls the entered number.

# Additional features

By connecting lcd with I2C converter to address 0x27 you can see what's going on. What number you enter and access menu. In the menu you can send SMS, Call to someone in your phonebook and more.

If you connect by I2CCom my Graphics Card [link here](https://github.com/dommilosz/graphics_card/) you can get pretty nice VGA output from it. Even play simple game!

# Schematic

Everything is powered by the same 5V stable power supply with at least a few amps of current. USB Hub is there for I2CCom

![image](https://user-images.githubusercontent.com/54410730/167308553-8880e31e-c8fa-444a-8582-a00643255efd.png)
