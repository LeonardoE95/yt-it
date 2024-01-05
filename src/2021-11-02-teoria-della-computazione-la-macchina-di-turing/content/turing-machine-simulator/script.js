// -------------------------------------------------------------------------------------
// GLOBAL VARIABLES

// TODO
//   - trasformare bottone calcola! in ferma!
//   - sistemare descrizione pagina html


var TM = {
  // for the key we use a combination STATE/SYMBOL, the value for each
  // key instead is a 3 valued list [NEW_STATE, SYMBOL_WRITTEN,
  // MOVEMENT]
  "instructions": {},

  // number, representing the index of the tape where the head is
  // current residing. This means that the symbol viewed at any time
  // from the machine can be viewed with TM["tape"][TM["head_pos"]].
  "head_pos": null,
  
  // simple list of values
  "tape": [],

  // simple value representing a state, must be present in the given
  // instruction.
  "current_state": null,
};

const TM_PRESETS = {
  "1": {
    "description": "Somma numeri unario",
    "instructions": {
      "q_0/1": ["q_1", "*", "D"],
      "q_1/1": ["q_1", "1", "D"],
      "q_1/+": ["q_F", "1", "D"],
    },
    "head_pos": 0,
    "tape": ["1", "1", "1", "1", "+", "1", "1", "1"],
    "current_state": "q_0",
  },
  
  "2": {
    "description": "Somma numeri binario",
    "instructions": {
      "q_0/0": ["q_0", "0", "D"],
      "q_0/1": ["q_0", "1", "D"],
      "q_0/*": ["q_1", "*", "S"],      
      "q_0/-": ["q_1", "-", "S"],

      "q_1/0": ["q_letto^0", "*", "S"],
      "q_1/1": ["q_letto^1", "*", "S"],
      "q_1/*": ["q_1", "*", "S"],
      "q_1/+": ["q_F", "+", "F"],

      // -----------------------

      "q_letto^0/0": ["q_letto^0", "0", "S"],
      "q_letto^0/1": ["q_letto^0", "1", "S"],
      "q_letto^0/+": ["q_2^0", "+","S"],

      "q_letto^1/0": ["q_letto^1", "0", "S"],
      "q_letto^1/1": ["q_letto^1", "1", "S"],
      "q_letto^1/+": ["q_2^1", "+","S"],

      // -----------------------

      "q_2^0/*": ["q_2^0", "*", "S"],
      "q_2^0/0": ["q_letto^00", "*", "S"],
      "q_2^0/1": ["q_letto^01", "*", "S"],

      "q_2^1/*": ["q_2^1", "*", "S"],
      "q_2^1/0": ["q_letto^10", "*", "S"],
      "q_2^1/1": ["q_letto^11", "*", "S"],

      // -----------------------

      "q_letto^00/0": ["q_letto^00", "0", "S"],
      "q_letto^00/1": ["q_letto^00", "1", "S"],
      "q_letto^00/-": ["q_check^00", "-", "S"],

      "q_letto^01/0": ["q_letto^01", "0", "S"],
      "q_letto^01/1": ["q_letto^01", "1", "S"],
      "q_letto^01/-": ["q_check^01", "-", "S"],

      "q_letto^10/0": ["q_letto^10", "0", "S"],
      "q_letto^10/1": ["q_letto^10", "1", "S"],
      "q_letto^10/-": ["q_check^10", "-", "S"],

      "q_letto^11/0": ["q_letto^11", "0", "S"],
      "q_letto^11/1": ["q_letto^11", "1", "S"],
      "q_letto^11/-": ["q_check^11", "-", "S"],

      // -----------------------

      "q_check^00/0": ["q_scrivi^0", "0", "D"],
      "q_check^00/1": ["q_scrivi^1", "0", "D"],

      "q_check^10/0": ["q_scrivi^1", "0", "D"],
      "q_check^10/1": ["q_scrivi^0", "1", "D"],

      "q_check^01/0": ["q_scrivi^1", "0", "D"],
      "q_check^01/1": ["q_scrivi^0", "1", "D"],

      "q_check^11/0": ["q_scrivi^0", "1", "D"],
      "q_check^11/1": ["q_scrivi^1", "1", "D"],

      // -----------------------

      "q_scrivi^0/-": ["q_3^0", "-", "D"],
      "q_scrivi^1/-": ["q_3^1", "-", "D"],

      "q_3^0/0": ["q_3^0", "0", "D"],
      "q_3^0/1": ["q_3^0", "1", "D"],
      "q_3^0/+": ["q_3^0", "+", "D"],
      "q_3^0/*": ["q_3^0", "*", "D"],
      "q_3^0/-": ["q_4^0", "-", "D"],

      "q_3^1/0": ["q_3^1", "0", "D"],
      "q_3^1/1": ["q_3^1", "1", "D"],
      "q_3^1/+": ["q_3^1", "+", "D"],
      "q_3^1/*": ["q_3^1", "*", "D"],
      "q_3^1/-": ["q_4^1", "-", "D"],

      // -----------------------

      "q_4^0/0": ["q_4^0", "0", "D"],
      "q_4^0/1": ["q_4^0", "1", "D"],
      "q_4^0/?": ["q_5", "0", "S"],

      "q_4^1/0": ["q_4^1", "0", "D"],
      "q_4^1/1": ["q_4^1", "1", "D"],
      "q_4^1/?": ["q_5", "1", "S"],

      // -----------------------

      "q_5/0": ["q_5", "0", "S"],
      "q_5/1": ["q_5", "1", "S"],
      "q_5/-": ["q_5", "-", "S"],
      "q_5/?": ["q_5", "?", "S"],
      "q_5/*": ["q_5", "*", "S"],
      "q_5/+": ["q_0", "+", "D"],

      // -----------------------

      "q_F/+": ["q_F", "+", "S"],
      "q_F/*": ["q_F", "*", "S"],
      "q_F/-": ["q_6", "-", "S"],

      "q_6/0": ["q_6^0", "0", "D"],
      "q_6/1": ["q_6^1", "0", "D"],

      "q_6^0/-": ["q_6^0", "-", "D"],
      "q_6^0/*": ["q_6^0", "*", "D"],
      "q_6^0/+": ["q_6^0", "+", "D"],
      "q_6^0/1": ["q_6^0", "1", "D"],
      "q_6^0/0": ["q_6^0", "0", "D"],
      "q_6^0/?": ["q_done", "0", "F"],

      "q_6^1/-": ["q_6^1", "-", "D"],
      "q_6^1/*": ["q_6^1", "*", "D"],
      "q_6^1/+": ["q_6^1", "+", "D"],
      "q_6^1/1": ["q_6^1", "1", "D"],
      "q_6^1/0": ["q_6^1", "0", "D"],
      "q_6^1/?": ["q_done", "1", "F"],      
    },
    
    "head_pos": 6,
    "tape": ["0", "-", "1", "1", "1", "+", "1", "0", "1", "-", "?", "?", "?", "?"],
    "current_state": "",    
  },
}

