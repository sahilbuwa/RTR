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
var shaderProgramObject;
var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;

var perspectiveProjectionMatrix;

var laUniform;
var ldUniform;
var lsUniform;
var lightPositionUniform;

var kaUniform;
var kdUniform;
var ksUniform;
var materialShininessUniform;

var lightingEnableUniform;

var bLight = false;

var lightAmbient = [0.0, 0.0, 0.0];
var lightDiffuse = [1.0, 1.0, 1.0];
var lightSpecular = [1.0, 1.0, 1.0];
var lightPosition = [0.0, 0.0, 0.0, 1.0];

var materialAmbient = [];
var materialDiffuse = [];
var materialSpecular = [];
var materialShininess = 0.0;

var angleForXRotation = 0.0;
var angleForYRotation = 0.0;
var angleForZRotation = 0.0;

var keyPressed = 0;

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

	laUniform = gl.getUniformLocation(shaderProgramObject, "u_la");
	ldUniform = gl.getUniformLocation(shaderProgramObject, "u_ld");
	lsUniform = gl.getUniformLocation(shaderProgramObject, "u_ls");
	lightPositionUniform = gl.getUniformLocation(shaderProgramObject, "u_lightPosition");

	kaUniform = gl.getUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = gl.getUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = gl.getUniformLocation(shaderProgramObject, "u_ks");
	materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "u_materialShininess");

	lightingEnableUniform = gl.getUniformLocation(shaderProgramObject, "u_lightingEnabled");

	// Declaration of vertex data arrays
	sphere = new Mesh();
	makeSphere(sphere, 0.5, 30, 30); 

	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	// Clear the screen with gray color
	gl.clearColor(0.5, 0.5, 0.5, 1.0);

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

	// Transformations
	var modelMatrix = mat4.create();
	var viewMatrix = mat4.create();
	var translationMatrix = mat4.create();
	mat4.lookAt(viewMatrix, [-1.0, 0.0, 0.0], [0.0, 0.0, 50.0], [0.0, 1.0, 0.0]);

	gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
	gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

	// Sending Light Related Uniforms
	if(bLight == true)
	{
		gl.uniform1i(lightingEnableUniform, 1);

		gl.uniform3fv(laUniform, lightAmbient);
		gl.uniform3fv(ldUniform, lightDiffuse);
		gl.uniform3fv(lsUniform, lightSpecular);
		gl.uniform4fv(lightPositionUniform, lightPosition);
	}
	else
	{
		 gl.uniform1i(lightingEnableUniform, 0);
	}

	// ***** 1st sphere on 1st column, emerald *****
	// ambient material
	materialAmbient[0] = 0.0215; // r
	materialAmbient[1] = 0.1745; // g
	materialAmbient[2] = 0.0215; // b

	// diffuse material
	materialDiffuse[0] = 0.07568; // r
	materialDiffuse[1] = 0.61424; // g
	materialDiffuse[2] = 0.07568; // b
	
	// specular material
	materialSpecular[0] = 0.633;    // r
	materialSpecular[1] = 0.727811; // g
	materialSpecular[2] = 0.633;    // b
	
	// materialShininess
	materialShininess = 0.6 * 128;
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// Geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [4.0, 3.15, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();

		// // *******************************************************

	// ***** 2nd sphere on 1st column, jade *****
	// ambient material
	materialAmbient[0] = 0.135;  // r
	materialAmbient[1] = 0.2225; // g
	materialAmbient[2] = 0.1575; // b
	  
	

	// diffuse material
	materialDiffuse[0] = 0.54; // r
	materialDiffuse[1] = 0.89; // g
	materialDiffuse[2] = 0.63; // b
	
	

	// specular material
	materialSpecular[0] = 0.316228; // r
	materialSpecular[1] = 0.316228; // g
	materialSpecular[2] = 0.316228; // b
	    
	

	// materialShininess
	materialShininess = 0.1 * 128;
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [4.0,1.9, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 3rd sphere on 1st column, obsidian *****
	// ambient material
	materialAmbient[0] = 0.05375; // r
	materialAmbient[1] = 0.05;    // g
	materialAmbient[2] = 0.06625; // b
	   
	

	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;    // g
	materialDiffuse[2] = 0.22525; // b
	   
	

	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	    
	

	// materialShininess
	materialShininess = 0.3 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [4.0,0.65, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 4th sphere on 1st column, pearl *****
	// ambient material
	materialAmbient[0] = 0.25;    // r
	materialAmbient[1] = 0.20725; // g
	materialAmbient[2] = 0.20725; // b
	   
	

	// diffuse material
	materialDiffuse[0] = 1.0;   // r
	materialDiffuse[1] = 0.829; // g
	materialDiffuse[2] = 0.829; // b
	 
	

	// specular material
	materialSpecular[0] = 0.296648; // r
	materialSpecular[1] = 0.296648; // g
	materialSpecular[2] = 0.296648; // b
	    
	

	// materialShininess
	materialShininess = 0.088 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [4.0, -0.6, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 5th sphere on 1st column, ruby *****
	// ambient material
	materialAmbient[0] = 0.1745;  // r
	materialAmbient[1] = 0.01175; // g
	materialAmbient[2] = 0.01175; // b
	   
	

	// diffuse material
	materialDiffuse[0] = 0.61424; // r
	materialDiffuse[1] = 0.04136; // g
	materialDiffuse[2] = 0.04136; // b
	   
	

	// specular material
	materialSpecular[0] = 0.727811; // r
	materialSpecular[1] = 0.626959; // g
	materialSpecular[2] = 0.626959; // b
	    
	

	// materialShininess
	materialShininess = 0.6 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [4.0, -1.85, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 6th sphere on 1st column, turquoise *****
	// ambient material
	materialAmbient[0] = 0.1;     // r
	materialAmbient[1] = 0.18725; // g
	materialAmbient[2] = 0.1745;  // b
	   
	

	// diffuse material
	materialDiffuse[0] = 0.396;   // r
	materialDiffuse[1] = 0.74151; // g
	materialDiffuse[2] = 0.69102; // b
	   
	

	// specular material
	materialSpecular[0] = 0.297254; // r
	materialSpecular[1] = 0.30829;  // g
	materialSpecular[2] = 0.306678; // b
	    
	

	// materialShininess
	materialShininess = 0.1 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	// scaleMatrix = scale(1.0, 1.0, 0.8);
	mat4.translate(translationMatrix, translationMatrix, [4.0, -3.1, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 2nd column, brass *****
	// ambient material
	materialAmbient[0] = 0.329412; // r
	materialAmbient[1] = 0.223529; // g
	materialAmbient[2] = 0.027451; // b
	    
	

	// diffuse material
	materialDiffuse[0] = 0.780392; // r
	materialDiffuse[1] = 0.568627; // g
	materialDiffuse[2] = 0.113725; // b
	    
	

	// specular material
	materialSpecular[0] = 0.992157; // r
	materialSpecular[1] = 0.941176; // g
	materialSpecular[2] = 0.807843; // b
	    
	

	// materialShininess
	materialShininess = 0.21794872 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [1.0,3.15, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 2nd sphere on 2nd column, bronze *****
	// ambient material
	materialAmbient[0] = 0.2125; // r
	materialAmbient[1] = 0.1275; // g
	materialAmbient[2] = 0.054;  // b
	  
	

	// diffuse material
	materialDiffuse[0] = 0.714;   // r
	materialDiffuse[1] = 0.4284;  // g
	materialDiffuse[2] = 0.18144; // b
	   
	

	// specular material
	materialSpecular[0] = 0.393548; // r
	materialSpecular[1] = 0.271906; // g
	materialSpecular[2] = 0.166721; // b
	    
	

	// materialShininess
	materialShininess = 0.2 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [1.0,1.9, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 3rd sphere on 2nd column, chrome *****
	// ambient material
	materialAmbient[0] = 0.25; // r
	materialAmbient[1] = 0.25; // g
	materialAmbient[2] = 0.25; // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	
	

	// specular material
	materialSpecular[0] = 0.774597; // r
	materialSpecular[1] = 0.774597; // g
	materialSpecular[2] = 0.774597; // b
	    
	

	// materialShininess
	materialShininess = 0.6 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [1.0,0.65, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 4th sphere on 2nd column, copper *****
	// ambient material
	materialAmbient[0] = 0.19125; // r
	materialAmbient[1] = 0.0735;  // g
	materialAmbient[2] = 0.0225;  // b
	   
	

	// diffuse material
	materialDiffuse[0] = 0.7038;  // r
	materialDiffuse[1] = 0.27048; // g
	materialDiffuse[2] = 0.0828;  // b
	   
	

	// specular material
	materialSpecular[0] = 0.256777; // r
	materialSpecular[1] = 0.137622; // g
	materialSpecular[2] = 0.086014; // b
	    
	

	// materialShininess
	materialShininess = 0.1 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [1.0, -0.6, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 5th sphere on 2nd column, gold *****
	// ambient material
	materialAmbient[0] = 0.24725; // r
	materialAmbient[1] = 0.1995;  // g
	materialAmbient[2] = 0.0745;  // b
	   
	

	// diffuse material
	materialDiffuse[0] = 0.75164; // r
	materialDiffuse[1] = 0.60648; // g
	materialDiffuse[2] = 0.22648; // b
	   
	

	// specular material
	materialSpecular[0] = 0.628281; // r
	materialSpecular[1] = 0.555802; // g
	materialSpecular[2] = 0.366065; // b
	    
	

	// materialShininess
	materialShininess = 0.4 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [1.0, -1.85, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 6th sphere on 2nd column, silver *****
	// ambient material
	materialAmbient[0] = 0.19225; // r
	materialAmbient[1] = 0.19225; // g
	materialAmbient[2] = 0.19225; // b
	   
	

	// diffuse material
	materialDiffuse[0] = 0.50754; // r
	materialDiffuse[1] = 0.50754; // g
	materialDiffuse[2] = 0.50754; // b
	   
	

	// specular material
	materialSpecular[0] = 0.508273; // r
	materialSpecular[1] = 0.508273; // g
	materialSpecular[2] = 0.508273; // b
	    
	

	// materialShininess
	materialShininess = 0.4 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [1.0, -3.1, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 3rd column, black *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	
	

	// specular material
	materialSpecular[0] = 0.50; // r
	materialSpecular[1] = 0.50; // g
	materialSpecular[2] = 0.50; // b
	
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-2.0,3.15, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 2nd sphere on 3rd column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.1;  // g
	materialAmbient[2] = 0.06; // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.0;        // r
	materialDiffuse[1] = 0.50980392; // g
	materialDiffuse[2] = 0.50980392; // b
	      
	

	// specular material
	materialSpecular[0] = 0.50196078; // r
	materialSpecular[1] = 0.50196078; // g
	materialSpecular[2] = 0.50196078; // b
	      
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-2.0,1.9, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 3rd sphere on 2nd column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.1;  // r
	materialDiffuse[1] = 0.35; // g
	materialDiffuse[2] = 0.1;  // b
	
	

	// specular material
	materialSpecular[0] = 0.45; // r
	materialSpecular[1] = 0.55; // g
	materialSpecular[2] = 0.45; // b
	
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-2.0,0.65, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 4th sphere on 3rd column, red *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.0;  // g
	materialDiffuse[2] = 0.0;  // b
	
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.6;  // g
	materialSpecular[2] = 0.6;  // b
	
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-2.0, -0.6, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 5th sphere on 3rd column, white *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.55; // r
	materialDiffuse[1] = 0.55; // g
	materialDiffuse[2] = 0.55; // b
	
	

	// specular material
	materialSpecular[0] = 0.70; // r
	materialSpecular[1] = 0.70; // g
	materialSpecular[2] = 0.70; // b
	
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-2.0, -1.85, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 6th sphere on 3rd column, yellow plastic *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.0;  // b
	
	

	// specular material
	materialSpecular[0] = 0.60; // r
	materialSpecular[1] = 0.60; // g
	materialSpecular[2] = 0.50; // b
	
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-2.0, -3.1, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 4th column, black *****
	// ambient material
	materialAmbient[0] = 0.02; // r
	materialAmbient[1] = 0.02; // g
	materialAmbient[2] = 0.02; // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	
	

	// specular material
	materialSpecular[0] = 0.4;  // r
	materialSpecular[1] = 0.4;  // g
	materialSpecular[2] = 0.4;  // b
	
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	// scaleMatrix = scale(1.0, 1.0, 0.8);
	mat4.translate(translationMatrix, translationMatrix, [-5.0,3.15, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 2nd sphere on 4th column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	
	

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-5.0,1.9, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 3rd sphere on 4th column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	
	

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-5.0,0.65, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 4th sphere on 4th column, red *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.04; // g
	materialSpecular[2] = 0.04; // b
	
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-5.0, -0.6, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 5th sphere on 4th column, white *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	mat4.translate(translationMatrix, translationMatrix, [-5.0, -1.85, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();
	// *******************************************************

	// ***** 6th sphere on 4th column, yellow rubber *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	gl.uniform3fv(kaUniform, materialAmbient);
	gl.uniform3fv(kdUniform, materialDiffuse);
	gl.uniform3fv(ksUniform, materialSpecular);
	gl.uniform1f(materialShininessUniform, materialShininess);

	// geometry
	modelMatrix = mat4.create();
	translationMatrix = mat4.create();
	// scaleMatrix = scale(1.0, 1.0, 0.8);
	mat4.translate(translationMatrix, translationMatrix, [-5.0, -3.1, 8.0]);
	mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
	gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
	
	sphere.draw();

	gl.useProgram(null);
	// Update Call
	update();
	// Double buffering emulation
	requestAnimationFrame(display, canvas);
}

function update(){
	// Code
	if(keyPressed == 1){
		angleForXRotation += 0.25;
		if(angleForXRotation >= 360.0)
			angleForXRotation -= 360.0;
		lightPosition = [0.0, 100.0 * Math.cos(degToRad(angleForXRotation)), 100.0 *  Math.sin(degToRad(angleForXRotation)), 1.0];
	}
	else if(keyPressed == 2){
		angleForYRotation += 0.25;
		if(angleForYRotation >= 360.0)
			angleForYRotation -= 360.0;
		lightPosition = [100.0 *  Math.cos(degToRad(angleForYRotation)), 0.0, 100.0 * Math.sin(degToRad(angleForYRotation)), 1.0];
	}
	else if(keyPressed == 3){
		angleForZRotation += 0.25;
		if(angleForZRotation >= 360.0)
			angleForZRotation -= 360.0;
		lightPosition = [100.0 * Math.cos(degToRad(angleForZRotation)), 100.0 * Math.sin(degToRad(angleForZRotation)), 0.0, 1.0];
	}
	else{
		lightPosition = [0.0, 0.0, 10.0, 1.0];
	}
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
		case 88:
			keyPressed = 1;
			angleForXRotation = 0.0;
			break;
		case 89:
			keyPressed = 2;
			angleForYRotation = 0.0;
			break;
		case 90:
			keyPressed = 3;
			angleForZRotation = 0.0;
			break;
		default:
			keyPressed = 0;
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
