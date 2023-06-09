#pragma once
#include "Vector.h"
#include "cvar.h"

struct ClientClass;

#pragma region Defines

//LIFESTATE
#define	LIFE_ALIVE				0
#define	LIFE_DYING				1
#define	LIFE_DEAD				2
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

//USERCMD BUTTONS
#define IN_ATTACK		(1 << 0) 
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client_panorama.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

#pragma endregion

#pragma region Classes

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

//enum class CSGOHitboxID
//{
//	Head = 0,
//	Neck,
//	Pelvis,
//	Stomach,
//	LowerChest,
//	Chest,
//	UpperChest,
//	RightThigh,
//	LeftThigh,
//	RightShin,
//	LeftShin,
//	LeftFoot,
//	RightFoot,
//	RightHand,
//	LeftHand,
//	RightUpperArm,
//	RightLowerArm,
//	LeftUpperArm,
//	LeftLowerArm,
//};

enum CSGOHitboxID : int {
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	LeftFoot,
	RightFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
	hitbox_max
};

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

struct player_info_t
{
	__int64         unknown;            //0x0000 
	union
	{
		__int64       steamID64;          //0x0008 - SteamID64
		struct
		{
			__int32     xuidLow;
			__int32     xuidHigh;
		};
	};
	char            name[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            friendsName[128];
	bool            fakeplayer;
	bool            ishltv;
	unsigned int    customFiles[4];
	unsigned char   filesDownloaded;
};

#pragma endregion

#pragma region ShitClasses

struct datamap_t;
class typedescription_t;

enum
{
	TD_OFFSET_NORMAL = 0,
	TD_OFFSET_PACKED = 1,

	// Must be last
	TD_OFFSET_COUNT,
};


class typedescription_t
{
public:
	int32_t fieldType; //0x0000
	char* fieldName; //0x0004
	int fieldOffset[TD_OFFSET_COUNT]; //0x0008
	int16_t fieldSize_UNKNWN; //0x0010
	int16_t flags_UNKWN; //0x0012
	char pad_0014[12]; //0x0014
	datamap_t* td; //0x0020
	char pad_0024[24]; //0x0024
}; //Size: 0x003C

struct datamap_t
{
	typedescription_t    *dataDesc;
	int                    dataNumFields;
	char const            *dataClassName;
	datamap_t            *baseMap;

	bool                chains_validated;
	// Have the "packed" offsets been computed
	bool                packed_offsets_computed;
	int                    packed_size;
};

class CCSWeaponInfo
{
public:
	std::byte	m_pad0[0x14];				// 0x0000
	int			m_iMaxClip1;				// 0x0014
	int			m_iMaxClip2;				// 0x0018
	int			m_iDefaultClip1;			// 0x001C
	int			m_iDefaultClip2;			// 0x0020
	int			m_iPrimaryMaxReserveAmmo; // 0x0024
	int			m_iSecondaryMaxReserveAmmo; // 0x0028
	char*		m_szWorldModel;			// 0x002C
	char*		m_szViewModel;			// 0x0030
	char*		m_szDroppedModel;			// 0x0034
	std::byte	m_pad1[0x50];				// 0x0038
	char*		m_szHudName;				// 0x0088
	char*		m_szWeaponName;			// 0x008C
	std::byte	m_pad2[0x2];				// 0x0090
	bool		m_bIsMeleeWeapon;			// 0x0092
	std::byte	m_pad3[0x9];				// 0x0093
	float		m_flWeaponWeight;			// 0x009C
	std::byte	m_pad4[0x28];				// 0x00A0
	int			m_iWeaponType;			// 0x00C8
	std::byte	m_pad5[0x4];				// 0x00CC
	int			m_iWeaponPrice;			// 0x00D0
	int			m_iKillAward;				// 0x00D4
	std::byte	m_pad6[0x4];				// 0x00D8
	float		m_flCycleTime;			// 0x00DC
	float		m_flCycleTimeAlt;			// 0x00E0
	std::byte	m_pad7[0x8];				// 0x00E4
	bool		m_bFullAuto;				// 0x00EC
	std::byte	m_pad8[0x3];				// 0x00ED
	int			m_iDamage;				// 0x00F0
	float       m_flHeadshotMultiplier;
	float		m_flArmorRatio;			// 0x00F4
	int			m_iBullets;				// 0x00F8
	float		m_flPenetration;			// 0x00FC
	std::byte	m_pad9[0x8];				// 0x0100
	float		m_flRange;				// 0x0108
	float		m_flRangeModifier;		// 0x010C
	float		m_flThrowVelocity;		// 0x0110
	std::byte	m_pad10[0xC];				// 0x0114
	bool		m_bHasSilencer;			// 0x0120
	std::byte	m_pad11[0xB];				// 0x0121
	char*		m_szBulletType;			// 0x012C
	float		m_flMaxSpeed;				// 0x0130
	float		m_flMaxSpeedAlt;			// 0x0134
	float		m_flSpread;				// 0x0138
	float		m_flSpreadAlt;			// 0x013C
	std::byte	m_pad12[0x44];			// 0x0180
	int			m_iRecoilSeed;			// 0x0184
	std::byte	m_pad13[0x20];			// 0x0188
};

class CGameTrace;
struct Ray_t;
typedef CGameTrace trace_t;

struct brushdata_t
{
	void* pShared;
	int				firstmodelsurface;
	int				nummodelsurfaces;

	// track the union of all lightstyles on this brush.  That way we can avoid
	// searching all faces if the lightstyle hasn't changed since the last update
	int				nLightstyleLastComputedFrame;
	unsigned short	nLightstyleIndex;	// g_ModelLoader actually holds the allocated data here
	unsigned short	nLightstyleCount;

