//------------------------------------------------------- ----------------------
// File: Animation.h
//-----------------------------------------------------------------------------

#pragma once

#define ANIMATION_TYPE_ONCE			0
#define ANIMATION_TYPE_LOOP			1
#define ANIMATION_TYPE_PINGPONG		2
#define ANIMATION_CALLBACK_EPSILON	0.015f

class CGameObject; 
class CSkinnedMesh; 

struct CALLBACKKEY
{
    float                          	 m_fTime = 0.0f;
    void                            *m_pCallbackData = NULL;
};
                    
#define _WITH_ANIMATION_INTERPOLATION

class CAnimationCallbackHandler
{
public:
    CAnimationCallbackHandler() { }
    ~CAnimationCallbackHandler() { }

public:
    virtual void HandleCallback(void* pCallbackData) { }
};


class CAnimationSet
{
public:
	CAnimationSet(float fLength, int nFramesPerSecond, int nKeyFrameTransforms, int nSkinningBones, char* pstrName);
	~CAnimationSet();

public:
	char							m_pstrAnimationSetName[64];

	float							m_fLength = 0.0f;
	int								m_nFramesPerSecond = 0; //m_fTicksPerSecond

	int								m_nKeyFrames = 0;
	float                         * m_pfKeyFrameTimes = NULL;
	XMFLOAT4X4                   ** m_ppxmf4x4KeyFrameTransforms = NULL;

	float 							m_fPosition = 0.0f;
	int 							m_nType = ANIMATION_TYPE_LOOP; //Once, Loop, PingPong

	int 							m_nCallbackKeys = 0;
	CALLBACKKEY                   * m_pCallbackKeys = NULL;

	CAnimationCallbackHandler     * m_pAnimationCallbackHandler = NULL;

public:
	void* GetCallbackData();

	void SetPosition(float fTrackPosition);

	XMFLOAT4X4 GetSRT(int nBone);

	void SetCallbackKeys(int nCallbackKeys);
	void SetCallbackKey(int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(CAnimationCallbackHandler* pCallbackHandler);

};


class CAnimationSets
{
private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	CAnimationSets(int nAnimationSets);
	~CAnimationSets();

public:
	int								m_nAnimationSets = 0;
	CAnimationSet                ** m_ppAnimationSets = NULL;

public:
	void SetCallbackKeys(int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationSet, CAnimationCallbackHandler* pCallbackHandler);
};


class CAnimationTrack
{
public:
	CAnimationTrack() { }
	~CAnimationTrack() { }

public:
	BOOL 							m_bEnable = true;
	float 							m_fSpeed = 1.0f;
	float 							m_fPosition = 0.0f;
	float 							m_fWeight = 1.0f;

	int 							m_nAnimationSet = 0;

public:
	void SetAnimationSet(int nAnimationSet) { m_nAnimationSet = nAnimationSet; }

	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetWeight(float fWeight) { m_fWeight = fWeight; }
	void SetPosition(float fPosition) { m_fPosition = fPosition; }
};

class CLoadedModelInfo
{
public:
	CLoadedModelInfo() { }
	~CLoadedModelInfo();

	CGameObject                      * m_pModelRootObject = NULL;
						   
	int 				  	   		   m_nSkinnedMeshes = 0;
	CSkinnedMesh                    ** m_ppSkinnedMeshes = NULL; //[SkinnedMeshes], Skinned Mesh Cache
						   
	CAnimationSets                  ** m_ppAnimationSets = NULL;
						   
	int                              * m_pnAnimatedBoneFrames = NULL; //[SkinnedMeshes]
	CGameObject                    *** m_pppAnimatedBoneFrameCaches = NULL; //[SkinnedMeshes][Bones]
};

class CAnimationController
{
public:
	CAnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, CLoadedModelInfo* pModel);
	~CAnimationController();

public:
	float 						    	m_fTime = 0.0f;

	int 							    m_nAnimationTracks = 0;
	CAnimationTrack                   * m_pAnimationTracks = NULL;

	int 							    m_nSkinnedMeshes = 0;

	CAnimationSets                   ** m_ppAnimationSets = NULL;
	CSkinnedMesh                     ** m_ppSkinnedMeshes = NULL; //[SkinnedMeshes], Skinned Mesh Cache

	int                               * m_pnAnimatedBoneFrames = NULL;
	CGameObject                     *** m_pppAnimatedBoneFrameCaches = NULL; //[SkinnedMeshes][Bones]

	ID3D12Resource                   ** m_ppd3dcbSkinningBoneTransforms = NULL; //[SkinnedMeshes]
	XMFLOAT4X4                       ** m_ppcbxmf4x4MappedSkinningBoneTransforms = NULL;

public:
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackEnable(int nAnimationTrack, bool bEnable);
	void SetTrackPosition(int nAnimationTrack, float fPosition);
	void SetTrackSpeed(int nAnimationTrack, float fSpeed);
	void SetTrackWeight(int nAnimationTrack, float fWeight);

	void SetCallbackKeys(int nSkinnedMesh, int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nSkinnedMesh, int nAnimationSet, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nSkinnedMesh, int nAnimationSet, CAnimationCallbackHandler* pCallbackHandler);

	void AdvanceTime(float fElapsedTime, CGameObject* pRootGameObject);
};