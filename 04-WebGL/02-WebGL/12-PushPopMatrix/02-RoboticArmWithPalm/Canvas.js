// Global variables
var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;

const webGLMacros = 
{
	SAB_ATTRIBUTE_POSITION : 0,
    SAB_ATTRIBUTE_COLOR : 1,
    SAB_ATTRIBUTE_NORMAL : 2,
    SAB_ATTRIBUTE_TEXTURE0 : 3
};

const MAX_STACK_SIZE = 32;

var sphere = null;
var shaderProgramObject;
var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;

var perspectiveProjectionMatrix;

var sphereColorUniform;
var matrixStack = [];
var top = -1;

var shoulder = 0;
var elbow = 0;

var wrist = 0;

var requestAnimationFrame = window.requestAnimationFrame ||
							window.mozRequestAnimationFrame ||
							window.webkitRequestAnimationFrame ||
							window.oRequestAnimationFrame ||
							window.msRequestAnimationFrame;

function main(){
	// Code
	// Get canvas
	canvas = document.getElementById("SAB");
	if(!canvas)
		console.log("Obtaining canvas failed.\n");
	else
		console.log("Obtaining canvas succeeded.\n");
	
	// Backup canvas dimensions
	canvas_original_width = canvas.width;
	canvas_original_height = canvas.height;

	// Initialize
	initialize();

	// Resize
	resize();

	// Display
	display();

	window.addEventListener("resize", resize, false);
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

function initialize(){
	// Code
	// Get WebGL 2.0 context from canvas into gl
	gl = canvas.getContext("webgl2");
	if(!gl)
		console.log("Obtaining WebGL2.0 context failed.\n");
	else
		console.log("Obtaining WebGL2.0 context succeeded.\n");
	
	// Set viewport width and height of context
	gl.viewportWidth = canvas.width;
	gl.viewportHeight = canvas.height;

	// Vertex Shader
	var vertexShaderSourceCode = 
		"#version 300 es" +
		"\n" +
		"in vec4 a_position;" +
		"in vec4 a_normal;" +
		"uniform mat4 u_modelMatrix;" +
		"uniform mat4 u_viewMatrix;" +
		"uniform mat4 u_projectionMatrix;" +
		"void main(void)" +
		"{" +
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
		"}";

	var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);
	gl.compileShader(vertexShaderObject);

	if(gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(vertexShaderObject);
		if(error.length() > 0)
		{
			var errorString = "Vertex shader error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Fragment Shader
	var fragmentShaderSourceCode = 
		"#version 300 es" +
		"\n" +
		"precision highp float;" +
		"uniform vec3 u_color;" +
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"	FragColor = vec4(u_color, 1.0);" +
		"}";
	

	var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);
	gl.compileShader(fragmentShaderObject);

	if(gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(fragmentShaderObject);
		if(error.length() > 0)
		{
			var errorString = "Fragment shader error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Shader Program
	shaderProgramObject = gl.createProgram();
	gl.attachShader(shaderProgramObject, vertexShaderObject);
	gl.attachShader(shaderProgramObject, fragmentShaderObject);
	// Pre linking shader attribute binding
	gl.bindAttribLocation(shaderProgramObject, webGLMacros.SAB_ATTRIBUTE_POSITION, "a_position");
	// Shader program linking
	gl.linkProgram(shaderProgramObject);
	if(gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObject);
		if(error.length() > 0)
		{
			var errorString = "Shader program error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Uniform locations
	modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_modelMatrix");
	viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_viewMatrix");
	projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_projectionMatrix");
	sphereColorUniform = gl.getUniformLocation(shaderProgramObject, "u_color");
	
	// Declaration of vertex data arrays
	sphere = new Mesh();
	makeSphere(sphere, 2.0, 30, 30); 

	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	// Clear the screen with black color
	gl.clearColor(0.0, 0.0, 0.0, 1.0);

	// Initialize stack
	initializeMatrixStack();

	// Perspective projection matrix
	perspectiveProjectionMatrix =  mat4.create();
	
}

function resize(){
	// Code
	if(bFullscreen == true)
	{
		canvas.width = window.innerWidth;
		canvas.height = window.innerHeight;
	}
	else
	{
		canvas.width = canvas_original_width;
		canvas.height = canvas_original_height;
	}

	if(canvas.height == 0)
		height = 1;

	gl.viewport(0, 0, canvas.width, canvas.height);

	mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width)/parseFloat(canvas.height), 0.1, 100);
}

function display(){
	// Code
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	// Use the shader program object
	gl.useProgram(shaderProgramObject);

	// Transformations
	var modelMatrix = mat4.create();
	var viewMatrix = mat4.create();
	var rotationMatrixZ1 = mat4.create();
	var rotationMatrixZ2 = mat4.create();
	var rotationMatrixZ3 = mat4.create();
	var scaleMatrix = mat4.create();

	var translationMatrix = mat4.create();

	gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);
	mat4.translate(viewMatrix, viewMatrix, [0.0, 0.0, -20.0]);

	gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

	pushMatrix(viewMatrix);

	mat4.rotateZ(rotationMatrixZ1, rotationMatrixZ1, parseFloat(shoulder));
	mat4.translate(translationMatrix, translationMatrix, [4.0, 0.0, 0.0]);
	mat4.scale(scaleMatrix, scaleMatrix, [2.0, 0.5, 1.0]);

	mat4.multiply(rotationMatrixZ1, rotationMatrixZ1, translationMatrix);
	mat4.multiply(modelMatrix, modelMatrix, rotationMatrixZ1);

	pushMatrix(modelMatrix);

	mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	gl.uniform3f(sphereColorUniform, 0.5, 0.35, 0.05);
	// Draw Shoulder
	sphere.draw();

	modelMatrix = popMatrix();
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

	mat4.rotateZ(rotationMatrixZ2, rotationMatrixZ2, parseFloat(elbow));

	mat4.multiply(rotationMatrixZ2, rotationMatrixZ2, translationMatrix);
	mat4.multiply(modelMatrix, modelMatrix, rotationMatrixZ2);

	pushMatrix(modelMatrix);

	mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);
	
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

	// Draw Forearm
	sphere.draw();

	modelMatrix = popMatrix();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [4.0, 0.0, 0.0]);

	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

	mat4.rotateZ(rotationMatrixZ3, rotationMatrixZ3, parseFloat(wrist));
	
	scaleMatrix = mat4.create();
	mat4.scale(scaleMatrix, scaleMatrix, [1.0, 0.5, 1.0]);
	
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [2.0, 0.0, 0.0]);

	mat4.multiply(rotationMatrixZ3, rotationMatrixZ3, translationMatrix);
	mat4.multiply(rotationMatrixZ3, rotationMatrixZ3, scaleMatrix);
	mat4.multiply(modelMatrix, modelMatrix, rotationMatrixZ3);

	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

	// Draw Palm
	sphere.draw();

	popMatrix();

	gl.useProgram(null);

	update();
	// Double buffering emulation
	requestAnimationFrame(display, canvas);
}