	unsigned short	renderHandle;
	unsigned short	firstnode;
};

struct spritedata_t
{
	int				numframes;
	int				width;
	int				height;
	void* sprite;
};

struct model_t
{
	void* fnHandle;
	char name[260];
	__int32 nLoadFlags;
	__int32 nServerCount;
	__int32 type;
	__int32 flags;
	Vector mins, maxs;
	float radius;
	void* m_pKeyValues;
	union
	{
		brushdata_t brush;
		MDLHandle_t studio;
		spritedata_t sprite;
	};
};

class IHandleEntity;
class ICollideable
{
public:
	virtual IHandleEntity*      GetEntityHandle() = 0;
	virtual Vector&				OBBMins() const = 0;
	virtual Vector&				OBBMaxs() const = 0;
	virtual void world_space_trigger_bounds(Vector* mins, Vector* maxs) const = 0;
	virtual bool test_collision(const Ray_t& ray, unsigned int mask, trace_t& trace) = 0;
	virtual bool test_hitboxes(const Ray_t& ray, unsigned int mask, trace_t& trace) = 0;
	virtual int get_collision_model_index() = 0;
	virtual const model_t* get_collision_model() = 0;
	virtual Vector&	get_collision_origin() const = 0;
	virtual Vector& get_collision_angles() const = 0;
	virtual const matrix& collision_to_world_transform() const = 0;
	virtual int get_solid() const = 0;
	virtual int get_solid_flags() const = 0;
	virtual void* get_client_unknown() = 0;
	virtual int get_collision_group() const = 0;
	virtual void world_space_surrounding_bounds(Vector* mins, Vector* maxs) = 0;
	virtual bool should_touch_trigger(int flags) const = 0;
	virtual const matrix* get_root_parent_to_world_transform() const = 0;
};

class IBaseCombatWeapon;
class IBasePlayer;
class IClientUnknown;

typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8;
class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual Vector& const GetRenderOrigin(void) = 0;
	virtual Vector& const GetRenderAngles(void) = 0;
	virtual bool ShouldDraw(void) = 0;
	virtual int GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
	virtual void Unused(void) const
	{
	}
	virtual HANDLE GetShadowHandle() const = 0;
	virtual HANDLE& RenderHandle() = 0;
	virtual const model_t* GetModel() const = 0;
	virtual int DrawModel(int flags, const int /*RenderableInstance_t*/ & instance) = 0;
	virtual int GetBody() = 0;
	virtual void Getcolor_tModulation(float* color) = 0;
	virtual bool LODTest() = 0;
	virtual void SetupWeights(const matrix* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void DoAnimationEvents(void) = 0;
	virtual void* /*IPVSNotify*/ GetPVSNotifyInterface() = 0;
	virtual void GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual void GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	virtual void GetShadowRenderBounds(Vector& mins, Vector& maxs, int /*ShadowType_t*/ shadowType) = 0;
	virtual bool ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool GetShadowCastDistance(float* pDist, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool GetShadowCastDirection(Vector* pDirection, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool IsShadowDirty() = 0;
	virtual void MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual int /*ShadowType_t*/ ShadowCastType() = 0;
	virtual void CreateModelInstance() = 0;
	virtual HANDLE GetModelInstance() = 0;
	virtual const matrix& RenderableToWorldTransform() = 0;
	virtual int LookupAttachment(const char* pAttachmentName) = 0;
	virtual bool GetAttachment(int number, Vector& origin, Vector& angles) = 0;
	virtual bool GetAttachment(int number, matrix& matrix) = 0;
	virtual float* GetRenderClipPlane(void) = 0;
	virtual int GetSkin() = 0;
	virtual void OnThreadedDrawSetup() = 0;
	virtual bool UsesFlexDelayedWeights() = 0;
	virtual void RecordToolMessage() = 0;
	virtual bool ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual uint8 OverrideAlphaModulation(uint8 nAlpha) = 0;
	virtual uint8 OverrideShadowAlphaModulation(uint8 nAlpha) = 0;
};
class IClientNetworkable
{
public:
	virtual IClientUnknown*  GetIClientUnknown() = 0;
	virtual void             Release() = 0;
	virtual ClientClass*     GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void             __unkn(void) = 0;
	virtual bool             IsDormant(void) = 0;
	virtual int              EntIndex(void) const = 0;
	virtual void             ReceiveMessage() = 0;
	virtual void*            GetDataTableBasePtr() = 0;
	virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};

class IClientThinkable;
using CBaseHandle = uint32_t;
class IHandleEntity
{
public:
	virtual ~IHandleEntity() = default;
	virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};



class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable*       GetCollideable() = 0;
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual IBasePlayer*		GetIClientEntity() = 0;
	virtual IBasePlayer*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};

class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};
struct studiohdr_t;
struct mstudiohitboxset_t;
struct mstudiobbox_t
{
	int     bone;                 // 0x0000
	int     group;                // 0x0004
	Vector  bbmin;                 // 0x0008
	Vector  bbmax;                 // 0x0014
	int     name_id;				// 0x0020
	Vector   rotation;                // 0x0024
	float   radius;               // 0x0030
	PAD(0x10);                    // 0x0034
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	//    DATA_UPDATE_ENTERED_PVS,
	DATA_UPDATE_DATATABLE_CHANGED
	//    DATA_UPDATE_LEFT_PVS,
	//DATA_UPDATE_DESTROYED,
};

class CAnimationLayer
{
public:
	bool            m_bClientBlend;            //0x00
	float            m_flBlendIn;                //0x04
	void* m_pStudioHdr;                //0x08
	int                m_nDispatchedSrc;            //0x0C
	int                m_nDispatchedDst;            //0x10
	std::uintptr_t  m_iOrder;                    //0x14
	std::uintptr_t  m_nSequence;                //0x18
	float            m_flPrevCycle;            //0x1C
	float            m_flWeight;                //0x20
	float            m_flWeightDeltaRate;        //0x24
	float            m_flPlaybackRate;            //0x28
	float            m_flCycle;                //0x2C
	void* m_pOwner;                    //0x30
	int                m_nInvalidatePhysicsBits;    //0x34
};

enum animstate_layer_t
{
	ANIMATION_LAYER_AIMMATRIX = 0,
	ANIMATION_LAYER_WEAPON_ACTION,
	ANIMATION_LAYER_WEAPON_ACTION_RECROUCH,
	ANIMATION_LAYER_ADJUST,
	ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL,
	ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB,
	ANIMATION_LAYER_MOVEMENT_MOVE,
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE,
	ANIMATION_LAYER_WHOLE_BODY,
	ANIMATION_LAYER_FLASHED,
	ANIMATION_LAYER_FLINCH,
	ANIMATION_LAYER_ALIVELOOP,
	ANIMATION_LAYER_LEAN,
	ANIMATION_LAYER_COUNT,
};

#define USE_ANIMLAYER_RAW_INDEX false
typedef const int* animlayerpreset;
#define get_animlayerpreset( _n ) s_animLayerOrder ## _n
#define REGISTER_ANIMLAYER_ORDER( _n ) static const int s_animLayerOrder ## _n [ANIMATION_LAYER_COUNT]

REGISTER_ANIMLAYER_ORDER(Default) = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

// animations can trigger the player to re-order their layers according to hardcoded presets, e.g.:
REGISTER_ANIMLAYER_ORDER(WeaponPost) = {
	ANIMATION_LAYER_AIMMATRIX,
	ANIMATION_LAYER_WEAPON_ACTION_RECROUCH,
	ANIMATION_LAYER_ADJUST,
	ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL,
	ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB,
	ANIMATION_LAYER_MOVEMENT_MOVE,
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE,
	ANIMATION_LAYER_WEAPON_ACTION,
	ANIMATION_LAYER_WHOLE_BODY,
	ANIMATION_LAYER_FLASHED,
	ANIMATION_LAYER_FLINCH,
	ANIMATION_LAYER_ALIVELOOP,
	ANIMATION_LAYER_LEAN,
};

enum animstate_pose_param_idx_t
{
	PLAYER_POSE_PARAM_FIRST = 0,
	PLAYER_POSE_PARAM_LEAN_YAW = PLAYER_POSE_PARAM_FIRST,
	PLAYER_POSE_PARAM_SPEED,
	PLAYER_POSE_PARAM_LADDER_SPEED,
	PLAYER_POSE_PARAM_LADDER_YAW,
	PLAYER_POSE_PARAM_MOVE_YAW,
	PLAYER_POSE_PARAM_RUN,
	PLAYER_POSE_PARAM_BODY_YAW,
	PLAYER_POSE_PARAM_BODY_PITCH,
	PLAYER_POSE_PARAM_DEATH_YAW,
	PLAYER_POSE_PARAM_STAND,
	PLAYER_POSE_PARAM_JUMP_FALL,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_IDLE,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_IDLE,
	PLAYER_POSE_PARAM_STRAFE_DIR,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_WALK,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_RUN,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_RUN,
	PLAYER_POSE_PARAM_MOVE_BLEND_CROUCH_WALK,
	//PLAYER_POSE_PARAM_STRAFE_CROSS,
	PLAYER_POSE_PARAM_COUNT,
};

struct animstate_pose_param_cache_t
{
	bool		m_initialized;
	int			m_index;
	const char* m_name;

	animstate_pose_param_cache_t()
	{
		m_initialized = false;
		m_index = -1;
		m_name = "";
	}

	int		get_index(void);
	float	get_value(IBasePlayer* pPlayer);
	void	set_value(IBasePlayer* pPlayer, float flValue);
	//bool	init(C_BasePlayer* pPlayer, const char* szPoseParamName);
};

#define CSGO_ANIM_AIMMATRIX_DEFAULT_YAW_MAX 58.0f
#define CSGO_ANIM_AIMMATRIX_DEFAULT_YAW_MIN -58.0f
#define CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MAX 90.0f
#define CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MIN -90.0f

class CCSGOPlayerAnimState
{
public:
	/*typedef CCSGOPlayerAnimState ThisClass;

CCSGOPlayerAnimState(C_BasePlayer* m_player);*/
	CCSGOPlayerAnimState() {};
	CCSGOPlayerAnimState(const CCSGOPlayerAnimState& animstate)
	{
		memcpy(this, &animstate, sizeof(CCSGOPlayerAnimState));
	};

	void reset(void);
	const char* GetWeaponPrefix(void);
	void release(void) { delete this; }
	//float get_primary_cycle(void) { return m_primary_cycle; }



	int* m_layer_order_preset = nullptr;
	bool					m_first_run_since_init = false;

	bool					m_first_foot_plant_since_init = false;
	int						m_last_update_tick = 0;
	float					m_eye_position_smooth_lerp = 0.0f;

