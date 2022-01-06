let lcd;
document.body.style.transform = "translateX(25%) translateY(25%) scale(1.5)";

function initLcd() {
    document.querySelector("#lcd").innerHTML = "";
    lcd = new CharLCD({at: 'lcd', rows: 2, cols: 16, rom: 'jp', off: "#bac720"});
    document.querySelector("#lcd > div").style.backgroundColor = "#cd2";
}

initLcd();

let test_arr = [0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111]

function button(num) {
    port.writer.writeLine("#d" + num);
}

function buttonConfirm(long) {
    port.writer.writeLine(`#b${long}`);
}

function hangButton(answer){
    port.writer.writeLine(`#h${answer}`);
}


let cursor = [];

read_callback = async function (hasLine) {
    if (!hasLine) return;
    try {
        while (port.reader.hasNextLine()) {
            let line = await port.reader.readLine();
            parseLine(line);

        }

    } catch (e) {
        console.log(e);
    }
}

connect_callback = async function(){
    await port.writer.writeLine("#l1");
}

function parseLine(line) {
    if (line.startsWith("#LCD: ")) {
        let cmd = line.split("#LCD: ")[1];
        if (cmd.startsWith("setCursor=")) {
            let data = cmd.split("setCursor=")[1].split(";")[0];
            data = data.split(',');
            cursor = data;
        } else if (cmd.startsWith("write=")) {
            let data = cmd.split("write=")[1].split(";")[0];
            lcd_char(data)
        } else if (cmd.startsWith("clear")) {
            cursor = [0, 0];
            port.reader.data = [];
            lcd.clear();
        } else {
            console.log(cmd);
        }


    }
}

function lcd_char(c) {
    c = String.fromCharCode(c);
    lcd_txt(cursor[1], cursor[0], String(c));
    cursor[0]++;

    function lcd_txt(c1, c2, str) {
        try {
            lcd.text(Number(c1), Number(c2), str);
        } catch {
            console.log(`lcd.text(${c1},${c2},${str})`);
        }
    }
}