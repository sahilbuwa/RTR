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

var shaderProgramObject;
var vao_square;
var vbo_quad;
var vao_circle;
var vbo_circle_position;
var vao_triangle;
var vbo_triangle_position;
var vao_graphs;
var vao_axes;
var vbo_graph_lines;
var vbo_axes_lines;

var mvpMatrixUniform;
var colorUniform;
var circleVertices = new Float32Array(1080);
var graphLineVertices = new Float32Array(480);
var perspectiveProjectionMatrix;

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
		"uniform mat4 u_mvpMatrix;" +
		"void main(void)" +
		"{" +
		"gl_Position = u_mvpMatrix * a_position;" +
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
		"FragColor = vec4(u_color, 1.0);" +
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
	mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_mvpMatrix");
	colorUniform = gl.getUniformLocation(shaderProgramObject, "u_color");
	// Declaration of vertex data arrays
	var squareVertices = new Float32Array
		([	
			1.0, 1.0, 0.0,
			-1.0, 1.0, 0.0, 
			-1.0, -1.0, 0.0, 
			1.0, -1.0, 0.0
		]);
	
	var triangleVertices = new Float32Array
		([	
			Math.cos(Math.PI / 2), Math.sin(Math.PI / 2), 0.0,
			Math.cos(-(Math.PI/6.0)), Math.sin(-(Math.PI/6.0)) , 0.0,
			Math.cos((7.0*Math.PI)/6.0), Math.sin((7.0*Math.PI)/6.0), 0.0
		]);
	var angle = 0.0;
	for(var i = 0; i < 360; i++)
	{
		angle = degToRad(i);
		circleVertices[i * 3] = Math.cos(angle);
		circleVertices[i * 3 + 1] = Math.sin(angle);
		circleVertices[i * 3 + 2] = 0.0;
	}
	var index = 0;
	for(var i = -1.25;i <= 1.25; i += 0.0625)
	{
		if(i==0.0)
			continue;
		graphLineVertices[index * 3] = 1.25;
		graphLineVertices[index * 3 + 1] = i; 
		graphLineVertices[index * 3 + 2] = 0.0;
		index++;
		graphLineVertices[index * 3] = -1.25;
		graphLineVertices[index * 3 + 1] = i;
		graphLineVertices[index * 3 + 2] = 0.0; 
		index++;
		graphLineVertices[index * 3] = i;
		graphLineVertices[index * 3 + 1] = 1.25; 
		graphLineVertices[index * 3 + 2] = 0.0; 
		index++;
		graphLineVertices[index * 3] = i;
		graphLineVertices[index * 3 + 1] = -1.25; 
		graphLineVertices[index * 3 + 2] = 0.0; 
		index++;
	}

	var axesVertices = new Float32Array
	([	
		0.0, 1.25, 0.0,
		0.0, -1.25, 0.0,
		1.25, 0.0, 0.0,
		-1.25, 0.0, 0.0
	]);

	// Vao and vbo code
	vao_square = gl.createVertexArray();
	gl.bindVertexArray(vao_square);
	vbo_quad = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_quad);
	gl.bufferData(gl.ARRAY_BUFFER, squareVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);

	gl.bindBuffer(gl.ARRAY_BUFFER, null);
	gl.bindVertexArray(null);

	vao_graphs = gl.createVertexArray();
	gl.bindVertexArray(vao_graphs);
	vbo_graph_lines = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_graph_lines);
	gl.bufferData(gl.ARRAY_BUFFER, graphLineVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);

	gl.bindBuffer(gl.ARRAY_BUFFER, null);
	gl.bindVertexArray(null);

	vao_axes = gl.createVertexArray();
	gl.bindVertexArray(vao_axes);
	vbo_axes_lines = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_axes_lines);
	gl.bufferData(gl.ARRAY_BUFFER, axesVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);

	gl.bindBuffer(gl.ARRAY_BUFFER, null);
	gl.bindVertexArray(null);

	vao_circle = gl.createVertexArray();
	gl.bindVertexArray(vao_circle);
	vbo_circle_position = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_circle_position);
	gl.bufferData(gl.ARRAY_BUFFER, circleVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);

	gl.bindBuffer(gl.ARRAY_BUFFER, null);
	gl.bindVertexArray(null);

	vao_triangle = gl.createVertexArray();
	gl.bindVertexArray(vao_triangle);
	vbo_triangle_position = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_triangle_position);
	gl.bufferData(gl.ARRAY_BUFFER, triangleVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);

	gl.bindBuffer(gl.ARRAY_BUFFER, null);
	gl.bindVertexArray(null);

	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	// Clear the screen with black color
	gl.clearColor(0.0, 0.0, 0.0, 1.0);

	// Perspective projection matrix
	perspectiveProjectionMatrix =  mat4.create();
	
}

function degToRad(degrees){
	return degrees * Math.PI / 180.0;
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
	var modelViewProjectionMatrix = mat4.create();
	var translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -3.25]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelMatrix);

	gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

	gl.lineWidth(1.0);
	gl.uniform3f(colorUniform, 0.0, 0.0, 1.0);
	gl.bindVertexArray(vao_graphs);

	gl.drawArrays(gl.LINES, 0, 160);

	gl.bindVertexArray(null);
	// Axes
	gl.lineWidth(2.0);
	gl.uniform3f(colorUniform, 0.0, 1.0, 0.0);
	gl.bindVertexArray(vao_axes);

	gl.drawArrays(gl.LINES, 0, 2);

	gl.uniform3f(colorUniform, 1.0, 0.0, 0.0);
	gl.drawArrays(gl.LINES, 2, 2);

	gl.bindVertexArray(null);

	// Square
	gl.uniform3f(colorUniform, 1.0, 1.0, 0.0);
	gl.bindVertexArray(vao_square);

	gl.drawArrays(gl.LINE_LOOP, 0, 4);

	gl.bindVertexArray(null);

	// Triangle
	gl.bindVertexArray(vao_triangle);

	gl.drawArrays(gl.LINE_LOOP, 0, 3);

	gl.bindVertexArray(null);

	// Circle
	gl.bindVertexArray(vao_circle);

	gl.drawArrays(gl.LINE_LOOP, 0, 360);

	gl.bindVertexArray(null);

	gl.useProgram(null);

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
	if(vao){
		gl.deleteVertexArray(vao);
		vao = null;
	}
	if(vbo){
		gl.deleteBuffer(vbo);
		vbo = null;
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
