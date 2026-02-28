#include "RespondTrans.h"

#define _USE_MATH_DEFINES
#include "math.h"

RespondTrans::RespondTrans() :
					HundredProc(100),
					tenF(10.0f),
					tenInt(10),
					StepKoef(0.05f),

					VRChMode(0x01),
					VRChUstMode(0x02),
					TimeDMode(0x04),
					SovmMode(0x20),
					IsMKS(0x40),
					InfoMode(0x80),

					MaxUInt8(255),
					OscillLength(220),

					MaxkUsVal(84)

{
}


RespondTrans::~RespondTrans(void)
{
}

bool RespondTrans::IsSignal(const Respond &Respond) const
{
#if 0
	if(Strob3Set(Respond))
	{
		if(Respond.max_ak >= Respond.por_3)
			return true;
	}
	else if(Strob3Idile(Respond))
	{
		if((Respond.ampl >= Respond.por) && (Respond.max_ak >= Respond.por_3))
			return true;
	}
	else
	{
		if(Respond.ampl >= Respond.por)
			return true;
	}

#endif
/*	if(Respond.ampl >= Respond.por || Respond.max_ak >= Respond.por_3)
			return true;
*/
	return false;
}

bool RespondTrans::IsVRChMode(const Respond &Respond) const
{
	return Respond.vrchON & VRChMode;
}

bool RespondTrans::IsVRChUstMode(const Respond &Respond) const
{
	return Respond.vrchON & VRChUstMode;
}

bool RespondTrans::IsTimeDMode(const Respond &Respond) const
{
	return Respond.vrchON & TimeDMode;
}

bool RespondTrans::IsSovmMode(const Respond &Respond) const
{
	return Respond.vrchON & SovmMode;
}

bool RespondTrans::IsMKSMode(const Respond &Respond) const
{
	return Respond.vrchON & IsMKS;
}

bool RespondTrans::IsInfoMode(const Respond &Respond) const
{
	return Respond.vrchON & InfoMode;
}


float RespondTrans::ToRadian(const Respond &Respond) const
{
	return (float)Respond.alpha * M_PI / 360;
}

float RespondTrans::GetAl(const Respond &Respond) const
{
	return Respond.alpha / 2.0f;
}

float RespondTrans::GetCosAl(const Respond &Respond) const
{
	return cos( ToRadian( Respond ) );
}

float RespondTrans::GetTPrizm(const Respond &Respond) const
{
	return Respond.Tpr / 100.0f;
}

int RespondTrans::GetSoundSpeed(const Respond &Respond) const
{
	return Respond.cc;
}

int RespondTrans::GetPreobNum(const Respond &Respond) const
{
	return Respond.nPep;
}	
	
float RespondTrans::GetOscBeg(const Respond &Respond, const bool MKS, const float cos_al) const
{
#if 0
	if(MKS)
		return Respond.beg / tenF;
		
	return Respond.cc * cos_al * Respond.beg / 20000.0f;
#endif
return 0.0;
}
	
	
float RespondTrans::GetFullOscLen(const Respond &Respond, const bool MKS, const float cos_al) const
{

///	if(MKS)
		return (Respond.step_r + 1) * OscillLength * StepKoef ;
		
///	return Respond.cc * cos_al * (Respond.step + 1) * OscillLength * StepKoef / 2000.0f;
}
	
	
float RespondTrans::GetOscStart(const Respond &Respond, const float cos_al) const
{		
	return Respond.cc * cos_al * Respond.Tpr / 200000.0f;
}	
	
int RespondTrans::GetOscLen(const Respond &Respond, const bool MKS, const float cos_al, const float OscStart) const
{
	if(MKS)
		return (int)GetFullOscLen(Respond, MKS, cos_al);
		
	int temp = (int)(GetFullOscLen(Respond, MKS, cos_al) - OscStart);
		
	if(temp < 0)
		temp = 0;
		
	return	temp;
}



float RespondTrans::GetFullStrobBeg(const Respond &Respond, const bool MKS, const float cos_al) const
{	
#if 0
	if(MKS)
		return Respond.bStr / tenF;
		
	return Respond.cc * cos_al * Respond.bStr  / 20000.0f;
#endif
return 0.0;
}
	
float RespondTrans::GetStrobBeg(const Respond &Respond, const bool MKS, const float cos_al, const float OscStart) const
{		
	if(MKS)
		return GetFullStrobBeg(Respond, MKS, cos_al);
		
	float temp = GetFullStrobBeg(Respond, MKS, cos_al) - OscStart;
		
	if(temp < 0)
		temp = 0;
		
	return temp;
}

float RespondTrans::GetStrobLen(const Respond &Respond, const bool MKS, const float cos_al) const
{	
#if 0
	if(MKS)
		return Respond.lStr / tenF;
		
	return Respond.cc * cos_al * Respond.lStr  / 20000.0f;
#endif
return 0.0;
}