const INSTRUCTION_TEMPLATE = `<tr>
<td id="item-button">
  <button type="button" class="btn btn-primary" onClick="extract_new_instruction(this.parentNode.parentNode)">+</button>
</td>

<td><input type="text" maxlength="10" value="" class="input-instruction-box"></input></td>
<td><input type="text" maxlength="10" value="" class="input-instruction-box"></input></td>
<td></td>
<td><input type="text" maxlength="10" value="" class="input-instruction-box"></input></td>
<td><input type="text" maxlength="10" value="" class="input-instruction-box"></input></td>
<td><select name="movement" class="input-instruction-box">
  <option value="F"> F </option>
  <option value="S"> S </option>
  <option value="D"> D </option>
  </select> </td>
</tr>`;

const ARROW_HTML = `<b style="font-size: 25px;"> &#8595; </b>`;

const EMPTY_TAPE_CELL = `<td><input type="text" maxlength="1" value="" class="input-instruction-box-1"> </td>`;

const START_BUTTON = `<button type="button" class="btn btn-success" onClick="execute()">Calcola!</button>`

const STOP_BUTTON = `<button type="button" class="btn btn-danger" onClick="stop()">Ferma!</button>`

// -------------------------------------------------------------------------------------
// MANIPULATION OF TURING MACHINE OBJECT

// Adds a new tuple to the TM object. If an instruction with the same
// prefix exists, then it doesn't add it.

function tm_add_instruction (tuple) {
  prefix_key = tuple[0] + "/" + tuple[1];

  if (prefix_key in TM["instructions"]) {
    // -- error: instruction prefix already present!
    return -1;
  }

  TM["instructions"][prefix_key] = [tuple[2], tuple[3], tuple[4]];
}

// Deletes a tuple from the TM object. If the tuple-prefix is not
// present in the object, it doesn't delete it.

