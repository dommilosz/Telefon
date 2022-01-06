let port;
let connect_button = document.querySelector("#btn_connect");

function btn_connect() {
    if (connect_button.innerHTML === "Connect") {
        navigator.serial.requestPort({}).then(async (_port) => {
            port = _port;
            await port.open({baudRate: 115200});
            connect_button.innerHTML = "Disconnect";

            port.reader = port.readable.getReader();

            port.reader.readLine = async function () {
                let line = await port.reader.readUntil(10);
                return line;
            }
            port.reader.readLines = async function(){
                let lines = [];
                while(port.reader.data.includes(10)){
                    let line = await port.reader.readLine();
                    lines.push(line);
                }
                return lines;
            }
            port.reader.hasNextLine = function(){
                return port.reader.data.includes(10)
            }
            port.reader.readBytes = async function (length, noString) {
                if (length === undefined) length = 1;

                await awaitFor(() => port.reader.data.length >= length, 500, 5);
                let data = (port.reader.data.splice(0, length))
                if (!noString) {
                    data = String.fromCharCode(...data)
                }

                console.log("Read: " + data);
                return data;
            }

            port.reader.readBool = async function () {
                return !!(await port.reader.readBytes(1, true))[0];
            }

            port.reader.readInt16 = async function () {
                let bytes = await port.reader.readBytes(2, true);
                let countI = bytes[1];
                countI |= bytes[0] << 8;
                return countI;
            }

            port.reader.readAll = async function () {
                let data = String.fromCharCode(...port.reader.data.splice(0, port.reader.data.length))
                console.log("Read: " + data);
                return data;
            }

            port.reader.readUntil = async function (char, skip) {
                await awaitFor(() => port.reader.data.includes(char), 500, 5);

                let data = String.fromCharCode(...port.reader.data.splice(0, port.reader.data.indexOf(char) + 1))
                if (skip) {
                    data = data.slice(0, -1);
                }
                //console.log("Read: " + data);
                return data;
            }

            port.reader.waitUntil = async function (char) {
                await awaitFor(() => port.reader.data.includes(char), 500, 5);
            }

            port.reader.readUntilString = async function (string) {
                let data = "";

                while (!data.includes(string)) {
                    data += await port.reader.readUntil(string.charCodeAt(0));
                    data += await port.reader.readUntil(string.charCodeAt(string.length - 1));
                }

                console.log("Read: " + data);
                return data;
            }

            const writer = port.writable.getWriter();
            port.writer = writer;

            writer.writeLine = async function (data) {
                await writer.writeText(data + "\n");
            }
            writer.writeText = async function (data) {
                console.log("Write text: " + data);
                let bytes = stringToBytes(data)
                await writer.write(bytes);
            }
            writer.writeByte = async function (data) {
                let byte = new Uint8Array([data]);
                console.log(`Write byte: ${data} (${String.fromCharCode(data)})`);
                await writer.write(byte);
            }
            writer.writeBytes = async function (data) {
                let byte = new Uint8Array(data);
                console.log(`Write bytes: (${String.fromCharCode(...data)})`);
                await writer.write(byte);
            }
            ReadLoop()
            connect_callback();

        }).catch((e) => {
            connect_button.innerHTML = "Connect";
            alert(e);
        });
    } else {
        Disconnect();

    }


}

navigator.serial.addEventListener("disconnect", (event) => {
    btn_connect();
});

async function awaitFor(boolFunction, attempts, delay) {
    for (let i = 0; i < attempts; i++) {
        await sleep(delay)
        if (boolFunction()) {
            return;
        }
    }
    console.log("ERROR: Await For Timed Out!")
    throw new Error("Await For Timed Out!");
}

function stringToBytes(string) {
    let bytes = string.split("");
    return new Uint8Array(bytes.map(e => {
        return e.charCodeAt(0)
    }))
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

let read_callback;
let connect_callback;

async function ReadLoop() {
    while (true) {
        let code = await ReadOnce();
        if (code === false) {
            await Disconnect();
            return;
        }
        if (code === "e") {
            await sleep(500);
        }
        try {
            read_callback(port.reader.data.includes(10));
        } catch {
        }
    }
}

async function ReadOnce() {
    if (!port.reader.data) port.reader.data = [];
    try {
        let val = (await port.reader.read()).value
        if (val === undefined) return false;
        val.forEach(el => {
            port.reader.data.push(el);
        })
    } catch {
        return "e";
    }

}

async function Disconnect() {
    try {
        await port.writer.abort();
        await port.reader.cancel();
        await port.reader.releaseLock();
        await port.writer.releaseLock();
    } catch (e) {
        alert(e);
    }
    try {
        port.close();
        connect_button.innerHTML = "Connect";
    } catch (e) {
        alert(e);
    }
}