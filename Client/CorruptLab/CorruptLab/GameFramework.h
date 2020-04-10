#pragma once

#include "Timer.h"
#include "Shader_MRT.h"
#include "Object_Player.h"
#include "Scene.h"
#include "Shader_LightMRT.h"
#include "CShader_ShadowMRT.h"

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateSwapChainRenderTargetViews();
	void CreateDepthStencilView();
	

	void ChangeSwapChainState();

	void CreateOffScreenRenderTargetViews();
	void CreateLightRenderTargetViews();
	void CreateShadowRenderTargetViews();


	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE						    m_hInstance;
	HWND							    m_hWnd;

	int								    m_nWndClientWidth;
	int								    m_nWndClientHeight;

	IDXGIFactory4                     * m_pdxgiFactory = NULL;
	IDXGISwapChain3                   * m_pdxgiSwapChain = NULL;
	ID3D12Device                      * m_pd3dDevice = NULL;

	bool							    m_bMsaa4xEnable = false;
	UINT							    m_nMsaa4xQualityLevels = 0;

	static const UINT				    m_nSwapChainBuffers = 2;
	UINT							    m_nSwapChainBufferIndex = 0;

	ID3D12Resource                    * m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap              * m_pd3dRtvDescriptorHeap = NULL;
	ID3D12DescriptorHeap			  * m_pd3dLightDescriptorHeap = NULL;
	ID3D12DescriptorHeap              * m_pd3dShadowDescriptorHeap = NULL;


	UINT							    m_nRtvDescriptorIncrementSize;
	D3D12_CPU_DESCRIPTOR_HANDLE		    m_pd3dRtvSwapChainBackBufferCPUHandles[m_nSwapChainBuffers];

	ID3D12Resource                    * m_pd3dDepthStencilBuffer = NULL;
	ID3D12DescriptorHeap              * m_pd3dDsvDescriptorHeap = NULL;
	D3D12_CPU_DESCRIPTOR_HANDLE		    m_d3dDsvDepthStencilBufferCPUHandle;

	ID3D12CommandAllocator            * m_pd3dCommandAllocator = NULL;
	ID3D12CommandQueue                * m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList         * m_pd3dCommandList = NULL;

	ID3D12Fence                       * m_pd3dFence = NULL;
	UINT64							    m_nFenceValues[m_nSwapChainBuffers];
	HANDLE							    m_hFenceEvent;

	CGameTimer						    m_GameTimer;

	static const UINT				    m_nOffScreenRenderTargetBuffers = 5;
	ID3D12Resource                    * m_ppd3dOffScreenRenderTargetBuffers[m_nOffScreenRenderTargetBuffers];
	D3D12_CPU_DESCRIPTOR_HANDLE		    m_pd3dOffScreenRenderTargetBufferCPUHandles[m_nOffScreenRenderTargetBuffers];
	
	static const UINT				    m_nOffScreenLightBuffers = 1;
	ID3D12Resource                    * m_ppd3dLightMapRenderTargetBuffers[m_nOffScreenLightBuffers];
	D3D12_CPU_DESCRIPTOR_HANDLE		    m_pd3dOffScreenLightBufferCPUHandles[m_nOffScreenLightBuffers];

	static const UINT				    m_nOffScreenShadowBuffers = 1;
	ID3D12Resource                    * m_ppd3dShadowRenderTargetBuffers[m_nOffScreenShadowBuffers];
	D3D12_CPU_DESCRIPTOR_HANDLE		    m_pd3dOffScreenShadowBufferCPUHandles[m_nOffScreenShadowBuffers];

     POINT							    m_ptOldCursorPos;

	_TCHAR							    m_pszFrameRate[50];

public:
	CTexture                          * m_pFinalTexture = NULL;
	CTexture                          * m_pShadowMap = NULL;

private:
	CPostProcessingShader             * m_pPostProcessingShader = NULL;
	CLightTarget                      * m_pLightProcessingShader = NULL; 
	Shader_ShadowMRT                  * m_pShadowShader = NULL; 

	CGameScene                        * m_pScene = NULL;
	CPlayer                           * m_pPlayer = NULL;
	CCamera                           * m_pCamera = NULL;
};