function tm_remove_instruction (tuple) {
  prefix_key = tuple[0] + "/" + tuple[1];

  if (!(prefix_key in TM["instructions"])) {
    // -- error: instruction prefix not present!
    return -1;
  }

  delete TM["instructions"][prefix_key];
}

// Updates information regarding the tape, such as the tape contents
// and the current position of the head.

function tm_update_tape (tape) {
  TM["tape"] = tape;
}

function tm_update_head (head_pos) {
  // TODO: maybe extra check, for example if the pos is valid given
  // current tape?
  TM["head_pos"] = head_pos;
}

function tm_update_state (state) {
  TM["current_state"] = state == "" ? null : state;
}

// Returns all possible state of the machine. The states are computed
// dynamically based on the current set of instructions.

function tm_get_possible_states() {
  // A state is something that can either happen in...
  const states = new Set();

  for (const ins in TM["instructions"]) {
    // ... the first part prefix-key
    states.add(ins.split("/")[0]);
    // ... or in the first element of the value list
    states.add(TM["instructions"][ins][0]);
  }

  return states;
}

// -------------------------------------------------------------------------------------
// INSTRUCTION TABLE FUNCTIONS 

function extract_instruction (trow) {
  cols = trow.getElementsByTagName("td");

  if (cols.length < 7) {
    alert("[ERROR]: Row is malfored: not enough TDs!");
    return null;
  }
  
  ins = [ cols[1].firstChild.value, cols[2].firstChild.value,
	    cols[4].firstChild.value, cols[5].firstChild.value,
	    cols[6].firstChild.value ];

  for(i = 0; i < ins.length; i++) {
    if (ins[i] == "") {
      alert("[ERROR]: Row data is not enough!");
      return null;
    }
  }  

  return ins;
}

function disable_add_instruction_button () {
  rows = document.getElementById("item-button")
    .parentNode
    .parentNode
    .getElementsByTagName("tr");

  button = rows[rows.length - 1]
    .getElementsByTagName("td")[0]
    .getElementsByTagName("button")[0]  

  button.setAttribute("disabled", true);
}

function enable_add_instruction_button () {
  rows = document.getElementById("item-button")
    .parentNode
    .parentNode
    .getElementsByTagName("tr");

  button = rows[rows.length - 1]
    .getElementsByTagName("td")[0]
    .getElementsByTagName("button")[0]

  button.removeAttribute("disabled");
}

// Executed when user adds new row by clicking on the '+' button. This
// function checks if the row is valid, and if it is it adds the newly
// added instruction to the current TM state. A new empty row is then
// added to the DOM for further instructions.
//
// The *trow* param refers to the row being added.
// 
function extract_new_instruction (trow) {
  tuple = extract_instruction(trow);

  if (!tuple) {
    return;
  }

  // update TM internal state 
  if (tm_add_instruction(tuple) < 0) {
    alert("[ERROR]: Instruction prefix already exists!");
    return;
  }

  // update DOM to reflect new instruction

  // -- make all input fields read-only
  make_row_readonly(trow);  

  // -- change button style
  button = trow.getElementsByTagName("td")[0].getElementsByTagName("button")[0];
  button.innerHTML = '-';
  button.className = "btn btn-danger";
    
  // -- update onclick function
  button.setAttribute("onClick", "remove_instruction(this.parentNode.parentNode)");

  // -- add new row below 
  trow.insertAdjacentHTML("afterend", INSTRUCTION_TEMPLATE);

  // -- update state selection
  update_state_selection_options(TM);
}

// Executed when user deletes an already existing row by clicking on
// the '-' button. This function checks if the row is valid, and if it
// is it deletes the instruction contained in the selected row from
// the current TM state. 
//
// The *trow* param refers to the row being deleted.
// 

function remove_instruction (trow) {
  tuple = extract_instruction(trow);
  
  if (!tuple) {
    return;
  }

  // update TM internal state
  tm_remove_instruction(tuple);

  // update DOM to reflect deletion
  trow.innerHTML = "";
  trow.remove();
  update_state_selection_options(TM);
}

function make_row_readonly (trow) {
  input_box = trow.getElementsByTagName("input");
  for (i = 0; i < input_box.length; i++) {
    input_box[i].setAttribute("readonly", true);
  }

  select_box = trow.getElementsByTagName("select")[0];
  select_box.setAttribute("disabled", true);
}

