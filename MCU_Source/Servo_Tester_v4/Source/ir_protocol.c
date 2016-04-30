#include "board.h"

/*
----TESTED CAMERA----

Sony DSLR :ALPHA, NEX5
Sony CAM :
Pentax : K200D, Pentax Kx and Pentax K-7
CANON : 5D, 500D
Olympus : PEN
CANON WLDC100 : S60/S70/S1IS/G1/G2/G3/G5/G6/Pro90/Pro1
*/

#define DATA_SIZE(x) 		sizeof(x)/sizeof(x[0])
bool IR_Action_flag=false;	
ir_data_t ir_data;



uint16_t ir_pulse[80];		//가장 큰 ir_Protocol의 배열값보다 커야됨.

//프로토콜 참조 사이트
//http://sebastian.setz.name/arduino/my-libraries/multi-camera-ir-control/

// DSLR & CAM IR Protocol ----------------------------------------------------
uint16_t Sony_DSLT_ShutterDelay[] =	{1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1};		//동작테스트  완료

uint16_t Sony_DSLT_ShutterNow[] = 	{1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1};		//동작테스트  완료
uint16_t Sony_DSLT_RecToggle[] =  	{0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,0,1,1,1,1};		//동작테스트  완료

uint16_t Sony_CAM_Photo[] =				{1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0};		//동작테스트  완료
uint16_t Sony_CAM_Rec_Toggle[] = 	{0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0};		//동작테스트  완료
uint16_t Sony_CAM_ZoomT[] =			{0,1,0,1,1,0,0,1,0,0,1,1,0,1,1};					//동작테스트 완료
uint16_t Sony_CAM_ZoomW[] = 			{1,1,0,1,1,0,0,1,0,0,1,1,0,1,1};					//동작테스트  완료 

uint16_t CanonDSLR_Photo[] = 			{515,7200,515};		//Canon DSLR Photo
uint16_t CanonDSLR_REC[] = 				{160, 5400, 200};		//Canon DSLR REC
uint16_t NikonDSLR_Photo[] = 			{2000,28000,400,1600,400,3600,400};	//실행되는지 확인요!!
uint16_t NikonDSLR_ShutterNow[] = 	{2000,27830,390,1580,410,3580,400};	//인터넷에서 구함.

uint16_t Olympus_ShutterNow [] = 		{0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1};	//인터넷에서 구함
uint16_t Olympus_ZoomT [] = 				{0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1};		//인터넷에서 구함
uint16_t Olympus_ZoomW[] = 				{0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1};		//인터넷에서 구함

uint16_t Pentax_ShutterNow[] = 			{13000,3000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};			// 인터넷에서 구함
uint16_t Pentax_FocusOn[] = 				{13000,3000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,3000,1000,1000};	//인터넷에서 구함

uint16_t CanonWLDC100[]	=				{0,1,0,1,0,0,1,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1};	//인터넷에서 구함.

