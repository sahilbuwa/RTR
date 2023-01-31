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
var vao_pyramid;
var vbo_pyramid_position;
var vbo_pyramid_color;
var mvpMatrixUniform;
var anglePyramid = 0.0;
var vao_cube;
var vbo_cube_position;
var vbo_cube_color;
var angleCube = 0.0;

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
		"in vec4 a_color;" +
		"uniform mat4 u_mvpMatrix;" +
		"out vec4 a_color_out;" +
		"void main(void)" +
		"{" +
		"gl_Position = u_mvpMatrix * a_position;" +
		"a_color_out = a_color;" +
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
		"in vec4 a_color_out;" +
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"FragColor = a_color_out;" +
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
	gl.bindAttribLocation(shaderProgramObject, webGLMacros.SAB_ATTRIBUTE_COLOR, "a_color");

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

	// Declaration of vertex data arrays
	var pyramidVertices = new Float32Array
		([	
			// front
			0.0, 1.0, 0.0,
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,
	
			// right
			0.0, 1.0, 0.0,
			1.0, -1.0, 1.0,
			1.0, -1.0, -1.0,
	
			// back
			0.0, 1.0, 0.0,
			1.0, -1.0, -1.0,
			-1.0, -1.0, -1.0,
	
			// left
			0.0, 1.0, 0.0,
			-1.0, -1.0, -1.0,
			-1.0, -1.0, 1.0
		]);

	var pyramidColor = new Float32Array
		([	
			// front
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0,
	
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
	
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0,
	
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0
		]);

	var cubeVertices = new Float32Array
		([	
			// top
			1.0, 1.0, -1.0,
			-1.0, 1.0, -1.0, 
			-1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,  

			// bottom
			1.0, -1.0, -1.0,
			-1.0, -1.0, -1.0,
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,

			// front
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,

			// back
			1.0, 1.0, -1.0,
			-1.0, 1.0, -1.0,
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,

			// right
			1.0, 1.0, -1.0,
			1.0, 1.0, 1.0,
			1.0, -1.0, 1.0,
			1.0, -1.0, -1.0,

			// left
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0, 
			-1.0, -1.0, -1.0, 
			-1.0, -1.0, 1.0
		]);

	var cubeColor = new Float32Array
		([	
			// top
			0.0, 1.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 1.0, 0.0,
	
			1.0, 0.5, 0.0,
			1.0, 0.5, 0.0,
			1.0, 0.5, 0.0,
			1.0, 0.5, 0.0,
	
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
	
			1.0, 1.0, 0.0,
			1.0, 1.0, 0.0,
			1.0, 1.0, 0.0,
			1.0, 1.0, 0.0,
	
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
	
			1.0, 0.0, 1.0,
			1.0, 0.0, 1.0,
			1.0, 0.0, 1.0,
			1.0, 0.0, 1.0
		]);

	// Vao and vbo code
	vao_cube = gl.createVertexArray();
	gl.bindVertexArray(vao_cube);
	// Position
	vbo_cube_position = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_cube_position);
	gl.bufferData(gl.ARRAY_BUFFER, cubeVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);

	vbo_cube_color = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_cube_color);
	gl.bufferData(gl.ARRAY_BUFFER, cubeColor, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_COLOR);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);

	gl.bindVertexArray(null);

	vao_pyramid = gl.createVertexArray();
	gl.bindVertexArray(vao_pyramid);
	// Position
	vbo_pyramid_position = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_pyramid_position);
	gl.bufferData(gl.ARRAY_BUFFER, pyramidVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);

	vbo_pyramid_color = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_pyramid_color);
	gl.bufferData(gl.ARRAY_BUFFER, pyramidColor, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_COLOR);
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

function degToRad(degrees){
	return degrees * Math.PI / 180.0;
}

function display(){
	// Code
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	// Use the shader program object
	gl.useProgram(shaderProgramObject);

	// Transformations Triangle
	var modelMatrix = mat4.create();
	var modelViewProjectionMatrix = mat4.create();
	var translationMatrix = mat4.create();
	var rotationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-1.5, 0.0, -6.0]);
	mat4.rotateY(rotationMatrix, rotationMatrix, anglePyramid);
	mat4.multiply(modelMatrix, translationMatrix, rotationMatrix);
	mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelMatrix);

	gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

	gl.bindVertexArray(vao_pyramid);

	gl.drawArrays(gl.TRIANGLES, 0, 12);

	gl.bindVertexArray(null);

	// Transformations Triangle
	modelMatrix = mat4.create();
	modelViewProjectionMatrix = mat4.create();
	translationMatrix = mat4.create();
	rotationMatrix = mat4.create();
	var rotationMatrixX = mat4.create();
	var rotationMatrixY = mat4.create();
	var rotationMatrixZ = mat4.create();
	var scaleMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [1.5, 0.0, -6.0]);
	mat4.rotateX(rotationMatrixX, rotationMatrixX, angleCube);
	mat4.rotateY(rotationMatrixY, rotationMatrixY, angleCube);
	mat4.rotateZ(rotationMatrixZ, rotationMatrixZ, angleCube);
	mat4.scale(scaleMatrix, scaleMatrix, [0.75,0.75,0.75]);
	mat4.multiply(rotationMatrix, rotationMatrixX, rotationMatrixY);
	mat4.multiply(rotationMatrix, rotationMatrix, rotationMatrixZ);
	mat4.multiply(modelMatrix, translationMatrix, scaleMatrix);
	mat4.multiply(modelMatrix, modelMatrix, rotationMatrix);
	mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelMatrix);

	gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

	gl.bindVertexArray(vao_cube);

	gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);


	gl.bindVertexArray(null);

	gl.useProgram(null);

	// Update Call
	update();
	// Double buffering emulation
	requestAnimationFrame(display, canvas);
}

function update(){
	// Code
	anglePyramid = anglePyramid + 0.01;
	if(anglePyramid > 2 * Math.PI)
		anglePyramid -= 2 * Math.PI;
	
	angleCube = angleCube + 0.01;
	if(angleCube > 2 * Math.PI)
		angleCube -= 2 * Math.PI;

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
	if(vbo_cube_color){
		gl.deleteBuffer(vbo_cube_color);
		vbo_cube_color = null;
	}
	if(vbo_cube_position){
		gl.deleteBuffer(vbo_cube_position);
		vbo_cube_position = null;
	}
	if(vao_cube){
		gl.deleteVertexArray(vao_cube);
		vao_cube = null;
	}
	if(vbo_pyramid_color){
		gl.deleteBuffer(vbo_pyramid_color);
		vbo_pyramid_color = null;
	}
	if(vbo_pyramid_position){
		gl.deleteBuffer(vbo_pyramid_position);
		vbo_pyramid_position = null;
	}
	if(vao_pyramid){
		gl.deleteVertexArray(vao_pyramid);
		vao_pyramid = null;
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
