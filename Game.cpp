#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Timer.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 768;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	Window win;
	Shader shader;
	Triangle tri;
	ConstantBuffer2 buffer = ConstantBuffer2();
	Timer timer;
	DXCore device;

	win.Create(WIDTH, HEIGHT, "My Window", false, 100, 100);
	device = win.GetDevice();

	shader.Init("Shaders/VertexShader.hlsl", "Shaders/PixelShader.hlsl", device);
	shader.InitConstBuffer(sizeof(buffer), device);

	tri.Init(device, 3);

	while (true)
	{
		// update constant buffer
		buffer.time += timer.dt();
		for (int i = 0; i < 4; i++)
		{
			float angle = buffer.time + (i * M_PI / 2.0f);
			buffer.lights[i] = Vec4(WIDTH / 2.0f + (cosf(angle) * (WIDTH * 0.3f)),
				HEIGHT / 2.0f + (sinf(angle) * (HEIGHT * 0.3f)),
				0, 0);
		}

		win.ProcessMessages();
		win.Clear();

		shader.UpdateConstBuffer(&buffer, sizeof(buffer), device);
		shader.ApplyShader(device);
		tri.Draw(device);

		win.Present();
	}

	return 0;
}