	float					m_strafe_change_weight_smooth_fall_off = 0.0f;

	float	m_stand_walk_duration_state_has_been_valid = 0.0f;
	float	m_stand_walk_duration_state_has_been_invalid = 0.0f;
	float	m_stand_walk_how_long_to_wait_until_transition_can_blend_in = 0.0f;
	float	m_stand_walk_how_long_to_wait_until_transition_can_blend_out = 0.0f;
	float	m_stand_walk_blend_value = 0.0f;

	float	m_stand_run_duration_state_has_been_valid = 0.0f;
	float	m_stand_run_duration_state_has_been_invalid = 0.0f;
	float	m_stand_run_how_long_to_wait_until_transition_can_blend_in = 0.0f;
	float	m_stand_run_how_long_to_wait_until_transition_can_blend_out = 0.0f;
	float	m_stand_run_blend_value = 0.0f;

	float	m_crouch_walk_duration_state_has_been_valid = 0.0f;
	float	m_crouch_walk_duration_state_has_been_invalid = 0.0f;
	float	m_crouch_walk_how_long_to_wait_until_transition_can_blend_in = 0.0f;
	float	m_crouch_walk_how_long_to_wait_until_transition_can_blend_out = 0.0f;
	float	m_crouch_walk_blend_value = 0.0f;

	//aimmatrix_transition_t	m_stand_walk_aim = {};
	//aimmatrix_transition_t	m_stand_run_aim = {};
	//aimmatrix_transition_t	m_crouch_walk_aim = {};

	int						m_cached_model_index = 0;

	float					m_step_height_left = 0.0f;
	float					m_step_height_right = 0.0f;

	void* m_weapon_last_bone_setup = nullptr;

	void* m_player = nullptr;//0x0060 
	void* m_weapon = nullptr;//0x0064
	void* m_weapon_last = nullptr;//0x0068

	float					m_last_update_time = 0.0f;//0x006C	
	int						m_last_update_frame = 0;//0x0070 
	float					m_last_update_increment = 0.0f;//0x0074 

	float					m_eye_yaw = 0.0f; //0x0078 
	float					m_eye_pitch = 0.0f; //0x007C 
	float					m_abs_yaw = 0.0f; //0x0080 
	float					m_abs_yaw_last = 0.0f; //0x0084 
	float					m_move_yaw = 0.0f; //0x0088 
	float					m_move_yaw_ideal = 0.0f; //0x008C 
	float					m_move_yaw_current_to_ideal = 0.0f; //0x0090 	
	float					m_time_to_align_lower_body;

	float					m_primary_cycle = 0.0f; //0x0098
	float					m_move_weight = 0.0f; //0x009C 

	float					m_move_weight_smoothed = 0.0f;
	float					m_anim_duck_amount = 0.0f; //0x00A4
	float					m_duck_additional = 0.0f; //0x00A8
	float					m_recrouch_weight = 0.0f;

	Vector					m_position_current = Vector(); //0x00B0
	Vector					m_position_last = Vector(); //0x00BC 

	Vector					m_velocity = Vector(); //0x00C8
	Vector					m_velocity_normalized = Vector(); // 
	Vector					m_velocity_normalized_non_zero = Vector(); //0x00E0
	float					m_velocity_length_xy = 0.0f; //0x00EC
	float					m_velocity_length_z = 0.0f; //0x00F0

	float					m_speed_as_portion_of_run_top_speed = 0.0f; //0x00F4
	float					m_speed_as_portion_of_walk_top_speed = 0.0f; //0x00F8 
	float					m_speed_as_portion_of_crouch_top_speed = 0.0f; //0x00FC

	float					m_duration_moving = 0.0f; //0x0100
	float					m_duration_still = 0.0f; //0x0104

	bool					m_on_ground = false; //0x0108 

	bool					m_landing = false; //0x0109
	float					m_jump_to_fall = 0.0f;
	float					m_duration_in_air = 0.0f; //0x0110
	float					m_left_ground_height = 0.0f; //0x0114 
	float					m_land_anim_multiplier = 0.0f; //0x0118 

	float					m_walk_run_transition = 0.0f; //0x011C

	bool					m_landed_on_ground_this_frame = false;
	bool					m_left_the_ground_this_frame = false;
	float					m_in_air_smooth_value = 0.0f;

	bool					m_on_ladder = false; //0x0124
	float					m_ladder_weight = 0.0f; //0x0128
	float					m_ladder_speed = 0.0f;

	bool					m_walk_to_run_transition_state = false;

	bool					m_defuse_started = false;
	bool					m_plant_anim_started = false;
	bool					m_twitch_anim_started = false;
	bool					m_adjust_started = false;

	//CUtlVector<int>		m_activity_modifiers = {};
	char					m_activity_modifiers_server[20] = {};

	float					m_next_twitch_time = 0.0f;

	float					m_time_of_last_known_injury = 0.0f;

	float					m_last_velocity_test_time = 0.0f;
	Vector					m_velocity_last = Vector();
	Vector					m_target_acceleration = Vector();
	Vector					m_acceleration = Vector();
	float					m_acceleration_weight = 0.0f;

	float					m_aim_matrix_transition = 0.0f;
	float					m_aim_matrix_transition_delay = 0.0f;

	bool					m_flashed = false;

	float					m_strafe_change_weight = 0.0f;
	float					m_strafe_change_target_weight = 0.0f;
	float					m_strafe_change_cycle = 0.0f;
	int						m_strafe_sequence = 0;
	bool					m_strafe_changing = false;
	float					m_duration_strafing = 0.0f;

	float					m_foot_lerp = 0.0f;

	bool					m_feet_crossed = false;

	bool					m_player_is_accelerating = false;

	animstate_pose_param_cache_t m_pose_param_mappings[PLAYER_POSE_PARAM_COUNT] = {};

	float					m_duration_move_weight_is_too_high = 0.0f;
	float					m_static_approach_speed = 0.0f;

	int						m_previous_move_state = 0;
	float					m_stutter_step = 0.0f;

	float					m_action_weight_bias_remainder = 0.0f;

	Vector m_foot_left_pos_anim = Vector();
	Vector m_foot_left_pos_anim_last = Vector();
	Vector m_foot_left_pos_plant = Vector();
	Vector m_foot_left_plant_vel = Vector();
	float m_foot_left_lock_amount = 0.0f;
	float m_foot_left_last_plant_time = 0.0f;

	Vector m_foot_right_pos_anim = Vector();
	Vector m_foot_right_pos_anim_last = Vector();
	Vector m_foot_right_pos_plant = Vector();
	Vector m_foot_right_plant_vel = Vector();
	float m_foot_right_lock_amount = 0.0f;
	float m_foot_right_last_plant_time = 0.0f;

	float					m_camera_smooth_height = 0.0f;
	bool					m_smooth_height_valid = false;
	float					m_last_time_velocity_over_ten = 0.0f;

	float					m_aim_yaw_min = 0.0f;//0x0330
	float					m_aim_yaw_max = 0.0f;//0x0334
	float					m_aim_pitch_min = 0.0f;
	float					m_aim_pitch_max = 0.0f;

	int						m_animstate_model_version = 0;
};

class CBoneAccessor
{
public:
	inline matrix* get_bone_array_for_write(void) const
	{
		return m_pBones;
	}
	inline void set_bone_array_for_write(matrix* bonearray)
	{
		m_pBones = bonearray;
	}
	alignas(16) matrix* m_pBones;

	int m_ReadableBones;		// Which bones can be read.
	int m_WritableBones;		// Which bones can be written.
};
#pragma endregion

#define offset(func, type, offset) type& func { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }

#define NO_INTERP 8
enum
{
	EFL_KILLME = (1 << 0),	// This entity is marked for death -- This allows the game to actually delete ents at a safe time
	EFL_DORMANT = (1 << 1),	// Entity is dormant, no updates to client
	EFL_NOCLIP_ACTIVE = (1 << 2),	// Lets us know when the noclip command is active.
	EFL_SETTING_UP_BONES = (1 << 3),	// Set while a model is setting up its bones.
	EFL_KEEP_ON_RECREATE_ENTITIES = (1 << 4), // This is a special entity that should not be deleted when we restart entities only

