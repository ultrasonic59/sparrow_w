#pragma once


#include "respondauto.h"

class RespondFrame
{
protected:

	quint64 Size;
	quint64 MaxSize;
	RespondAuto *Frame;

public:
	RespondFrame(void);
	RespondFrame(quint64 InitMaxSize);
	virtual ~RespondFrame(void);

	quint64 GetSize() const;
	bool SetSize(const quint64 NewSize);

	bool SetNewMaxSize(const quint64 NewMaxSize, const bool save_old);
	quint64 GetMaxSize() const;

	bool CopyFromArray(const RespondAuto *Src_array, const quint64 resp_count, const bool scaling);

	bool AddRespondAuto(RespondAuto &resp);
	void clear(void);

	void ShiftFrameArray(const qint32 positions);


	const RespondAuto *GetFrameArray(void) const;

	quint16 GetPriorityType(const quint16 type1, const quint16 type2) const;

};

