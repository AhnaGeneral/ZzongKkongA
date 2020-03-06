#pragma once


#define RANDOM_COLOR	XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))

// ROOT PARAMETER =====================================
#define ROOT_PARAMETER_CAMERA				0
#define ROOT_PARAMETER_OBJECT				1
#define ROOT_PARAMETER_PLAYER				1

#define ROOT_PARAMETER_LIGHT				2
#define ROOT_PARAMETER_ALBEDO_TEX		    3
#define ROOT_PARAMETER_SPECULAR_TEX		    4
#define ROOT_PARAMETER_NORMAL_TEX		    5
#define ROOT_PARAMETER_METALLIC_TEX		    6
#define ROOT_PARAMETER_EMISSION_TEX		    7
#define ROOT_PARAMETER_DETAIL_ALEBEDO_TEX	8
#define ROOT_PARAMETER_DETAIL_NORMAL_TEX	9
#define ROOT_PARAMETER_BONE_TRANSFORMS      10
#define ROOT_PARAMETER_BONE_OFFSETS         11
#define ROOT_PARAMETER_TERRAIN_TEX		    12
#define ROOT_PARAMETER_SKYBOX_TEX		    13
#define ROOT_PARAMETER_CLOUD_TEX		    14

#define ROOT_PARAMETER_ORTHO				1

//=====================================================



//---------------------------------------------------


#define JOHNSON_ANIAMATION_IDLE			0
#define JOHNSON_ANIAMATION_WALK			1
#define JOHNSON_ANIAMATION_RUN			2



#define EPSILON						1.0e-10f