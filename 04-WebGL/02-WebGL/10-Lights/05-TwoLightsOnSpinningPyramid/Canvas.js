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
var vbo_pyramid_normal;
var anglePyramid = 0.0;

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;

var laUniform = Array(2);
var ldUniform = Array(2);
var lsUniform = Array(2);
var lightPositionUniform = Array(2);

var kaUniform;
var kdUniform;
var ksUniform;
var materialShininessUniform;

var lightingEnableUniform;

var bLight = false;

var lightAmbient = Array(2);
var lightDiffuse = Array(2);
var lightSpecular = Array(2);
var lightPosition = Array(2);

var materialAmbient = [0.0, 0.0, 0.0];
var materialDiffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShininess = 50.0;


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
	"precision highp float;" +
	"precision highp int;" +
    "in vec4 a_position;\n" +
    "in vec3 a_normal;\n" +
    "uniform mat4 u_modelMatrix;\n" +
    "uniform mat4 u_viewMatrix;\n" +
    "uniform mat4 u_projectionMatrix;\n" +
    "uniform vec3 u_la[2];\n" +
    "uniform vec3 u_ld[2];\n" +
    "uniform vec3 u_ls[2];\n" +
    "uniform vec4 u_lightPosition[2];\n" +
    "uniform vec3 u_ka;\n" +
    "uniform vec3 u_kd;\n" +
    "uniform vec3 u_ks;\n" +
    "uniform float u_materialShininess;\n" +
    "uniform int u_lightingEnabled;\n" +
    "out vec3 fong_ads_light;\n" +
    "void main(void)\n" +
    "{\n" +
    "if(u_lightingEnabled == 1)\n" +
    "{\n" +
    "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
    "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
    "vec3 transformedNormals = normalize(normalMatrix * a_normal);\n" +
    "vec3 viewerVector = normalize(-eyeCoordinates.xyz);\n" +
    "vec3 ambient[2];\n" +
    "vec3 lightDirection[2];\n" +
    "vec3 diffuse[2];\n" +
    "vec3 specular[2];\n" +
    "vec3 reflectionVector[2];" +
    "for(int i=0;i<2;i++)\n" +
    "{\n" +
    "ambient[i] = u_la[i] * u_ka;\n" +
    "lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);\n" +
    "diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i], transformedNormals), 0.0);\n" +
    "reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n" +
    "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininess);\n" +
    "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" +
    "}\n" +
    "}\n" +
    "else\n" +
    "{\n" +
    "fong_ads_light = vec3(1.0, 1.0, 1.0);\n" +
    "}\n" +
    "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" +
    "}\n";

	var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);
	gl.compileShader(vertexShaderObject);

	if(gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(vertexShaderObject);
		if(error.length > 0)
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
		"precision highp int;" +
		"in vec3 fong_ads_light;" +
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"FragColor = vec4(fong_ads_light,1.0);" +
		"}";

	var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);
	gl.compileShader(fragmentShaderObject);

	if(gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(fragmentShaderObject);
		if(error.length > 0)
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
	gl.bindAttribLocation(shaderProgramObject, webGLMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
	// Shader program linking
	gl.linkProgram(shaderProgramObject);
	if(gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObject);
		if(error.length > 0)
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

	laUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_la[0]");
	ldUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_ld[0]");
	lsUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_ls[0]");
	lightPositionUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_lightPosition[0]");

	laUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_la[1]");
	ldUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_ld[1]");
	lsUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_ls[1]");
	lightPositionUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_lightPosition[1]");

	kaUniform = gl.getUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = gl.getUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = gl.getUniformLocation(shaderProgramObject, "u_ks");
	materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "u_materialShininess");

	lightingEnableUniform = gl.getUniformLocation(shaderProgramObject, "u_lightingEnabled");

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

	var pyramidNormals = new Float32Array
		([	
			0.0, 0.447214, 0.894427,// front-top
			0.0, 0.447214, 0.894427,// front-left
			0.0, 0.447214, 0.894427,// front-right
	
			0.894427, 0.447214, 0.0, // right-top
			0.894427, 0.447214, 0.0, // right-left
			0.894427, 0.447214, 0.0, // right-right
	
			0.0, 0.447214, -0.894427, // back-top
			0.0, 0.447214, -0.894427, // back-left
			0.0, 0.447214, -0.894427, // back-right
	
			-0.894427, 0.447214, 0.0, // left-top
			-0.894427, 0.447214, 0.0, // left-left
			-0.894427, 0.447214, 0.0 // left-right
		]);

	// Vao and vbo code
	vao_pyramid = gl.createVertexArray();
	gl.bindVertexArray(vao_pyramid);
	// Position
	vbo_pyramid_position = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_pyramid_position);
	gl.bufferData(gl.ARRAY_BUFFER, pyramidVertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);

	vbo_pyramid_normal = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_pyramid_normal);
	gl.bufferData(gl.ARRAY_BUFFER, pyramidNormals, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_NORMAL);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);

	gl.bindVertexArray(null);

	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	lightAmbient[0] = [0.0, 0.0, 0.0];
	lightDiffuse[0] = [1.0, 0.0, 0.0];
	lightSpecular[0] = [1.0, 0.0, 0.0];
	lightPosition[0] = [-2.0, 0.0, 0.0, 1.0];

	lightAmbient[1] = [0.0, 0.0, 0.0];
	lightDiffuse[1] = [0.0, 0.0, 1.0];
	lightSpecular[1] = [0.0, 0.0, 1.0];
	lightPosition[1] = [2.0, 0.0, 0.0, 1.0];

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
	var viewMatrix = mat4.create();
	var translationMatrix = mat4.create();
	var rotationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);
	mat4.rotateY(rotationMatrix, rotationMatrix, anglePyramid);
	mat4.multiply(modelMatrix, translationMatrix, rotationMatrix);

	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
	gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

	// Sending Light Related Uniforms
	if(bLight == true)
	{
		gl.uniform1i(lightingEnableUniform, 1);
		gl.uniform3fv(kaUniform, materialAmbient);
		gl.uniform3fv(kdUniform, materialDiffuse);
		gl.uniform3fv(ksUniform, materialSpecular);
		gl.uniform1f(materialShininessUniform, materialShininess);

		for(var i = 0; i < 2; i++){
			gl.uniform3fv(laUniform, lightAmbient[i]);
			gl.uniform3fv(ldUniform, lightDiffuse[i]);
			gl.uniform3fv(lsUniform, lightSpecular[i]);
			gl.uniform4fv(lightPositionUniform, lightPosition[i]);
		}

	}
	else
	{
		 gl.uniform1i(lightingEnableUniform, 0);
	}
	gl.bindVertexArray(vao_pyramid);

	gl.drawArrays(gl.TRIANGLES, 0, 12);

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
		case 76:
			if(bLight == false){
				bLight = true;
			}
			else{
				bLight = false;
			}
			break;
	}
}

// Mouse event listener
function mouseDown(){
	// Code
}

function uninitialize(){
	// Code
	if(vbo_pyramid_normal){
		gl.deleteBuffer(vbo_pyramid_normal);
		vbo_pyramid_normal = null;
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
