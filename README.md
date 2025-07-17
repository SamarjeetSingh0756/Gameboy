<h1 align="center">ESP32 Gameboy</h1>

<p align="center">
  <em>A compact handheld Gameboy built with the ESP32 microcontroller, coded in C </em>
</p>

<hr>

<h2>Overview</h2>

<p>
  This project showcases a custom-built Gameboy powered by the <strong>ESP32</strong> microcontroller. All code is written in <strong>C</strong> to ensure fast performance and efficient memory usage — perfect for embedded hardware like the ESP32.
</p>

<h2>Author</h2>

<p>
  Created with passion by <strong>Samarjeet Singh</strong>.
</p>

<hr>

<h2>Games Included</h2>

<ul>
  <li><strong>Snake</strong> – A fully original version of the classic snake game</li>
  <li><strong>Tetris</strong> – Adapted and enhanced from an open-source GitHub project (credits below)</li>
</ul>

<h2>Features</h2>

<ul>
  <li>Written entirely in C for lightweight and fast performance</li>
  <li>Optimized to run within the ESP32’s limited memory</li>
  <li>Simple, custom-designed handheld hardware</li>
</ul>

<h2>Credits</h2>

<p>
  A big thanks to the original contributor of the Tetris game code on GitHub — your work served as an excellent starting point for this project.
</p>

<h2>Components Used</h2>
<ul>
  <li>AAA Batteries (x3)</li>
  <li>Push Buttons (x5)</li>
  <li>Push Switch (x1)</li>
  <li>ESP32 Dev Module (x1)</li>
  <li>2.4" SPI TFT Display (x1)</li>
  <li>Assorted Rainbow Wires</li>
  <li>Zero PCB (as required)</li>
  <li>Soldering Iron</li>
  <li>Solder Wire</li>
</ul>

<h2>Build Guide</h2>

<h3>Step 1: Setting Up Arduino IDE</h3>

<ol>
  <li>Download the <strong>Arduino IDE</strong> from <a href="https://www.arduino.cc/en/software" target="_blank">here</a>.</li>
  <li>Open the IDE and go to <code>File > Preferences</code>. Under "Additional Board Manager URLs", paste:
    <pre>https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json</pre>
  </li>
  <li>Next, go to <code>Tools > Board > Boards Manager</code>. Search for <strong>ESP32</strong> and install it.</li>
  <li>Select your board from <code>Tools > Board</code> (e.g., "ESP32 Dev Module").</li>
  <li>Connect your ESP32 using a USB cable and select the correct COM port under <code>Tools > Port</code>.</li>
  <li>Write your game code using <strong>C</strong> for speed and minimal memory usage.</li>
</ol>

<h3>Step 2: Testing and Debugging</h3>

<ol>
  <li>Click the <strong>✓</strong> (check icon) to compile and verify your code.</li>
  <li>If there are any errors, troubleshoot them patiently, one at a time.</li>
  <li>Keep optimizing your code to make it run smoother and use less memory.</li>
</ol>

<h3>Step 3: Uploading and Hardware Connections</h3>

<ol>
  <li>Once your code compiles successfully, click the <strong>→</strong> (upload arrow) to flash it to the ESP32.</li>
  <li>After uploading, begin connecting the TFT display and buttons as described below.</li>
</ol>

<h3>Step 4: Wiring the Display and Buttons</h3>

<p><strong>TFT Display SPI Pinout:</strong></p>

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

<p><strong>Game Control Button Mapping:</strong></p>

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

<p><em>Each push button has two pins: connect one to the specified ESP32 GPIO, and the other to GND.</em></p>

<h3>Step 5: Powering Your Gameboy</h3>

<ul>
  <li>Use <strong>3 x 1.5V AAA batteries</strong> connected in series.</li>
  <li>Connect the <strong>positive (+)</strong> end to the <strong>VIN</strong> pin of the ESP32.</li>
  <li>Connect the <strong>negative (–)</strong> end to <strong>GND</strong> through a power switch or button.</li>
</ul>

<p><strong>⚠️ Tip:</strong> Always double-check your connections before turning the power on to avoid short circuits.</p>

<h3>Done!</h3>

<p>
  Once powered:
  <ul>
    <li>The TFT display should light up.</li>
    <li>Use the <strong>Game Change</strong> button to toggle between games.</li>
    <li>Use the directional buttons to control gameplay.</li>
  </ul>
</p>

<hr>

<h2>Gameplay Demos</h2>

<p align="center">
  <img src="tetris-demo.gif" width="250" alt="Tetris Gameplay">
  <img src="snake-demo.gif" width="250" alt="Snake Gameplay">
</p>

<hr>

<p align="center">
  <em>📦 Feel free to fork, contribute, and build on this project!</em>
</p>
