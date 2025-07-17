<h1 align="center">ESP32 Gameboy</h1>

<p align="center">
  <em>A compact Gameboy built using ESP32, coded in C for memory efficiency and performance.</em>
</p>

<hr>

<h2>Overview</h2>

<p>
  This project is a custom handheld Gameboy designed and built using the <strong>ESP32</strong> microcontroller. The entire codebase is written in <strong>C</strong> to optimize for speed and memory usage on embedded hardware.
</p>

<h2>Author</h2>

<p>
  Created with passion by <strong>Samarjeet Singh</strong>
</p>

<hr>

<h2>Games Included</h2>

<ul>
  <li><strong>Snake</strong> – Classic arcade-style snake game fully coded from scratch</li>
  <li><strong>Tetris</strong> – A version adapted from an open-source GitHub project (credits below)</li>
</ul>

<h2>Features</h2>

<ul>
  <li>Lightweight and fast performance using native C</li>
  <li>Optimized for the ESP32's memory constraints</li>
  <li>Custom-designed hardware interface</li>
</ul>


<h2>Credits</h2>

<p>
  Special thanks to the original Tetris code contributor whose GitHub repository helped inspire and guide part of the implementation.
</p>

<h2>Building Guide</h2>

<h3>Step 1: Setting Up the Arduino IDE and Coding</h3>

<ol>
  <li>Download and install the <strong>Arduino IDE</strong> from <a href="https://www.arduino.cc/en/software" target="_blank">here</a>.</li>
  <li>Open the IDE and go to <code>File > Preferences</code>. In the "Additional Board Manager URLs" box, paste:
    <pre>https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json</pre>
  </li>
  <li>Go to <code>Tools > Board > Boards Manager</code>, search for <strong>ESP32</strong> and install it.</li>
  <li>Select your board from <code>Tools > Board</code> (e.g., "ESP32 Dev Module").</li>
  <li>Connect your ESP32 to the PC with a USB cable and select the correct COM port from <code>Tools > Port</code>.</li>
  <li>Write your code in <strong>C language</strong> to make it efficient and lightweight.</li>
</ol>

<h3>Step 2: Testing Code and Fixing Bugs</h3>

<ol>
  <li>Click the <strong>✓ (checkmark icon)</strong> to compile and verify your code.</li>
  <li>If errors appear, carefully read and fix them step-by-step.</li>
  <li>Try to optimize your code to use less memory and run smoother.</li>
</ol>

<h3>Step 3: Uploading Code and Connecting Hardware</h3>

<ol>
  <li>Once verified, click the <strong>→ (upload arrow)</strong> to upload the code to ESP32.</li>
  <li>After uploading, begin connecting the hardware: TFT screen and buttons.</li>
</ol>

<h3>Step 4: Wiring the Display and Buttons</h3>

<p><strong>TFT SPI Display Wiring:</strong></p>

<table>
  <thead>
    <tr><th>TFT Display Pin</th><th>ESP32 Pin</th></tr>
  </thead>
  <tbody>
    <tr><td>VCC</td><td>3V3</td></tr>
    <tr><td>GND</td><td>GND</td></tr>
    <tr><td>CS</td><td>D15</td></tr>
    <tr><td>RESET</td><td>D4</td></tr>
    <tr><td>DC / RS</td><td>D2</td></tr>
    <tr><td>MOSI</td><td>D23</td></tr>
    <tr><td>SCK</td><td>D18</td></tr>
    <tr><td>LED</td><td>3V3</td></tr>
  </tbody>
</table>

<p><strong>Game Control Buttons Wiring:</strong></p>

<table>
  <thead>
    <tr><th>Button</th><th>ESP32 Pin</th></tr>
  </thead>
  <tbody>
    <tr><td>Game Change</td><td>D22</td></tr>
    <tr><td>Up</td><td>D27</td></tr>
    <tr><td>Down</td><td>D25</td></tr>
    <tr><td>Left</td><td>D14</td></tr>
    <tr><td>Right</td><td>D26</td></tr>
  </tbody>
</table>

<p><em>Each button has two legs: connect one leg to the specified ESP32 pin and the other leg to GND.</em></p>

<h3>Step 5: Powering the Gameboy</h3>

<ul>
  <li>Use <strong>3 x 1.5V AAA batteries</strong> in a battery holder.</li>
  <li>Connect the <strong>positive (+)</strong> terminal to the <strong>VIN</strong> pin of ESP32.</li>
  <li>Connect the <strong>negative (–)</strong> terminal to <strong>GND</strong> on ESP32 via a button or power switch.</li>
</ul>

<p><strong>Tip:</strong> Always double-check your wiring before powering the circuit to prevent any short circuits.</p>

<h3>That's It!</h3>

<p>
  When you power on the device:
  <ul>
    <li>The screen will light up.</li>
    <li>You can switch between games using the "Game Change" button.</li>
    <li>Control gameplay using the directional buttons (Up, Down, Left, Right).</li>
  </ul>
</p>



<hr>

<p align="center">
  <em>📦 Feel free to fork, modify, and contribute to this project!</em>
</p>
