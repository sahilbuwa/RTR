function main(){
	// Code
	// Get canvas
	var canvas = document.getElementById("SAB");
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

	// Adding Keyboard and Mouse listeners
	window.addEventListener("keydown", keyDown, false);
	window.addEventListener("click", mouseDown, false);
	
}
// Keyboard Event Listener
function keyDown(Event){
	// Code
	alert("A key is pressed.");
}

// Mouse event listener
function mouseDown(){
	// Code
	alert("A mouse button is clicked.");
}

