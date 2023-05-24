// Global variables
var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;

const FBO_WIDTH = 1024;
const FBO_HEIGHT = 1024;
const webGLMacros = 
{
	SAB_ATTRIBUTE_POSITION : 0,
    SAB_ATTRIBUTE_COLOR : 1,
    SAB_ATTRIBUTE_NORMAL : 2,
    SAB_ATTRIBUTE_TEXTURE0 : 3
};
var mvpMatrixUniform;
var textureSamplerUniform;

var shaderProgramObject;
var vao_cube;
var vbo_cube_position;
var vbo_cube_texcoord;
var angleCube = 0.0;

var perspectiveProjectionMatrix;

// FBO related global variables
var fbo;
var rbo;
var fbo_texture;
var bFBOResult = false;

// Sphere related variables
var sphere = null;
var shaderProgramObject_sphere;

var modelMatrixUniform_sphere;
var viewMatrixUniform_sphere;
var projectionMatrixUniform_sphere;

var perspectiveProjectionMatrix_sphere;

var laUniformF = Array(3);
var ldUniformF = Array(3);
var lsUniformF = Array(3);
var lightPositionUniformF = Array(3);

var kaUniformF;
var kdUniformF;
var ksUniformF;
var materialShininessUniformF;

var lightingEnableUniformF;

var bLight = false;

var lightAmbient = Array(3);
var lightDiffuse = Array(3);
var lightSpecular = Array(3);
var lightPosition = Array(3);

var materialAmbient = [0.0, 0.0, 0.0];
var materialDiffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShininess = 50.0;
var redLightAngle = 0.0,
        greenLightAngle = 0.0,
        blueLightAngle = 0.0;

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
		"in vec2 a_texcoord;" +
		"uniform mat4 u_mvpMatrix;" +
		"out vec2 a_texcoord_out;" +
		"void main(void)" +
		"{" +
		"gl_Position = u_mvpMatrix * a_position;" +
		"a_texcoord_out = a_texcoord;" +
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
		"in vec2 a_texcoord_out;" +
		"uniform sampler2D u_textureSampler;" +
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"FragColor = texture(u_textureSampler, a_texcoord_out);" +
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
	gl.bindAttribLocation(shaderProgramObject, webGLMacros.SAB_ATTRIBUTE_TEXTURE0, "a_texcoord");
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
	textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "u_textureSampler");
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

	var cubeTexcoords = new Float32Array
		([	
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
	
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
	
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			// Bottom
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			1.0, 0.0,
	
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
	
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0
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

	vbo_cube_texcoord = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_cube_texcoord);
	gl.bufferData(gl.ARRAY_BUFFER, cubeTexcoords, gl.STATIC_DRAW);
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_TEXTURE0);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);

	gl.bindVertexArray(null);

	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	// Clear the screen with white color
	gl.clearColor(1.0, 1.0, 1.0, 1.0);

	// Perspective projection matrix
	perspectiveProjectionMatrix =  mat4.create();
	// FBO initialize code
	bFBOResult = createFBO();
	var retVal;
	if(bFBOResult == true)
	{
		retVal = initialize_sphere();
	}
	else
	{
		console.log("Create FBO failed.\n");
	}
	
}

function createFBO(){
	// Local variable declarations
	var maxRenderbufferSize = 0;
	// Code
	maxRenderbufferSize = gl.getParameter(gl.MAX_RENDERBUFFER_SIZE);
	if(maxRenderbufferSize < FBO_WIDTH || maxRenderbufferSize < FBO_HEIGHT)
	{
		console.log("Insufficient renderbuffer size.\n");
	}

	fbo = gl.createFramebuffer();
	gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);

	rbo = gl.createRenderbuffer();
	gl.bindRenderbuffer(gl.RENDERBUFFER, rbo);

	gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, FBO_WIDTH, FBO_HEIGHT);

	fbo_texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, fbo_texture);

	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, FBO_WIDTH, FBO_HEIGHT, 0, gl.RGB, gl.UNSIGNED_SHORT_5_6_5, null);

	gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, fbo_texture, 0);

	gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, gl.RENDERBUFFER, rbo);

	var result = gl.checkFramebufferStatus(gl.FRAMEBUFFER);
	if(result != gl.FRAMEBUFFER_COMPLETE)
	{
		console.log("Framebuffer not complete.\n");
		return false;
	}
	gl.bindFramebuffer(gl.FRAMEBUFFER, null);
	return true;
}

