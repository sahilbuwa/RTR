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
var vao_cube;
var vbo_cube;
var angleCube = 0.0;

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;

var laUniform;
var ldUniform;
var lsUniform;
var lightPositionUniform;

var kaUniform;
var kdUniform;
var ksUniform;
var materialShininessUniform;

var lightingEnabledUniform;

var bLight = false;

var lightAmbient = [0.1, 0.1, 0.1];
var lightDiffuse = [1.0, 1.0, 1.0];
var lightSpecular = [1.0, 1.0, 1.0];
var lightPosition = [100.0, 100.0, 100.0, 1.0];

var materialAmbient = [0.1, 0.1, 0.1];
var materialDiffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShininess = 50.0;

var textureSamplerUniform;
var texture_marble;

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

	texture_marble = gl.createTexture();
	texture_marble.image = new Image();
	texture_marble.image.src = "marble.png";
	texture_marble.image.onload = function()
	{
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
		gl.bindTexture(gl.TEXTURE_2D, texture_marble);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST); // Nearest is all browser compatible
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture_marble.image);
		gl.generateMipmap(gl.TEXTURE_2D);
		gl.bindTexture(gl.TEXTURE_2D, null);
	};

	// Vertex Shader
	var vertexShaderSourceCode = 
	"#version 300 es" +
	"\n" +
	"precision highp float;" +
	"precision highp int;" +
	"in vec4 a_position;\n" +
	"in vec3 a_normal;\n" +
	"in vec2 a_texcoord;\n" +
	"in vec4 a_color;\n" +
	"uniform mat4 u_modelMatrix;\n" +
	"uniform mat4 u_viewMatrix;\n" +
	"uniform mat4 u_projectionMatrix;\n" +
	"uniform vec4 u_lightPosition;\n" +
	"uniform int u_lightingEnabled;\n" +
	"out vec3 transformedNormals;\n" +
	"out vec3 lightDirection;\n" +
	"out vec3 viewerVector;\n" +
	"out vec2 a_texcoord_out;\n" +
	"out vec4 a_color_out;\n" +
	"void main(void)\n" +
	"{\n" +
	"if(u_lightingEnabled == 1)\n" +
	"{\n" +
	"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
	"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
	"transformedNormals = normalMatrix * a_normal;\n" +
	"lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;\n" +
	"viewerVector = -eyeCoordinates.xyz;\n" +
	"}\n" +
	"a_texcoord_out = a_texcoord;\n" +
	"a_color_out = a_color;\n" +
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
		"in vec4 a_color_out;" +
		"in vec2 a_texcoord_out;\n" +
		"in vec3 transformedNormals;\n" +
		"in vec3 lightDirection;\n" +
		"in vec3 viewerVector;\n" +
		"uniform sampler2D u_textureSampler;\n" +
		"uniform vec3 u_la;\n" +
		"uniform vec3 u_ld;\n" +
		"uniform vec3 u_ls;\n" +
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
		"vec3 ambient = u_la * u_ka;\n" +
		"vec3 normalized_transformedNormals = normalize(transformedNormals);\n" +
		"vec3 normalized_lightDirection = normalize(lightDirection);\n" +
		"vec3 diffuse = u_ld * u_kd * max(dot(normalized_lightDirection, normalized_transformedNormals), 0.0);\n" +
		"vec3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformedNormals);\n" +
		"vec3 normalized_viewerVector = normalize(viewerVector);\n" +
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewerVector), 0.0), u_materialShininess);\n" +
		"fong_ads_light = vec4(ambient + diffuse + specular, 1.0);\n" +
		"}\n" +
		"else\n" +
		"{\n" +
		"fong_ads_light = vec4(1.0, 1.0, 1.0, 1.0);\n" +
		"}\n" +
		"vec4 tex = texture(u_textureSampler, a_texcoord_out);\n" +
		"FragColor = fong_ads_light * tex * a_color_out;\n" +
		"}\n";

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
	gl.bindAttribLocation(shaderProgramObject, webGLMacros.SAB_ATTRIBUTE_COLOR, "a_color");
	gl.bindAttribLocation(shaderProgramObject, webGLMacros.SAB_ATTRIBUTE_TEXTURE0, "a_texcoord");
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

	laUniform = gl.getUniformLocation(shaderProgramObject, "u_la");
	ldUniform = gl.getUniformLocation(shaderProgramObject, "u_ld");
	lsUniform = gl.getUniformLocation(shaderProgramObject, "u_ls");
	lightPositionUniform = gl.getUniformLocation(shaderProgramObject, "u_lightPosition");

	kaUniform = gl.getUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = gl.getUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = gl.getUniformLocation(shaderProgramObject, "u_ks");
	materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "u_materialShininess");

	lightingEnabledUniform = gl.getUniformLocation(shaderProgramObject, "u_lightingEnabled");
	textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "u_textureSampler");

	// Declaration of vertex data arrays
	var cubepcnt = new Float32Array
		([	
		//front             //color-red         //normal-front      //texture-front
		1.0, 1.0, 1.0,   1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   1.0, 0.0,
		-1.0, 1.0, 1.0,  1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0,
		-1.0, -1.0, 1.0, 1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   0.0, 1.0,
		1.0, -1.0, 1.0,  1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   1.0, 1.0,

		//right             //color-green       //normal-right      //texture-right
		1.0, 1.0, -1.0,  0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0,
		1.0, 1.0, 1.0,   0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   0.0, 0.0,
		1.0, -1.0, 1.0,  0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   0.0, 1.0,
		1.0, -1.0, -1.0, 0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   1.0, 1.0,

		//back              //color-blue        //normal-back       //texture-back
		-1.0, 1.0, -1.0, 0.0, 0.0, 1.0,   0.0, 0.0, -1.0,  1.0, 0.0,
		1.0, 1.0, -1.0,  0.0, 0.0, 1.0,   0.0, 0.0, -1.0,  0.0, 0.0,
		1.0, -1.0, -1.0, 0.0, 0.0, 1.0,   0.0, 0.0, -1.0,  0.0, 1.0,
		-1.0, -1.0, -1.0,0.0, 0.0, 1.0,   0.0, 0.0, -1.0,  1.0, 1.0,

		//left              //color-cyan        //normal-left       //texture-back
		-1.0, 1.0, 1.0,  0.0, 1.0, 1.0,   -1.0, 0.0, 0.0,  1.0, 0.0,
		-1.0, 1.0, -1.0, 0.0, 1.0, 1.0,   -1.0, 0.0, 0.0,  0.0, 0.0,
		-1.0, -1.0, -1.0,0.0, 1.0, 1.0,   -1.0, 0.0, 0.0,  0.0, 1.0,
		-1.0, -1.0, 1.0, 0.0, 1.0, 1.0,   -1.0, 0.0, 0.0,  1.0, 1.0,

		//top               //color-magenta     //normal-top        //texture-top
		1.0, 1.0, -1.0,  1.0, 0.0, 1.0,   0.0, 1.0, 0.0,   1.0, 0.0,
		-1.0, 1.0, -1.0, 1.0, 0.0, 1.0,   0.0, 1.0, 0.0,   0.0, 0.0,
		-1.0, 1.0, 1.0,  1.0, 0.0, 1.0,   0.0, 1.0, 0.0,   0.0, 1.0,
		1.0, 1.0, 1.0,   1.0, 0.0, 1.0,   0.0, 1.0, 0.0,   1.0, 1.0,

		//bottom            //color-yellow      //normal-bottom     //texture-bottom
		1.0, -1.0, -1.0, 1.0, 1.0, 0.0,   0.0, -1.0, 0.0,  1.0, 0.0,
		-1.0, -1.0, -1.0,1.0, 1.0, 0.0,   0.0, -1.0, 0.0,  0.0, 0.0,
		-1.0, -1.0, 1.0, 1.0, 1.0, 0.0,   0.0, -1.0, 0.0,  0.0, 1.0,
		1.0, -1.0, 1.0,  1.0, 1.0, 0.0,   0.0, -1.0, 0.0,  1.0, 1.0

		]);

	// Vao and vbo code
	vao_cube = gl.createVertexArray();
	gl.bindVertexArray(vao_cube);
	// Position
	vbo_cube = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_cube);
	gl.bufferData(gl.ARRAY_BUFFER, cubepcnt, gl.STATIC_DRAW);
	// Position
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 11 * 4, 0);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_POSITION);

	// Color
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 11 * 4, 12);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_COLOR);
	
	// Normals
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 11 * 4, 24);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_NORMAL);

	// Texture
	gl.vertexAttribPointer(webGLMacros.SAB_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 11 *4, 36);
	gl.enableVertexAttribArray(webGLMacros.SAB_ATTRIBUTE_TEXTURE0);

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
	var viewMatrix = mat4.create();
	var translationMatrix = mat4.create();
	var rotationMatrix = mat4.create();
	var rotationMatrixX = mat4.create();
	var rotationMatrixY = mat4.create();
	var rotationMatrixZ = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);
	mat4.rotateX(rotationMatrixX, rotationMatrixX, angleCube);
	mat4.rotateY(rotationMatrixY, rotationMatrixY, angleCube);
	mat4.rotateZ(rotationMatrixZ, rotationMatrixZ, angleCube);
	mat4.multiply(rotationMatrix, rotationMatrixX, rotationMatrixY);
	mat4.multiply(rotationMatrix, rotationMatrix, rotationMatrixZ);
	mat4.multiply(modelMatrix, translationMatrix, rotationMatrix);

	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
	gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

		// Sending Light Related Uniforms
	if(bLight == true)
	{
		gl.uniform1i(lightingEnabledUniform, 1);

		gl.uniform3fv(laUniform, lightAmbient);
		gl.uniform3fv(ldUniform, lightDiffuse);
		gl.uniform3fv(lsUniform, lightSpecular);
		gl.uniform4fv(lightPositionUniform, lightPosition);

		gl.uniform3fv(kaUniform, materialAmbient);
		gl.uniform3fv(kdUniform, materialDiffuse);
		gl.uniform3fv(ksUniform, materialSpecular);
		gl.uniform1f(materialShininessUniform, materialShininess);
	}
	else
	{
		 gl.uniform1i(lightingEnabledUniform, 0);
	}

	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, texture_marble);
	gl.uniform1i(textureSamplerUniform, 0);

	gl.bindVertexArray(vao_cube);

	gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
	gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);

	gl.bindVertexArray(null);
	gl.bindTexture(gl.TEXTURE_2D, null);

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
	if(texture_marble)
	{
		gl.deleteTextures(1, texture_marble);
		texture_marble = 0;
	}
	if(vbo_cube){
		gl.deleteBuffer(vbo_cube);
		vbo_cube = null;
	}
	if(vao_cube){
		gl.deleteVertexArray(vao_cube);
		vao_cube = null;
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