	EFL_DIRTY_SHADOWUPDATE = (1 << 5),	// Client only- need shadow manager to update the shadow...
	EFL_NOTIFY = (1 << 6),	// Another entity is watching events on this entity (used by teleport)

	// The default behavior in ShouldTransmit is to not send an entity if it doesn't
	// have a model. Certain entities want to be sent anyway because all the drawing logic
	// is in the client DLL. They can set this flag and the engine will transmit them even
	// if they don't have a model.
	EFL_FORCE_CHECK_TRANSMIT = (1 << 7),

	EFL_BOT_FROZEN = (1 << 8),	// This is set on bots that are frozen.
	EFL_SERVER_ONLY = (1 << 9),	// Non-networked entity.
	EFL_NO_AUTO_EDICT_ATTACH = (1 << 10), // Don't attach the edict; we're doing it explicitly

	// Some dirty bits with respect to abs computations
	EFL_DIRTY_ABSTRANSFORM = (1 << 11),
	EFL_DIRTY_ABSVELOCITY = (1 << 12),
	EFL_DIRTY_ABSANGVELOCITY = (1 << 13),
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS = (1 << 14),
	EFL_DIRTY_SPATIAL_PARTITION = (1 << 15),
	EFL_HAS_PLAYER_CHILD = (1 << 16),	// One of the child entities is a player.

	EFL_IN_SKYBOX = (1 << 17),	// This is set if the entity detects that it's in the skybox.
	// This forces it to pass the "in PVS" for transmission.
	EFL_USE_PARTITION_WHEN_NOT_SOLID = (1 << 18),	// Entities with this flag set show up in the partition even when not solid
	EFL_TOUCHING_FLUID = (1 << 19),	// Used to determine if an entity is floating
	
	// FIXME: Not really sure where I should add this...
	EFL_IS_BEING_LIFTED_BY_BARNACLE = (1 << 20),
	EFL_NO_ROTORWASH_PUSH = (1 << 21),		// I shouldn't be pushed by the rotorwash
	EFL_NO_THINK_FUNCTION = (1 << 22),
	EFL_NO_GAME_PHYSICS_SIMULATION = (1 << 23),
	
	EFL_CHECK_UNTOUCH = (1 << 24),
	EFL_DONTBLOCKLOS = (1 << 25),		// I shouldn't block NPC line-of-sight
	EFL_DONTWALKON = (1 << 26),		// NPC;s should not walk on this entity
	EFL_NO_DISSOLVE = (1 << 27),		// These guys shouldn't dissolve
	EFL_NO_MEGAPHYSCANNON_RAGDOLL = (1 << 28),	// Mega physcannon can't ragdoll these guys.
	EFL_NO_WATER_VELOCITY_CHANGE = (1 << 29),	// Don't adjust this entity's velocity when transitioning into water
	EFL_NO_PHYSCANNON_INTERACTION = (1 << 30),	// Physcannon can't pick these up or punt them
	EFL_NO_DAMAGE_FORCES = (1 << 31),	// Doesn't accept forces from physics damage
};

class CStudioHdr;
class Quaternion;

enum InvalidatePhysicsBits_t
{
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,		// Means cycle has changed, or any other event which would cause render-to-texture shadows to need to be rerendeded
	BOUNDS_CHANGED = 0x10,		// Means render bounds have changed, so shadow decal projection is required, etc.
	SEQUENCE_CHANGED = 0x20,		// Means sequence has changed, only interesting when surrounding bounds depends on sequence																				
};

class CUserCmd;
class IBaseViewModel;

class IBasePlayer : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	__forceinline int GetIndex() {
		return EntIndex();
	}

	float& m_flNextAttack();
	Vector& GetMins();
	Vector& GetMaxs();
	__forceinline HANDLE GetObserverTargetHandle();
	Vector& GetAbsVelocity();
	int &GetGroundEntity();
	void InvalidateBoneCache();
	float &HealthShotBoostExpirationTime();
	void ResetAnimationState(CCSGOPlayerAnimState *state);
	void CreateAnimationState(CCSGOPlayerAnimState *state);
	void UpdateAnimationState(CCSGOPlayerAnimState *state, Vector angle);
	IBasePlayer* GetDefuser();
	float GetTimerLength();
	float GetDefuseCooldown();
	float GetC4Blow();
	float GetBombTicking();
	bool IsBombDefused();
	int* GetButtons();
	int& GetButtonLast();
	int& GetButtonPressed();
	int& GetButtonReleased();

	int m_fireCount();
	bool* m_bFireIsBurning();
	int* m_fireXDelta();
	int* m_fireYDelta();
	int* m_fireZDelta();

	bool m_bDidSmokeEffect();
	int m_nSmokeEffectTickBegin();
	int GetButtonDisabled();
	int GetButtonForced();
	int* GetNextThinkTick();
	int &GetTickBasePtr();
	float GetLastSeenTime();
	
	int& GetTakeDamage() {
		return *(int*)((DWORD)this + 0x280);
	}

	int GetHitboxSet();
	std::string GetName();
	bool PhysicsRunThink(int unk01);

	int GetRagdollHandle();
	void SetModelIndex(int model);
	int& GetModelIndex();
	int GetNumAnimOverlays()
	{
		return 13; //13
	}

	int GetMoveType()
	{
		return *(int*)((DWORD)this + 0x25C);
	}

	CAnimationLayer *GetAnimOverlays()
	{
		return *(CAnimationLayer**)((DWORD)this + 0x2990);
	}

	CAnimationLayer *GetAnimOverlay(int index)
	{
		if (index < 15)
			return &GetAnimOverlays()[index];
		else
			return nullptr;
	}

	bool& GetClientSideAnims();
	float GetDSYDelta();
	CUtlVector<CAnimationLayer>& GetAnimOverlayVector();
	void SetAbsAngles(const Vector &angles);
	void SetAbsOrigin(const Vector &origin);
	float_t m_surfaceFriction();
	datamap_t *GetDataDescMap();
	datamap_t *GetPredDescMap();
	bool IsWeapon();
	std::array<float, 24> &m_flPoseParameter();
	float& GetFlashDuration();
	bool IsFlashed();
	bool HaveDefuser();
	Vector GetPunchAngle();
	Vector& GetPunchAngleVel();
	Vector* GetPunchAnglePtr();
	Vector GetViewPunchAngle();
	Vector* GetViewPunchAnglePtr();
	
	void UpdateClientSideAnimation()
	{
		typedef void(__thiscall *o_updateClientSideAnimation)(void*);
		getvfunc<o_updateClientSideAnimation>(this, 224)(this);
	}

	IBaseCombatWeapon* GetWeapon();
	int GetArmor();

	bool isAlive()
	{
		if (!this)
			return false;
		BYTE lifestate = *(BYTE*)((DWORD)this + 0x25F);
		return (lifestate == 0);
	}

	bool IsValid();
	
	uint32_t& GetOcclusionFlags()
	{
		return *(uint32_t*)((DWORD)this + 0xA28);
	}

	uint32_t& GetOcclusionFramecount()
	{
		return *(uint32_t*)((DWORD)this + 0xA30);
	}

	uint32_t& GetReadableBones()
	{
		return *(uint32_t*)((DWORD)this + 0x26AC);
	}

	uint32_t& GetWritableBones()
	{
		return *(uint32_t*)((DWORD)this + 0x26B0);
	}

	matrix* &GetBoneArrayForWrite()
	{
		return *(matrix**)((DWORD)this + 0x26A8);
	}

	void InvalidatePhysicsRecursive(int32_t flags);
	CCSGOPlayerAnimState* GetPlayerAnimState();
	void SetRenderMode(int mode, bool forceUpdate);
	void SetAlpha(int alpha);
	void SetAnimState(CCSGOPlayerAnimState* anims);
	Vector& GetVecForce();

	int GetFlags()
	{
		return *(int*)((DWORD)this + 0x104);
	}

	int &GetFlagsPtr()
	{
		return *(int*)((DWORD)this + 0x104);
	}

	int& GetEFlags();
	int& GetEffects();

	int GetHealth()
	{
		if (!this)
			return -1;
		return *(int*)((DWORD)this + 0x100);
	}