function initialize_sphere(){
	// Code
	// PerFragmentShader
	// Vertex Shader
	var vertexShaderSourceCodeF = 
		"#version 300 es" +
		"\n" +
		"precision highp float;\n" +
		"precision highp int;\n" +
		"in vec4 a_position;\n" +
		"in vec3 a_normal;\n" +
		"uniform mat4 u_modelMatrix;\n" +
		"uniform mat4 u_viewMatrix;\n" +
		"uniform mat4 u_projectionMatrix;\n" +
		"uniform int u_lightingEnabled;\n" +
		"out vec3 transformedNormals;\n" +
		"out vec3 viewerVector;\n" +
		"void main(void)\n" +
		"{\n" +
		"if(u_lightingEnabled == 1)\n" +
		"{\n" +
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
		"transformedNormals = normalMatrix * a_normal;\n" +
		"viewerVector = -eyeCoordinates.xyz;\n" +
		"}\n" +
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" +
		"}\n";

	var vertexShaderObjectF = gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vertexShaderObjectF, vertexShaderSourceCodeF);
	gl.compileShader(vertexShaderObjectF);

	if(gl.getShaderParameter(vertexShaderObjectF, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(vertexShaderObjectF);
		if(error.length > 0)
		{
			var errorString = "Vertex shader error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Fragment Shader
	var fragmentShaderSourceCodeF = 
		"#version 300 es" +
		"\n" +
		"precision highp float;\n" +
		"precision highp int;\n" +
		"in vec3 transformedNormals;\n" +
		"in vec3 viewerVector;\n" +
		"uniform vec3 u_la[3];\n" +
		"uniform vec3 u_ld[3];\n" +
		"uniform vec3 u_ls[3];\n" +
		"uniform vec4 u_lightPosition[3];\n" +
		"uniform vec3 u_ka;\n" +
		"uniform vec3 u_kd;\n" +
		"uniform vec3 u_ks;\n" +
		"uniform float u_materialShininess;\n" +
		"uniform int u_lightingEnabled;\n" +
		"out vec4 FragColor;\n" +
		"void main(void)\n" +
		"{\n" +
		"vec4 fong_ads_light;" +
		"if(u_lightingEnabled == 1)\n" +
		"{\n" +
		"vec3 ambient[3];\n" +
		"vec3 diffuse[3];\n" +
		"vec3 specular[3];\n" +
		"vec3 normalized_lightDirection[3];\n" +
		"vec3 reflectionVector[3];\n" +
		"vec3 normalized_transformedNormals = normalize(transformedNormals);\n" +
		"vec3 normalized_viewerVector = normalize(viewerVector);\n" +
		"for(int i=0;i<3;i++)\n" +
		"{\n" +
		"ambient[i] = u_la[i] * u_ka;\n" +
		"normalized_lightDirection[i] = normalize(vec3(u_lightPosition[i]));\n" +
		"diffuse[i] = u_ld[i] * u_kd * max(dot(normalized_lightDirection[i], normalized_transformedNormals), 0.0);\n" +
		"reflectionVector[i] = reflect(-normalized_lightDirection[i], normalized_transformedNormals);\n" +
		"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], normalized_viewerVector), 0.0), u_materialShininess);\n" +
		"fong_ads_light = fong_ads_light + vec4(ambient[i] + diffuse[i] + specular[i], 1.0);\n" +
		"}\n" +
		"}\n" +
		"else\n" +
		"{\n" +
		"fong_ads_light = vec4(1.0, 1.0, 1.0, 1.0);\n" +
		"}\n" +
		"FragColor = fong_ads_light;\n" +
		"}\n";

	var fragmentShaderObjectF = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fragmentShaderObjectF, fragmentShaderSourceCodeF);
	gl.compileShader(fragmentShaderObjectF);

	if(gl.getShaderParameter(fragmentShaderObjectF, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(fragmentShaderObjectF);
		if(error.length > 0)
		{
			var errorString = "Fragment shader error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Shader Program
	shaderProgramObject_sphere = gl.createProgram();
	gl.attachShader(shaderProgramObject_sphere, vertexShaderObjectF);
	gl.attachShader(shaderProgramObject_sphere, fragmentShaderObjectF);
	// Pre linking shader attribute binding
	gl.bindAttribLocation(shaderProgramObject_sphere, webGLMacros.SAB_ATTRIBUTE_POSITION, "a_position");
	gl.bindAttribLocation(shaderProgramObject_sphere, webGLMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
	// Shader program linking
	gl.linkProgram(shaderProgramObject_sphere);
	if(gl.getProgramParameter(shaderProgramObject_sphere, gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObject_sphere);
		if(error.length > 0)
		{F
			var errorString = "Shader program error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Uniform locations
	modelMatrixUniform_sphere = gl.getUniformLocation(shaderProgramObject_sphere, "u_modelMatrix");
	viewMatrixUniform_sphere = gl.getUniformLocation(shaderProgramObject_sphere, "u_viewMatrix");
	projectionMatrixUniform_sphere = gl.getUniformLocation(shaderProgramObject_sphere, "u_projectionMatrix");

    laUniformF[0] = gl.getUniformLocation(shaderProgramObject_sphere, "u_la[0]");
    ldUniformF[0] = gl.getUniformLocation(shaderProgramObject_sphere, "u_ld[0]");
    lsUniformF[0] = gl.getUniformLocation(shaderProgramObject_sphere, "u_ls[0]");
    lightPositionUniformF[0] = gl.getUniformLocation(shaderProgramObject_sphere, "u_lightPosition[0]");

    laUniformF[1] = gl.getUniformLocation(shaderProgramObject_sphere, "u_la[1]");
    ldUniformF[1] = gl.getUniformLocation(shaderProgramObject_sphere, "u_ld[1]");
    lsUniformF[1] = gl.getUniformLocation(shaderProgramObject_sphere, "u_ls[1]");
    lightPositionUniformF[1] = gl.getUniformLocation(shaderProgramObject_sphere, "u_lightPosition[1]");

    laUniformF[2] = gl.getUniformLocation(shaderProgramObject_sphere, "u_la[2]");
    ldUniformF[2] = gl.getUniformLocation(shaderProgramObject_sphere, "u_ld[2]");
    lsUniformF[2] = gl.getUniformLocation(shaderProgramObject_sphere, "u_ls[2]");
    lightPositionUniformF[2] = gl.getUniformLocation(shaderProgramObject_sphere, "u_lightPosition[2]");

	kaUniformF = gl.getUniformLocation(shaderProgramObject_sphere, "u_ka");
	kdUniformF = gl.getUniformLocation(shaderProgramObject_sphere, "u_kd");
	ksUniformF = gl.getUniformLocation(shaderProgramObject_sphere, "u_ks");
	materialShininessUniformF = gl.getUniformLocation(shaderProgramObject_sphere, "u_materialShininess");

	lightingEnableUniformF = gl.getUniformLocation(shaderProgramObject_sphere, "u_lightingEnabled");

	// Declaration of vertex data arrays
	sphere = new Mesh();
	makeSphere(sphere, 2.0, 40, 40); 

	// Clear the screen with black color
	gl.clearColor(0.0, 0.0, 0.0, 1.0);
	
	lightAmbient[0] = [0.0, 0.0, 0.0];
	lightDiffuse[0] = [1.0, 0.0, 0.0];
	lightSpecular[0] = [1.0, 0.0, 0.0];
	lightPosition[0] = [0.0, 0.0, 0.0, 1.0];

	lightAmbient[1] = [0.0, 0.0, 0.0];
	lightDiffuse[1] = [0.0, 1.0, 0.0];
	lightSpecular[1] = [0.0, 1.0, 0.0];
	lightPosition[1] = [0.0, 0.0, 0.0, 1.0];

	lightAmbient[2] = [0.0, 0.0, 0.0];
	lightDiffuse[2] = [0.0, 0.0, 1.0];
	lightSpecular[2] = [0.0, 0.0, 1.0];
	lightPosition[2] = [0.0, 0.0, 0.0, 1.0];

	// Perspective projection matrix
	perspectiveProjectionMatrix_sphere =  mat4.create();

	// Warmup resize call
	resize_sphere(FBO_WIDTH, FBO_HEIGHT);
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
		canvas.height = 1;

	gl.viewport(0, 0, canvas.width, canvas.height);

	mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width)/parseFloat(canvas.height), 0.1, 100);
}

function resize_sphere(width, height){
	// Code
	if(height == 0)
		height = 1;

	gl.viewport(0, 0, width, height);

	mat4.perspective(perspectiveProjectionMatrix_sphere, 45.0, parseFloat(width)/parseFloat(height), 0.1, 100);
}

function degToRad(degrees){
	return degrees * Math.PI / 180.0;
}

function display(){
	// Code
	if(bFBOResult == true)
	{
		display_sphere();
	}
	// Screen background white kar
	gl.clearColor(1.0, 1.0, 1.0, 1.0);
	resize();

	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	// Use the shader program object
	gl.useProgram(shaderProgramObject);
	// Transformations Cube
	var modelMatrix = mat4.create();
	var modelViewProjectionMatrix = mat4.create();
	var translationMatrix = mat4.create();
	var rotationMatrix = mat4.create();
	var rotationMatrixX = mat4.create();
	var rotationMatrixY = mat4.create();
	var rotationMatrixZ = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -4.0]);
	mat4.rotateX(rotationMatrixX, rotationMatrixX, angleCube);
	mat4.rotateY(rotationMatrixY, rotationMatrixY, angleCube);
	mat4.rotateZ(rotationMatrixZ, rotationMatrixZ, angleCube);
	mat4.multiply(rotationMatrix, rotationMatrixX, rotationMatrixY);
	mat4.multiply(rotationMatrix, rotationMatrix, rotationMatrixZ);
	mat4.multiply(modelMatrix, translationMatrix, rotationMatrix);
	mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelMatrix);

	gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, fbo_texture);
	gl.uniform1i(textureSamplerUniform, 0);
	gl.bindVertexArray(vao_cube);

	gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);

	gl.bindTexture(gl.TEXTURE_2D, null);
	gl.bindVertexArray(null);

	gl.useProgram(null);

	// Update Call
	update();
	// Double buffering emulation
	requestAnimationFrame(display, canvas);
}