// Sets the instruction table based on the given TM object.
function set_instruction_table (TM) {
  var table = document.getElementById("mt-instruction-table")
      .getElementsByTagName("tbody")[0];
  var table_row_header = table.getElementsByTagName("tr")[0];
  
  table.innerHTML = "";
  table.insertAdjacentHTML("afterbegin", table_row_header.innerHTML);

  for (ins in TM["instructions"]) {
    var curr_state = ins.split("/")[0];
    var curr_symbol = ins.split("/")[1];
    var new_state = TM["instructions"][ins][0];
    var new_symbol = TM["instructions"][ins][1];
    var movement = TM["instructions"][ins][2];

    // TODO: make this a global variable and format it somehow?
    var new_row = `<tr>
  <td id="item-button"><button type="button" class="btn btn-danger" onclick="remove_instruction(this.parentNode.parentNode)">-</button></td>
  
  <td><input type="text" maxlength="1" value="${curr_state}" class="input-instruction-box" readonly="true"></td>
  <td><input type="text" maxlength="1" value="${curr_symbol}" class="input-instruction-box" readonly="true"></td>

  <td></td>
  
  <td><input type="text" maxlength="1" value="${new_state}" class="input-instruction-box" readonly="true"></td>
  <td><input type="text" maxlength="1" value="${new_symbol}" class="input-instruction-box" readonly="true"></td>
  <td><select name="movement" class="input-instruction-box" disabled="true">
      <option ${movement == 'F' ? 'selected' : ''} value="F"> F </option>
      <option ${movement == 'S' ? 'selected' : ''} value="S"> S </option>
      <option ${movement == 'D' ? 'selected' : ''} value="D"> D </option>
  </select> </td>
</tr>`
    
    table.insertAdjacentHTML("beforeend", new_row);
  }

  table.insertAdjacentHTML("beforeend", INSTRUCTION_TEMPLATE);
}

// -------------------------------------------------------------------------------------
// TAPE FUNCTIONS

function extract_tape() {
  // -- extract tape contents
  var tape = [];
  var tape_row = document.getElementById("mt-tape-content")
      .getElementsByTagName("td");

  // -- get tape contents
  for (let i = 0; i < tape_row.length; i++) {
    tape.push(tape_row[i].firstChild.value);
  }

  // -- update TM internal state
  tm_update_tape(tape);
}

// Add new cell on the right side of the left-most cell.
function add_tape_cell_left () {
  console.log("[INFO] - add_tape_cell_left ()");

  const tape_row = document.getElementById("mt-tape-content")
	.getElementsByTagName("td");
  const meta_tape_row = document.getElementById("mt-arrow-tape")
  	.getElementsByTagName("td");

  tape_row[0].insertAdjacentHTML("afterend", EMPTY_TAPE_CELL);
  meta_tape_row[0].insertAdjacentHTML("afterend", `<td onClick="update_head(this)"></td>`);
}

// Add new cell on the left side of the right-most cell.
function add_tape_cell_right() {
  console.log("[INFO] - add_tape_cell_right ()");

  const tape_row = document.getElementById("mt-tape-content")
	.getElementsByTagName("td");
  const meta_tape_row = document.getElementById("mt-arrow-tape")
  	.getElementsByTagName("td");
  
  tape_row[tape_row.length - 1].insertAdjacentHTML("beforebegin", EMPTY_TAPE_CELL);
  meta_tape_row[meta_tape_row.length - 1].insertAdjacentHTML("beforebegin", `<td onClick="update_head(this)"></td>`);  
}

function update_tape(pos) {
  var tape_row = document.getElementById("mt-tape-content")
      .getElementsByTagName("td");

  tape_row[pos].firstChild.value = TM["tape"][pos];
}

function get_head_pos () {
  var head_pos = null;  
  var tape_meta_row = document.getElementById("mt-arrow-tape")
      .getElementsByTagName("td");

  // -- get old head pos
  for (let i = 0; i < tape_meta_row.length; i++) {
    if (tape_meta_row[i].innerHTML != "") {
      head_pos = i;
      break;
    }
  }

  return head_pos;
}

function extract_head_pos() {
  // -- extract head pos in tape
  var head_pos = null;  
  head_pos = get_head_pos();

  // -------------------------------------
  // update TM internal state  

  tm_update_head(head_pos);
}

function make_tape_readonly() {
  var tape_row = document.getElementById("mt-tape-content")
      .getElementsByTagName("td");

  for (let i = 0; i < tape_row.length; i++) {
    tape_row[i].firstChild.setAttribute("readonly", true);
  }
}

