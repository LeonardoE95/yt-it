const path = require('path');
const fs = require('fs');
const { spawn } = require('child_process');

const express = require('express');
const app = express();
const port = 3000;

app.use(express.urlencoded({
    extended: false
}));

app.use(express.static(path.join(__dirname, 'static')));

function isAllowed(code){
    if(typeof code !== 'string'){
        return false;
    }
    if(code.indexOf('import ') >= 0){
        return false;
    }
    if(code.indexOf('eval') >= 0){
        return false;
    }
    if(code.indexOf('.system') >= 0){
        return false;
    }
    if(code.indexOf('exec') >= 0){
        return false;
    }

    return true;
}

function findAndInsert(str, find, insert){
    const i = str.indexOf(find) + find.length;

    return str.slice(0, i) + insert + str.slice(i);
}

function formatOut(code, output){
    const testingPanelHTML = fs.readFileSync('static/super-secret-admin-testing-panel.html');

    const insertedInput = findAndInsert(testingPanelHTML, '<textarea class="form-control mb-3" name="code">', code);
    const insertedOutput = findAndInsert(insertedInput, '<textarea class="form-control mb-3" readonly>', output);

    return insertedOutput;
}

app.post('/super-secret-admin-testing-panel.html', (req, res) => {
    const code = req.body.code;

    if(isAllowed(code)){
        // Execute the code
        const name = `scripts/${Math.floor(Math.random() * 100000000000)}.py`;
        fs.writeFileSync(name, code);

        const python = spawn('python3', [name]);

        let output = '';
        python.stdout.on('data', (data) => {
            output += data.toString();
        })
        python.stderr.on('data', (data) => {
            output += data.toString();
        })
        python.on('close', (exit_code) => {
            fs.unlinkSync(name);

            output += '\nExit code ' + exit_code;

            res.send(formatOut(code, output));
        })
    }else {
        res.send(formatOut(code, 'Security threat detected!'));
    }
})

app.listen(port, () => console.log('Listening!'));


Exit code 0