//---------------------------------------------------------------------------------
//Protocol name | protocol size | IR PWM frequency | repeat? | repat cycle | repat wait delay | start sync time |  wait state time | high state time | low state time
 ir_portocol ir_Table[] = {
		{&Sony_DSLT_ShutterNow[0],DATA_SIZE(Sony_DSLT_ShutterNow),37900, false, 1, 11000, 2400, 635, 1100, 538},
		{&Sony_DSLT_RecToggle[0],DATA_SIZE(Sony_DSLT_RecToggle),37900, false, 1, 11000, 2400, 635, 1100, 538},

		{&Sony_CAM_Photo[0],DATA_SIZE(Sony_CAM_Photo),37900, false, 3, 11000, 2400, 635, 1400, 538},
		{&Sony_CAM_Rec_Toggle[0],DATA_SIZE(Sony_CAM_Rec_Toggle), 37900, false, 2, 11000, 2400, 635, 1400, 538},
		{&Sony_CAM_ZoomT[0],DATA_SIZE(Sony_CAM_ZoomT), 37900, true, 1, 11000, 2400, 635, 1400, 538},
		{&Sony_CAM_ZoomW[0],DATA_SIZE(Sony_CAM_ZoomW),37900, true, 1, 11000, 2400, 635, 1400, 538},

		{&CanonDSLR_Photo[0],DATA_SIZE(CanonDSLR_Photo),37900, false, 2, 11000},
		{&CanonDSLR_REC[0],DATA_SIZE(CanonDSLR_REC),37900, false, 2, 11000},

		{&NikonDSLR_ShutterNow[0],DATA_SIZE(NikonDSLR_ShutterNow),37900, false, 2, 11000},

		{&Olympus_ShutterNow[0],DATA_SIZE(Olympus_ShutterNow),37900, false, 3, 11000 },
		{&Olympus_ZoomT[0],DATA_SIZE(Olympus_ZoomT),37900, true, 1, 30466},
		{&Olympus_ZoomW[0],DATA_SIZE(Olympus_ZoomW),37900, true, 1, 30466},

		{&Pentax_ShutterNow[0],DATA_SIZE(Pentax_ShutterNow),37900, false, 3, 11000},
		{&Pentax_FocusOn[0],DATA_SIZE(Pentax_FocusOn),37900, false, 3, 11000},

		{&CanonWLDC100[0],DATA_SIZE(CanonWLDC100),37900, false, 2, 11000},

		
 };

void ir_Output	(uint8_t model) {
	
		switch(model) {
				case NONE_CONTRL:
					break;

			case SONY_DSLR_PHOTO:
					sony_IR_generation(&ir_Table[SONY_DSLR_PHOTO]);
					break;
		
			case SONY_DSLR_VIDEO:
					sony_IR_generation(&ir_Table[SONY_DSLR_VIDEO]);
					break;

			case SONY_CAM_PHOTO:
					sony_IR_generation(&ir_Table[SONY_CAM_PHOTO]);
					break;

			case SONY_CAM_REC:
					sony_IR_generation(&ir_Table[SONY_CAM_REC]);
					break;

			case SONY_CAM_ZOOMT:
					sony_IR_generation(&ir_Table[SONY_CAM_ZOOMT]);
					break;

			case SONY_CAM_ZOOMW:
					sony_IR_generation(&ir_Table[SONY_CAM_ZOOMW]);
					break;
	
			case CANON_DSLR_PHOTO:
						ect_IR_generation(&ir_Table[CANON_DSLR_PHOTO]);
					break;

			case CANON_DSLR_REC:
						ect_IR_generation(&ir_Table[CANON_DSLR_REC]);
					break;

			case NIKON_DSLR_PHOTO:
						ect_IR_generation(&ir_Table[NIKON_DSLR_PHOTO]);
					break;
			
			case OLYMPUS_PHOTO:
						Olympus_Shutter_generation(&ir_Table[OLYMPUS_PHOTO]);
					break;

			case OLYMOUS_ZOOMT:
						Olympus_Zoom(&ir_Table[OLYMOUS_ZOOMT]);
					break;

			case OLYMOUS_ZOOMW:
						Olympus_Zoom(&ir_Table[OLYMOUS_ZOOMW]);
					break;

			case PENTAX_PHOTO:
						ect_IR_generation(&ir_Table[PENTAX_PHOTO]);
					break;

			case PENTAX_FOCUS_ON:
						ect_IR_generation(&ir_Table[PENTAX_FOCUS_ON]);
					break;

			case CANON_WLDC_PHOTO:
						CanonWLDC(&ir_Table[CANON_WLDC_PHOTO]);
					break;
			

			default:
					break;
		}
	
}

static void ect_IR_generation (ir_portocol * value) {

		ir_pwm_clock_set(value->IR_Freq);
		ir_data.Time = value->protocol;		//포인터 위치 확인 필요.
		ir_data.Size = value->size;
		ir_data.rep_cycle = value->rep_cycle;
		ir_data.rep_delay= value->rep_delay;
		IR_Action_flag=true;			//LED action flag

		ir_Generator_start();
}

