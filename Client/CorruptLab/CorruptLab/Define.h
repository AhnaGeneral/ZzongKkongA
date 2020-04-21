#pragma once

#define RANDOM_COLOR	XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))

//[ Scene ]=====================================
#define ROOT_PARAMETER_CAMERA				0
#define ROOT_PARAMETER_OBJECT				1 
#define ROOT_PARAMETER_PLAYER				1

#define ROOT_PARAMETER_ALBEDO_TEX		    2
#define ROOT_PARAMETER_SPECULAR_TEX		    3
#define ROOT_PARAMETER_NORMAL_TEX		    4
#define ROOT_PARAMETER_METALLIC_TEX		    5
#define ROOT_PARAMETER_EMISSION_TEX		    6

#define ROOT_PARAMETER_BONE_TRANSFORMS      7
#define ROOT_PARAMETER_BONE_OFFSETS         8

#define ROOT_PARAMETER_SKYBOX_TEX		    9
#define ROOT_PARAMETER_CLOUD_TEX		    10
#define ROOT_PARAMETER_TERRAIN_TEX		    11

#define ROOT_PARMAMETER_WATER_NORMAL_TEX	12
#define ROOT_PARAMETER_NOISEBUFFER		    13
#define ROOT_PARAMETER_DISTORTIONBUFFER     14
#define ROOT_PARAMETER_NOISE_TEX            15
#define ROOT_PARAMETER_FOG_TEX              16

#define ROOT_PARAMETER_SHADOWCAMERA	        17
#define ROOT_PARAMETER_SHADOWMAP	        18
#define ROOT_PARAMETER_SCENEDEPTHTEX	    19



//[ FinalMRT ]=====================================
#define ROOT_PARAMETER_CDN_MRT				0 
// 1.   ROOT_PARAMETER_OBJECT               1
#define ROOT_PARAMETER_ORTHO				2
#define ROOT_PARAMETER_LIGHT_MRT			3
#define ROOT_PARAMETER_SHADOW_MRT           4
//ForMinimap
#define ROOT_PARAMETER_PLAYER_POS			5
#define ROOT_PARAMETER_HP_TEX               6 



//[ LightMRT ]=====================================
// 0.   ROOT_PARAMETER_CDN_MRT              0
#define ROOT_PARAMETER_LIGHT				1
#define ROOT_PARAMETER_CAMERA2				2 


//[LobbyScene]=====================================
#define ROOT_PARAMETER_LOBBYTEX             0


//[ Object Type ]====================================
//Static
#define OBJECT_TYPE_ROCK				0
#define OBJECT_TYPE_FENCE				1
#define OBJECT_TYPE_TRMTOWER			2

// Dynamic
#define OBJECT_TYPE_ITEMBOX				0

//---------------------------------------------------
#define MONSTER_TYPE_YANGMAL			0

#define MONSTER_STATE_IDLE				0
#define MONSTER_STATE_WALK				1
#define MONSTER_STATE_ATTACK			2
//---------------------------------------------------
#define JOHNSON_ANIAMATION_IDLE			0
#define JOHNSON_ANIAMATION_WALK			1
#define JOHNSON_ANIAMATION_RUN			2


#define EPSILON						1.0e-10f