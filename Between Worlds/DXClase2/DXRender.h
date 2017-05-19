#ifndef _DXRender_
#define _DXRender_

#include "Constants.h"

#include "Camara.h"
#include "Light.h"
#include "MeshPrimitives.h"
#include "MObject.h"

class DXRender
{
private:
	//Global Declarations//
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	struct Projectile {
		MPrimitives *knife;
		float startX;
		float startZ;
		float startY;
		float x;
		float y;
		float z;
		bool flying = false;
		bool started = false;
		D3DXVECTOR3 target;
	} projectile;
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;
	MPrimitives *espina;
	D3DXMATRIX View;
	D3DXMATRIX Projection;
	D3DXMATRIX dunaTraslacion[10],panTraslacion[5],casaTraslacion[4], calaveraTrasladar[7], cactusTraslacion[50], casaTraslacionChica[7], chileTraslacion[5],traslacionTequila[5];
	int rotacionYcasas[4], traslacionXcasas[4], traslacionZcasas[4];
	int numeroEspinas;
	int rotacionYcasasChicas[7], traslacionXcasasChicas[7], traslacionZcasasChicas[7],numeroSaltos;
	MPrimitives *cubo;
	MPrimitives *esfera;
	MPrimitives *toroide;
	MPrimitives *cono;
	MPrimitives *cilindro;
	MPrimitives *billboard;
	MPrimitives *skydomo;
	D3DXMATRIX espinaMat;
	D3DXVECTOR3 vectorEspina;
	MPrimitives *terreno;
	MPrimitives *Bitmap,*Busca,*X,*PrimerN,*SegundoN,*GameOver;
	MPrimitives *BoundingSphere1;
	MPrimitives *BoundingSphere2;
	MPrimitives *calavera[7],*cactus,*cactusNormales[50];
	MObject *duna[10];
	MPrimitives *pan[5];
	MObject *iglesia;
	float speed,aumento;
	MPrimitives *tequila[5];
	MPrimitives *chile[5];
	MPrimitives *casa[4];
	MObject *casaChicaC[3];
	bool cambiaEscenario,limiteCalavera[7],tiempoVelocidad,puedeSaltar,tocoSpacio;
	Camara *camara;
	Camara *skyCamara;
	float posx, posy, posz, rotx, roty, rotz,posCamaraY, movX, movZ;
	float rotPerFrame;
	float sphereSpeed;
	float vida,salto,vidaCalavera[7];
	bool disparo,vuela;
	D3DXVECTOR2 BoundingSphereTranslation;
	D3DXVECTOR2 DummyBoundyEvents;
	D3DXVECTOR2 LastBoundingPosition;
	float movXCalavera[7];

public:
	DXRender()
	{
		rotPerFrame = 0.0f;
	}

	float getPosx() {
		return posx;
	}

	float getPosy() {
		return posy;
	}

	float getPosz() {
		return posz;
	}

	void setPosx(float posx) {
		this->posx = posx;
	}

	void setPosy(float posy) {
		this->posy = posy;
	}

	void setPosz(float posz) {
		this->posz = posz;
	}