function update(){
	// Code
	angleCube = angleCube + 0.01;
	if(angleCube > 2 * Math.PI)
		angleCube -= 2 * Math.PI;

}

function display_sphere(){
	// Code
	// Bind framebuffer object
	gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);
	// FBO background black kar
	gl.clearColor(0.0, 0.0, 0.0, 1.0);
	resize_sphere(FBO_WIDTH, FBO_HEIGHT);

	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	// Transformations
	var modelMatrix = mat4.create();
	var viewMatrix = mat4.create();
	var translationMatrix = mat4.create();
	
	mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

    gl.useProgram(shaderProgramObject_sphere);  

	gl.uniformMatrix4fv(modelMatrixUniform_sphere, false, modelMatrix);
	gl.uniformMatrix4fv(viewMatrixUniform_sphere, false, viewMatrix);
	gl.uniformMatrix4fv(projectionMatrixUniform_sphere, false, perspectiveProjectionMatrix_sphere);
        
	if(bLight == true)
	{
		gl.uniform1i(lightingEnableUniformF, 1);
	
		for(var i = 0; i < 3; i++){
			gl.uniform3fv(laUniformF[i], lightAmbient[i]);
			gl.uniform3fv(ldUniformF[i], lightDiffuse[i]);
			gl.uniform3fv(lsUniformF[i], lightSpecular[i]);
			gl.uniform4fv(lightPositionUniformF[i], lightPosition[i]);
		}		
		gl.uniform3fv(kaUniformF, materialAmbient);
		gl.uniform3fv(kdUniformF, materialDiffuse);
		gl.uniform3fv(ksUniformF, materialSpecular);
		gl.uniform1f(materialShininessUniformF, materialShininess);
	}
	else
	{
		gl.uniform1i(lightingEnableUniformF, 0);
	}

	sphere.draw();

	gl.useProgram(null);
	gl.bindFramebuffer(gl.FRAMEBUFFER, null);
	// Update Call
	update_sphere();

}

