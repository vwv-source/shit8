# SHIT-8
A CHIP-8 emulator I wrote in 2 days for fun
<br>
`./shit8 <ROM> <DELAY> (2) <SCALE> (10)`

<details>
  <summary>Screenshots</summary>

![image](https://imgur.com/M4VTnUd.png)

![image](https://imgur.com/5mxAEtt.png)
</details>
<br>

[This](https://austinmorlan.com/posts/chip8_emulator/) guide was greatly used to create this emulator


It uses SDL2 for drawing the screen buffer (although you can basically use anything since the buffer is extremely simple)
<br><br>
ATM it doesn't have audio since SDL2 kinda sux for that but I might revisit it later, It should also compile for Windows but I haven't tested it (usleep for delays might be an issue)
<br>
Also only roms from [this](https://github.com/kripod/chip8-roms/) repo worked for me for some reason ¯\\\_(ツ)\_/¯
<br><br>
My next goal will be the GameBoy so lets hope I dont give up lol