function make_tape_writable() {
  var tape_row = document.getElementById("mt-tape-content")
      .getElementsByTagName("td");

  for (let i = 0; i < tape_row.length; i++) {
    tape_row[i].firstChild.removeAttribute("readonly");
  }  
}

function update_head_pos () {
  var tape_meta_row = document.getElementById("mt-arrow-tape")
      .getElementsByTagName("td");
  var old_head_pos = get_head_pos();

  // -- update head pos
  tape_meta_row[old_head_pos].innerHTML = "";
  tape_meta_row[TM["head_pos"]].innerHTML = ARROW_HTML;
}

// This function can be used in the setting up of the TM to change the
// initial position of the head. Everytime the user clicks above a
// cell, this function is called.

function update_head (td) {
  // NOTE: shall we take care of concurrency?
  if (EXECUTING) {
    return;
  }

  const head_pos = TM["head_pos"] ? TM["head_pos"] : get_head_pos();

  // 1. Remove previous arrow
  document.getElementById("mt-arrow-tape")
    .getElementsByTagName("td")[head_pos].innerHTML = "";
  
  // 2. Set new arrow
  td.innerHTML = ARROW_HTML;

  // 3. Update TM state
  extract_head_pos();
}

function set_tape_and_head_pos (TM) {
  // TODO: implement this
  const tape = TM["tape"];
  const head_pos = TM["head_pos"];

  const meta_row = document.getElementById("mt-arrow-tape");
  const data_row = document.getElementById("mt-tape-content");

  meta_row.innerHTML = "";
  data_row.innerHTML = "";

  // add first cols
  meta_row.insertAdjacentHTML("afterbegin", `<td></td>`);
  data_row.insertAdjacentHTML("afterbegin", `<td><button type="button" class="btn btn-primary" onClick="add_tape_cell_left()">+</button></td>`);

  // add actual data cols
  for (let i = 0; i < tape.length; i++) {
    // check if need to insert arrow
    if (i == head_pos) {
      meta_row.insertAdjacentHTML("beforeend", `<td onClick="update_head(this)"> <b style="font-size: 25px;"> &#8595; </b> </td>`);
    } else {
      meta_row.insertAdjacentHTML("beforeend", `<td onClick="update_head(this)"></td>`);
    }    
    data_row.insertAdjacentHTML("beforeend", `<td><input type="text" maxlength="1" value="${tape[i]}" class="input-instruction-box-1"> </td>`);          
  }  

  // add last cols
  meta_row.insertAdjacentHTML("beforeend", `<td></td>`);
  data_row.insertAdjacentHTML("beforeend", `<td><button type="button" class="btn btn-primary" onClick="add_tape_cell_right()">+</button></td>`);    
}

// -------------------------------------------------------------------------------------
// STATE SELECTION FUNCTIONS

function extract_current_state () {
  const select = document.getElementById("current-state-select-box");
  tm_update_state(select.value);
}

// This function updates the option values used in the selection of
// the current state based on the instructions added so far to the TM
// object.

function update_state_selection_options (TM) {
  const select = document.getElementById("current-state-select-box");
  const states = tm_get_possible_states(TM);

  // -- reset previous state
  select.innerHTML = "";

  // -- add new states
  for (const s of states) {
    select.insertAdjacentHTML("beforeend", `<option value="${s}"> ${s} </option>`);
  }

  if (!(TM["current_state"])) {
    // -- extract new state
    extract_current_state();
  }
}

function update_state_selection (TM) {
  const select = document.getElementById("current-state-select-box");
  select.value = TM["current_state"];
}

function make_state_selection_readonly () {
  const select = document.getElementById("current-state-select-box");
  select.setAttribute("disabled", true);
}

function make_state_selection_writable () {
  const select = document.getElementById("current-state-select-box");
  select.removeAttribute("disabled");
}

// -------------------------------------------------------------------------------------
// OTHER FUNCTIONS

function update_execution_velocity(new_vel) {
  // console.log("[INFO]: update_execution_velocity()");
  TIME_STEP = new_vel;
}

function change_main_button() {
  const form = document.getElementById("mt-control-panel-form");
  form.innerHTML = '';
  if (EXECUTING) {
    console.log("Hello");
    // -- transform 'Calcola!' to 'Ferma!'
    form.insertAdjacentHTML("afterbegin", STOP_BUTTON);
  } else {
    // -- transform 'Ferma!' to 'Calcola!'
    console.log("Hellooo");
    form.insertAdjacentHTML("afterbegin", START_BUTTON);
  }
}

