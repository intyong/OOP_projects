////////////////////////////////////////////////////////////////////////////////
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <random>
#include <list>

using namespace std;

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

bool isGameOver = false;
clock_t startTime;

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

void game_over() {

	clock_t endTime = clock();
	float scoreTime = (float)(endTime - startTime) / CLOCKS_PER_SEC;

	char temp[100];
	sprintf(temp, "     GAME OVER ...\n\nSurvival time : %.2f초", scoreTime);

	LPD3DXFONT g_pFont;
	D3DXCreateFont(Device, 80, 40, 1000, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "신명조", &g_pFont);

	//사각형 영역 설정 , -1은 옵션에 NOCLIP을 넣었기 때문에 , 값을 주지 않은것이다
	RECT rc = { 90, 170, -1, -1 };

	g_pFont->DrawText(NULL, temp, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffff00);

	g_pFont->Release();
}

// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private:
	float					center_x, center_y, center_z;
	float                   m_radius;
	float					m_velocity_x;
	float					m_velocity_z;

public:
	CSphere(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
		m_pSphereMesh = NULL;
	}
	~CSphere(void) {}

public:
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
			return false;
		return true;
	}

	void destroy(void)
	{
		if (m_pSphereMesh != NULL) {
			m_pSphereMesh->Release();
			m_pSphereMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		float x_squared = (center_x - ball.center_x) * (center_x - ball.center_x);
		float y_squared = (center_y - ball.center_y) * (center_y - ball.center_y);
		float z_squared = (center_z - ball.center_z) * (center_z - ball.center_z);
		float total_squared = x_squared + y_squared + z_squared;

		float r_squared = M_RADIUS * 2 * M_RADIUS * 2;

		if (r_squared >= total_squared) {
			return true;
		}

		return false;
	}

	void hitBy(CSphere& ball, bool Bcheck)  // Bcheck = 폭탄이면 true, 구슬이면 false
	{
		//폭탄이면) 플레이어 공 정지시키고, 시작 위치로 보냄
		if (Bcheck == true) {
			(*this).setPower(0, 0);
			(*this).setCenter(.0f, (float)M_RADIUS, -2.5f);
		}

		//구슬이면) 플레이어 공과 구슬 정지시키고, 게임오버 시킴
		else if (Bcheck == false) {
			(*this).setPower(0, 0);
			ball.setPower(0, 0);

			if (isGameOver == false) {
				game_over();
				isGameOver = true;
			}
			return;
		}
	}

	void ballUpdate(float timeDiff)
	{
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if (vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
			float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;

			//correction of position of ball
			// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
			/*if(tX >= (4.5 - M_RADIUS))
				tX = 4.5 - M_RADIUS;
			else if(tX <=(-4.5 + M_RADIUS))
				tX = -4.5 + M_RADIUS;
			else if(tZ <= (-3 + M_RADIUS))
				tZ = -3 + M_RADIUS;
			else if(tZ >= (3 - M_RADIUS))
				tZ = 3 - M_RADIUS;*/

			this->setCenter(tX, cord.y, tZ);
		}
		else { this->setPower(0, 0); }
		//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
		double rate = 1 - (1 - DECREASE_RATE) * timeDiff * 400;
		if (rate < 0)
			rate = 0;
		this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
	}

	double getVelocity_X() { return this->m_velocity_x; }
	double getVelocity_Z() { return this->m_velocity_z; }

	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x = x;	center_y = y;	center_z = z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getRadius(void)  const { return (float)(M_RADIUS); }
	const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(center_x, center_y, center_z);
		return org;
	}

private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pSphereMesh;

};



// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:

	float					m_x;
	float					m_z;
	float                   m_width;
	float                   m_depth;
	float					m_height;

public:
	CWall(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_width = 0;
		m_depth = 0;
		m_pBoundMesh = NULL;
	}
	~CWall(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		m_width = iwidth;
		m_depth = idepth;

		if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
			return false;
		return true;
	}
	void destroy(void)
	{
		if (m_pBoundMesh != NULL) {
			m_pBoundMesh->Release();
			m_pBoundMesh = NULL;
		}
	}
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		// 벽 인스턴스의 x좌표, z좌표와 width를 이용하여 ball과 벽이 접촉하였는지를 판단하는 분기 코드

		float center_x = ball.getCenter().x;
		float center_z = ball.getCenter().z;

		bool case1;

		if (m_x > -0.01f && m_x < 0.01f) {
			if (m_z < 0) {
				case1 = center_z < m_z;
			}
			else {
				case1 = center_z > m_z;
			}
		}
		else {
			if (m_x < 0) {
				case1 = center_x < m_x;
			}
			else {
				case1 = center_x > m_x;
			}
		}

		if (case1) {
			return true;
		}

		return false;
	}

	void hitBy(CSphere& ball, int Bcheck)   // 플레이어의 공 : 1, 폭탄 : 2, 구슬 : 3
	{

		if (Bcheck == 1) {          // 플레이어의 공 case	정지시키고, 원래 위치로 보냄
			ball.setPower(0, 0);
			ball.setCenter(.0f, (float)M_RADIUS, -2.5f);
		}
		else if (Bcheck == 2) {     // 폭탄 case			게임오버시킴
			if (isGameOver == false) {
				game_over();
				isGameOver = true;
			}
		}
		else if (Bcheck == 3) {     // 구슬 case
			//특별한 동작 없음
		}
	}

	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getHeight(void) const { return M_HEIGHT; }