	void ParsePoseParameter(std::array<float, 24>& param);
	void SetPoseParameter(std::array<float, 24> param);
	void ParseAnimOverlays(CAnimationLayer* anim);
	void SetAnimOverlays(CAnimationLayer* anim);
	bool ComputeHitboxSurroundingBox(Vector* mins, Vector* maxs);
	bool& TargetSpotted();
	CStudioHdr* GetModelPtr();
	void AttachmentHelper();
	int& GetPlayerState();
	bool& IsDefusing();
	bool& WaitForNoAttack();
	CBoneAccessor* GetBoneAccessor();
	CUtlVector<matrix>& GetBoneCache();
	void SetBoneCache(matrix* m);
	void ShootPos(Vector* vec);
	void UpdateVisibilityAllEntities();
	Vector GetEyePosition();
	void DrawServerHitboxes();
	uint8_t* GetServerEdict();
	float &GetVelocityModifier();
	Vector GetEyeAngles();
	int GetSequence();
	float& GetFallVelocity();
	Vector* GetEyeAnglesPointer();
	bool IsPlayer();
	bool IsRagdoll();
	bool DormantWrapped();
	int GetTeam();
	player_info_t GetPlayerInfo();

	Vector GetOrigin()
	{
		if (!this)
			return Vector(0, 0, 0);
		return *(Vector*)((DWORD)this + 0x138);
	}

	bool GrenadeExploded()
	{
		return *(bool*)((DWORD)this + 0x29E8);
	}

	Vector& GetVecViewOffset();

	float& GetDuckSpeed();
	const char* GetLastPlace();
	float & GetDuckAmount();
	void SetObserverMode(int value);
	int32_t GetObserverMode();

	Vector& GetVelocity()
	{
		return *(Vector*)((DWORD)this + 0x114);
	}

	int GetShootsFired()
	{
		return *reinterpret_cast<int*>((DWORD)this + 0x103E0);
	}