// -------------------------------------------------------------------------------------
// EXECUTION FUNCTIONS

var DONE = false;

var ABORT = false;

var EXECUTING = false;

var STEPS_TAKEN = 0;

// time between the execution of each step in the computation.
var TIME_STEP = 1 * 1000;

// NOTE: this is initialized in main()
var STEPS_COUNTER = null;

function stop () {
  ABORT = true;
}

function execute_single_step() {  
  // is there an instruction for the given (STATE, SYMBOL)?
  prefix = TM["current_state"] + "/" + TM["tape"][TM["head_pos"]];
  
  if (!(prefix in TM["instructions"])) {
    return -1;
  }

  // instruction found, now we execute it!
  // console.log("[INFO] instruction found: " + TM["instructions"][prefix]);

  [new_state, symbol_to_write, movement] = TM["instructions"][prefix];

  // -------------------------------------
  // update TM internal state

  const old_pos = TM["head_pos"];

  // ---- 1. Write symbol on tape
  TM["tape"][TM["head_pos"]] = symbol_to_write;

  // ---- 2. Do the movement
  // NOTE: the last and first cells are reserved for the add-cell button
  if (movement == 'S') {
    // S for Sinistra, in english 'left'

    if (TM["head_pos"] > 1) {
      TM["head_pos"] -= 1;
    }

    
  } else if (movement == 'D') {
    // D for Destra, in italian 'right'

    if (TM["head_pos"] < TM["tape"].length - 2) {
      TM["head_pos"] += 1;
    }
  }
  
  // ---- 3. Change internal state
  TM["current_state"] = new_state;

  // -------------------------------------
  // update DOM to reflect deletion

  // 1. update state shown in state selection
  update_state_selection(TM);
    
  // 2. update tape contents
  update_tape(old_pos);

  // 3. update head pos
  update_head_pos();

  // 4. update steps counter
  // update_execution_steps_counter();
  STEPS_TAKEN += 1;
  STEPS_COUNTER.value = STEPS_TAKEN;

  return 0;
}

async function execute() {
  console.log("[INFO] - Starting execution!");
  
  // prepare for execution:
  DONE = false;
  EXECUTING = true;
  STEPS_TAKEN = 0;
  
  make_state_selection_readonly();
  make_tape_readonly();
  disable_add_instruction_button();

  // -- change buttom from 'Calcula!' to 'Ferma!'
  change_main_button();
  
  // -- extract tape contents to TM
  extract_tape();
  extract_head_pos();
  
  while (!DONE && !ABORT) {
    await new Promise(done => setTimeout(
      () => {
	DONE = execute_single_step() < 0;	
	done();	
      } ,TIME_STEP));
  }
  
  // epilogue
  EXECUTING = false;
  ABORT = false;

 // -- change buttom from 'Ferma!' to 'Calcola!'
  change_main_button();  
  
  make_state_selection_writable();
  make_tape_writable();
  enable_add_instruction_button();

  console.log("[INFO] - Execution is over!");
  console.log("[INFO] - Total steps: " + STEPS_TAKEN);
}

// -------------------------------------------------------------------------------------
// LOAD PRESETS

// Loads in the HTML DOM the various buttons to load the various TM
// presets.
function load_presets() {
  const list = document.getElementById("example-list");
  for (id in TM_PRESETS) {
    var list_elem = `<li><button type="button" class="btn btn-primary" onClick="set_DOM(${id})"> Esempio ${id}: ${TM_PRESETS[id]["description"]}</button></li>`;
    list.insertAdjacentHTML("beforeend", list_elem);
  }
}

function set_DOM(id) {
  // -- get proper TM
  TM = TM_PRESETS[id];
  
  // 1. Set instructions in table
  set_instruction_table(TM);

  // 2. Set tape position and tape contents
  set_tape_and_head_pos(TM);

  // 3. Set current state
  update_state_selection_options(TM);
  update_state_selection(TM);
}

// -------------------------------------------------------------------------------------

function main() {
  console.log("BEGIN main()");
  load_presets();

  STEPS_COUNTER = document.getElementById("execution-steps-display-box");
  STEPS_COUNTER.value = 0;
  
  console.log("END main()");
}

window.onload = () => {
  main();
}