private:
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pBoundMesh;
};

// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

class CLight {
public:
	CLight(void)
	{
		static DWORD i = 0;
		m_index = i++;
		D3DXMatrixIdentity(&m_mLocal);
		::ZeroMemory(&m_lit, sizeof(m_lit));
		m_pMesh = NULL;
		m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bound._radius = 0.0f;
	}
	~CLight(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
	{
		if (NULL == pDevice)
			return false;
		if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
			return false;

		m_bound._center = lit.Position;
		m_bound._radius = radius;

		m_lit.Type = lit.Type;
		m_lit.Diffuse = lit.Diffuse;
		m_lit.Specular = lit.Specular;
		m_lit.Ambient = lit.Ambient;
		m_lit.Position = lit.Position;
		m_lit.Direction = lit.Direction;
		m_lit.Range = lit.Range;
		m_lit.Falloff = lit.Falloff;
		m_lit.Attenuation0 = lit.Attenuation0;
		m_lit.Attenuation1 = lit.Attenuation1;
		m_lit.Attenuation2 = lit.Attenuation2;
		m_lit.Theta = lit.Theta;
		m_lit.Phi = lit.Phi;
		return true;
	}
	void destroy(void)
	{
		if (m_pMesh != NULL) {
			m_pMesh->Release();
			m_pMesh = NULL;
		}
	}
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return false;

		D3DXVECTOR3 pos(m_bound._center);
		D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
		D3DXVec3TransformCoord(&pos, &pos, &mWorld);
		m_lit.Position = pos;

		pDevice->SetLight(m_index, &m_lit);
		pDevice->LightEnable(m_index, TRUE);
		return true;
	}

	void draw(IDirect3DDevice9* pDevice)
	{
		if (NULL == pDevice)
			return;
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
		pDevice->SetTransform(D3DTS_WORLD, &m);
		pDevice->SetMaterial(&d3d::WHITE_MTRL);
		m_pMesh->DrawSubset(0);
	}

	D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh* m_pMesh;
	d3d::BoundingSphere m_bound;
};

class Spawner {
private:
	list<CSphere*> g_bomb;
	list<CSphere*> g_marble;
	list<CSphere*>::iterator iter;

	float minForce = 1.4f;
	float maxForce = 1.9f;

	float random(float min, float max) {
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<float> dis(min, max);

		return dis(gen);
	}

	void spawnBomb() {
		float rand;
		CSphere* newBall = new CSphere();
		newBall->create(Device, d3d::BLACK);

		//플레이어 공 스폰되는 중간 라인에는 스폰이 안되도록 함
		do {
			rand = random(-2.3f, 2.4f);
		} while (rand < 0.7f && rand > -0.5f);

		newBall->setCenter(rand, (float)M_RADIUS, 2.6f);
		newBall->setPower(0, -random(minForce, maxForce));

		g_bomb.push_back(newBall);
	}

	void spawnMarble() {
		float rand;
		CSphere* newBall = new CSphere();
		newBall->create(Device, d3d::WHITE);

		do {
			rand = random(-2.3f, 2.4f);
		} while (rand < 0.5f && rand > -0.5f);

		newBall->setCenter(rand, (float)M_RADIUS, 2.6f);
		newBall->setPower(0, -random(minForce, maxForce));

		g_marble.push_back(newBall);
	}

public:

	list<CSphere*>::iterator getBombBegin() { return g_bomb.begin(); }
	list<CSphere*>::iterator getBombEnd() { return g_bomb.end(); }
	list<CSphere*>::iterator getMarbleBegin() { return g_marble.begin(); }
	list<CSphere*>::iterator getMarbleEnd() { return g_marble.end(); }

	void removeBomb(CSphere* target) { g_bomb.remove(target); }
	void removeMarble(CSphere* target) { g_marble.remove(target); }

	//4분의 1 확률로 구슬
	void spawnRandomBall() {
		int rand = random(1, 5);
		if (rand == 1) spawnMarble();
		else spawnBomb();
	}

	void drawAllBalls() {
		for (iter = g_bomb.begin(); iter != g_bomb.end(); iter++)
		{
			(*iter)->draw(Device, g_mWorld);
		}
		for (iter = g_marble.begin(); iter != g_marble.end(); iter++)
		{
			(*iter)->draw(Device, g_mWorld);
		}
	}
	void updateAllBalls(float timeDelta) {
		for (iter = g_bomb.begin(); iter != g_bomb.end(); iter++)
		{
			(*iter)->ballUpdate(timeDelta);
		}
		for (iter = g_marble.begin(); iter != g_marble.end(); iter++)
		{
			(*iter)->ballUpdate(timeDelta);
		}
	}