	float& GetLBY();
	Vector& GetAbsAngles();
	Vector& GetAbsOriginVirtual();
	Vector GetAbsOrigin();
	int Thrower();
	bool HasHelmet();
	bool HeavyArmor();
	Vector GetAbsOriginVec();
	int GetSequenceActivity(int sequence);
	matrix& GetrgflCoordinateFrame();
	IBaseViewModel* GetViewModel();
	std::vector< IBaseCombatWeapon* > GetWeapons();
	float GetOldSimulationTime();
	int GetTickBase();
	int* GetImpulse();
	float& GetCycle();
	float& GetAnimtime();
	bool IsScoped();
	bool SetupBones(matrix* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	float& GetSimulationTime();
	float CameFromDormantTime();

	float& GetSpawnTime()
	{
		return *(float*)((DWORD)this + 0x103C0);
	}

	__forceinline Vector GetBonePos(matrix* mat, int i)
	{
		if (!mat)
			return Vector(0, 0, 0);

		return Vector(mat[i][0][3], mat[i][1][3], mat[i][2][3]);
	}

	Vector GetNetworkOrigin();
	bool HasGunGameImmunity();
	float& GetMaxSpeed();
	bool& IsDucking();
	bool& IsWalking();
	int& GetMoveState();
	float& GetThirdpersonRecoil();
	int GetChokedPackets();
	void BuildTransformations(CStudioHdr* hdr, Vector* pos, Quaternion* q, const matrix& transform, int mask, uint8_t* computed);
	void StandardBlendingRules(CStudioHdr* hdr, Vector* pos, Quaternion* q, float time, int mask);
};

class IBaseViewModel : public IBasePlayer {
public:
	float& GetAnimtime();
	float& GetCycle();
	CBaseHandle GetViewmodelWeapon();
	int GetViewModelIndex();
	float& GetCurrentCycle();
	float& GetModelAnimTime();
	int& GetCurrentSequence();
	int& GetAnimationParity();
};

class ClassIdManager
{
public:
	ClassIdManager();
private:
	int GetClassID(const char * classname);
public:
	int CAI_BaseNPC = -1;
	int CAK47 = -1;
	int CBaseAnimating = -1;
	int CBaseAnimatingOverlay = -1;
	int CBaseAttributableItem = -1;
	int CBaseButton = -1;
	int CBaseCombatCharacter = -1;
	int CBaseCombatWeapon = -1;
	int CBaseCSGrenade = -1;
	int CBaseCSGrenadeProjectile = -1;
	int CBaseDoor = -1;
	int CBaseEntity = -1;
	int CBaseFlex = -1;
	int CBaseGrenade = -1;
	int CBaseParticleEntity = -1;
	int CBasePlayer = -1;
	int CBasePropDoor = -1;
	int CBaseTeamObjectiveResource = -1;
	int CBaseTempEntity = -1;
	int CBaseToggle = -1;
	int CBaseTrigger = -1;
	int CBaseViewModel = -1;
	int CBaseVPhysicsTrigger = -1;
	int CBaseWeaponWorldModel = -1;
	int CBeam = -1;
	int CBeamSpotlight = -1;
	int CBoneFollower = -1;
	int CBRC4Target = -1;
	int CBreachCharge = -1;
	int CBreachChargeProjectile = -1;
	int CBreakableProp = -1;
	int CBreakableSurface = -1;
	int CC4 = -1;
	int CCascadeLight = -1;
	int CChicken = -1;
	int CColorCorrection = -1;
	int CColorCorrectionVolume = -1;
	int CCSGameRulesProxy = -1;
	int CCSPlayer = -1;
	int CCSPlayerResource = -1;
	int CCSRagdoll = -1;
	int CCSTeam = -1;
	int CDangerZone = -1;
	int CDangerZoneController = -1;
	int CDEagle = -1;
	int CDecoyGrenade = -1;
	int CDecoyProjectile = -1;
	int CDrone = -1;
	int CDronegun = -1;
	int CDynamicLight = -1;
	int CDynamicProp = -1;
	int CEconEntity = -1;
	int CEconWearable = -1;
	int CEmbers = -1;
	int CEntityDissolve = -1;
	int CEntityFlame = -1;
	int CEntityFreezing = -1;
	int CEntityParticleTrail = -1;
	int CEnvAmbientLight = -1;
	int CEnvDetailController = -1;
	int CEnvDOFController = -1;
	int CEnvGasCanister = -1;
	int CEnvParticleScript = -1;
	int CEnvProjectedTexture = -1;
	int CEnvQuadraticBeam = -1;
	int CEnvScreenEffect = -1;
	int CEnvScreenOverlay = -1;
	int CEnvTonemapController = -1;
	int CEnvWind = -1;
	int CFEPlayerDecal = -1;
	int CFireCrackerBlast = -1;
	int CFireSmoke = -1;
	int CFireTrail = -1;
	int CFish = -1;
	int CFists = -1;
	int CFlashbang = -1;
	int CFogController = -1;
	int CFootstepControl = -1;
	int CFunc_Dust = -1;
	int CFunc_LOD = -1;
	int CFuncAreaPortalWindow = -1;
	int CFuncBrush = -1;
	int CFuncConveyor = -1;
	int CFuncLadder = -1;
	int CFuncMonitor = -1;
	int CFuncMoveLinear = -1;
	int CFuncOccluder = -1;
	int CFuncReflectiveGlass = -1;
	int CFuncRotating = -1;
	int CFuncSmokeVolume = -1;
	int CFuncTrackTrain = -1;
	int CGameRulesProxy = -1;
	int CGrassBurn = -1;
	int CHandleTest = -1;
	int CHEGrenade = -1;
	int CHostage = -1;
	int CHostageCarriableProp = -1;
	int CIncendiaryGrenade = -1;
	int CInferno = -1;
	int CInfoLadderDismount = -1;
	int CInfoMapRegion = -1;
	int CInfoOverlayAccessor = -1;
	int CItem_Healthshot = -1;
	int CItemCash = -1;
	int CItemDogtags = -1;
	int CKnife = -1;
	int CKnifeGG = -1;
	int CLightGlow = -1;
	int CMaterialModifyControl = -1;
	int CMelee = -1;
	int CMolotovGrenade = -1;
	int CMolotovProjectile = -1;
	int CMovieDisplay = -1;
	int CParadropChopper = -1;
	int CParticleFire = -1;
	int CParticlePerformanceMonitor = -1;
	int CParticleSystem = -1;
	int CPhysBox = -1;
	int CPhysBoxMultiplayer = -1;
	int CPhysicsProp = -1;
	int CPhysicsPropMultiplayer = -1;
	int CPhysMagnet = -1;
	int CPhysPropAmmoBox = -1;
	int CPhysPropLootCrate = -1;
	int CPhysPropRadarJammer = -1;
	int CPhysPropWeaponUpgrade = -1;
	int CPlantedC4 = -1;
	int CPlasma = -1;
	int CPlayerResource = -1;
	int CPointCamera = -1;
	int CPointCommentaryNode = -1;
	int CPointWorldText = -1;
	int CPoseController = -1;
	int CPostProcessController = -1;
	int CPrecipitation = -1;
	int CPrecipitationBlocker = -1;
	int CPredictedViewModel = -1;
	int CProp_Hallucination = -1;
	int CPropCounter = -1;
	int CPropDoorRotating = -1;
	int CPropJeep = -1;
	int CPropVehicleDriveable = -1;
	int CRagdollManager = -1;
	int CRagdollProp = -1;
	int CRagdollPropAttached = -1;
	int CRopeKeyframe = -1;
	int CSCAR17 = -1;
	int CSceneEntity = -1;
	int CSensorGrenade = -1;
	int CSensorGrenadeProjectile = -1;
	int CShadowControl = -1;
	int CSlideshowDisplay = -1;
	int CSmokeGrenade = -1;
	int CSmokeGrenadeProjectile = -1;
	int CSmokeStack = -1;
	int CSnowball = -1;
	int CSnowballPile = -1;
	int CSnowballProjectile = -1;
	int CSpatialEntity = -1;
	int CSpotlightEnd = -1;
	int CSprite = -1;
	int CSpriteOriented = -1;
	int CSpriteTrail = -1;
	int CStatueProp = -1;
	int CSteamJet = -1;
	int CSun = -1;
	int CSunlightShadowControl = -1;
	int CSurvivalSpawnChopper = -1;
	int CTablet = -1;
	int CTeam = -1;
	int CTeamplayRoundBasedRulesProxy = -1;
	int CTEArmorRicochet = -1;
	int CTEBaseBeam = -1;
	int CTEBeamEntPoint = -1;
	int CTEBeamEnts = -1;
	int CTEBeamFollow = -1;
	int CTEBeamLaser = -1;
	int CTEBeamPoints = -1;
	int CTEBeamRing = -1;
	int CTEBeamRingPoint = -1;
	int CTEBeamSpline = -1;
	int CTEBloodSprite = -1;
	int CTEBloodStream = -1;
	int CTEBreakModel = -1;
	int CTEBSPDecal = -1;
	int CTEBubbles = -1;
	int CTEBubbleTrail = -1;
	int CTEClientProjectile = -1;
	int CTEDecal = -1;
	int CTEDust = -1;
	int CTEDynamicLight = -1;
	int CTEEffectDispatch = -1;
	int CTEEnergySplash = -1;
	int CTEExplosion = -1;
	int CTEFireBullets = -1;
	int CTEFizz = -1;
	int CTEFootprintDecal = -1;
	int CTEFoundryHelpers = -1;
	int CTEGaussExplosion = -1;
	int CTEGlowSprite = -1;
	int CTEImpact = -1;
	int CTEKillPlayerAttachments = -1;
	int CTELargeFunnel = -1;
	int CTEMetalSparks = -1;
	int CTEMuzzleFlash = -1;
	int CTEParticleSystem = -1;
	int CTEPhysicsProp = -1;
	int CTEPlantBomb = -1;
	int CTEPlayerAnimEvent = -1;
	int CTEPlayerDecal = -1;
	int CTEProjectedDecal = -1;
	int CTERadioIcon = -1;
	int CTEShatterSurface = -1;
	int CTEShowLine = -1;
	int CTesla = -1;
	int CTESmoke = -1;
	int CTESparks = -1;
	int CTESprite = -1;
	int CTESpriteSpray = -1;
	int CTest_ProxyToggle_Networkable = -1;
	int CTestTraceline = -1;
	int CTEWorldDecal = -1;
	int CTriggerPlayerMovement = -1;
	int CTriggerSoundOperator = -1;
	int CVGuiScreen = -1;
	int CVoteController = -1;
	int CWaterBullet = -1;
	int CWaterLODControl = -1;
	int CWeaponAug = -1;
	int CWeaponAWP = -1;
	int CWeaponBaseItem = -1;
	int CWeaponBizon = -1;
	int CWeaponCSBase = -1;
	int CWeaponCSBaseGun = -1;
	int CWeaponCycler = -1;
	int CWeaponElite = -1;
	int CWeaponFamas = -1;
	int CWeaponFiveSeven = -1;
	int CWeaponG3SG1 = -1;
	int CWeaponGalil = -1;
	int CWeaponGalilAR = -1;
	int CWeaponGlock = -1;
	int CWeaponHKP2000 = -1;
	int CWeaponM249 = -1;
	int CWeaponM3 = -1;
	int CWeaponM4A1 = -1;
	int CWeaponMAC10 = -1;
	int CWeaponMag7 = -1;
	int CWeaponMP5Navy = -1;
	int CWeaponMP7 = -1;
	int CWeaponMP9 = -1;
	int CWeaponNegev = -1;
	int CWeaponNOVA = -1;
	int CWeaponP228 = -1;
	int CWeaponP250 = -1;
	int CWeaponP90 = -1;
	int CWeaponSawedoff = -1;
	int CWeaponSCAR20 = -1;
	int CWeaponScout = -1;
	int CWeaponSG550 = -1;
	int CWeaponSG552 = -1;
	int CWeaponSG556 = -1;
	int CWeaponSSG08 = -1;
	int CWeaponTaser = -1;
	int CWeaponTec9 = -1;
	int CWeaponTMP = -1;
	int CWeaponUMP45 = -1;
	int CWeaponUSP = -1;
	int CWeaponXM1014 = -1;
	int CWorld = -1;
	int CWorldVguiText = -1;
	int DustTrail = -1;
	int MovieExplosion = -1;
	int ParticleSmokeGrenade = -1;
	int RocketTrail = -1;
	int SmokeTrail = -1;
	int SporeExplosion = -1;
	int SporeTrail = -1;
};

extern ClassIdManager * g_ClassID;

enum weapons
{
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_ump = 24,
	weapon_mp5sd = 23,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516
};

enum
{
	deagle = 1,
	p2000 = 32,
	fiveseven = 3,
	glock = 4,
	dualelites = 2,
	ak = 7,
	aug = 8,
	awp = 9,
	famas = 10,
	g3sg1 = 11,
	galil = 13,
	p250 = 36,
	tec9 = 30,
	m249 = 14,
	m4 = 16,
	mac10 = 17,
	p90 = 19,
	ump45 = 24,
	ppbizon = 26,
	negev = 28,
	mp7 = 33,
	mp9 = 34,
	scar30 = 38,
	sg553 = 39,
	ssg08 = 40,
	goldknife = 41,
	ctknife = 42,
	tknife = 59,
	flashgren = 43,
	hegren = 44,
	smoke = 45,
	molotov = 46,
	incendiary = 48,
	decoygren = 47,
	bomb = 49,
	nova = 35,
	xm1014 = 25,
	sawedoff = 29,
	mag7 = 27,
	zeus = 31
};
enum eWeaponType1 {
	WEAPON_DEAGLE = 1,
	WEAPON_DUALBERETTA = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_ZEUSX27 = 31,
	WEAPON_USP = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE_GG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INC = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1S = 60,
	WEAPON_USPS = 61,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

extern unsigned int FindInDataMap(datamap_t* pMap, const char* name);

class IBaseCombatWeapon
{
public:
	bool CanFire();
	int Owner();
	bool InReload();
	float& NextSecondaryAttack();
	short& GetItemDefinitionIndex();
	float NextPrimaryAttack();
	float GetLastShotTime();
	int GetAmmo(bool second);
	int GetZoomLevel();
	bool m_bReloadVisuallyComplete();
	float &GetAccuracyPenalty();
	float &GetRecoilIndex();
	float LastShotTime();
	bool StartedArming();
	float GetPostponeFireReadyTime();
	void SetPostPoneTime(float asdasdasd);