static void sony_IR_generation (ir_portocol * value) {
		uint8_t j=0;
		uint8_t size=0;
		uint16_t * ptrData = &ir_pulse[0];
	
		ir_pwm_clock_set(value->IR_Freq);
		ptrData[size] = value->sync;		//sync start
			size++;
	
		for(j=0;j<value->size;j++) {
				ptrData[size] = value->wait;
				size++;
				if(value->protocol[j]) {
						ptrData[size] = value->high;
				}
				else {
						ptrData[size] = value->low;
				}
				size++;
		}
		ir_data.Time = &ir_pulse[0];
		ir_data.Size = size;
		ir_data.rep_cycle = value->rep_cycle;
		ir_data.rep_delay= value->rep_delay;
		IR_Action_flag=true;			//LED action flag

		ir_Generator_start();
}

static void CanonWLDC( ir_portocol *value) {
	uint8_t j=0;
	uint8_t size=0;
	uint16_t * ptrData = &ir_pulse[0];

//	ir_pwm_clock_set(value->IR_Freq);
		ptrData[size] = 9042;
		size++;		
		ptrData[size] = 4379;

		for(j=0; j < value->size; j++) {
					if(!value->protocol[j]) {			// low bit
							size++;		
							ptrData[size] = 612; 
							size++;		
							ptrData[size] = 512; 
					}
					else {										//high bit
							size++;		
							ptrData[size] = 612; 
							size++;		
							ptrData[size] = 1621; 
					}
		}
		size++;		
		ptrData[size] = 599; 
	
		ir_data.Time = &ir_pulse[0];
		ir_data.Size = size;
		ir_data.rep_cycle = value->rep_cycle;
		ir_data.rep_delay= value->rep_delay;
		IR_Action_flag=true;			//LED action flag
		
		ir_Generator_start();
}

static void Olympus_Shutter_generation( ir_portocol *value) {
	uint8_t j=0;
	uint8_t size=0;
	uint16_t * ptrData = &ir_pulse[0];

//	ir_pwm_clock_set(value->IR_Freq);
		ptrData[size] = 9000;
		size++;		
		ptrData[size] = 4500;
		size++;		
		ptrData[size] = 500;

		for(j=0; j < value->size; j++) {
			if(!value->protocol[j]) {			// low bit
					size++;		
					ptrData[size] = 500; 
					size++;		
					ptrData[size] = 500; 
			}
			else {				//high bit
					size++;		
					ptrData[size] = 1500; 
					size++;		
					ptrData[size] = 500; 
			}
		}
	
		ir_data.Time = &ir_pulse[0];
		ir_data.Size = size;
		ir_data.rep_cycle = value->rep_cycle;
		ir_data.rep_delay= value->rep_delay;
		IR_Action_flag=true;			//LED action flag
		
		ir_Generator_start();
}

static void Olympus_Zoom( ir_portocol *value) {
	uint8_t j=0;
	uint8_t size=0;
	uint16_t * ptrData = &ir_pulse[0];

//	ir_pwm_clock_set(value->IR_Freq);
		ptrData[size] = 9000;
		size++;		
		ptrData[size] = 4500;
		size++;		
		ptrData[size] = 500;

		for(j=0; j < value->size; j++) {
			if(!value->protocol[j]) {			// low bit
					size++;		
					ptrData[size] = 500; 
					size++;		
					ptrData[size] = 500; 
			}
			else {				//high bit
					size++;		
					ptrData[size] = 1500; 
					size++;		
					ptrData[size] = 500; 
			}
		}
		size++;		
		ptrData[size] = 40000; 		//wait

//		if (pct>100) pct = 100;

//		pct = (pct*52)/100 + 1;
//		for (j=1; j<pct; j++){
			size++;		
			ptrData[size] = 9000;			//high
			size++;		
			ptrData[size] = 2000;			//wait
			size++;		
			ptrData[size] = 500;				//low
			size++;		
			ptrData[size] = 65534;			//wait		원래 96000임 
//			size++;		
//			ptrData[size] = 1;				//wait		분활
//			size++;		
//		ptrData[size] = 36000;			//wait		분활 
//		}
		
		ir_data.Time = &ir_pulse[0];
		ir_data.Size = size;
		ir_data.rep_cycle = value->rep_cycle;
		ir_data.rep_delay= value->rep_delay;
		IR_Action_flag=true;			//LED action flag
		
		ir_Generator_start();
}


