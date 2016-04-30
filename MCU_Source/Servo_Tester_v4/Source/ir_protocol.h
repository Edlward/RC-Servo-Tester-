#pragma once


//typedef struct ir_data_tDef {
//	bool Output[32];
//	uint16_t Time[32];
//	uint8_t Size;
//} ir_data_t_def;
//extern const ir_portocol ir_Table[];



typedef struct ir_data_tDef_t {
	bool  *Output;
	uint16_t  *Time;
	uint8_t Size;
	uint8_t	rep_cycle;
	uint16_t rep_delay;
} ir_data_t;


typedef enum {
												SONY_DSLR_PHOTO, 
												SONY_DSLR_VIDEO, 

												SONY_CAM_PHOTO,
												SONY_CAM_REC, 
												SONY_CAM_ZOOMT, 
												SONY_CAM_ZOOMW, 
	
												CANON_DSLR_PHOTO, 
												CANON_DSLR_REC,

												NIKON_DSLR_PHOTO,

												OLYMPUS_PHOTO,
												OLYMOUS_ZOOMT, 
												OLYMOUS_ZOOMW, 
	
												PENTAX_PHOTO,
												PENTAX_FOCUS_ON,

												CANON_WLDC_PHOTO,



												NONE_CONTRL =100,
} _protocol;



typedef struct {
//    const char *name;
//		void (* function) (void);
		uint16_t * protocol;
		uint8_t size;
		uint16_t IR_Freq;
		bool repeat;
		uint8_t	rep_cycle;
		uint16_t rep_delay;

		uint16_t sync;
		uint16_t wait;
		uint16_t high;
		uint16_t low;
	
//	void (* function) (void);
	
} ir_portocol;


//extern ir_data_t_def ir_data_t;


extern ir_data_t ir_data;
extern bool IR_Action_flag;	
extern 	uint16_t ir_pulse[];

extern  ir_portocol ir_Table[]; 
	
static void sony_IR_generation (ir_portocol * value) ;
static void ect_IR_generation (ir_portocol * value) ;
static void Olympus_Shutter_generation( ir_portocol *value);
static void Olympus_Zoom( ir_portocol *value);
static void CanonWLDC( ir_portocol *value);

//extern ir_data_t  *CanonShutter;
 void output_IR_Signal(uint8_t model);
void ir_Output	(uint8_t model);