	int& AccountID();
	int& ItemIDHigh();
	int& EntityQuality();
	char* CustomName();
	unsigned int& FallbackPaintKit();
	float& FallbackSeed();
	int& FallbackWear();
	void preDataUpdate(int updateType);
	void postDataUpdate(int updateType);
	unsigned int& FallbackStatTrak();
	bool& Initialized();

	bool IsCanScope()
	{
		if (!this)
			return false;

		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP || WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1 || WeaponId == WEAPON_SSG08 || WeaponId == WEAPON_SG553 || WeaponId == WEAPON_AUG;
	}

	bool IsAuto()
	{
		return this->GetItemDefinitionIndex() == WEAPON_SCAR20 || this->GetItemDefinitionIndex() == WEAPON_G3SG1;
	}

	bool IsAWP()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP;
	}

	float GetInaccuracy()
	{
		typedef float(__thiscall* OriginalFn)(void*);
		return getvfunc< OriginalFn >(this, 483)(this);
	}

	float GetSpread()
	{
		typedef float(__thiscall* OriginalFn)(void*);
		return getvfunc< OriginalFn >(this, 453)(this);
	}

	int GetMaxTickbaseShift()
	{
		if (!DTable())
			return 14;

		auto idx = GetItemDefinitionIndex();
		auto max_tickbase_shift = 0;

		switch (idx)
		{
		case WEAPON_M249:
		case WEAPON_MAC10:
		case WEAPON_P90:
		case WEAPON_MP5SD:
		case WEAPON_NEGEV:
		case WEAPON_MP9:
			max_tickbase_shift = 5;
			break;
		case WEAPON_DUALBERETTA:
		case WEAPON_UMP45:
		case WEAPON_BIZON:
		case WEAPON_TEC9:
		case WEAPON_MP7:
			max_tickbase_shift = 6;
			break;
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_FAMAS:
		case WEAPON_GALIL:
		case WEAPON_M4A1:
		case WEAPON_M4A1S:
		case WEAPON_CZ75:
			max_tickbase_shift = 7;
			break;
		case WEAPON_FIVESEVEN:
		case WEAPON_GLOCK:
		case WEAPON_P250:
		case WEAPON_SG553:
			max_tickbase_shift = 8;
			break;
		case WEAPON_USP:
		case WEAPON_USPS:
			max_tickbase_shift = 9;
			break;
		case WEAPON_DEAGLE:
			max_tickbase_shift = 13;
			break;
		case WEAPON_G3SG1:
		case WEAPON_SCAR20:
			max_tickbase_shift = 14;
			break;
		}

		return max_tickbase_shift;
	}

	CCSWeaponInfo* GetCSWpnData();
	int32_t WeaponMode();

	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* oUpdateAccuracyPenalty)(PVOID);
		return getvfunc<oUpdateAccuracyPenalty>(this, 484)(this);
	}

	bool IsZeus()
	{
		if (!this)
			return false;

		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == eWeaponType1::WEAPON_ZEUSX27;
	}

	bool IsBomb()
	{
		int iWeaponID = this->GetItemDefinitionIndex();

		return iWeaponID == bomb;
	}

	bool IsNade();

	bool IsMiscWeapon()
	{
		int iWeaponID = this->GetItemDefinitionIndex();
		return (iWeaponID == WEAPON_HEALTHSHOT || iWeaponID == tknife || iWeaponID == ctknife
			|| iWeaponID == goldknife || iWeaponID == bomb
			|| iWeaponID == hegren || iWeaponID == decoygren
			|| iWeaponID == flashgren || iWeaponID == molotov
			|| iWeaponID == smoke || iWeaponID == incendiary 
			|| iWeaponID == 59 || iWeaponID == 57
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}

	bool IsKnife();

	bool isPistol()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_DEAGLE || WeaponId == eWeaponType1::WEAPON_DUALBERETTA || WeaponId == WEAPON_FIVESEVEN || WeaponId == WEAPON_P250 ||
			WeaponId == WEAPON_GLOCK || WeaponId == WEAPON_USPS || WeaponId == WEAPON_CZ75 || WeaponId == WEAPON_USP ||
			WeaponId == WEAPON_TEC9 || WeaponId == weapon_revolver;
	}

	bool IsHeavyPistol() {
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_DEAGLE || WeaponId == weapon_revolver;
	}

	bool isShotgun()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_XM1014 || WeaponId == WEAPON_NOVA || WeaponId == WEAPON_SAWEDOFF || WeaponId == WEAPON_MAG7;
	}

	bool isRifle()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AK47 || WeaponId == WEAPON_M4A1 || WeaponId == WEAPON_M4A1S || WeaponId == WEAPON_GALIL || WeaponId == WEAPON_FAMAS ||
			WeaponId == WEAPON_AUG || WeaponId == WEAPON_SG553;
	}
	bool isSniper()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP || WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1 || WeaponId == WEAPON_SSG08;
	}

	bool DTable() {
		int WeaponId = this->GetItemDefinitionIndex();
		return IsGun() && !isShotgun() && WeaponId != WEAPON_SSG08 && WeaponId != WEAPON_AWP;
	}

	bool isAutoSniper()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1;
	}

	bool IsGun()
	{
		if (this->IsKnife())
			return false;
		int id = this->GetItemDefinitionIndex();
		if (!id)
			return false;
		if (id == WEAPON_HEGRENADE || id == WEAPON_DECOY || id == WEAPON_INC ||
			id == WEAPON_MOLOTOV || id == WEAPON_C4 ||
			id == WEAPON_FLASHBANG || id == WEAPON_SMOKEGRENADE || id == WEAPON_HEALTHSHOT)
			return false;
		else
			return true;
	}

	std::string GetGunName()
	{

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_AK47:
			return str("AK47");
			break;
		case WEAPON_AUG:
			return str("AUG");
			break;
		case weapon_revolver:
			return str("REVOLVER");
			break;
		case WEAPON_AWP:
			return str("AWP");
			break;
		case WEAPON_BIZON:
			return str("Bizon");
			break;
		case WEAPON_C4:
			return str("Bomb");
			break;
		case WEAPON_CZ75:
			return str("CZ75");
			break;
		case WEAPON_DEAGLE:
			return str("Deagle");
			break;
		case WEAPON_DECOY:
			return str("Decoy");
			break;
		case WEAPON_DUALBERETTA:
			return str("Dual Berettas");
			break;
		case WEAPON_FAMAS:
			return str("Famas");
			break;
		case WEAPON_FIVESEVEN:
			return str("FiveSeven");
			break;
		case WEAPON_FLASHBANG:
			return str("Flash");
			break;
		case WEAPON_G3SG1:
			return str("G3SG1");
			break;
		case WEAPON_GALIL:
			return str("Galil");
			break;
		case WEAPON_GLOCK:
			return str("Glock-18");
			break;
		case WEAPON_INC:
			return str("Incendiary");
			break;
		case WEAPON_MOLOTOV:
			return str("Molotov");
			break;
		case WEAPON_SSG08:
			return str("SSG-08");
			break;
		case WEAPON_HEGRENADE:
			return str("HE Grenade");
			break;
		case WEAPON_M249:
			return str("M249");
			break;
		case WEAPON_M4A1:
			return str("M4A1");
			break;
		case WEAPON_MAC10:
			return str("MAC10");
			break;
		case WEAPON_MAG7:
			return str("MAG7");
			break;
		case WEAPON_MP7:
			return str("MP7");
			break;
		case WEAPON_MP9:
			return str("MP9");
			break;
		case WEAPON_NOVA:
			return str("Nova");
			break;
		case WEAPON_NEGEV:
			return str("Negev");
			break;
		case WEAPON_P250:
			return str("P250");
			break;
		case WEAPON_P90:
			return str("P90");
			break;
		case WEAPON_SAWEDOFF:
			return str("SawedOff");
			break;
		case WEAPON_SCAR20:
			return str("SCAR20");
			break;
		case WEAPON_SMOKEGRENADE:
			return str("Smoke");
			break;
		case WEAPON_SG553:
			return str("SG553");
			break;
		case WEAPON_TEC9:
			return str("TEC9");
			break;
		case WEAPON_USP:
			return str("P2000");
			break;
		case WEAPON_USPS:
			return str("USP-S");
			break;
		case WEAPON_UMP45:
			return str("UMP");
			break;
		case WEAPON_XM1014:
			return str("XM1014");
			break;
		case WEAPON_ZEUSX27:
			return str("Zeus");
			break;
		case WEAPON_M4A1S:
			return str("M4A1-S");
			break;
		case WEAPON_HEALTHSHOT:
			return str("Healthshot");
			break;
		}
		if (IsKnife())
			return str("Knife");
		return str("?");
	}

};