int RespondTrans::GetStrobPorog(const Respond &Respond) const
{		
#if 0
	return (int)(Respond.por * HundredProc / MaxUInt8);
#endif
return 0;
}




bool RespondTrans::Strob2On(const Respond &Respond) const
{
#if 0
	return Respond.mode_2 > 0;
#endif
return false;
}

bool RespondTrans::Strob2Set(const Respond &Respond) const
{
#if 0
	return Respond.mode_2 == 3;
#endif
return false;
}

float RespondTrans::GetFullStrob2Beg(const Respond &Respond, const bool MKS, const float cos_al) const
{	
#if 0
	if(MKS)
		return Respond.bStr_2 / tenF;
		
	return Respond.cc * cos_al * Respond.bStr_2  / 20000.0f;
#endif
return 0.0;
}
	
float RespondTrans::GetStrob2Beg(const Respond &Respond, const bool MKS, const float cos_al, const float OscStart) const
{		
	if(MKS)
		return GetFullStrob2Beg(Respond, MKS, cos_al);
		
	float temp = GetFullStrob2Beg(Respond, MKS, cos_al) - OscStart;
		
	if(temp < 0)
		temp = 0;
		
	return temp;
}

float RespondTrans::GetStrob2Len(const Respond &Respond, const bool MKS, const float cos_al) const
{	
#if 0
	if(MKS)
		return Respond.lStr_2 / tenF;
		
	return Respond.cc * cos_al * Respond.lStr_2  / 20000.0f;
#endif
return 0.0;
}

int RespondTrans::GetStrob2Porog(const Respond &Respond) const
{	
#if 0
	return (int)(Respond.por_2 * HundredProc / MaxUInt8);
#endif
return 0;
}


int RespondTrans::GetAruVal(const Respond &Respond) const
{
	return Respond.ampl_aru;
}



bool RespondTrans::Strob3On(const Respond &Respond) const
{
#if 0
	return Respond.mode_3 > 0;
#endif
return false;
}

bool RespondTrans::Strob3Idile(const Respond &Respond) const
{
#if 0
	return Respond.mode_3 == 1;
#endif
return false;
}

bool RespondTrans::Strob3Set(const Respond &Respond) const
{
////	return Respond.mode_3 == 3;
return false;
}

float RespondTrans::GetFullStrob3Beg(const Respond &Respond, const bool MKS, const float cos_al) const
{
#if 0
	if(MKS)
		return Respond.bStr_3 / tenF;
		
	return Respond.cc * cos_al * Respond.bStr_3  / 20000.0f;
#endif
	return 0.0;
}
	
float RespondTrans::GetStrob3Beg(const Respond &Respond, const bool MKS, const float cos_al, const float OscStart) const
{
#if 0
	if(MKS)
		return GetFullStrob3Beg(Respond, MKS, cos_al);
		
	float temp = GetFullStrob3Beg(Respond, MKS, cos_al) - OscStart;
		
	if(temp < 0)
		temp = 0;
		
	return temp;
#endif
	return 0.0;
}

float RespondTrans::GetStrob3Len(const Respond &Respond, const bool MKS, const float cos_al) const
{	
#if 0
	if(MKS)
		return Respond.lStr_3 / tenF;
		
	return Respond.cc * cos_al * Respond.lStr_3  / 20000.0f;
#endif
		return 0.0;

}

int RespondTrans::GetStrob3Porog(const Respond &Respond) const
{		
///	return (int)(Respond.por_3 * HundredProc / MaxUInt8);
return 0;

}




float RespondTrans::GetDY(const Respond &Respond, const float TimeD, const float cos_al) const
{		
	float temp = Respond.cc * cos_al * (TimeD - (Strob3Idile(Respond) ? 0 : (Respond.Tpr / 100.0f))) / 2000.0f ;
		
	if(temp < 0)
		temp = 0;
		
	return temp;
}
	
	
float RespondTrans::GetDX(const Respond &Respond, const float TimeD) const
{	
	float temp = Respond.cc * sin( ToRadian( Respond ) ) * (TimeD - (Strob3Idile(Respond) ? 0 : (Respond.Tpr / 100.0f))) / 2000.0f;
		
	if(temp < 0)
		temp = 0;
		
	return temp;
}


float RespondTrans::GetTimeD(const Respond &Respond) const
{
	if(Strob3On(Respond))
	{
		if(Strob3Set(Respond))
			return Respond.timeD_3 / 20.0f;

		return abs((qint32)Respond.timeD - (qint32)Respond.timeD_3  )/ 20.0f;
	}

	return Respond.timeD / 20.0f;
}

quint8 RespondTrans::GetRejime(const Respond &Respond) const
{
////	return Respond.curr_Rej;
return 0;
}


float RespondTrans::GetVRChPos(const Respond &Respond, const quint8 i, const bool MKS, const float cos_al) const
{		
	if(MKS)
		return Respond.vrchT[i] + 3;
		
	return Respond.cc * cos_al * (Respond.vrchT[i] + 3)  / 2000.0f;
}