function update_sphere(){
	// Code
	redLightAngle += 0.1;
    if(redLightAngle >= 360.0)
        redLightAngle -= 360.0;
    lightPosition[0] = [0.0, 10.0 * Math.cos(degToRad(redLightAngle)), 10.0 *  Math.sin(degToRad(redLightAngle)), 1.0];

    greenLightAngle += 0.1;
    if(greenLightAngle >= 360.0)
        greenLightAngle -= 360.0;
    lightPosition[1] = [10.0 *  Math.cos(degToRad(greenLightAngle)), 0.0, 10.0 * Math.sin(degToRad(greenLightAngle)), 1.0];

    blueLightAngle += 0.1;
    if(blueLightAngle >= 360.0)
        blueLightAngle -= 360.0;
    lightPosition[2] = [10.0 * Math.cos(degToRad(blueLightAngle)), 10.0 * Math.sin(degToRad(blueLightAngle)), 0.0, 1.0];
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
	uninitialize_sphere();
	if(vbo_cube_position){
		gl.deleteBuffer(vbo_cube_position);
		vbo_cube_position = null;
	}
	if(vao_cube){
		gl.deleteVertexArray(vao_cube);
		vao_cube = null;
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

function uninitialize_sphere(){
	// Code
	if(sphere){
		sphere.deallocate();
		sphere = null;
	}
	if(shaderProgramObject_sphere){
		gl.useProgram(shaderProgramObject_sphere);
		var shaderObjects = gl.getAttachedShaders(shaderProgramObject_sphere);
		for(let i = 0; i < shaderObjects.length; i++)
		{
			gl.detachShader(shaderProgramObject_sphere, shaderObjects[i]);
			gl.deleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}
		gl.useProgram(null);
		gl.deleteProgram(shaderProgramObject_sphere);
		shaderProgramObject_sphere = 0;
	}
}
