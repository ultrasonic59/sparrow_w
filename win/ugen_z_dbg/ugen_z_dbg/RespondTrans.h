#pragma once

#include "respond.h"


class RespondTrans
{

private:
	const int HundredProc;
	const float tenF;
	const int tenInt;

	const float StepKoef;

	inline float ToMM(const quint16, bool SubTpr = false) const;

public:
	RespondTrans();
	virtual ~RespondTrans(void);

	const quint8 VRChMode;
	const quint8 VRChUstMode;
	const quint8 TimeDMode;
	const quint8 SovmMode;
	const quint8 IsMKS;
	const quint8 InfoMode;

	static const quint8 MenuFl = 0x0;
	static const quint8 DefectoscopeFl = 0x1;
	static const quint8 ThickmeterFl = 0x12;

	static const quint8 AutoMenuFl = 0xA3;
	static const quint8 AutoDefectoscopeFl = 0xA1;


	const int MaxUInt8;
	const int OscillLength;

	const quint8 MaxkUsVal;

	bool IsSignal(const Respond &Respond) const;

	bool IsVRChMode(const Respond &Respond) const;
	bool IsVRChUstMode(const Respond &Respond) const;
	bool IsSovmMode(const Respond &Respond) const;
	bool IsTimeDMode(const Respond &Respond) const;
	bool IsMKSMode(const Respond &Respond) const;
	bool IsInfoMode(const Respond &Respond) const;

	float ToRadian(const Respond &Respond) const;

	float GetAl(const Respond &) const;
	float GetCosAl(const Respond &) const;

	float GetTPrizm(const Respond &) const;
	int GetSoundSpeed(const Respond &) const;
	int GetPreobNum(const Respond &) const;

	float GetOscBeg(const Respond &, const bool MKS, const float cos_al) const;
	float GetFullOscLen(const Respond &, const bool MKS, const float cos_al) const;
	float GetOscStart(const Respond &, const float cos_al) const;
	int GetOscLen(const Respond &, const bool MKS, const float cos_al, const float OscStart) const;

	float GetFullStrobBeg(const Respond &, const bool MKS, const float cos_al) const;
	float GetStrobBeg(const Respond &, const bool MKS, const float cos_al, const float OscStart) const;
	float GetStrobLen(const Respond &, const bool MKS, const float cos_al) const;
	int GetStrobPorog(const Respond &) const;

	bool Strob2On(const Respond &) const;
	bool Strob2Set(const Respond &) const;
	float GetFullStrob2Beg(const Respond &, const bool MKS, const float cos_al) const;
	float GetStrob2Beg(const Respond &, const bool MKS, const float cos_al, const float OscStart) const;
	float GetStrob2Len(const Respond &, const bool MKS, const float cos_al) const;
	int GetStrob2Porog(const Respond &) const;

	int GetAruVal(const Respond &) const;

	bool Strob3On(const Respond &) const;
	bool Strob3Idile(const Respond &) const;
	bool Strob3Set(const Respond &) const;
	float GetFullStrob3Beg(const Respond &, const bool MKS, const float cos_al) const;
	float GetStrob3Beg(const Respond &, const bool MKS, const float cos_al, const float OscStart) const;
	float GetStrob3Len(const Respond &, const bool MKS, const float cos_al) const;
	int GetStrob3Porog(const Respond &) const;


	float GetDY(const Respond &, const float TimeD, const float cos_al) const;
	float GetDX(const Respond &, const float TimeD) const;

	float GetTimeD(const Respond &) const;

	quint8 GetRejime(const Respond &) const;

	float GetVRChPos(const Respond &Respond, const quint8 i, const bool MKS, const float cos_al) const;

};