function update(){
	// Code
	
}

// Keyboard Event Listener
function keyDown(event){
	// Code
	switch(event.keyCode)
	{
		case 83:
			shoulder = (shoulder + 1) % 360;
			break;
		case 115:
			shoulder = (shoulder - 1) % 360;
			break;
		case 76:
			elbow = (elbow + 1) % 360;
			break;
		case 108:
			elbow = (elbow - 1) % 360;
			break;
		case 87:
			wrist = (wrist + 1) % 360;
			break;
		case 119:
			wrist = (wrist - 1) % 360;
			break;
		case 69:
			uninitialize();
			// Request to browser to close but not all follow.
			window.close();
			break;
		case 70:
			toggleFullscreen();
			break;
	}
}

// Mouse event listener
function mouseDown(){
	// Code
}

function uninitialize(){
	// Code
	if(sphere){
		sphere.deallocate();
		sphere = null;
	}
	if(shaderProgramObject){
		gl.useProgram(shaderProgramObject);
		var shaderObjects = gl.getAttachedShaders(shaderProgramObject);
		for(let i = 0; i < shaderObjects.length; i++)
		{
			gl.detachShader(shaderProgramObject, shaderObjects[i]);
			gl.deleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}
		gl.useProgram(null);
		gl.deleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}
}

function initializeMatrixStack()
{
	for(var i = 0; i < MAX_STACK_SIZE; i++)
	{
		matrixStack[i] = mat4.create();
	}
}

function pushMatrix(matrix)
{
	if(top > MAX_STACK_SIZE)
	{
		console.log("Stack is full.\n");
		return;
	}
	top++;
	matrixStack[top] = mat4.clone(matrix);
}

function popMatrix()
{
	var matrix = mat4.create();
	if(top == 0)
	{
		console.log("Stack is empty.\n");
		return matrix;
	}
	matrix = mat4.clone(matrixStack[top]);
	top--;
	return matrix;
}
