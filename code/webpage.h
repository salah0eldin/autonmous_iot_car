#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>NodeMCU_Car</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    
    body { 
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      color: #333;
    }
    
    .container {
      max-width: 500px;
      margin: 0 auto;
      padding: 20px;
    }
    
    .header { 
      background: rgba(255, 255, 255, 0.95);
      backdrop-filter: blur(10px);
      color: #333;
      padding: 20px;
      font-size: 24px;
      font-weight: 600;
      text-align: center;
      border-radius: 20px;
      margin-bottom: 30px;
      box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
      border: 1px solid rgba(255, 255, 255, 0.2);
    }
    
    .controls { 
      display: grid; 
      grid-template-columns: repeat(3, 1fr); 
      gap: 15px; 
      margin: 30px 0;
      max-width: 300px;
      margin-left: auto;
      margin-right: auto;
    }
    
    .btn { 
      background: rgba(255, 255, 255, 0.9);
      border: none;
      outline: none;
      cursor: pointer;
      border-radius: 20px;
      padding: 15px;
      transition: all 0.3s ease;
      box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
      border: 1px solid rgba(255, 255, 255, 0.3);
    }
    
    .btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 8px 25px rgba(0, 0, 0, 0.15);
      background: rgba(255, 255, 255, 1);
    }
    
    .btn:active {
      transform: translateY(0);
    }
    
    .btn svg { 
      width: 40px; 
      height: 40px;
      filter: drop-shadow(0 2px 4px rgba(0, 0, 0, 0.1));
    }
    
    .slider-container { 
      background: rgba(255, 255, 255, 0.95);
      backdrop-filter: blur(10px);
      border-radius: 25px;
      padding: 30px;
      margin: 30px 0;
      box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
      border: 1px solid rgba(255, 255, 255, 0.2);
    }
    
    .slider-group {
      margin-bottom: 25px;
    }
    
    .slider-group:last-child {
      margin-bottom: 0;
    }
    
    .slider-label { 
      font-size: 16px; 
      font-weight: 600; 
      margin-bottom: 12px; 
      color: #333;
      display: block;
    }
    
    .slider-value { 
      font-size: 14px; 
      color: #666; 
      margin-left: 10px;
      font-weight: 500;
    }
    
    .slider { 
      width: 100%;
      height: 8px;
      border-radius: 5px;
      background: #e0e0e0;
      outline: none;
      -webkit-appearance: none;
      margin: 10px 0;
    }
    
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 24px;
      height: 24px;
      border-radius: 50%;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      cursor: pointer;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
      transition: all 0.2s ease;
    }
    
    .slider::-webkit-slider-thumb:hover {
      transform: scale(1.1);
      box-shadow: 0 6px 12px rgba(0, 0, 0, 0.3);
    }
    
    .slider::-moz-range-thumb {
      width: 24px;
      height: 24px;
      border-radius: 50%;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      cursor: pointer;
      border: none;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
    }
    
    .toggle-row { 
      display: flex; 
      flex-direction: column;
      gap: 15px;
      margin-top: 20px;
    }
    
    .toggle-btn, .home-btn {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      border: none;
      border-radius: 15px;
      padding: 15px 25px;
      font-size: 16px;
      font-weight: 600;
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0 4px 15px rgba(102, 126, 234, 0.3);
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }
    
    .toggle-btn:hover, .home-btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 8px 25px rgba(102, 126, 234, 0.4);
    }
    
    .toggle-btn:active, .home-btn:active {
      transform: translateY(0);
    }
    
    .toggle-btn.active {
      background: linear-gradient(135deg, #ff6b6b 0%, #ee5a24 100%);
      box-shadow: 0 4px 15px rgba(255, 107, 107, 0.3);
    }
    
    .home-btn { 
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    }
    
    .home-btn:disabled {
      background: #ccc;
      cursor: not-allowed;
      transform: none;
      box-shadow: none;
    }
    
    .home-btn:disabled:hover {
      transform: none;
      box-shadow: none;
    }
    
    .home-btn.active {
      background: linear-gradient(135deg, #4CAF50 0%, #45a049 100%);
      box-shadow: 0 4px 15px rgba(76, 175, 80, 0.3);
    }
    
    .home-btn.active:hover {
      box-shadow: 0 8px 25px rgba(76, 175, 80, 0.4);
    }
    
    @media (max-width: 600px) {
      .container {
        padding: 15px;
      }
      
      .header {
        font-size: 20px;
        padding: 15px;
      }
      
      .slider-container {
        padding: 20px;
      }
      
      .slider-label { 
        font-size: 14px; 
      }
      
      .slider-value { 
        font-size: 12px; 
      }
      
      .toggle-btn, .home-btn { 
        font-size: 14px; 
        padding: 12px 20px; 
      }
      
      .controls {
        gap: 10px;
      }
      
      .btn {
        padding: 12px;
      }
      
      .btn svg {
        width: 35px;
        height: 35px;
      }
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">NodeMCU Car Control</div>
    <div class="controls">
      <!-- Top left (should be bottom right) -->
      <button class="btn" onmousedown="sendCmd('J')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('J')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="30,50 50,30 30,30" fill="#667eea"/><polygon points="30,50 40,30 30,30" fill="#764ba2"/></svg>
      </button>
      <!-- Up -->
      <button class="btn" onmousedown="sendCmd('F')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('F')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="30,10 50,30 10,30" fill="#667eea"/><polygon points="30,10 40,30 20,30" fill="#764ba2"/></svg>
      </button>
      <!-- Top right (should be bottom left) -->
      <button class="btn" onmousedown="sendCmd('H')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('H')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="30,50 10,30 30,30" fill="#667eea"/><polygon points="30,50 20,30 30,30" fill="#764ba2"/></svg>
      </button>
      <!-- Left -->
      <button class="btn" onmousedown="sendCmd('L')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('L')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="10,30 30,50 30,10" fill="#667eea"/><polygon points="10,30 30,40 30,20" fill="#764ba2"/></svg>
      </button>
      <!-- Stop -->
      <button class="btn" onmousedown="sendCmd('S')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('S')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><circle cx="30" cy="30" r="25" stroke="#667eea" stroke-width="5" fill="none"/><rect x="20" y="20" width="20" height="20" fill="#764ba2"/></svg>
      </button>
      <!-- Right -->
      <button class="btn" onmousedown="sendCmd('R')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('R')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="50,30 30,50 30,10" fill="#667eea"/><polygon points="50,30 30,40 30,20" fill="#764ba2"/></svg>
      </button>
      <!-- Bottom left (should be top right) -->
      <button class="btn" onmousedown="sendCmd('I')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('I')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="30,10 50,30 30,30" fill="#667eea"/><polygon points="30,30 50,30 30,10" fill="#764ba2"/></svg>
      </button>
      <!-- Down -->
      <button class="btn" onmousedown="sendCmd('B')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('B')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="30,50 50,30 10,30" fill="#667eea"/><polygon points="30,50 40,30 20,30" fill="#764ba2"/></svg>
      </button>
      <!-- Bottom right (should be top left) -->
      <button class="btn" onmousedown="sendCmd('G')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('G')" ontouchend="sendCmd('S')">
        <svg viewBox="0 0 60 60"><polygon points="30,10 10,30 30,30" fill="#667eea"/><polygon points="30,30 10,30 30,10" fill="#764ba2"/></svg>
      </button>
    </div>
    <div class="slider-container">
      <div class="slider-group">
        <label class="slider-label">Car Speed</label>
        <input type="range" min="0" max="100" value="0" class="slider" id="speedSlider" oninput="updateSpeedValue(this.value); sendSpeed(this.value)">
        <span class="slider-value" id="speedValue">0%</span>
      </div>
      <div class="slider-group">
        <label class="slider-label">Steering Speed</label>
        <input type="range" min="1" max="100" value="75" class="slider" id="steerSlider" oninput="updateSteerValue(this.value); sendSteerSpeed(this.value)">
        <span class="slider-value" id="steerValue">75%</span>
      </div>
      <div class="toggle-row">
        <button id="autoHomeBtn" class="toggle-btn" onclick="toggleAutoHome()">Enable Auto Go Home</button>
        <button id="manualHomeBtn" class="home-btn" onclick="sendGoHome()">Go Home</button>
      </div>
    </div>
  </div>
  <script>
    function sendCmd(cmd) {
      fetch('/cmd?dir=' + cmd);
    }
    function sendSpeed(val) {
      fetch('/speed?car=' + val);
    }
    function sendSteerSpeed(val) {
      fetch('/speed?steer=' + val);
    }
    function updateSpeedValue(val) {
      document.getElementById('speedValue').textContent = val + '%';
    }
    function updateSteerValue(val) {
      document.getElementById('steerValue').textContent = val + '%';
    }
    function toggleAutoHome() {
      var btn = document.getElementById('autoHomeBtn');
      var manualBtn = document.getElementById('manualHomeBtn');
      var enabled = btn.classList.toggle('active');
      btn.textContent = enabled ? 'Disable Auto Go Home' : 'Enable Auto Go Home';
      manualBtn.disabled = enabled;
      if (enabled) {
        manualBtn.classList.remove('active');
      } else {
        manualBtn.classList.add('active');
      }
      fetch('/cmd?autoHome=' + (enabled ? '1' : '0'));
    }
    function sendGoHome() {
      var btn = document.getElementById('manualHomeBtn');
      fetch('/cmd?manualHome=' + '1');
    }
    window.onload = function() {
      var btn = document.getElementById('autoHomeBtn');
      var manualBtn = document.getElementById('manualHomeBtn');
      if (!btn.classList.contains('active')) {
        manualBtn.disabled = false;
        manualBtn.classList.add('active');
      }
    }
  </script>
</body>
</html>
)rawliteral";

#endif 