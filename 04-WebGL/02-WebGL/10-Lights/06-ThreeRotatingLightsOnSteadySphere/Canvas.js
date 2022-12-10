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

var laUniformV = Array(3);
var ldUniformV = Array(3);
var lsUniformV = Array(3);
var lightPositionUniformV = Array(3);

var kaUniformV;
var kdUniformV;
var ksUniformV;
var materialShininessUniformV;

var lightingEnableUniformV;

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
		"uniform vec3 u_la[3];\n" +
		"uniform vec3 u_ld[3];" +
		"uniform vec3 u_ls[3];" +
		"uniform vec4 u_lightPosition[3];" +
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
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
		"vec3 transformedNormals = normalize(normalMatrix * a_normal);\n" +
		"vec3 viewerVector = normalize(-eyeCoordinates.xyz);\n" +
		"vec3 ambient[3];\n" +
		"vec3 lightDirection[3];\n" +
		"vec3 diffuse[3];\n" +
		"vec3 specular[3];\n" +
		"vec3 reflectionVector[3];" +
		"for(int i=0;i<3;i++)\n" +
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

    laUniformV[0] = gl.getUniformLocation(shaderProgramObjectV, "u_la[0]");
    ldUniformV[0] = gl.getUniformLocation(shaderProgramObjectV, "u_ld[0]");
    lsUniformV[0] = gl.getUniformLocation(shaderProgramObjectV, "u_ls[0]");
    lightPositionUniformV[0] = gl.getUniformLocation(shaderProgramObjectV, "u_lightPosition[0]");

    laUniformV[1] = gl.getUniformLocation(shaderProgramObjectV, "u_la[1]");
    ldUniformV[1] = gl.getUniformLocation(shaderProgramObjectV, "u_ld[1]");
    lsUniformV[1] = gl.getUniformLocation(shaderProgramObjectV, "u_ls[1]");
    lightPositionUniformV[1] = gl.getUniformLocation(shaderProgramObjectV, "u_lightPosition[1]");

    laUniformV[2] = gl.getUniformLocation(shaderProgramObjectV, "u_la[2]");
    ldUniformV[2] = gl.getUniformLocation(shaderProgramObjectV, "u_ld[2]");
    lsUniformV[2] = gl.getUniformLocation(shaderProgramObjectV, "u_ls[2]");
    lightPositionUniformV[2] = gl.getUniformLocation(shaderProgramObjectV, "u_lightPosition[2]");

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

    laUniformF[0] = gl.getUniformLocation(shaderProgramObjectF, "u_la[0]");
    ldUniformF[0] = gl.getUniformLocation(shaderProgramObjectF, "u_ld[0]");
    lsUniformF[0] = gl.getUniformLocation(shaderProgramObjectF, "u_ls[0]");
    lightPositionUniformF[0] = gl.getUniformLocation(shaderProgramObjectF, "u_lightPosition[0]");

    laUniformF[1] = gl.getUniformLocation(shaderProgramObjectF, "u_la[1]");
    ldUniformF[1] = gl.getUniformLocation(shaderProgramObjectF, "u_ld[1]");
    lsUniformF[1] = gl.getUniformLocation(shaderProgramObjectF, "u_ls[1]");
    lightPositionUniformF[1] = gl.getUniformLocation(shaderProgramObjectF, "u_lightPosition[1]");

    laUniformF[2] = gl.getUniformLocation(shaderProgramObjectF, "u_la[2]");
    ldUniformF[2] = gl.getUniformLocation(shaderProgramObjectF, "u_ld[2]");
    lsUniformF[2] = gl.getUniformLocation(shaderProgramObjectF, "u_ls[2]");
    lightPositionUniformF[2] = gl.getUniformLocation(shaderProgramObjectF, "u_lightPosition[2]");

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

	// Transformations
	var modelMatrix = mat4.create();
	var viewMatrix = mat4.create();
	var translationMatrix = mat4.create();
	
	mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -4.0]);
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
	
			for(var i = 0; i < 3; i++){
				gl.uniform3fv(laUniformV[i], lightAmbient[i]);
				gl.uniform3fv(ldUniformV[i], lightDiffuse[i]);
				gl.uniform3fv(lsUniformV[i], lightSpecular[i]);
				gl.uniform4fv(lightPositionUniformV[i], lightPosition[i]);
			}	
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