class CBaseCSGrenade : IBaseCombatWeapon
{
public:
	float GetThrowTime();
};

class bf_read {
public:
	const char* m_pDebugName;
	bool m_bOverflow;
	int m_nDataBits;
	unsigned int m_nDataBytes;
	unsigned int m_nInBufWord;
	int m_nBitsAvail;
	const unsigned int* m_pDataIn;
	const unsigned int* m_pBufferEnd;
	const unsigned int* m_pData;

	bf_read() = default;

	bf_read(const void* pData, int nBytes, int nBits = -1) {
		StartReading(pData, nBytes, 0, nBits);
	}

	void StartReading(const void* pData, int nBytes, int iStartBit, int nBits) {
		// Make sure it's dword aligned and padded.
		m_pData = (uint32_t*)pData;
		m_pDataIn = m_pData;
		m_nDataBytes = nBytes;

		if (nBits == -1) {
			m_nDataBits = nBytes << 3;
		}
		else {
			m_nDataBits = nBits;
		}
		m_bOverflow = false;
		m_pBufferEnd = reinterpret_cast<uint32_t const*>(reinterpret_cast<uint8_t const*>(m_pData) + nBytes);
		if (m_pData)
			Seek(iStartBit);
	}

	bool Seek(int nPosition) {
		bool bSucc = true;
		if (nPosition < 0 || nPosition > m_nDataBits) {
			m_bOverflow = true;
			bSucc = false;
			nPosition = m_nDataBits;
		}
		int nHead = m_nDataBytes & 3; // non-multiple-of-4 bytes at head of buffer. We put the "round off"
									  // at the head to make reading and detecting the end efficient.

		int nByteOfs = nPosition / 8;
		if ((m_nDataBytes < 4) || (nHead && (nByteOfs < nHead))) {
			// partial first dword
			uint8_t const* pPartial = (uint8_t const*)m_pData;
			if (m_pData) {
				m_nInBufWord = *(pPartial++);
				if (nHead > 1)
					m_nInBufWord |= (*pPartial++) << 8;
				if (nHead > 2)
					m_nInBufWord |= (*pPartial++) << 16;
			}
			m_pDataIn = (uint32_t const*)pPartial;
			m_nInBufWord >>= (nPosition & 31);
			m_nBitsAvail = (nHead << 3) - (nPosition & 31);
		}
		else {
			int nAdjPosition = nPosition - (nHead << 3);
			m_pDataIn = reinterpret_cast<uint32_t const*>(
				reinterpret_cast<uint8_t const*>(m_pData) + ((nAdjPosition / 32) << 2) + nHead);
			if (m_pData) {
				m_nBitsAvail = 32;
				GrabNextDWord();
			}
			else {
				m_nInBufWord = 0;
				m_nBitsAvail = 1;
			}
			m_nInBufWord >>= (nAdjPosition & 31);
			m_nBitsAvail = min(m_nBitsAvail, 32 - (nAdjPosition & 31)); // in case grabnextdword overflowed
		}
		return bSucc;
	}

	FORCEINLINE void GrabNextDWord(bool bOverFlowImmediately = false) {
		if (m_pDataIn == m_pBufferEnd) {
			m_nBitsAvail = 1; // so that next read will run out of words
			m_nInBufWord = 0;
			m_pDataIn++; // so seek count increments like old
			if (bOverFlowImmediately)
				m_bOverflow = true;
		}
		else if (m_pDataIn > m_pBufferEnd) {
			m_bOverflow = true;
			m_nInBufWord = 0;
		}
		else {
			m_nInBufWord = DWORD(*(m_pDataIn++));
		}
	}
};

class bf_write {
public:
	unsigned char* m_pData;
	int m_nDataBytes;
	int m_nDataBits;
	int m_iCurBit;
	bool m_bOverflow;
	bool m_bAssertOnOverflow;
	const char* m_pDebugName;

	void StartWriting(void* pData, int nBytes, int iStartBit = 0, int nBits = -1) {
		// Make sure it's dword aligned and padded.
		// The writing code will overrun the end of the buffer if it isn't dword aligned, so truncate to force alignment
		nBytes &= ~3;

		m_pData = (unsigned char*)pData;
		m_nDataBytes = nBytes;

		if (nBits == -1) {
			m_nDataBits = nBytes << 3;
		}
		else {
			m_nDataBits = nBits;
		}

		m_iCurBit = iStartBit;
		m_bOverflow = false;
	}

	bf_write() {
		m_pData = NULL;
		m_nDataBytes = 0;
		m_nDataBits = -1; // set to -1 so we generate overflow on any operation
		m_iCurBit = 0;
		m_bOverflow = false;
		m_bAssertOnOverflow = true;
		m_pDebugName = NULL;
	}

	// nMaxBits can be used as the number of bits in the buffer.
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_write(void* pData, int nBytes, int nBits = -1) {
		m_bAssertOnOverflow = true;
		m_pDebugName = NULL;
		StartWriting(pData, nBytes, 0, nBits);
	}

	bf_write(const char* pDebugName, void* pData, int nBytes, int nBits = -1) {
		m_bAssertOnOverflow = true;
		m_pDebugName = pDebugName;
		StartWriting(pData, nBytes, 0, nBits);
	}
};

class CLC_Move{
private:
	char __PAD0[0x8]; // 0x0 two vtables
public:
	int m_nBackupCommands; // 0x8
	int m_nNewCommands;    // 0xC
	std::string* m_data;   // 0x10 std::string
	bf_read m_DataIn;      // 0x14
	bf_write m_DataOut;    // 0x38
};                       // size: 0x50

class INetChannel;
class INetMessage
{
public:
	virtual					~INetMessage() { }
	virtual void			SetNetChannel(void* pNetChannel) = 0;
	virtual void			SetReliable(bool bState) = 0;
	virtual bool			Process() = 0;
	virtual	bool			ReadFromBuffer(bf_read& buffer) = 0;
	virtual	bool			WriteToBuffer(bf_write& buffer) = 0;
	virtual bool			IsReliable() const = 0;
	virtual int				GetType() const = 0;
	virtual int				GetGroup() const = 0;
	virtual const char* GetName() const = 0;
	virtual void* GetNetChannel(void) const = 0;
	virtual const char* ToString() const = 0;
};

class INetChannel
{
public:
	char pad_0x0000[0x18]; //0x0000
	__int32 iOutSequenceNr; //0x0018 
	__int32 iInSequenceNr; //0x001C 
	__int32 iOutSequenceNrAck; //0x0020 
	__int32 iOutReliableState; //0x0024 
	__int32 iInReliableState; //0x0028 
	__int32 iChokedPackets; //0x002C

	void Transmit(bool onlyreliable)
	{
		using Fn = bool(__thiscall*)(void*, bool);
		getvfunc<Fn>(this, 49)(this, onlyreliable);
	}

	void SetChoked()
	{
		typedef void(__thiscall* oSetChocked)(INetChannel*);
		getvfunc<oSetChocked>(this, 45)(this);
	}

	void SendDatagram()
	{
		using Fn = int(__thiscall*)(void*, void*);
		getvfunc<Fn>(this, 46)(this, 0);
	}

	void SetTimeOut(float seconds)
	{
		using Fn = void(__thiscall*)(void*, float);
		return getvfunc<Fn>(this, 4)(this, seconds);
	}

	int RequestFile(const char* filename)
	{
		using Fn = int(__thiscall*)(void*, const char*);
		return getvfunc<Fn>(this, 62)(this, filename);
	}
};

template < typename T >
class CNetMessagePB : public INetMessage, public T {};
using CCLCMsg_Move_t = CNetMessagePB< CLC_Move >;