	void speedUp() {
		minForce += 0.1f;
		maxForce += 0.1f;
	}
};

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
CWall	g_legowall[4];
CSphere	g_target_blueball;
CLight	g_light;

Spawner spawner;
clock_t  uTime1, uTime2, sTime1, sTime2;
float speedUpDelay = 5.0f;
float spawnDelay = 1.0f;

double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
}

// initialization
bool Setup()
{
	int i;
	D3DXMATRIX a;
	D3DXMATRIX b;

	startTime = clock();
	uTime1 = startTime;
	sTime1 = startTime;

	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);

	// create plane and set the position	(?,?,?,가로길이,두께,세로길이,색깔)
	if (false == g_legoPlane.create(Device, -1, -1, 5, 0.03f, 6, d3d::WHITE)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);

	D3DXMatrixRotationY(&a, 0.0f);
	D3DXMatrixRotationX(&b, -7.0f);
	g_mWorld = g_mWorld * a * b;

	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 5, 0.3f, 0.12f, d3d::YELLOW)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 5, 0.3f, 0.12f, d3d::YELLOW)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::YELLOW)) return false;
	g_legowall[2].setPosition(2.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::YELLOW)) return false;
	g_legowall[3].setPosition(-2.56f, 0.12f, 0.0f);

	// create blue ball for set direction
	if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS, -2.5f);

	// light setting 
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	lit.Specular = d3d::WHITE * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	lit.Range = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;

	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);

	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
	g_legoPlane.destroy();
	for (int i = 0; i < 4; i++) {
		g_legowall[i].destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();
}

float timef = 0;

// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i = 0;
	int j = 0;

	if (Device)
	{
		if (!isGameOver) {
			Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
			Device->BeginScene();

			for (i = 0; i < 4; i++) {
				if (g_legowall[i].hasIntersected(g_target_blueball)) {
					g_legowall[i].hitBy(g_target_blueball, 1);
				}
			}

			list<CSphere*>::iterator iter;
			CSphere* removeBomb = 0;
			CSphere* removeMarble = 0;

			//bomb 리스트의 처음부터 끝까지 탐색하는 for문
			for (iter = spawner.getBombBegin(); iter != spawner.getBombEnd(); iter++) {
				if (g_legowall[1].hasIntersected(**iter)) {
					g_legowall[1].hitBy(**iter, 2);
				}
				if (g_target_blueball.hasIntersected(**iter)) {
					g_target_blueball.hitBy(**iter, true);
					removeBomb = *iter;
				}
			}
			if (removeBomb != 0) spawner.removeBomb(removeBomb);

			//marble 리스트의 처음부터 끝까지 탐색하는 for문
			for (iter = spawner.getMarbleBegin(); iter != spawner.getMarbleEnd(); iter++) {
				if (g_legowall[1].hasIntersected(**iter))
				{
					g_legowall[1].hitBy(**iter, 3);
					removeMarble = *iter;
				}
				if (g_target_blueball.hasIntersected(**iter)) {
					g_target_blueball.hitBy(**iter, false);
				}
			}
			if (removeMarble != 0) spawner.removeMarble(removeMarble);


			g_target_blueball.ballUpdate(timeDelta);
			spawner.updateAllBalls(timeDelta);

			uTime2 = clock();
			sTime2 = clock();
			if ((double)(uTime2 - uTime1) / CLOCKS_PER_SEC > speedUpDelay) {
				spawner.speedUp();
				uTime1 = clock();
			}
			if ((double)(sTime2 - sTime1) / CLOCKS_PER_SEC > spawnDelay) {
				spawner.spawnRandomBall();
				sTime1 = clock();
			}

			// draw plane, walls, and spheres
			g_legoPlane.draw(Device, g_mWorld);
			for (i = 0; i < 4; i++) {
				g_legowall[i].draw(Device, g_mWorld);
			}
			g_target_blueball.draw(Device, g_mWorld);

			spawner.drawAllBalls();

			g_light.draw(Device);

			Device->EndScene();
			Device->Present(0, 0, 0, 0);
			Device->SetTexture(0, NULL);
		}
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		float mX, mY, dx, dy;

		mX = (float)LOWORD(lParam) - (float)Width * 0.5f;	// 마우스 x 좌표
		mY = (Height - (float)HIWORD(lParam));				// 마우스 y 좌표

		g_target_blueball.setCenter(.0f, (float)M_RADIUS, -2.5f);

		dx = (mX - g_target_blueball.getCenter().x) * 0.01f;
		dy = (mY - 165) * 0.01f;							//시작점의 마우스 상 Y좌표가 165

		g_target_blueball.setPower(dx, dy);

		break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));

	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}