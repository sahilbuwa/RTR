// Global variables
var canvas = null;
var bFullscreen = false;

function main(){
	// Code
	// Get canvas
	canvas = document.getElementById("SAB");
	if(!canvas)
		console.log("Obtaining canvas failed.\n");
	else
		console.log("Obtaining canvas succeeded.\n");
	// Get 2D context from canvas
	var context = canvas.getContext("2d");
	if(!context)
		console.log("Obtaining context failed.\n");
		else
		console.log("Obtaining context succeeded.\n");
	// Paint background of canvas by black color
	context.fillStyle = "black"; // In place of black we may write its hexadecimal code as "#000000".
	context.fillRect(0, 0, canvas.width, canvas.height);

	// Declare the string
	var str = "Hello World!!!";
	// Decide the font
	context.font = "48px sans-serif";
	// Decide text color
	context.fillStyle = "white"; // In place of white we may write its hexadecimal code as "#ffffff".
	// Put the text in the middle of the canvas vertically and horizontally
	context.textAlign = "center"; // This is horizontal center.
	context.textBaseline = "middle"; // Vertical center.
	// Display the text
	context.fillText(str, canvas.width/2, canvas.height/2);
	// Adding Keyboard and Mouse listeners
	window.addEventListener("keydown", keyDown, false);
	window.addEventListener("click", mouseDown, false);
	
}

function toggleFullscreen(){
	// Code
	var fullscreen_element = document.fullscreenElement ||
							 document.mozFullScreenElement ||
							 document.webkitFullscreenElement ||
							 document.msFullscreenElement ||
							 null;
	if(fullscreen_element == null) // If not fullscreen
	{
		if(canvas.requestFullscreen)
			canvas.requestFullscreen();
		else if(canvas.mozRequestFullScreen)
			canvas.mozRequestFullScreen();
		else if(canvas.webkitRequestFullscreen)
			canvas.webkitRequestFullscreen();
		else if(canvas.msRequestFullscreen)
			canvas.msRequestFullscreen();
		bFullscreen = true;
	}
	else
	{
		if(document.exitFullscreen)
			document.exitFullscreen();
		else if(document.mozExitFullScreen)
			document.mozExitFullScreen();
		else if(document.webkitExitFullscreen)
			document.webkitExitFullscreen();
		else if(document.msExitFullscreen)
			document.msExitFullscreen();
		bFullscreen = false;
	}
}
// Keyboard Event Listener
function keyDown(event){
	// Code
	switch(event.keyCode)
	{
		case 70:
			toggleFullscreen();
			drawText("Hello World!!!");
			break;
	}
}

// Mouse event listener
function mouseDown(){
	// Code
}

// Fullscreen steps
// 1.Check fullscreen support by browser

// 2.Call respective browser fullscreen ()

// 3.If fullscreen already again call fullscreen()
