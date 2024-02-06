const char MAIN_page[] PROGMEM = R"=====(
<html>
<style>
  body {background-color: #FF0000; font-family: Times; font-size: 400% ; text-align: center; margin:1px auto; padding-top: 300px; width:auto; user-select: none;}
</style>
<body>     
 
  <h1 style="font-size:80%;"> This Needs a Bluetooth Controller - None Connected --  press a button when connected</h1>


<script type="text/javascript">

// global gamepad object
let gamepadIndex;
window.addEventListener('gamepadconnected', (event) => {
  gamepadIndex = event.gamepad.index;
});

  let oldspeed = 0;
  let oldsteer = 90;

setInterval(() => {

  if(gamepadIndex !== undefined) {
    // a gamepad is connected and has an index
   
    const myGamepad = navigator.getGamepads()[gamepadIndex];
    document.body.style.backgroundColor = "green" ;
    document.body.innerHTML = "  Perky Goes Wild!"; // reset page
 
    let y = myGamepad.axes[0];             // left joystick   - Direction   (servo)  
    let v = myGamepad.axes[1];             //  not used
    let z = myGamepad.axes[2];             //  right joystick - Speed    (motor)
    let x = myGamepad.axes[3];             // not used

    if(y<.3 && y >-.3){steer(90); }   //  center dead spot for the steering and speed joystick
    else { if(y < -0.3){steer(90-((y+.3)*(28/.7)*-1)); }      //  joystick steer Left adjusting for dead spot
           else{ if(y > 0.3){steer(90+(y-.3)*(28/.7));}}}  //  joystick right 
  
    if(x<.2 && x >-.2){speed(0,"A");    }     //  center dead spot for the steering and speed joystick
    else { if(x < -0.2){speed((((x+.2)*(255-75)/.8+75*-1)*-1),"A"); }      //  joystick forward pressed   ******************
           else{ if(x > 0.2){speed(((x-.2)*(255-125)/.8+125),"B");}}}   //  joystick back pressed
  
              
    myGamepad.buttons.map(e => e.pressed).forEach((isPressed, buttonIndex) => { 
      if(isPressed) {

        if(buttonIndex == 0){ toggle("off");}                 // stop the motors
        
        // if a button is pressed; indicate this on the page by uncommenting the next two lines, very handy for debugging  and identifying the buttons.

        // document.body.style.backgroundColor = "blue" ;
        // document.body.innerHTML += `<h1>Button ${buttonIndex} is pressed  </h1>`;
      
      }
    })
   
   
  }
  
}, 100) // print buttons that are pressed 10 times per second


  function toggle(m) {       // for sending a single request
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + m, true);
     xhr.send();
  }

  function speed(howFast,m) {  // for sending two things SPEED and direction
     if(howFast != oldspeed){
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/SPEED?value="+ howFast , true);
        oldspeed = howFast;
        xhr.send();
        toggle(m);  // tell motor to go at the new speed
     }}

  function steer(pos) {       // for sending STEER triger and pos position
     if(pos != oldsteer){
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/STEER?value="+ pos , true);
        oldsteer = pos;
        xhr.send();
      }}

</script>
</body>
</html>

 )====="; 