//sony_ir_portocol	* IRdata;

// ir_data_t  *CanonShutter;
 
/*


uint16_t Sony_DSLR_Delay_Shutter[] =  {SonySync, S1,S1,S1,S0,S1,S1,S0,S0,S1,S0,S1,S1,S1,S0,S0,S0,S1,S1,S1,S1};
uint16_t Sony_DSLR_ShutterNow[] = 		{SonySync, S1,S0,S1,S1,S0,S1,S0,S0,S1,S0,S1,S1,S1,S0,S0,S0,S1,S1,S1,S1};
uint16_t Sony_DSLR_Video[]  =							{SonySync, S0,S0,S0,S1,S0,S0,S1,S0,S1,S0,S1,S1,S1,S0,S0,S0,S1,S1,S1,S1};
	
uint16_t SonyVideoToggle[] = 								{SonySync, S0,S0,S0,S1,S0,S0,S1,S0,S1,S0,S1,S1,S1,S0,S0,S0,S1,S1,S1,S1};

//uint16_t SonyCAM_REC[] = 									{SonySync, S0,S0,S0,S0,S1,S1,S0,S0,S1,S0,S1,S1,S1,S1,S0,S0,S0,S0,S0,S0};	//일치확인

	uint16_t SonyCAM_REC[] = 									{SonySync, S0,S0,S0,S0,S1,S1,S0,S0,S1,S0,S1,S1,S1,S1,S0,S0,S0,S0,S0,S0,SonyCAMRep,SonySync, S0,S0,S0,S0,S1,S1,S0,S0,S1,S0,S1,S1,S1,S1,S0,S0,S0,S0,S0,S0};	//동작확인됨 2회 이상 반복해야 동작됨

	
	uint16_t SonyCAM_Photo[] = 								{SonySync, S1,S0,S0,S0,S1,S1,S0,S0,S1,S0,S1,S1,S1,S1,S0,S0,S0,S0,S0,S0,SonyCAMRep,SonySync, S1,S0,S0,S0,S1,S1,S0,S0,S1,S0,S1,S1,S1,S1,S0,S0,S0,S0,S0,S0};	//동작확인됨 2회 이상 반복해야 동작됨
 
uint16_t SonyCAM_ZoomT[] =							{SonySync, S0,S1,S0,S1,S1,S0,S0,S1,S0,S0,S1,S1,S0,S1,S1, SonyCAMRep,SonySync, S0,S1,S0,S1,S1,S0,S0,S1,S0,S0,S1,S1,S0,S1,S1};		//동작 확인됨.
uint16_t SonyCAM_ZoomW[] = 						{SonySync, S1,S1,S0,S1,S1,S0,S0,S1,S0,S0,S1,S1,S0,S1,S1, SonyCAMRep,SonySync, S1,S1,S0,S1,S1,S0,S0,S1,S0,S0,S1,S1,S0,S1,S1};		//동작 확인됨.
	
	
 uint16_t SonyDSLR_Photo[] = {2400, 600,1200, 600, 600,600,1200,600,1200,600,600,600,1200,600,600,600,600,600,1200, 600,600,600,1200,600,1200,600,
														1200,600,600,600,600,600,600,600,1200,600,1200,600,1200,600,1200//};	//Sony DSLR Shutter
 
														,SonyRep,2400, 600,1200, 600, 600,600,1200,600,1200,600,600,600,1200,600,600,600,600,600,1200, 600,600,600,1200,600,1200,600,
														1200,600,600,600,600,600,600,600,1200,600,1200,600,1200,600,1200};	//Sony DSLR Shutter

														
//uint16_t SonyCAM_REC[] = {2400,600,600,600,600,600,600,600,600,600,1200,600,1200,600,600,600,600,600,1200,600,600,600,1200,600,1200,600,1200,600,1200,
//													600,600,600,600,600,600,600,600,600,600,600,600};

//uint16_t SonyCAM_Photo[] = {2400,600,1200,600,600,600,600,600,600,600,1200,600,1200,600,600,600,600,600,1200,600,600,600,1200,600,1200,600,1200,600,1200,
//												600,600,600,600,600,600,600,600,600,600,600,600};
//uint16_t SonyCAM_ZoomT[] = {SonySync,Ss ,Ss,Ss,Sl,Ss,Ss,Ss,Sl,Ss,Sl,Ss,Ss,Ss,Ss,Ss,Sl,Ss,Ss,Ss,Ss,Ss,Sl,Ss,Sl,Ss,Ss,Ss,Sl,Ss,Sl};
//uint16_t SonyCAM_ZoomW[] = {SonySync,Ss,Sl,Ss,Sl,Ss,Ss,Ss,Sl,Ss,Sl,Ss,Ss,Ss,Ss,Ss,Sl,Ss,Ss,Ss,Ss,Ss,Sl,Ss,Sl,Ss,Ss,Ss,Sl,Ss,Sl};


	
 void output_IR_Signal(uint8_t model) {
	 		IR_Action_flag=true;			//LED action flag

	switch(model) {
		case CANON_DSLR_PHOTO:
			ir_data_t.Time = &CanonDSLR_Photo[0];
			ir_data_t.Size = (sizeof(CanonDSLR_Photo)/sizeof(CanonDSLR_Photo[0]));
			break;

		case CANON_DSLR_REC:
			ir_data_t.Time = &CanonDSLR_REC[0];
			ir_data_t.Size = (sizeof(CanonDSLR_REC)/sizeof(CanonDSLR_REC[0]));
			break;

		case SONY_DSLR_PHOTO:
			ir_data_t.Time = &SonyDSLR_Photo[0];
			ir_data_t.Size = (sizeof(SonyDSLR_Photo)/sizeof(SonyDSLR_Photo[0]));
			break;

		case SONY_CAM_PHOTO:
			ir_data_t.Time = &SonyCAM_Photo[0];
			ir_data_t.Size = (sizeof(SonyCAM_Photo)/sizeof(SonyCAM_Photo[0]));
			break;
		
		case SONY_CAM_REC:
			ir_data_t.Time = &SonyCAM_REC[0];
			ir_data_t.Size = (sizeof(SonyCAM_REC)/sizeof(SonyCAM_REC[0]));
			break;

		case SONY_CAM_ZOOMT:
			ir_data_t.Time = &SonyCAM_ZoomT[0];
			ir_data_t.Size = (sizeof(SonyCAM_ZoomT)/sizeof(SonyCAM_ZoomT[0]));
			break;

		case SONY_CAM_ZOOMW:
			ir_data_t.Time = &SonyCAM_ZoomW[0];
			ir_data_t.Size = (sizeof(SonyCAM_ZoomW)/sizeof(SonyCAM_ZoomW[0]));
			break;

		case NIKON_DSLR_PHOTO:
			ir_data_t.Time = &NikonDSLR_Photo[0];
			ir_data_t.Size = (sizeof(NikonDSLR_Photo)/sizeof(NikonDSLR_Photo[0]));
			break;
		
		default:
			break;
		
	}
		TIM_Cmd(TIM14, ENABLE);
		TIM_ITConfig(TIM14, TIM_IT_Update ,ENABLE);

 }
 
 */
// CanonShutter[3].Time=0;// ={515, 7200, 515};