	bool InitDevice(HWND hwnd, int Width, int Height)
	{
		//Describe our Buffer
		DXGI_MODE_DESC bufferDesc;

		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = Width;
		bufferDesc.Height = Height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//Describe our SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//Create our SwapChain
		D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

		//Create our BackBuffer
		ID3D11Texture2D* BackBuffer;
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

		//Create our Render Target
		d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
		BackBuffer->Release();
		cambiaEscenario = false;
		//Crea la descripcion del raster, el cual determina como se dibujaran los poligonos
		D3D11_RASTERIZER_DESC rasterDesc;
		ID3D11RasterizerState* rasterState;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;//se puede manipular para transparencias, cambiar a D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0;

		d3d11Device->CreateRasterizerState(&rasterDesc, &rasterState);
		d3d11DevCon->RSSetState(rasterState);

		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ID3D11Texture2D* depthStencilBuffer;

		depthStencilDesc.Width     = Width;
		depthStencilDesc.Height    = Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0; 
		depthStencilDesc.MiscFlags      = 0;

		//Create the Depth/Stencil View
		d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

		//Set our Render Target
		d3d11DevCon->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );
		rotx = 0;
		for (int i = 0; i <= 6; i++) {
			vidaCalavera[i] = 10.0f;
		}
		billboard = new MPrimitives(d3d11Device, d3d11DevCon, 2.0f, 2.0f, L"Texturas/sol.png");
		iglesia = new MObject(d3d11Device, d3d11DevCon, "Models/iglesia.txt", L"Texturas/iglesia.jpg", hwnd);
		skydomo = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1000.0f, L"Texturas/skydome.bmp", L"Texturas/escudoNormal.jpg", ShaderSkydome);
		terreno = new MPrimitives(d3d11Device, d3d11DevCon, 500.0f, L"Texturas/arena.jpg", L"Texturas/arena.jpg", "Texturas/desert.bmp", ShaderMultitextura);
		for (int i = 0; i <= 6; i++) {
		calavera[i] = new MPrimitives(d3d11Device, d3d11DevCon, "Models/calavera.txt", L"Texturas/calavera.jpg", L"Texturas/calaveraNormal.jpg",ShaderNormalmap);
		}
		projectile.knife = new MPrimitives(d3d11Device, d3d11DevCon, "Models/espina.txt", L"Texturas/espina.jpg", L"Texturas/espina.jpg", ShaderDiffuse);
		numeroEspinas = 0;
		posCamaraY = 7.0f;
		aumento = 0.01;
		D3DXMatrixTranslation(&calaveraTrasladar[0], 100, 10, 0);
		D3DXMatrixTranslation(&calaveraTrasladar[1], 100, 7, 100);
		D3DXMatrixTranslation(&calaveraTrasladar[2], 150, 10, 150);
		D3DXMatrixTranslation(&calaveraTrasladar[3], 200, 10, 10);
		D3DXMatrixTranslation(&calaveraTrasladar[4], 0, 10, 10);
		D3DXMatrixTranslation(&calaveraTrasladar[5], 10, 10,200); 
		D3DXMatrixTranslation(&calaveraTrasladar[6], 43, 10, 45);
		vida = 90.0f;
		salto = 5.0f;
		tocoSpacio = false;
		for (int i = 0; i <= 9; i++) {
			duna[i] = new MObject(d3d11Device, d3d11DevCon, "Models/duna.txt", L"Texturas/arena.jpg", hwnd);
			D3DXMatrixTranslation(&dunaTraslacion[i], (rand() % 150), 4, (rand() % 150));
		}
		for (int i = 0; i <= 4; i++) {
			pan[i] = new MPrimitives(d3d11Device, d3d11DevCon, "Models/pan.txt", L"Texturas/pan.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
			int x = (rand() % 50);
			int z = (rand() % 150);
			D3DXMatrixTranslation(&panTraslacion[i],x, terreno->getTerrainCollision(D3DXVECTOR2(x, z))+ 3, z);
		}
		for(int i = 0; i <= 49; i++) {
			cactusNormales[i] = new MPrimitives(d3d11Device, d3d11DevCon, "Models/cactusNormal.txt", L"Texturas/cactusN.jpg", L"Texturas/cactusNNormal.jpg", ShaderDiffuse);
			int x = (rand() % 250);
			int z = (rand() % 250);
			D3DXMatrixTranslation(&cactusTraslacion[i], x, terreno->getTerrainCollision(D3DXVECTOR2(x, z)),z);
		}
		numeroSaltos = 0;
		traslacionXcasasChicas[0] = 30;
		traslacionZcasasChicas[0] = 100;
		rotacionYcasasChicas[0] = 180;
		speed = 0.05f;
		for (int i = 0; i <= 0; i++) {
			casaChicaC[i] = new MObject(d3d11Device, d3d11DevCon, "Models/casa1.txt", L"Texturas/casaChica.jpg", hwnd);
			D3DXMatrixTranslation(&casaTraslacionChica[i], traslacionXcasasChicas[i], terreno->getTerrainCollision(D3DXVECTOR2(traslacionXcasasChicas[i], traslacionZcasasChicas[i]))+3, traslacionZcasasChicas[i]);
		}
		for (int i = 0; i <= 4; i++) {
			int x = (rand() % 150);
			int z = (rand() % 50);
			chile[i] = new MPrimitives(d3d11Device, d3d11DevCon, "Models/chile.txt", L"Texturas/chile.png", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
			D3DXMatrixTranslation(&chileTraslacion[i], x, terreno->getTerrainCollision(D3DXVECTOR2(x, z)) + 3, z);
		}
		for (int i = 0; i <= 4; i++) {
			int x = (rand() % 200);
			int z = (rand() % 110);
			tequila[i] = new MPrimitives(d3d11Device, d3d11DevCon, "Models/tequila.txt", L"Texturas/vaso.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
			D3DXMatrixTranslation(&traslacionTequila[i], x, terreno->getTerrainCollision(D3DXVECTOR2(x, z)) + 3, z);
		}
		traslacionXcasas[0] = 65;
		traslacionZcasas[0] = 146;
		traslacionXcasas[1] = 30;
		traslacionZcasas[1] = 54;
		traslacionXcasas[2] = 200;
		traslacionZcasas[2] = 50;
		traslacionXcasas[3] = 140;
		traslacionZcasas[3] = 210;
		puedeSaltar = false;
		for (int i = 0; i <= 3; i++) {
			casa[i] = new MPrimitives(d3d11Device, d3d11DevCon, "Models/casa.txt", L"Texturas/casa.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
			D3DXMatrixTranslation(&casaTraslacion[i], traslacionXcasas[i], terreno->getTerrainCollision(D3DXVECTOR2(traslacionXcasas[i], traslacionZcasas[i])) + 0, traslacionZcasas[i]);
		}
		cactus = new MPrimitives(d3d11Device, d3d11DevCon, "Models/cactus.txt", L"Texturas/cactus.jpg", L"Texturas/cactusNormal.jpg", ShaderNormalmap);
		BoundingSphereTranslation = D3DXVECTOR2(0, 0);
		posx = 50;
		tiempoVelocidad = 0;
		posz = 50;
		rotacionYcasas[0] = -89;
		rotacionYcasas[1] = 90;
		rotacionYcasas[2] = 25;
		return true;
	}

	bool InitDirectInput(HINSTANCE hInstance, HWND hwnd)
	{
		DirectInput8Create(hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&DirectInput,
			NULL); 

		DirectInput->CreateDevice(GUID_SysKeyboard,
			&DIKeyboard,
			NULL);

		DirectInput->CreateDevice(GUID_SysMouse,
			&DIMouse,
			NULL);

		DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
		DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		DIMouse->SetDataFormat(&c_dfDIMouse);
		DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

		return true;
	}

	void DetectInput()
	{
		DIMOUSESTATE mouseCurrState;

		BYTE keyboardState[256];

		DIKeyboard->Acquire();
		DIMouse->Acquire();

		DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

		DIKeyboard->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);

		float moveLeftRight = 0;
		float moveBackForward = 0;

		float camYaw = 0;
		float camPitch = 0;

		if(keyboardState[DIK_A])
		{
			moveLeftRight -= speed;
			posx -= speed;
		}
		if(keyboardState[DIK_D])
		{
			moveLeftRight += speed;
			posx += moveLeftRight;
		}
		if(keyboardState[DIK_W])
		{
			moveBackForward += speed;
			posz += moveBackForward;
		}
		if(keyboardState[DIK_S])
		{
			moveBackForward -= speed;
			posz += moveBackForward;
		}
		if (keyboardState[DIK_LEFT])
		{
			DummyBoundyEvents.x = -.01f;
		}
		if (keyboardState[DIK_RIGHT])
		{
			DummyBoundyEvents.x = .01f;
		}
		if (keyboardState[DIK_UP])
		{
			DummyBoundyEvents.y = .01f;
		}
		if (keyboardState[DIK_DOWN])
		{
			DummyBoundyEvents.y = -.01f;
		}

		if (keyboardState[DIK_P]) {
			if (projectile.started == true) {
				projectile.flying = false;
			}
			projectile.started = true;
		}

		if (keyboardState[DIK_SPACE])
		{
			if (puedeSaltar) {
				numeroSaltos++;
				posy = 10;
				tocoSpacio = true;
			}
		}

		if((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
		{
			float smoothness = 0.001f;
			camYaw = mouseCurrState.lX * smoothness;
			camPitch = mouseCurrState.lY * smoothness;
			rotx = smoothness;
			mouseLastState = mouseCurrState;
		}

		camara->UpdateCamera(moveBackForward, moveLeftRight, camYaw, camPitch); 
		skyCamara->setSkyCamara(camara);
	}

	void ReleaseObjects()
	{
		//Release the COM Objects we created
		SwapChain->Release();
		d3d11Device->Release();
		d3d11DevCon->Release();
		renderTargetView->Release();
		depthStencilView->Release();
	}

	bool InitScene(int Width, int Height)
	{
		//Create the Viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Width;
		viewport.Height = Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		//set light values
	/*	light.direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		light.ambient = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
		light.diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);*/

		//Set the Viewport
		d3d11DevCon->RSSetViewports(1, &viewport);

		//Create the camera
		D3DXVECTOR3 Position = D3DXVECTOR3(50, 7.0f, 36.0f);
		D3DXVECTOR3 Target = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camara = new Camara(Position,Target,Up, Width, Height);
		skyCamara = new Camara(Position,Target,Up, Width, Height);
		
		Bitmap = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/GUICactus.png", camara);
		Busca = new MPrimitives(d3d11Device, d3d11DevCon, 0.5, 0.3, L"Texturas/calavera.png", camara);
		GameOver = new MPrimitives(d3d11Device, d3d11DevCon, 4.0,4.0, L"Texturas/Over.jpg", camara);
		X = new MPrimitives(d3d11Device, d3d11DevCon, 0.3, 0.3, L"Texturas/GUIX.png", camara);
		PrimerN = new MPrimitives(d3d11Device, d3d11DevCon, 0.6, 0.4, L"Texturas/primerN.png", camara);
		SegundoN = new MPrimitives(d3d11Device, d3d11DevCon, 0.6, 0.4, L"Texturas/primerN.png", camara);
		return true;
	}

	void UpdateScene()
	{
		
		LastBoundingPosition = BoundingSphereTranslation;
		BoundingSphereTranslation += DummyBoundyEvents;
		rotPerFrame += 0.1f;
		movX += 0.1f;
		movZ += 0.1f;
		D3DXMATRIX rotEspina,scaleEspina,movimientoEspina,rotMatY,rotMatCalavera,northMat, westMat , eastMat,rotSol,rotCactusCamX,matrizTraslacionCalavera,rotIglesia,southMat,panScale,solScale,iglesiaScale, tequilaScale, scale, rotMatX,MatAcomodo,cactusMat,cactusRot,casaRot,calaveraRot,casaScale,cactusScale,casaNormalScale,chileScale;
		D3DXMatrixScaling(&scaleEspina, 1.5f, 1.5f, 1.5f);
		D3DXMATRIX Bounding1, Bounding2, BoundinMatTranslation;
		if (movXCalavera[0] <= 60.0 && !limiteCalavera[0]){
			movXCalavera[0] += 0.01f;
			D3DXMatrixRotationY(&rotMatCalavera, 0);
		}
		else{
			if (movXCalavera[0] <= 0.0f) {
				limiteCalavera[0] = false;
			}
			else {
				D3DXMatrixRotationY(&rotMatCalavera, 355);
				limiteCalavera[0] = true;
				movXCalavera[0] -= 0.01f;
			}
		}
		D3DXMatrixTranslation(&matrizTraslacionCalavera, 0, 0, movXCalavera[0]);
		calavera[0]->UpdateScene(rotMatCalavera*matrizTraslacionCalavera *calaveraTrasladar[0]);
		if (movXCalavera[1] <= 60.0 && !limiteCalavera[1]) {
			movXCalavera[1] += 0.01f;
			D3DXMatrixRotationY(&rotMatCalavera, 0);
		}
		else {
			if (movXCalavera[1] <= 0.0f) {
				limiteCalavera[1] = false;
			}
			else {
				D3DXMatrixRotationY(&rotMatCalavera, 355);
				limiteCalavera[1] = true;
				movXCalavera[1] -= 0.01f;
			}
		}
		D3DXMatrixRotationY(&rotEspina, 180);
		if (projectile.started) {
			D3DXMATRIX knifePos;
			if (projectile.flying) {
				D3DXVECTOR3 oldPos;
				oldPos.x = projectile.x;
				oldPos.y = projectile.y;
				oldPos.z = projectile.z;
				oldPos += 1 * projectile.target;
				D3DXMatrixTranslation(&knifePos, oldPos.x, oldPos.y, oldPos.z);
				projectile.knife->UpdateScene(rotEspina*knifePos);
				projectile.x = oldPos.x;
				projectile.y = oldPos.y;
				projectile.z = oldPos.z;
			}
			else {
				projectile.startX = camara->Position.x;
				projectile.startZ = camara->Position.z;
				projectile.startY = camara->Position.y;
				projectile.x = camara->Position.x;
				projectile.z = camara->Position.z;
				projectile.y = camara->Position.y;
				D3DXMatrixTranslation(&knifePos, projectile.x, projectile.y, projectile.z);
				projectile.knife->UpdateScene(rotEspina*knifePos);
				projectile.flying = true;
				projectile.target = camara->Forward;
			}
		}
		D3DXMatrixTranslation(&matrizTraslacionCalavera, 0, 0, movXCalavera[1]);
		calavera[1]->UpdateScene(rotMatCalavera*matrizTraslacionCalavera *calaveraTrasladar[1]);
		if (movXCalavera[2] <= 60.0 && !limiteCalavera[2]) {
			movXCalavera[2] += 0.01f;
			D3DXMatrixRotationY(&rotMatCalavera, 0);
		}
		else {
			if (movXCalavera[2] <= 0.0f) {
				limiteCalavera[2] = false;
			}
			else {
				D3DXMatrixRotationY(&rotMatCalavera, 355);
				limiteCalavera[2] = true;
				movXCalavera[2] -= 0.01f;
			}
		}
		D3DXMatrixTranslation(&matrizTraslacionCalavera, 0, 0, movXCalavera[2]);
		calavera[2]->UpdateScene(rotMatCalavera*matrizTraslacionCalavera *calaveraTrasladar[2]);
		if (movXCalavera[3] <= 10.0 && !limiteCalavera[3]) {
			movXCalavera[3] += 0.01f;
			D3DXMatrixRotationY(&rotMatCalavera, 0);
		}
		else {
			if (movXCalavera[3] <= 0.0f) {
				limiteCalavera[3] = false;
			}
			else {
				D3DXMatrixRotationY(&rotMatCalavera, 355);
				limiteCalavera[3] = true;
				movXCalavera[3] -= 0.01f;
			}
		}
		D3DXMatrixTranslation(&matrizTraslacionCalavera, 0, 0, movXCalavera[3]);
		calavera[3]->UpdateScene(rotMatCalavera*matrizTraslacionCalavera *calaveraTrasladar[3]);
		if (movXCalavera[4] <= 60.0 && !limiteCalavera[4]) {
			movXCalavera[4] += 0.01f;
			D3DXMatrixRotationY(&rotMatCalavera, 0);
		}
		else {
			if (movXCalavera[4] <= 0.0f) {
				limiteCalavera[4] = false;
			}
			else {
				D3DXMatrixRotationY(&rotMatCalavera, 355);
				limiteCalavera[4] = true;
				movXCalavera[4] -= 0.01f;
			}
		}
		D3DXMatrixTranslation(&matrizTraslacionCalavera, 0, 0, movXCalavera[4]);
		calavera[4]->UpdateScene(rotMatCalavera*matrizTraslacionCalavera *calaveraTrasladar[4]);
		if (movXCalavera[5] <= 30.0 && !limiteCalavera[5]) {
			movXCalavera[5] += 0.01f;
			D3DXMatrixRotationY(&rotMatCalavera, 0);
		}
		else {
			if (movXCalavera[5] <= 0.0f) {
				limiteCalavera[5] = false;
			}
			else {
				D3DXMatrixRotationY(&rotMatCalavera, 355);
				limiteCalavera[5] = true;
				movXCalavera[5] -= 0.01f;
			}
		}
		D3DXMatrixTranslation(&matrizTraslacionCalavera, 0, 0, movXCalavera[5]);
		calavera[5]->UpdateScene(rotMatCalavera*matrizTraslacionCalavera *calaveraTrasladar[5]);
		if (movXCalavera[6] <= 20.0 && !limiteCalavera[6]) {
			movXCalavera[6] += 0.01f;
			D3DXMatrixRotationY(&rotMatCalavera, 0);
		}
		else {
			if (movXCalavera[6] <= 0.0f) {
				limiteCalavera[6] = false;
			}
			else {
				D3DXMatrixRotationY(&rotMatCalavera, 355);
				limiteCalavera[6] = true;
				movXCalavera[6] -= 0.01f;
			}
		}
		D3DXMatrixTranslation(&matrizTraslacionCalavera, 0, 0, movXCalavera[6]);
		calavera[6]->UpdateScene(rotMatCalavera*matrizTraslacionCalavera *calaveraTrasladar[6]);
     	D3DXMatrixRotationY(&rotMatY, D3DXToRadian(rotPerFrame));
		D3DXMatrixTranslation(&MatAcomodo, 50, 3, 50);
		D3DXMatrixTranslation(&northMat, 0, 30, 0);
		D3DXMatrixTranslation(&cactusMat, posx, terreno->getTerrainCollision(D3DXVECTOR2(posx, posz)), posz);
		D3DXMatrixRotationY(&cactusRot,355);
		D3DXMatrixRotationZ(&rotCactusCamX, D3DXToRadian(rotx));
		D3DXMatrixRotationX(&rotSol, -96);
		D3DXMatrixRotationY(&casaRot, -89);
		D3DXMatrixRotationY(&rotIglesia, -60);
		D3DXMatrixTranslation(&southMat, 50, terreno->getTerrainCollision(D3DXVECTOR2(50, 20)), 20);
		D3DXMatrixTranslation(&eastMat, 3, 0, 0);
		D3DXMatrixTranslation(&westMat, -3, 0, 0);
		D3DXMatrixTranslation(&Bounding1, 3, 6, 0);
		D3DXMatrixTranslation(&Bounding2, -3, 6, 0);
		D3DXMatrixTranslation(&BoundinMatTranslation, BoundingSphereTranslation.x, BoundingSphereTranslation.y, 0);
		D3DXMatrixScaling(&scale, 0.03f, 0.03f, 0.03f);
		D3DXMatrixScaling(&casaScale, 7.0f,7.0f, 7.0f);
		D3DXMatrixScaling(&casaNormalScale, 1.5f, 1.5f, 1.5f);
		D3DXMatrixScaling(&cactusScale, 0.3f, 0.3f, 0.3f);
		D3DXMatrixScaling(&calaveraRot, 1.0f, 1.0f, 1.0f);
		D3DXMatrixScaling(&chileScale, 0.3f, 0.3f, 0.3f);
		D3DXMatrixScaling(&iglesiaScale, 3.0f, 3.0f, 3.0f);
		D3DXMatrixScaling(&panScale, 3.0f, 3.0f, 3.0f);
		D3DXMatrixScaling(&solScale, 6.0f, 6.0f, 6.0f);
		D3DXMatrixScaling(&tequilaScale, 0.5f, 0.5f, 0.5f);
		if (tocoSpacio) {
			D3DXMatrixTranslation(&cactusMat, posx,posy, posz);
			camara->Position.y = posy + 5;
			tocoSpacio = false;
		}
		else {
			D3DXMatrixTranslation(&cactusMat, posx, terreno->getTerrainCollision(D3DXVECTOR2(posx, posz)), posz);
			camara->Position.y = terreno->getTerrainCollision(camara->getXZ()) + 6;
		}
		cactus->UpdateScene(cactusRot*cactusMat*rotCactusCamX);
		iglesia->UpdateScene(rotIglesia*iglesiaScale*southMat);
		for (int i = 0; i <= 4; i++) {
			duna[i]->UpdateScene(dunaTraslacion[i]);
		}
		/*for (int i = 0; i <= 4; i++) {
			tequila[i]->UpdateScene(tequilaScale*traslacionTequila[i]);
		}*/
		for (int i = 0; i <= 49; i++) {
		cactusNormales[i]->UpdateScene(cactusScale*cactusTraslacion[i]);
		}
		casa[0]->UpdateScene(casaRot*casaScale * casaTraslacion[0]);
		casa[1]->UpdateScene(casaRot*casaScale * casaTraslacion[1]);
		D3DXMatrixRotationY(&casaRot, rotacionYcasas[1]);
		casa[2]->UpdateScene(casaRot*casaScale * casaTraslacion[2]);
		D3DXMatrixRotationY(&casaRot, rotacionYcasas[2]);
		casa[3]->UpdateScene(casaRot*casaScale * casaTraslacion[3]);
		/*for (int i = 0; i <= 4; i++) {
			chile[i]->UpdateScene(chileScale*chileTraslacion[i]);
		}*/
		D3DXMATRIX espinaMat;
		D3DXMatrixTranslation(&espinaMat, camara->Position.x, camara->Position.y, camara->Position.z);
		//espina->UpdateScene(espinaMat);
		//D3DXMatrixTranslation(&espinaMat, camara->Position.x, camara->Position.y, camara->Position.z);
		casaChicaC[0]->UpdateScene(rotacionYcasasChicas[0] * casaNormalScale * casaTraslacionChica[0]);
		billboard->UpdateScene(rotSol*solScale*westMat * northMat* MatAcomodo);
		D3DXMATRIX skyRotMat, terrMat;
		D3DXMatrixRotationX(&skyRotMat, D3DXToRadian(-90));
		skydomo->UpdateScene(skyRotMat);
		D3DXMatrixTranslation(&terrMat, -50,-60, -50);
		D3DXMatrixIdentity(&terrMat);
		terreno->UpdateScene(terrMat);

		//BoundingSphere1->UpdateScene(Bounding1 * MatAcomodo * BoundinMatTranslation);
		//BoundingSphere2->UpdateScene(Bounding2 * MatAcomodo);
		for (int i = 0; i <= 4; i++) {
			if (CheckBoundingSphereCollision(cactus->PrimitiveBoundings->Radius,
				cactus->PrimitiveBoundings->PositionM, cactus->getWorldMatrix(),
				pan[i]->PrimitiveBoundings->Radius,
				pan[i]->PrimitiveBoundings->PositionM, pan[i]->getWorldMatrix())) {
				D3DXMatrixTranslation(&panTraslacion[i], 0, -100, 0);
				pan[i]->UpdateScene(panScale*panTraslacion[i]);
				vida += 0.01f;
			}
			else {				
				pan[i]->UpdateScene(panScale*panTraslacion[i]);
			}
			if (CheckBoundingSphereCollision(cactus->PrimitiveBoundings->Radius,
				cactus->PrimitiveBoundings->PositionM, cactus->getWorldMatrix(),
				chile[i]->PrimitiveBoundings->Radius,
				chile[i]->PrimitiveBoundings->PositionM, chile[i]->getWorldMatrix())) {
				D3DXMatrixTranslation(&chileTraslacion[i], 0, -100, 0);
				chile[i]->UpdateScene(chileScale*chileTraslacion[i]);
				tiempoVelocidad = 0;
				speed = 0.09f;
			}
			else {
				chile[i]->UpdateScene(chileScale*chileTraslacion[i]);
				if (tiempoVelocidad > 20) {
					speed = 0.05f;
				}
			}
			if (CheckBoundingSphereCollision(cactus->PrimitiveBoundings->Radius,
				cactus->PrimitiveBoundings->PositionM, cactus->getWorldMatrix(),
				tequila[i]->PrimitiveBoundings->Radius,
				tequila[i]->PrimitiveBoundings->PositionM, tequila[i]->getWorldMatrix())) {
				D3DXMatrixTranslation(&traslacionTequila[i], 0, -100, 0);
				tequila[i]->UpdateScene(tequilaScale*traslacionTequila[i]);
				puedeSaltar = true;
				numeroSaltos = 0;
			}
			else {
				tequila[i]->UpdateScene(tequilaScale*traslacionTequila[i]);
			}
		}
		for (int i = 0; i <= 6; i++) {
			if (CheckBoundingSphereCollision(cactus->PrimitiveBoundings->Radius,
				cactus->PrimitiveBoundings->PositionM, cactus->getWorldMatrix(),
				calavera[i]->PrimitiveBoundings->Radius,
				calavera[i]->PrimitiveBoundings->PositionM, calavera[i]->getWorldMatrix())) {
				vida -= 0.1;
			}
		}

		/*if (numeroSaltos > 6) {
			puedeSaltar = false;
		}*/
		tiempoVelocidad++;
		DummyBoundyEvents.x = 0;
		DummyBoundyEvents.y = 0;
	}

	void DrawScene()
	{
		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(0.0f, 0.0f, 0.25f, 1.0f);
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		//Refresh the Depth/Stencil view
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
		if(vida > 0){
			if (projectile.started) {
				projectile.knife->DrawScene(camara);
			}
			if (vidaCalavera[0] > 0) {
				calavera[0]->DrawScene(camara);
			}
			if (vidaCalavera[1] > 0) {
				calavera[1]->DrawScene(camara);
			}
			if (vidaCalavera[2] > 0) {
				calavera[2]->DrawScene(camara);
			}
			if (vidaCalavera[3] > 0) {
				calavera[3]->DrawScene(camara);
			}
			if (vidaCalavera[4] > 0) {
				calavera[4]->DrawScene(camara);
			}
			if (vidaCalavera[5] > 0) {
				calavera[5]->DrawScene(camara);
			}
			if (vidaCalavera[6] > 0) {
				calavera[6]->DrawScene(camara);
			}
			if (!cambiaEscenario) {
				for (int i = 0; i <= 4; i++) {
					duna[i]->DrawScene(camara);
				}
				for (int i = 0; i <= 3; i++) {
				casa[i]->DrawScene(camara);
				}
				casa[1]->DrawScene(camara);
				for (int i = 0; i <= 4; i++) {
					pan[i]->DrawScene(camara);
				}

				for (int i = 0; i <= 4; i++) {
					chile[i]->DrawScene(camara);
				}

				for (int i = 0; i <= 4; i++) {
					tequila[i]->DrawScene(camara);
				}
			}
			else {
				iglesia->DrawScene(camara);
			}
			for (int i = 0; i <= 49; i++) {
				cactusNormales[i]->DrawScene(camara);
			}
			billboard->DrawScene(camara);
			terreno->DrawScene(camara);
			cactus->DrawScene(camara);
			skydomo->DrawScene(skyCamara);
			//BoundingSphere1->DrawScene(camara);
			//BoundingSphere2->DrawScene(camara);

			Bitmap->DrawBitmap(camara, 0.7f, -0.7f);
			Busca->DrawBitmap(camara, 0.5f, 0.8f);
			X->DrawBitmap(camara, 0.66f, 0.8f);
			PrimerN->DrawBitmap(camara, 0.75f, 0.8f);
			SegundoN->DrawBitmap(camara, 0.84f, 0.8f);
		}
		else {
			GameOver->DrawBitmap(camara,0.0f,0.0f);
		}
		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}

	
	bool CheckBoundingSphereCollision(float firstSphereRadius,
		D3DXVECTOR3 firstSpherePos,
		D3DXMATRIX firstObjWorldSpace,
		float secondSphereRadius,
		D3DXVECTOR3 secondSpherePos,
		D3DXMATRIX secondObjWorldSpace)
	{
		//Declare local variables
		D3DXVECTOR3 world_1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 world_2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float objectsDistance = 0.0f;

		//Transform the objects world space to objects REAL center in world space
		D3DXVec3TransformCoord(&world_1, &firstSpherePos, &firstObjWorldSpace);
		D3DXVec3TransformCoord(&world_2, &secondSpherePos, &secondObjWorldSpace);

		// Create Vector to verify distance
		world_1 -= world_2;

		//Get the distance between the two objects
		objectsDistance = D3DXVec3Length(&world_1);
		//objectsDistance = XMVectorGetX(XMVector3Length(world_1 - world_2));

		//If the distance between the two objects is less than the sum of their bounding spheres...
		if (objectsDistance <= (firstSphereRadius + secondSphereRadius))
			return true;

		//If the bounding spheres are not colliding, return false
		return false;
	}
	
	
};

#endif