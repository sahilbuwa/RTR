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

var sphere = null;
var shaderProgramObjectF;
var shaderProgramObjectV;
var choosenProgramObject;

var modelMatrixUniformV;
var viewMatrixUniformV;
var projectionMatrixUniformV;

var modelMatrixUniformF;
var viewMatrixUniformF;
var projectionMatrixUniformF;

var perspectiveProjectionMatrix;

var laUniformV;
var ldUniformV;
var lsUniformV;
var lightPositionUniformV;

var kaUniformV;
var kdUniformV;
var ksUniformV;
var materialShininessUniformV;

var lightingEnableUniformV;

var laUniformF;
var ldUniformF;
var lsUniformF;
var lightPositionUniformF;

var kaUniformF;
var kdUniformF;
var ksUniformF;
var materialShininessUniformF;

var lightingEnableUniformF;

var bLight = false;

var lightAmbient = [0.1, 0.1, 0.1];
var lightDiffuse = [1.0, 1.0, 1.0];
var lightSpecular = [1.0, 1.0, 1.0];
var lightPosition = [100.0, 100.0, 100.0, 1.0];

var materialAmbient = [0.0, 0.0, 0.0];
var materialDiffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShininess = 50.0;

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

	// PerVertexShader
	// Vertex Shader
	var vertexShaderSourceCodeV = 
		"#version 300 es" +
		"\n" +
		"precision highp float;\n" +
		"precision highp int;\n" +
		"in vec4 a_position;\n" +
		"in vec3 a_normal;\n" +
		"uniform mat4 u_modelMatrix;\n" +
		"uniform mat4 u_viewMatrix;\n" +
		"uniform mat4 u_projectionMatrix;\n" +
		"uniform vec3 u_la;\n" +
		"uniform vec3 u_ld;" +
		"uniform vec3 u_ls;" +
		"uniform vec4 u_lightPosition;" +
		"uniform vec3 u_ka;" +
		"uniform vec3 u_kd;" +
		"uniform vec3 u_ks;" +
		"uniform float u_materialShininess;" +
		"uniform int u_lightingEnabled;" +
		"out vec3 fong_ads_light;" +
		"void main(void)" +
		"{" +
		"if(u_lightingEnabled == 1)" +
		"{" +
		"vec3 ambient = u_la * u_ka;" +
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
		"vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
		"vec3 lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" +
		"vec3 diffuse = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);" +
		"vec3 reflectionVector = reflect(-lightDirection, transformedNormals);" +
		"vec3 viewerVector = normalize(-eyeCoordinates.xyz);" +
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, viewerVector), 0.0), u_materialShininess);" +
		"fong_ads_light = ambient + diffuse + specular;" +
		"}" +
		"else" +
		"{" +
		"fong_ads_light = vec3(1.0, 1.0, 1.0);" +
		"}" +
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
		"}";

	var vertexShaderObjectV = gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vertexShaderObjectV, vertexShaderSourceCodeV);
	gl.compileShader(vertexShaderObjectV);

	if(gl.getShaderParameter(vertexShaderObjectV, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(vertexShaderObjectV);
		if(error.length > 0)
		{
			var errorString = "Vertex shader error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Fragment Shader
	var fragmentShaderSourceCodeV = 
		"#version 300 es" +
		"\n" +
		"precision highp float;\n" +
		"precision highp int;\n" +
		"in vec3 fong_ads_light;" +
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"FragColor = vec4(fong_ads_light,1.0);" +
		"}";

	var fragmentShaderObjectV = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fragmentShaderObjectV, fragmentShaderSourceCodeV);
	gl.compileShader(fragmentShaderObjectV);

	if(gl.getShaderParameter(fragmentShaderObjectV, gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(fragmentShaderObjectV);
		if(error.length > 0)
		{
			var errorString = "Fragment shader error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Shader Program
	shaderProgramObjectV = gl.createProgram();
	gl.attachShader(shaderProgramObjectV, vertexShaderObjectV);
	gl.attachShader(shaderProgramObjectV, fragmentShaderObjectV);
	// Pre linking shader attribute binding
	gl.bindAttribLocation(shaderProgramObjectV, webGLMacros.SAB_ATTRIBUTE_POSITION, "a_position");
	gl.bindAttribLocation(shaderProgramObjectV, webGLMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
	// Shader program linking
	gl.linkProgram(shaderProgramObjectV);
	if(gl.getProgramParameter(shaderProgramObjectV, gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObjectV);
		if(error.length > 0)
		{F
			var errorString = "Shader program error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Uniform locations
	modelMatrixUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_modelMatrix");
	viewMatrixUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_viewMatrix");
	projectionMatrixUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_projectionMatrix");

	laUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_la");
	ldUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_ld");
	lsUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_ls");
	lightPositionUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_lightPosition");

	kaUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_ka");
	kdUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_kd");
	ksUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_ks");
	materialShininessUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_materialShininess");

	lightingEnableUniformV = gl.getUniformLocation(shaderProgramObjectV, "u_lightingEnabled");
	
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
		"uniform vec4 u_lightPosition;\n" +
		"uniform int u_lightingEnabled;\n" +
		"out vec3 transformedNormals;\n" +
		"out vec3 lightDirection;\n" +
		"out vec3 viewerVector;\n" +
		"void main(void)\n" +
		"{\n" +
		"if(u_lightingEnabled == 1)" +
		"{\n" +
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
		"transformedNormals = normalMatrix * a_normal;\n" +
		"lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;\n" +
		"viewerVector = -eyeCoordinates.xyz;\n" +
		"}\n" +
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" +
		"}";

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
		"in vec3 lightDirection;\n" +
		"in vec3 viewerVector;\n" +
		"uniform vec3 u_la;\n" +
		"uniform vec3 u_ld;\n" +
		"uniform vec3 u_ls;\n" +
		"uniform vec3 u_ka;\n" +
		"uniform vec3 u_kd;\n" +
		"uniform vec3 u_ks;\n" +
		"uniform float u_materialShininess;\n" +
		"uniform int u_lightingEnabled;\n" +
		"out vec4 FragColor;\n" +
		"void main(void)" +
		"{" +
		"vec4 fong_ads_light;\n" +
		"if(u_lightingEnabled == 1)\n" +
		"{" +
		"vec3 ambient = u_la * u_ka;\n" +
		"vec3 normalized_transformedNormals = normalize(transformedNormals);\n" +
		"vec3 normalized_lightDirection = normalize(lightDirection);\n" +
		"vec3 diffuse = u_ld * u_kd * max(dot(normalized_lightDirection, normalized_transformedNormals), 0.0);\n" +
		"vec3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformedNormals);\n" +
		"vec3 normalized_viewerVector = normalize(viewerVector);\n" +
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewerVector), 0.0), u_materialShininess);\n" +
		"fong_ads_light = vec4(ambient + diffuse + specular, 1.0);\n" +
		"}\n" +
		"else" +
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
	shaderProgramObjectF = gl.createProgram();
	gl.attachShader(shaderProgramObjectF, vertexShaderObjectF);
	gl.attachShader(shaderProgramObjectF, fragmentShaderObjectF);
	// Pre linking shader attribute binding
	gl.bindAttribLocation(shaderProgramObjectF, webGLMacros.SAB_ATTRIBUTE_POSITION, "a_position");
	gl.bindAttribLocation(shaderProgramObjectF, webGLMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
	// Shader program linking
	gl.linkProgram(shaderProgramObjectF);
	if(gl.getProgramParameter(shaderProgramObjectF, gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObjectF);
		if(error.length > 0)
		{F
			var errorString = "Shader program error : "+ error;
			alert(errorString);
			uninitialize();
		}
	}

	// Uniform locations
	modelMatrixUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_modelMatrix");
	viewMatrixUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_viewMatrix");
	projectionMatrixUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_projectionMatrix");

	laUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_la");
	ldUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_ld");
	lsUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_ls");
	lightPositionUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_lightPosition");

	kaUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_ka");
	kdUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_kd");
	ksUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_ks");
	materialShininessUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_materialShininess");

	lightingEnableUniformF = gl.getUniformLocation(shaderProgramObjectF, "u_lightingEnabled");

	// Default initialization of toggled state
	choosenProgramObject = 'v';
	// Declaration of vertex data arrays
	sphere = new Mesh();
	makeSphere(sphere, 2.0, 40, 40); 

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

function display(){
	// Code
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	// Transformations
	var modelMatrix = mat4.create();
	var viewMatrix = mat4.create();
	var translationMatrix = mat4.create();
	
	mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

	    // Use the Shader Program Object
    if(choosenProgramObject == 'v')
    {
        gl.useProgram(shaderProgramObjectV);  

		gl.uniformMatrix4fv(modelMatrixUniformV, false, modelMatrix);
		gl.uniformMatrix4fv(viewMatrixUniformV, false, viewMatrix);
		gl.uniformMatrix4fv(projectionMatrixUniformV, false, perspectiveProjectionMatrix);
        
		if(bLight == true)
		{
			gl.uniform1i(lightingEnableUniformV, 1);
	
			gl.uniform3fv(laUniformV, lightAmbient);
			gl.uniform3fv(ldUniformV, lightDiffuse);
			gl.uniform3fv(lsUniformV, lightSpecular);
			gl.uniform4fv(lightPositionUniformV, lightPosition);
	
			gl.uniform3fv(kaUniformV, materialAmbient);
			gl.uniform3fv(kdUniformV, materialDiffuse);
			gl.uniform3fv(ksUniformV, materialSpecular);
			gl.uniform1f(materialShininessUniformV, materialShininess);
		}
		else
		{
			 gl.uniform1i(lightingEnableUniformV, 0);
		}
    }
    else
    {
        gl.useProgram(shaderProgramObjectF);  

		gl.uniformMatrix4fv(modelMatrixUniformF, false, modelMatrix);
		gl.uniformMatrix4fv(viewMatrixUniformF, false, viewMatrix);
		gl.uniformMatrix4fv(projectionMatrixUniformF, false, perspectiveProjectionMatrix);
        
		if(bLight == true)
		{
			gl.uniform1i(lightingEnableUniformF, 1);
	
			gl.uniform3fv(laUniformF, lightAmbient);
			gl.uniform3fv(ldUniformF, lightDiffuse);
			gl.uniform3fv(lsUniformF, lightSpecular);
			gl.uniform4fv(lightPositionUniformF, lightPosition);
	
			gl.uniform3fv(kaUniformF, materialAmbient);
			gl.uniform3fv(kdUniformF, materialDiffuse);
			gl.uniform3fv(ksUniformF, materialSpecular);
			gl.uniform1f(materialShininessUniformF, materialShininess);
		}
		else
		{
			 gl.uniform1i(lightingEnableUniformF, 0);
		}
    }

	sphere.draw();

	gl.useProgram(null);
	// Update Call
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
		case 68:
			toggleFullscreen();
			break;
		case 69:
			uninitialize();
			// Request to browser to close but not all follow.
			window.close();
			break;
		case 70:
			choosenProgramObject = 'f';
			break;
		case 76:
			if(bLight == false){
				bLight = true;
			}
			else{
				bLight = false;
			}
			break;
        case 86:
            choosenProgramObject = 'v';
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
