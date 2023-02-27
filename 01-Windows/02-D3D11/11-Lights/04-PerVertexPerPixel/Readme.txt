Map with OpenGL code.
Vertex ani pixel shader related variable should be doubled.
Constant buffers should also be doubled.
InputOutput layout will also be 2.
Initialize->VSSetShader() and PSSetShader() should be in display as set shader will be on toggle.
API changes are in keypressed block.. LightingEnabled block only contains ConstantBuffer changes.
IASetInputBuffer should be in display.
VSSetConstantBuffers and PSSetConstantBuffers should be in display.
Uninitialize all the new buffers.
