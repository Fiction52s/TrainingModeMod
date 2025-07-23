#include "../MexTK/mex.h"
#include "events.h"

#define TEXT_SCALE 4.2
#define WDJOBJ_TEXT 6
#define WDJOBJ_ARROW 3
#define WDARROW_OFFSET 0.36
#define WDARROW_ANIMFRAMES 4
#define WDFRAMES 15

#define TRGT_RANGEMAX 0.8
#define TRGT_RANGEMIN 0.55

#define TRGTJOBJ_AURA 3
#define TRGTJOBJ_LBOUND 4
#define TRGTJOBJ_RBOUND 5

#define TRGTSCL_DISTMIN 20
#define TRGTSCL_DISTMAX 50
#define TRGTSCL_SCALEMIN 1.0
#define TRGTSCL_SCALEMAX 1.7

typedef struct WavedashData WavedashData;
typedef struct WavedashAssets WavedashAssets;
typedef struct TargetData TargetData;
//typedef struct TaskData TaskData;

typedef struct TaskData
{
	u8 is_target_mode_on;
	u8 max_success;
	void(*Task_Init)(WavedashData *event_data);
	void(*Task_Think)(WavedashData *event_data, FighterData *hmn_data);
	bool(*Task_IsTargetSatisfied)(WavedashData *event_data, TargetData *target_data, FighterData *hmn_data, Vec3 pos);
	//void(*Task_TargetSuccess)(WavedashData *event_data, TargetData *target_data, FighterData *hmn_data, Vec3 pos);
	//void(*Task_Complete)(WavedashData *event_data);
} TaskData;

struct WavedashData
{
	EventDesc *event_desc;
	WavedashAssets *assets;
	struct
	{
		GOBJ *gobj;
		int canvas;
		Text *text_timing;
		Text *text_angle;
		Text *text_succession;
		float arrow_prevpos;
		float arrow_nextpos;
		int arrow_timer;
	} hud;
	struct
	{
		GOBJ *gobj;
		float scale;
		Vec3 left_offset;
		Vec3 center_offset;
		Vec3 right_offset;
	} target;
	struct
	{
		u8 shield_num;
	} tip;
	float wd_maxdstn;
	int timer;
	u8 airdodge_frame;
	u8 is_airdodge;
	u8 is_early_airdodge;
	u8 is_wavedashing;
	u8 since_wavedash;
	float wd_angle;
	int wd_attempted;
	int wd_succeeded;
	
	TaskData *task;
	
	u8 success_count;
	u8 event_success;
	
	struct
	{
		u8 is_dashing;
	} dash_dance;

	struct
	{
		u8 success_count;
		u8 is_running;
		u8 event_success;
	} crouch_out_of_run;

	u8 is_running;

	struct
	{
		u16 line_index;
		Vec2 pos;
	} restore;
	GXColor orig_colors[WDFRAMES];
};

struct WavedashAssets
{
	JOBJ *hud;
	void **hudmatanim; // pointer to array
	JOBJ *target_jobj;
	void **target_jointanim;
	void **target_matanim;
};

enum TargetState
{
	TRGSTATE_SPAWN,
	TRGSTATE_WAIT,
	TRGSTATE_DESPAWN,
};
struct TargetData
{
	int state;
	Vec3 pos;
	int line_index;
	float left;
	float right;
	CmSubject *cam;
	WavedashData *wavedashData;
};

float Bezier(float time, float start, float end);
float Target_GetWdashDistance(FighterData *hmn_data, float mag);
GOBJ *Target_Spawn(WavedashData *event_data, FighterData *fighter_data);
int Target_CheckArea(WavedashData *event_data, int line, Vec3 *pos, float x_offset, int *ret_line, Vec3 *ret_pos, Vec3 *ret_slope);
void Target_Init( WavedashData *event_data, FighterData *hmn_data);
void Target_Think(GOBJ *target_gobj);
void Wavedash_HUDCamThink(GOBJ *gobj);
void Event_Exit();
void Wavedash_Init(WavedashData *event_data);
void Wavedash_Think(WavedashData *event_data, FighterData *hmn_data);

void Dash_Dance_Init(WavedashData *event_data);
void Dash_Dance_Think(WavedashData *event_data, FighterData *hmn_data);
bool Dash_Dance_IsTargetSatisfied(WavedashData *event_data, TargetData *target_data, FighterData *hmn_data, Vec3 pos);

void Crouch_Out_Of_Run_Init(WavedashData *event_data);
void Crouch_Out_Of_Run_Think(WavedashData *event_data, FighterData *hmn_data);

void JCGrab_Think(WavedashData *event_data, FighterData *hmn_data);

